// AirCavLOSDlg.cpp : implementation file
//

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <winsock2.h>

#include "stdafx.h"
#include "AirCavCommon.h"
#include "AirCavLOS.h"
#include "AirCavLOSDlg.h"
#include "ScenarioDlg.h"
#include "OppFireDialog.h"
#include "KillDialog.h"
#include "KillSuppressDialog.h"
#include "UnitDetails.h"
#include "IndirectFire.h"

#include <string>
#include <vector>
#include <map>

#pragma warning(disable:4996)
#pragma warning(disable:4244)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BASIC_OBSERVATION 0

extern void CalcAdj (int dir, int cur_y, int cur_x, int *y, int *x);

char *TerrainStr[4] =
{
	"Clear", "Woods", "Town", "River"
};

static std::string file_dir = "C:\\AirCavLOS\\";
static std::string weapons_file = "weapons.txt";
static std::string units_file = "units.txt";


Artillery ArtilleryTypes[MAXINDIRECT] = 
{
//  name                        T   S   S   M   M    A   B   C   D   E   F     W   T   A   S
//                              y   c   c   i   a                              d   n   d   u
//                              p   n   d   n   x                              s       j   p

	{ "U.S. 81mm Mortar",       0,  3,  5,  2, 45,  10, 10, 10, -7, -4, -2,   -3, -4,  0,  0 }, // 0
	{ "U.S. 4.2\" Mortar",      0,  3,  5,  4, 46,  10, 10, -6, -4, -3, -1,   -2, -2, -5,  0 }, // 1
	{ "U.S. 160mm Mortar",      0,  2,  4,  0,  0,  -8, -7, -6, -4, -2, -1,   10, -2, -4,  0 }, // 2
	{ "U.S. 105mm Howitzer",    1,  3,  5,  0,  0,  -9, -7, -7, -5, -3, -1,   -1, -2, -4,  0 }, // 3
	{ "U.S. 155mm Howitzer",    1,  3,  5,  0,  0,  -8, -7, -6, -4, -1, -1,   10, -1, -3,  1 }, // 4
	{ "U.S. 203mm Rocket",      2,  3,  0,  0,  0,  -7, -6, -5, -4, -1, -1,   10, -1,  0,  1 }, // 5

	{ "Soviet 120mm Mortar",    0,  3,  3,  0,  0,  10, 10, -7, -4, -3, -1,   -1, -2, -5,  0 }, // 6
	{ "Soviet 122mm Howitzer",  1,  4,  4,  0,  0,  -8, -7, -6, -4, -2, -1,   -1, -2, -5,  0 }, // 7
	{ "Soviet 152mm Howitzer",  1,  4,  4,  0,  0,  -8, -7, -5, -3, -1, -1,   10, -1, -3,  1 }, // 8
	{ "Soviet 122mm Rocket",    2,  4,  4,  0,  0,  -8, -7, -5, -3, -1, -1,   10, -2,  0,  1 }  // 9
};

// --------------------------------------------------------------------------------------


// ==========================================================================================================
//                                                NETWORKING
// ==========================================================================================================

static int					s_networkActiveSide;
static int					s_thisActiveSide;
static std::string			s_hostIP;
static uint64_t				s_networkCommand;
static SOCKET				s_clientserversocket;
static SOCKET				s_serverclientsocket;
static char					s_networkBuffer[1024];

// --------------------------------------------------------- client ------------------------------------------------ //

// Function that receive data from server
DWORD WINAPI clientReceive(LPVOID lpParam)
{
	// Created buffer[] to receive message
	char buffer[1024] = { 0 };

	// Created server socket
	SOCKET server = *(SOCKET*)lpParam;

	// Client executes continuously
	while (true)
	{
		// If received buffer gives error then return -1
		if (recv(server, buffer, sizeof(buffer), 0)	== SOCKET_ERROR) {
			return -1;
		}

		// If Server exits
		if (strcmp(buffer, "exit") == 0) {
			return 1;
		}

		// parse the message
		if (strncmp(buffer, "select", 6) == 0)
		{
			s_networkCommand = MSG_UPDATE_SET_ACTIVE;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "info", 4) == 0)
		{
			s_networkCommand = MSG_UPDATE_UNIT_INFO;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "map", 3) == 0)
		{
			s_networkCommand = MSG_UPDATE_MAP_INFO;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "time", 4) == 0)
		{
			s_networkCommand = MSG_CHANGE_TIMEOFDAY;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "weather", 7) == 0)
		{
			s_networkCommand = MSG_CHANGE_WEATHER;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "switch", 6) == 0)
		{
			s_networkCommand = MSG_SWITCH_SIDES;
			strcpy(s_networkBuffer, buffer);
		}

		// Clear buffer message
		memset(buffer, 0, sizeof(buffer));
	}
	return 1;
}

DWORD WINAPI clientSend(const char *buffer)
{
	int size = (int)strlen(buffer);
	if (send(s_clientserversocket, buffer, size, 0) == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}

int create_client(std::string &ipaddr)
{
	// Input data
	WSADATA WSAData;

	// Created socket server
	SOCKADDR_IN addr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// If invalid socket created, return -1
	if ((s_clientserversocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		return -1;
	}

	addr.sin_addr.s_addr = inet_addr(ipaddr.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555);

	// If connection failed
	if (connect(s_clientserversocket,	(SOCKADDR*)&addr, sizeof(addr))	== SOCKET_ERROR) {
		return -1;
	}

	DWORD tid;

	// Create Thread t1
	HANDLE t1 = CreateThread(NULL,
		0,
		clientReceive,
		&s_clientserversocket,
		0, &tid);

	// Received Objects from client
	WaitForSingleObject(t1, INFINITE);

	// Socket closed
	closesocket(s_clientserversocket);
	WSACleanup();
	return 0;
}

// --------------------------------------------------------- server ------------------------------------------------ //

// Function that receive data from client
DWORD WINAPI serverReceive(LPVOID lpParam)
{
	// Created buffer[] to receive message
	char buffer[1024] = { 0 };

	// Created client socket
	SOCKET client = *(SOCKET*)lpParam;

	// Server executes continuously
	while (true)
	{
		// If received buffer gives error then return -1
		if (recv(client, buffer, sizeof(buffer), 0)	== SOCKET_ERROR) {
			return -1;
		}

		// If Client exits
		if (strcmp(buffer, "exit") == 0) {
			break;
		}

		// parse the message
		if (strncmp(buffer, "select", 6) == 0)
		{
			s_networkCommand = MSG_UPDATE_SET_ACTIVE;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "info", 4) == 0)
		{
			s_networkCommand = MSG_UPDATE_UNIT_INFO;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "map", 3) == 0)
		{
			s_networkCommand = MSG_UPDATE_MAP_INFO;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "time", 4) == 0)
		{
			s_networkCommand = MSG_CHANGE_TIMEOFDAY;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "weather", 7) == 0)
		{
			s_networkCommand = MSG_CHANGE_WEATHER;
			strcpy(s_networkBuffer, buffer);
		}
		else if (strncmp(buffer, "switch", 6) == 0)
		{
			s_networkCommand = MSG_SWITCH_SIDES;
			strcpy(s_networkBuffer, buffer);
		}

		// Clear buffer message
		memset(buffer, 0, sizeof(buffer));
	}
	return 1;
}

DWORD WINAPI serverSend(const char *buffer)
{
	// If sending failed return -1
	int size = (int)strlen(buffer);
	if (send(s_serverclientsocket, buffer, size, 0) == SOCKET_ERROR) {
		return -1;
	}
	return 0;
}

// Driver Code
int create_server()
{
	// Data
	WSADATA WSAData;

	// Created socket server and client
	SOCKET server;

	// Socket address for server and client
	SOCKADDR_IN serverAddr, clientAddr;

	WSAStartup(MAKEWORD(2, 0), &WSAData);

	// Making server
	server = socket(AF_INET, SOCK_STREAM, 0);

	// If invalid socket created, return -1
	if (server == INVALID_SOCKET) {
		return -1;
	}
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5555);

	// If socket error occurred, return -1
	if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		return -1;
	}

	// Get the request from server
	if (listen(server, 0) == SOCKET_ERROR) {
		return -1;
	}

	// Initialize client address
	int clientAddrSize = sizeof(clientAddr);

	// If connection established
	if ((s_serverclientsocket = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET)
	{
		// Create variable of type DWORD
		DWORD tid;

		// Create Thread t1
		HANDLE t1 = CreateThread(NULL,
			0,
			serverReceive,
			&s_serverclientsocket,
			0,
			&tid);

		// Received Objects from client
		WaitForSingleObject(t1,	INFINITE);

		// Close the socket
		closesocket(s_serverclientsocket);

		// If socket closing failed.
		if (closesocket(server)	== SOCKET_ERROR) {
			return -1;
		}
		WSACleanup();
	}
	return 0;
}

void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT_PTR nTimerID, DWORD dwTime)
{
	if (s_networkBuffer[0] != 0)
	{
		CAirCavLOSDlg* pMainWnd = (CAirCavLOSDlg*)AfxGetMainWnd();

		switch (s_networkCommand)
		{
		case MSG_UPDATE_SET_ACTIVE:
		{
			char cmd[10];
			int unitID;
			sscanf(s_networkBuffer, "%s %d", cmd, &unitID);
			pMainWnd->setActiveUnit(unitID);
			break;
		}
		case MSG_UPDATE_UNIT_INFO:
			pMainWnd->decodeUnitInfo(s_networkBuffer);
			break;
		case MSG_UPDATE_MAP_INFO:
			pMainWnd->decodeMapInfo(s_networkBuffer);
			break;
		case MSG_CHANGE_TIMEOFDAY:
			pMainWnd->decodeTimeOfDay(s_networkBuffer);
			break;
		case MSG_CHANGE_WEATHER:
			pMainWnd->decodeWeather(s_networkBuffer);
			break;
		case MSG_SWITCH_SIDES:
			pMainWnd->switchSides();
			break;
		}
		pMainWnd->updateActiveUnit(TRUE);

		memset(s_networkBuffer, 0, sizeof(s_networkBuffer));
	}
}


class CMyObject : public CObject
{
	DECLARE_DYNAMIC(CMyObject);

public:
	CMyObject() {}
	~CMyObject() {}

	void startServer()
	{
		if (s_thisActiveSide == SIDE_BLUE)
			create_server();
		else
			create_client(s_hostIP);
	}
};

IMPLEMENT_DYNAMIC(CMyObject, CObject)

UINT MyThreadProc(LPVOID pParam)
{
	CMyObject* pObject = (CMyObject*)pParam;

	if (pObject == NULL || !pObject->IsKindOf(RUNTIME_CLASS(CMyObject)))
		return 1;   // if pObject is not valid

	// do something with 'pObject'
	pObject->startServer();

	return 0;   // thread completed successfully
}

// ==========================================================================================================



// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAirCavLOSDlg dialog
CAirCavLOSDlg::CAirCavLOSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAirCavLOSDlg::IDD, pParent)
	, m_activeUnitName(_T(""))
	, m_currentWeather(_T("Clear"))
	, m_currentTimeOfDay(_T("Day"))
	, m_popSmokeWhileMoving(false)
{
	// data initialization
	for ( int w=0; w<MAXWEAP; w++ )
		weaponDataList[w] = 0;
	for ( int t=0; t<MAXTYPS; t++ )
		unitDataList[t] = 0;
	for ( int c=0; c<MAXCOUNTERS; c++ )
		counterDataList[c] = 0;
	mapData = 0;
	scenarioData = 0;
	m_maxCounters = 0;
	m_normalMap = true;

	// dialog initialization
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// local data initialization
	m_activeUnitWeapon = 0;
	m_contourLines = 0;
	m_smoke = 0;
	m_wreck = 0;
	m_roadN = 0;
	m_roadNW = 0;
	m_roadSW = 0;
	m_roadS = 0;
	m_roadSE = 0;
	m_roadNE = 0;
	m_autobahnN = 0;
	m_autobahnNW = 0;
	m_autobahnSW = 0;
	m_autobahnS = 0;
	m_autobahnSE = 0;
	m_autobahnNE = 0;
	m_streamN = 0;
	m_streamNW = 0;
	m_streamSW = 0;
	m_streamS = 0;
	m_streamSE = 0;
	m_streamNE = 0;
	m_activeUnitFired = 0;
	m_activeUnitMoved = 0;
	m_activeUnitEvading = 0;
	m_activeUnitInDefilade = 0;
	m_activeUnitIsSuppressed = 0;
	m_debugLOSMessages = 0;
	m_debugFKNMessages = 0;
	m_ActiveTarget = -1;
	m_Weather = 0;
	m_TimeOfDay = 0;
	m_lastArtilleryUnit = 0;
	m_lastArtilleryRow = 0;
	m_lastArtilleryCol = 0;

	m_unitTracking.unit = -1;
	m_unitTracking.OPs = 0;

	m_shutdown = false;

	s_networkActiveSide = -1;
}

void CAirCavLOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNITS, m_AvailableUnitsListBox);
	DDX_Control(pDX, IDC_LIST_SIGHTED_UNITS, m_SightedUnitsListBox);
	DDX_Control(pDX, IDC_LIST_SIGHTING_UNITS, m_SightingUnitsListBox);
	DDX_Control(pDX, IDC_LIST_FIRING_UNITS, m_FiringUnitsListBox);
	DDX_Control(pDX, IDC_COMBO_OPTICS, m_currentOpticsComboBox);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_NAME, m_activeUnitName);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_OP, m_activeUnitOPs);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_UNIT_TYPE, m_activeUnitUnitType);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_TARGET_TYPE, m_activeUnitTargetType);	
	DDX_Text(pDX, IDC_EDIT_ACTIVE_TARGET_TYPE2, m_activeUnitCarrierUnit);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_UNIT_COUNTRY, m_activeUnitCountry);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_MAIN_WPN1, m_activeUnitMainWpn1);	
	DDX_Text(pDX, IDC_EDIT_ACTIVE_MAIN_WPN2, m_activeUnitMainWpn2);	
	DDX_Text(pDX, IDC_EDIT_ACTIVE_MAIN_WPN3, m_activeUnitMainWpn3);	
	DDX_Text(pDX, IDC_EDIT_ACTIVE_SECONDARY_WPN1, m_activeUnitSecondaryWpn1);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_SECONDARY_WPN2, m_activeUnitSecondaryWpn2);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_UNIT_TYPE_NAME, m_activeUnitTypeName);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_LOC, m_activeUnitPosition);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_OFFSET, m_activeUnitOffset);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_ACTUALELEV, m_activeUnitActualElev);
	DDX_Radio(pDX, IDC_RADIO_ACTIVE_MAIN1, m_activeUnitWeapon);
	DDX_Text(pDX, IDC_EDIT_TERR_TER, m_terrainType);
	DDX_Text(pDX, IDC_EDIT_TERR_ELEV, m_terrainElev);
	DDX_Text(pDX, IDC_EDIT_TERR_STACK, m_terrainStack);
	DDX_Check(pDX, IDC_CHECK_CONTOUR_LINES, m_contourLines);
	DDX_Check(pDX, IDC_CHECK_SMOKE, m_smoke);
	DDX_Check(pDX, IDC_CHECK_WRECK, m_wreck);
	DDX_Check(pDX, IDC_CHECK_R_N, m_roadN);
	DDX_Check(pDX, IDC_CHECK_R_NW3, m_roadNW);
	DDX_Check(pDX, IDC_CHECK_R_SW3, m_roadSW);
	DDX_Check(pDX, IDC_CHECK_R_S3, m_roadS);
	DDX_Check(pDX, IDC_CHECK_R_SE3, m_roadSE);
	DDX_Check(pDX, IDC_CHECK_R_NE3, m_roadNE);
	DDX_Check(pDX, IDC_CHECK_R_N3, m_autobahnN);
	DDX_Check(pDX, IDC_CHECK_R_NW4, m_autobahnNW);
	DDX_Check(pDX, IDC_CHECK_R_SW4, m_autobahnSW);
	DDX_Check(pDX, IDC_CHECK_R_S4, m_autobahnS);
	DDX_Check(pDX, IDC_CHECK_R_SE4, m_autobahnSE);
	DDX_Check(pDX, IDC_CHECK_R_NE4, m_autobahnNE);
	DDX_Check(pDX, IDC_CHECK_R_N4, m_streamN);
	DDX_Check(pDX, IDC_CHECK_R_NW5, m_streamNW);
	DDX_Check(pDX, IDC_CHECK_R_SW5, m_streamSW);
	DDX_Check(pDX, IDC_CHECK_R_S5, m_streamS);
	DDX_Check(pDX, IDC_CHECK_R_SE5, m_streamSE);
	DDX_Check(pDX, IDC_CHECK_R_NE5, m_streamNE);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_FIRED, m_activeUnitFired);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_MOVED, m_activeUnitMoved);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_EVADING, m_activeUnitEvading);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_IN_DEF, m_activeUnitInDefilade);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_SIDE, m_activeCountry);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_SIDE2, m_activeSide);
	DDX_Text(pDX, IDC_EDIT_MY_SIDE, m_mySide);
	DDX_Check(pDX, IDC_CHECK_DEBUG, m_debugLOSMessages);
	DDX_Check(pDX, IDC_CHECK_DEBUG_PK, m_debugFKNMessages);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_LOWLEVEL, m_activeUnitLowLevel);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_SUPPRESSED, m_activeUnitIsSuppressed);
	DDX_CBString(pDX, IDC_COMBO_WEATHER, m_currentWeather);
	DDX_CBString(pDX, IDC_COMBO_TIME_OF_DAY, m_currentTimeOfDay);
	DDX_Check(pDX, IDC_CHECK_POP_SMOKE, m_popSmokeWhileMoving);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_RADAR_ON, m_activeUnitRadarOn);
}

BEGIN_MESSAGE_MAP(CAirCavLOSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_ACTIVE_NAME, &CAirCavLOSDlg::OnEnChangeEditActiveName)
	ON_EN_CHANGE(IDC_EDIT_ACTIVE_OP, &CAirCavLOSDlg::OnEnChangeEditActiveOp)
	ON_BN_CLICKED(IDC_RADIO_ACTIVE_MAIN1, &CAirCavLOSDlg::OnBnClickedRadioActiveMain1)
	ON_BN_CLICKED(IDC_RADIO_ACTIVE_MAIN2, &CAirCavLOSDlg::OnBnClickedRadioActiveMain2)
	ON_BN_CLICKED(IDC_RADIO_ACTIVE_MAIN3, &CAirCavLOSDlg::OnBnClickedRadioActiveMain3)
	ON_BN_CLICKED(IDC_RADIO_ACTIVE_SECONDARY1, &CAirCavLOSDlg::OnBnClickedRadioActiveSecondary1)
	ON_BN_CLICKED(IDC_RADIO_ACTIVE_SECONDARY2, &CAirCavLOSDlg::OnBnClickedRadioActiveSecondary2)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_PASS, &CAirCavLOSDlg::OnBnClickedButtonActionPopUp)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_FIRE, &CAirCavLOSDlg::OnBnClickedButtonActionFireGun)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_N, &CAirCavLOSDlg::OnBnClickedButtonActionMoveN)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_NW, &CAirCavLOSDlg::OnBnClickedButtonActionMoveNw)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_SW, &CAirCavLOSDlg::OnBnClickedButtonActionMoveSw)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_S, &CAirCavLOSDlg::OnBnClickedButtonActionMoveS)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_SE, &CAirCavLOSDlg::OnBnClickedButtonActionMoveSe)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_NE, &CAirCavLOSDlg::OnBnClickedButtonActionMoveNe)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_N2, &CAirCavLOSDlg::OnBnClickedButtonActionMoveN)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_NW2, &CAirCavLOSDlg::OnBnClickedButtonActionMoveNw)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_SW2, &CAirCavLOSDlg::OnBnClickedButtonActionMoveSw)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_S2, &CAirCavLOSDlg::OnBnClickedButtonActionMoveS)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_SE2, &CAirCavLOSDlg::OnBnClickedButtonActionMoveSe)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOVE_NE2, &CAirCavLOSDlg::OnBnClickedButtonActionMoveNe)
	ON_LBN_SELCHANGE(IDC_LIST_SIGHTED_UNITS, &CAirCavLOSDlg::OnLbnSelchangeListSightedUnits)
	ON_LBN_SELCHANGE(IDC_LIST_UNITS, &CAirCavLOSDlg::OnLbnSelchangeListUnits)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_ACTIVE, &CAirCavLOSDlg::OnBnClickedButtonMakeActive)
	ON_BN_CLICKED(IDC_BUTTON_RESET_ACTIVE, &CAirCavLOSDlg::OnBnClickedButtonResetActive)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_TERRAIN, &CAirCavLOSDlg::OnBnClickedButtonEditTerrain)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_TERRAIN2, &CAirCavLOSDlg::OnBnClickedButtonFillTerrain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CAirCavLOSDlg::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE_FIRED, &CAirCavLOSDlg::OnBnClickedCheckActiveFired)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE_MOVED, &CAirCavLOSDlg::OnBnClickedCheckActiveMoved)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_RESET, &CAirCavLOSDlg::OnBnClickedButtonActionReset)
	ON_EN_KILLFOCUS(IDC_EDIT_ACTIVE_LOC, &CAirCavLOSDlg::OnEnChangeEditActiveLoc)
	ON_BN_CLICKED(IDC_CHECK_DEBUG, &CAirCavLOSDlg::OnBnClickedCheckDebug)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_PK, &CAirCavLOSDlg::OnBnClickedCheckDebugPk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_EVADE, &CAirCavLOSDlg::OnBnClickedButtonActionEvade)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_DEFILADE, &CAirCavLOSDlg::OnBnClickedButtonActionDefilade)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE_SUPPRESSED, &CAirCavLOSDlg::OnBnClickedButtonActiveSuppressed)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE_EVADING, &CAirCavLOSDlg::OnBnClickedCheckActiveEvading)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE_IN_DEF, &CAirCavLOSDlg::OnBnClickedCheckActiveInDef)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_MOUNT, &CAirCavLOSDlg::OnBnClickedButtonActionMount)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_LOWLEVEL, &CAirCavLOSDlg::OnBnClickedButtonActionLowlevel)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OPPFIRE, &CAirCavLOSDlg::OnBnClickedButtonActionOppfire)
	ON_BN_CLICKED(IDC_BUTTON_RESET_FIRING, &CAirCavLOSDlg::OnBnClickedButtonResetFiring)
	ON_BN_CLICKED(IDC_BUTTON_RESOLVE_OPPFIRE, &CAirCavLOSDlg::OnBnClickedButtonResolveOppfire)
	ON_BN_CLICKED(IDC_BUTTON_LAYSMOKE, &CAirCavLOSDlg::OnBnClickedButtonLaysmoke)
	ON_BN_CLICKED(IDC_CHECK_SMOKE, &CAirCavLOSDlg::OnBnClickedCheckSmoke)
	ON_BN_CLICKED(IDC_CHECK_WRECK, &CAirCavLOSDlg::OnBnClickedCheckWreck)
	ON_BN_CLICKED(IDC_BUTTON_ELEVOFFSET, &CAirCavLOSDlg::OnBnClickedButtonElevoffset)
	ON_BN_CLICKED(IDC_BUTTON_LIST_ALL, &CAirCavLOSDlg::OnBnClickedButtonListUnits)
	ON_CBN_SELCHANGE(IDC_COMBO_WEATHER, &CAirCavLOSDlg::OnCbnSelchangeComboWeather)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_OF_DAY, &CAirCavLOSDlg::OnCbnSelchangeComboTimeOfDay)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_INDFIRE, &CAirCavLOSDlg::OnBnClickedButtonActionIndfire)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_SMOKE, &CAirCavLOSDlg::OnBnClickedButtonRemoveSmoke)
	ON_BN_CLICKED(IDC_CHECK_POP_SMOKE, &CAirCavLOSDlg::OnBnClickedCheckPopSmoke)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PROGRESS, &CAirCavLOSDlg::OnBnClickedButtonSaveProgress)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CAirCavLOSDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_CHECK_IGNORE_WATER, &CAirCavLOSDlg::OnBnClickedCheckIgnoreWater)
	ON_BN_CLICKED(IDC_CHECK_IGNORE_AUTOBAHN, &CAirCavLOSDlg::OnBnClickedCheckIgnoreAutobahn)
	ON_BN_CLICKED(IDC_CHECK_ROTATE_MAP, &CAirCavLOSDlg::OnBnClickedCheckRotateMap)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_PREVIOUS_MOVE, &CAirCavLOSDlg::OnBnClickedButtonActionPreviousMove)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTICS, &CAirCavLOSDlg::OnCbnSelchangeComboOptics)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_LASER, &CAirCavLOSDlg::OnBnClickedButtonActionLaser)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_RADAR, &CAirCavLOSDlg::OnBnClickedButtonActionRadar)
	ON_BN_CLICKED(IDC_CHECK_ACTIVE_RADAR_ON, &CAirCavLOSDlg::OnBnClickedCheckActiveRadarOn)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CAirCavLOSDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CAirCavLOSDlg message handlers

BOOL CAirCavLOSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// get current path and use this to open other files (assumes exe is in AirCavLOS\x64\Release or AirCavLOS\x64\Debug)
	TCHAR fileNameBuffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, fileNameBuffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(fileNameBuffer).find_last_of(L"\\/");
	std::wstring thisPathW = std::wstring(fileNameBuffer).substr(0, pos);
	std::string thisPath(thisPathW.begin(), thisPathW.end());
#if _WIN64
	file_dir = thisPath + "\\..\\..\\";
#else
	file_dir = thisPath + "\\..\\";
#endif

	// create the weapon data
	std::string weapons = file_dir + weapons_file;
	FILE *weaponFile;
	errno_t err = fopen_s(&weaponFile, weapons.c_str(), "rt");
	if ( err != 0 || !weaponFile )
	{
		MessageBox((CString)"Unable to open weapons.txt", (CString)"Error", MB_OK);
		return FALSE;
	}
	readWeaponTextFile( weaponFile );
	fclose( weaponFile );

	// create the unit data
	std::string units = file_dir + units_file;
	FILE *unitFile;
	err = fopen_s(&unitFile, units.c_str(), "rt");
	if ( err != 0 || !unitFile )
	{
		MessageBox((CString)"Unable to open units.txt", (CString)"Error", MB_OK);
		return FALSE;
	}
	readUnitTextFile( weaponFile );
	fclose( unitFile );

	// create the counter data
	scenarioData = new AirCavScenarioData();
	int numberOfScenarios = scenarioData->initScenarios();

	// create the scenario data
	for ( int c=0; c<numberOfScenarios; c++ )
	{
		FILE *scenarioFile = openScenarioTextFile(file_dir, c);
		if( scenarioFile )
		{
			readScenarioTextFile( scenarioFile, c );
			fclose( scenarioFile );
		}
	}

	// ask for which scenario to play and select that one here
	int scenarioToPlay = 0;
	ScenarioDlg dlg;
	// set dialog text based on scenarios loaded
	for ( int c=0; c<numberOfScenarios; c++ )
		dlg.setScenarioTitle( c, scenarioData->getName(c) );

	CString hostIP;
	if (dlg.DoModal() == IDOK)
	{
		scenarioToPlay = dlg.getScenario();
		s_thisActiveSide = dlg.getSide();
		hostIP = dlg.getHostIP();
	}

	m_maxCounters = scenarioData->getScenario( scenarioToPlay, counterDataList, unitDataList );
	SetWindowText((LPCTSTR)scenarioData->getName( scenarioToPlay ));

	// initialization of available counters
	m_ActiveUnit = m_originalActiveUnit = -1;
	for ( int c=0; c<m_maxCounters; c++ )
		m_AvailableUnitsListBox.AddString(CString(counterDataList[c]->getName()));

	// create the map data
	mapData = new AirCavMapData();
	if ( !mapData->OpenMapDataFile(file_dir) )
	{
		MessageBox( (CString)"Unable to open map.dat", (CString)"Error", MB_OK );
		return FALSE;
	}

	// identify dead units and set wreck values on map
	for (int c = 0; c < m_maxCounters; c++)
	{
		if (counterDataList[c]->getIsAlive() == false)
		{
			// no wrecks for infantry, just dead bodies which don't block traffic
			if ( !counterDataList[c]->getUnitInfo()->isInfantry() )
			{
				int hexColumn = counterDataList[c]->getHexCol();
				int hexRow = counterDataList[c]->getHexRow();
				mapData->setWreck(hexRow, hexColumn);
				sendMapInfo(hexRow, hexColumn);
			}
		}
	}

	// start networking if necessary
	// - BLUE side is server (host IP)
	// - RED side is client (connects to host IP)
	if (s_thisActiveSide != BOTH)
	{
		CT2CA hostipConvertedAnsiString(hostIP);
		std::string hostipStr(hostipConvertedAnsiString);
		
		s_hostIP = hostipStr;
		s_networkActiveSide = SIDE_BLUE;
		s_networkCommand = 0;

		CMyObject *pNewObject = new CMyObject;
		AfxBeginThread(MyThreadProc, pNewObject);

		SetTimer(1, 500, TimerProc);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAirCavLOSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAirCavLOSDlg::OnOK()
{
	int ret = MessageBox( (CString)"Are you sure you want to quit?", (CString)"Quit", MB_YESNO );
	if ( ret == IDYES )
	{
		// close the map file and delete the map data
		if ( mapData )
		{
			if ( mapData->getMapDataEdited() )
			{
				int ret = MessageBox((CString)"The map has been edited. Do you want to save the changes?", (CString)"Update Map", MB_YESNO);
				if (ret == IDYES)
				{
					char msgbox[128];
					mapData->SaveAndCloseMapDataFile(msgbox);
					MessageBox((CString)msgbox, (CString)"Map Progress", MB_OK);
				}
				else
				{
					mapData->CloseMapDataFile();
				}
			}
			else
			{
				mapData->CloseMapDataFile();
			}
			delete mapData;
			mapData = NULL;
		}

		CDialog::OnOK();
	}
}

void CAirCavLOSDlg::OnCloseDialog()
{
	if ( ! m_shutdown )
	{
		// delete the weapon data
		for ( int w=0; w<MAXWEAP; w++ )
		{
			if ( weaponDataList[w] )
				delete weaponDataList[w];
		}

		// delete the unit data
		for ( int t=0; t<MAXTYPS; t++ )
		{
			if ( unitDataList[t] )
				delete unitDataList[t];
		}

		// delete the counter data
		for ( int c=0; c<m_maxCounters; c++ )
		{
			if ( counterDataList[c] )
				delete counterDataList[c];
		}

		if ( scenarioData )
			delete scenarioData;

		if ( mapData )
		{
			mapData->CloseMapDataFile();
			delete mapData;
		}
		m_shutdown = true;
	}
}

CAirCavLOSDlg::~CAirCavLOSDlg()
{
	OnCloseDialog();
	//KillTimer(1);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAirCavLOSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAirCavLOSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAirCavLOSDlg::OnEnChangeEditActiveName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CAirCavLOSDlg::OnCbnSelchangeComboActiveType()
{
	// TODO: Add your control notification handler code here
}

void CAirCavLOSDlg::OnCbnSelchangeComboActiveTt()
{
	// TODO: Add your control notification handler code here
}

void CAirCavLOSDlg::OnEnChangeEditActiveOp()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CAirCavLOSDlg::OnBnClickedRadioActiveMain1()
{
	UpdateData(TRUE);
}

void CAirCavLOSDlg::OnBnClickedRadioActiveMain2()
{
	UpdateData(TRUE);
}

void CAirCavLOSDlg::OnBnClickedRadioActiveMain3()
{
	UpdateData(TRUE);
}

void CAirCavLOSDlg::OnBnClickedRadioActiveSecondary1()
{
	UpdateData(TRUE);
}

void CAirCavLOSDlg::OnBnClickedRadioActiveSecondary2()
{
	UpdateData(TRUE);
}

void CAirCavLOSDlg::OnCbnSelchangeComboActiveMain()
{
	// TODO: Add your control notification handler code here
}

void CAirCavLOSDlg::OnCbnSelchangeComboActiveSecondary()
{
	// TODO: Add your control notification handler code here
}

void CAirCavLOSDlg::OnBnClickedButtonActionPopUp()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	// pre-perform Popup to get other units opportunity to fire
	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);

		// only for Attack Helicopters
		if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isAttackHelicopter() )
		{
			if ( counterDataList[m_ActiveUnit]->getHeloOffset() > NAP_OF_EARTH_METERS)
			{
				counterDataList[m_ActiveUnit]->actionHeloOffset(mapData, counterDataList, NAP_OF_EARTH_METERS, true);
				counterDataList[m_ActiveUnit]->setIsPopUp(NOT_IN_POPUP);
			}
			else
			{
				counterDataList[m_ActiveUnit]->actionHeloOffset(mapData, counterDataList, LOW_LEVEL_METERS, true);
				counterDataList[m_ActiveUnit]->setIsPopUp(POPUP_NOT_FIRED);
			}
			counterDataList[m_ActiveUnit]->setActionTaken(true);
			updateActiveUnit();
		}
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionFireGun()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	int wpnType, FKN, FKNpercent, SUP, SUPpercent;
	bool popup = false;
	bool skylined = false;
	CString strSightedUnit;
	CString popupStr("[POPUP]");
	AirCavWeaponData *wpn;

	UpdateData(TRUE);

	// get the unit that the active unit has sighted
	int indexSelectedUnit = m_SightedUnitsListBox.GetCurSel();
	if (indexSelectedUnit < 0 )
	{
		CString msgstr = (CString)"No sighted unit has been selected to fire upon";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	// get the string, if this is a popup set the flag
	m_SightedUnitsListBox.GetText(indexSelectedUnit, strSightedUnit);
	if ( counterDataList[m_ActiveUnit]->getIsPopUp() )
		popup = true;

	// get the target unit
	int tgt = counterDataList[m_ActiveUnit]->getTgtOppFiring();
	if ( tgt < 0 )
		return;

	int nAmmoRemaining = 0;
	switch( m_activeUnitWeapon )
	{
		case 0:			// main weapon 1
			wpn = counterDataList[m_ActiveUnit]->getUnitInfo()->getMainWeapon1();
			wpnType = wpn->getType();
			nAmmoRemaining = counterDataList[m_ActiveUnit]->getAmmoMainWeapon1();
			break;
		case 1:			// main weapon 2
			wpn = counterDataList[m_ActiveUnit]->getUnitInfo()->getMainWeapon2();
			wpnType = wpn->getType();
			nAmmoRemaining = counterDataList[m_ActiveUnit]->getAmmoMainWeapon2();
			break;
		case 2:			// main weapon 3
			wpn = counterDataList[m_ActiveUnit]->getUnitInfo()->getMainWeapon3();
			wpnType = wpn->getType();
			nAmmoRemaining = counterDataList[m_ActiveUnit]->getAmmoMainWeapon3();
			break;
		case 3:			// secondary weapon 1
			wpn = counterDataList[m_ActiveUnit]->getUnitInfo()->getSecondaryWeapon1();
			wpnType = wpn->getType();
			nAmmoRemaining = counterDataList[m_ActiveUnit]->getAmmoSecondaryWeapon1();
			break;
		case 4:			// secondary weapon 2
			wpn = counterDataList[m_ActiveUnit]->getUnitInfo()->getSecondaryWeapon2();
			wpnType = wpn->getType();
			nAmmoRemaining = counterDataList[m_ActiveUnit]->getAmmoSecondaryWeapon2();
			break;
		default:
			return;
	}

	// is helicopter firing
	bool isHelicopterFiring = counterDataList[m_ActiveUnit]->getUnitInfo()->isHelicopter();

	// is helicopter at nap of earth
	bool isNapOfEarth = (counterDataList[m_ActiveUnit]->getHeloOffset() == NAP_OF_EARTH_METERS);

	// get target type modifier for guns and rockets only and only if not firing on helicopter (helicopters cannot be suppressed)
	int targetTypeModifier = 0;
	bool nonHelicopterTarget = !counterDataList[tgt]->getUnitInfo()->isHelicopter();
	if (nonHelicopterTarget)
	{
		switch (m_activeUnitWeapon)
		{
		case 0:			// main weapon 1
			targetTypeModifier = counterDataList[m_ActiveUnit]->getTTModm1();
			break;
		case 1:			// main weapon 2
			targetTypeModifier = counterDataList[m_ActiveUnit]->getTTModm2();
			break;
		case 2:			// main weapon 3
			targetTypeModifier = counterDataList[m_ActiveUnit]->getTTModm3();
			break;
		case 3:			// secondary weapon 1
			targetTypeModifier = counterDataList[m_ActiveUnit]->getTTMods1();
			break;
		case 4:			// secondary weapon 2
			targetTypeModifier = counterDataList[m_ActiveUnit]->getTTMods2();
			break;
		}
	}

	// get the target unit's location
	int tgtCol = counterDataList[tgt]->getHexCol();
	int tgtRow = counterDataList[tgt]->getHexRow();

	// determine if sighting unit is using Thermal Imaging (to see through smoke)
	bool usingTI = counterDataList[m_ActiveUnit]->getOpticsInUse() == OPTICS_THERMAL_IMAGER;

	// make sure there is a line of sight for helicopters - it might need to do a popup first
	if (isHelicopterFiring && isNapOfEarth)
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();

		int activeUnitOffset = counterDataList[m_ActiveUnit]->getElevOffset();
		int activeUnitHeloOffset = counterDataList[m_ActiveUnit]->getHeloOffset();
		activeUnitOffset += activeUnitHeloOffset;

		int targetUnitOffset = counterDataList[tgt]->getElevOffset();
		int targetUnitHeloOffset = counterDataList[tgt]->getHeloOffset();
		targetUnitOffset += targetUnitHeloOffset;

		char buffer[MAX_BUF_SIZE];
		int range = mapData->CalculateLOS(activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, usingTI,
			tgtRow, tgtCol, targetUnitOffset, skylined, false, buffer);

		if (range == 0)
		{
			// check if this is a laser designated weapon and the target is being painted with a laser by some unit (doesn't matter which)
			if (counterDataList[m_ActiveUnit]->getUnitInfo()->isWeaponLaserGuided(m_activeUnitWeapon) && counterDataList[tgt]->getLaserDesignatingUnit() >= 0)
			{
				// need to check if smoke hex or more than one blocking hex along LOS - this will prevent laser designated targeting
				int laserRange = mapData->CalculateLOS(activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, false,
					tgtRow, tgtCol, targetUnitOffset, skylined, true, buffer);
				if (laserRange == 0)
				{
					CString msgstr = (CString)"Path to laser designated target is blocked by terrain or smoke";
					MessageBox((LPCTSTR)msgstr);
					return;
				}
			}
			else
			{
				CString msgstr = (CString)"Active unit cannot fire on target without first doing a popup!";
				MessageBox((LPCTSTR)msgstr);
				return;
			}
		}
	}

	// firing rocket from nap-of-earth helicopter is at half range
	if ( isHelicopterFiring && isNapOfEarth && wpnType == ROCKET )
	{
		// get the active unit's location
		int unitCol = counterDataList[m_ActiveUnit]->getHexCol();
		int unitRow = counterDataList[m_ActiveUnit]->getHexRow();

		char buffer[MAX_BUF_SIZE];
		int activeUnitOffset = counterDataList[m_ActiveUnit]->getElevOffset();
		int targetUnitOffset = counterDataList[tgt]->getElevOffset();
		int rocketRange = mapData->CalculateLOS( unitRow, unitCol, targetUnitOffset, usingTI, tgtRow, tgtCol, activeUnitOffset, skylined, false, buffer );
		int wpnAdjustedMaxRange = wpn->getMaxRange() / 2;
		if ( rocketRange > wpnAdjustedMaxRange )
			return;
	}

	// loop through all of the counters, finding all those in this hex (rockets only)
	std::vector<int>	tgtUnitsList;
	tgtUnitsList.push_back( tgt );

	if ( wpnType == ROCKET )
	{
		for ( int c=0; c<m_maxCounters; c++ )
		{
			// ignore the original target
			if ( c != tgt )
			{
				int hexColumn = counterDataList[c]->getHexCol();
				int hexRow = counterDataList[c]->getHexRow();
				if ( tgtCol == hexColumn && tgtRow == hexRow )
				{
					// ignore unit if it is carried by another and not dismounted
					bool isCarried = ( counterDataList[c]->getIsCarriedBy() != -1 );
					bool isDismounted = ( counterDataList[c]->getIsDismounted() == 1 );
					if ( !isCarried || ( isCarried && isDismounted ) )
					{
						tgtUnitsList.push_back( c );
					}
				}
			}
		}
	}

	// cannot fire SAMs at nap-of-earth helicopters, only low level
	if ( wpnType == SAM )
	{
		if ( counterDataList[tgt]->getUnitInfo()->isHelicopter() )
		{
			if ( counterDataList[tgt]->getHeloOffset() == NAP_OF_EARTH_METERS)
			{
				CString msgstr = (CString)"Cannot fire SAM at a nap-of-earth helicopter!";
				MessageBox((LPCTSTR)msgstr);
				return;
			}
		}
	}

	// rockets cannot be fire during a popup!
	if (popup && wpnType == ROCKET)
	{
		CString msgstr = (CString)"Rockets cannot be fire during a popup!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	// figure out if there enough OPs and enough ammo to fire the selected weapon
	int succeeded = 0;

	// spend the OPs for the popup if not already in a popup
	if (popup && counterDataList[m_ActiveUnit]->getIsPopUp() == POPUP_NOT_FIRED)
	{
		succeeded = counterDataList[m_ActiveUnit]->popupFire();
		if (succeeded)
		{
			counterDataList[m_ActiveUnit]->setIsPopUp(POPUP_HAS_FIRED);
		}
		else
		{
			CString msgstr = (CString)"Not enough OPs to do a Popup attack!";
			MessageBox((LPCTSTR)msgstr);
			return;
		}
	}

	// spend the OPs for the fired weapon
	bool firingMissile = (wpnType == ATGM || wpnType == SAM || wpnType == ROCKET);
	if (firingMissile)
		succeeded = counterDataList[m_ActiveUnit]->fireMissile();
	else
		succeeded = counterDataList[m_ActiveUnit]->fireGun();

	// expend the ammunition in the case of SAMS, Rockets and ATGMs
	if ( firingMissile )
	{
		if (nAmmoRemaining > 0 )
		{
			if (succeeded)
			{
				switch (m_activeUnitWeapon)
				{
				case 0:			// main weapon 1
					counterDataList[m_ActiveUnit]->setAmmoMainWeapon1(--nAmmoRemaining);
					break;
				case 1:			// main weapon 2
					counterDataList[m_ActiveUnit]->setAmmoMainWeapon2(--nAmmoRemaining);
					break;
				case 2:			// main weapon 3
					counterDataList[m_ActiveUnit]->setAmmoMainWeapon3(--nAmmoRemaining);
					break;
				case 3:			// secondary weapon 1
					counterDataList[m_ActiveUnit]->setAmmoSecondaryWeapon1(--nAmmoRemaining);
					break;
				case 4:			// secondary weapon 2
					counterDataList[m_ActiveUnit]->setAmmoSecondaryWeapon2(--nAmmoRemaining);
					break;
				}
			}
		}
		else
		{
			CString msgstr = (CString)"The selected weapon is out of ammo!";
			MessageBox((LPCTSTR)msgstr);
			return;
		}
	}
	if ( succeeded == 0 )
	{
		CString msgstr = (CString)"Not enough OPs to fire "; msgstr += wpn->getName();
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	// fire at all units in this hex
	size_t listSize = tgtUnitsList.size();
	for ( size_t u=0; u<listSize; u++ )
	{
		int t = tgtUnitsList[u];

		// cannot fire Missiles on Infantry
		if ( counterDataList[t]->getUnitInfo()->isInfantry() && wpnType == ATGM )
		{
			CString msgstr = (CString)"Cannot fire missiles on infantry!";
			MessageBox((LPCTSTR)msgstr);
			continue;
		}

		// allow the target unit to Evade if it has the OPs
		int EVMmod = 0;
		if ( !counterDataList[t]->getEvading() && counterDataList[t]->enoughOPsForEvasiveManeuver() )
		{
			bool cannotEvade = ((counterDataList[t]->getUnitInfo()->isHelicopter() || counterDataList[t]->getUnitInfo()->isVehicle()) && wpnType == ROCKET);
			if (!cannotEvade)
			{
				CString tgtUnitName = counterDataList[t]->getName();

				char tName[32];
				int length = tgtUnitName.GetLength() + 1;
				for (int i = 0; i < length; i++)
					tName[i] = tgtUnitName.GetAt(i);

				// display the information
				char buffer1[MAX_BUF_SIZE];
				sprintf_s(buffer1, "Is the Unit \"%s\" going to Evade?", tName);
				if (MessageBox((CString)buffer1, _T("Evasive Maneuver"), MB_YESNO) == IDYES)
				{
					EVMmod = counterDataList[t]->evasiveManeuver(FALSE);
					if (EVMmod) sendUnitInfo(t);
				}
			}
		}

		// calculate initial final kill number based on the weapon
		switch( m_activeUnitWeapon )
		{
			case 0:			// main weapon 1
				FKN = counterDataList[m_ActiveUnit]->getFKN1() + EVMmod;
				break;
			case 1:			// main weapon 2
				FKN = counterDataList[m_ActiveUnit]->getFKN2() + EVMmod;
				break;
			case 2:			// main weapon 3
				FKN = counterDataList[m_ActiveUnit]->getFKN3() + EVMmod;
				break;
			case 3:			// secondary weapon 1
				FKN = counterDataList[m_ActiveUnit]->getFKN4() + EVMmod;
				break;
			case 4:			// secondary weapon 2
				FKN = counterDataList[m_ActiveUnit]->getFKN5() + EVMmod;
				break;
			default:
				return;
		}

		// Helicopter firing rockets at nap-of-earth are minus one
		if ( wpnType == ROCKET && isHelicopterFiring && isNapOfEarth )
			FKN--;

		// keep within 0 to 10
		if ( FKN < 0 ) FKN = 0;
		if ( FKN > 10 ) FKN = 10;

		// calculate final kill number, fudge it a little bit for randomness
		FKNpercent = FKN * 10;
		FKNpercent += ( rand() % 10 ) - 5.0;
		if ( FKNpercent < 0 ) FKNpercent = 0;
		if ( FKNpercent > 100 ) FKNpercent = 100;

		// suppression is FKN minus the target type modifier
		SUP = FKN - targetTypeModifier;

		// calculate suppression value
		SUPpercent = SUP * 10;
		SUPpercent += (rand() % 10) - 5.0;
		if (SUPpercent < 0) SUPpercent = 0;
		if (SUPpercent > 100) SUPpercent = 100;

		CString wpnName = wpn->getName();
		if ( wpnName == "None" )
			return;

		bool weaponCanSuppress = (wpnType != ATGM && wpnType != SAM);

		char sName[32], sType[32], tName[32], tType[32];
		int length;
		CString sightingUnitName = counterDataList[m_ActiveUnit]->getName();
		CString sightingUnitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getName();

		length = sightingUnitName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			sName[i] = sightingUnitName.GetAt(i);						
		length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			sType[i] = wpnName.GetAt(i);

		// get the target information
		CString tgtUnitName = counterDataList[t]->getName();
		CString tgtUnitType = counterDataList[t]->getUnitInfo()->getName();

		length = tgtUnitName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			tName[i] = tgtUnitName.GetAt(i);						
		length = tgtUnitType.GetLength() + 1;
		for (int i = 0; i < length; i++)
			tType[i] = tgtUnitType.GetAt(i);

		// display the information
		char buffer1[MAX_BUF_SIZE];
		sprintf_s( buffer1, "%s [%s] -> %s [%s]", sName, sType, tName, tType );
		char buffer2[MAX_BUF_SIZE];
		int result = IDCANCEL;
		if (nonHelicopterTarget && weaponCanSuppress)
		{
			sprintf_s(buffer2, "KILL: %d [%d%%]   SUP: %d [%d%%]", FKN, FKNpercent, SUP, SUPpercent);
			KillSuppressDialog dlg;
			dlg.setFKNText1((CString)buffer1);
			dlg.setFKNText2((CString)buffer2);
			result = dlg.DoModal();
		}
		else
		{
			sprintf_s(buffer2, "KILL: %d [%d%%]", FKN, FKNpercent);
			KillDialog dlg;
			dlg.setFKNText1((CString)buffer1);
			dlg.setFKNText2((CString)buffer2);
			result = dlg.DoModal();
		}

		if (result == IDOK2)
		{
			// suppress this unit
			counterDataList[t]->setIsSuppressed(TRUE);

			// infantry will go into defilade if suppressed
			if ( counterDataList[t]->getUnitInfo()->isInfantry() )
				counterDataList[t]->setDefilade(TRUE);

			sendUnitInfo(t);
		}
		else if (result == IDOK)
		{
			// kill this unit
			counterDataList[t]->kill();
			sendUnitInfo(t);

			// kill any mounted units as well
			int numMountedUnits = counterDataList[t]->getNumberOfMountedUnits();
			for (int i=0; i<numMountedUnits; i++ )
			{
				int id = counterDataList[t]->getMountedUnit(i);
				if (!counterDataList[id]->getIsDismounted())
				{
					counterDataList[id]->kill();
					sendUnitInfo(id);
				}
			}

			// if the target was a vehicle or helicopter, add a wreck marker to the target hex
			if ( counterDataList[t]->getUnitInfo()->getUnitType() != INF )
			{
				m_wreck = mapData->setWreck(tgtRow, tgtCol);
				sendMapInfo(tgtRow, tgtCol);
			}
		}

		// set Evasive Maneuver to false
		counterDataList[t]->setEvading(0);

		updateActiveUnit();
	}
}

void CAirCavLOSDlg::moveMountedUnits()
{
	int numMountedUnits = counterDataList[m_ActiveUnit]->getNumberOfMountedUnits();
	for (int i=0; i<numMountedUnits; i++ )
	{
		int id = counterDataList[m_ActiveUnit]->getMountedUnit(i);
		if ( !counterDataList[id]->getIsDismounted() )
		{
			int col = counterDataList[m_ActiveUnit]->getHexCol();
			int row = counterDataList[m_ActiveUnit]->getHexRow();
			counterDataList[id]->moveTo(mapData, counterDataList, col, row, false);
		}
	}
}

void CAirCavLOSDlg::resetLaserDesignation()
{
	// if this unit was painted with a laser designator, remove it
	// for example, if the unit moved the designating unit will need to re-acquire its target
	int unit = counterDataList[m_ActiveUnit]->getLaserDesignatingUnit();
	if ( unit >= 0)
	{
		counterDataList[m_ActiveUnit]->setLaserDesignatingUnit(-1);
		counterDataList[unit]->setLaserDesignatedUnit(-1);
	}

	// if this unit was painting another unit with a laser designator, remove it
	unit = counterDataList[m_ActiveUnit]->getLaserDesignatedUnit();
	if (unit >= 0)
	{
		counterDataList[m_ActiveUnit]->setLaserDesignatedUnit(-1);
		counterDataList[unit]->setLaserDesignatingUnit(-1);
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionMoveN()
{
	if (m_ActiveUnit < 0) return;
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isInfantry() && counterDataList[m_ActiveUnit]->getIsSuppressed() )
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	int previousOPs = counterDataList[m_ActiveUnit]->getOPs();
	int previousRow = counterDataList[m_ActiveUnit]->getHexRow();
	int previousColumn = counterDataList[m_ActiveUnit]->getHexCol();
	counterDataList[m_ActiveUnit]->moveNorth(mapData, counterDataList, m_popSmokeWhileMoving, m_TimeOfDay, m_Weather);
	doUnitTracking(previousOPs, previousRow, previousColumn);
	moveMountedUnits();
	resetLaserDesignation();
	updateActiveUnit();
	if ( m_popSmokeWhileMoving )
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		m_vehicleSmokeHexList.Add( activeUnitHexRow, activeUnitHexColumn );
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionMoveNw()
{
	if (m_ActiveUnit < 0) return;
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isInfantry() && counterDataList[m_ActiveUnit]->getIsSuppressed() )
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	int previousOPs = counterDataList[m_ActiveUnit]->getOPs();
	int previousRow = counterDataList[m_ActiveUnit]->getHexRow();
	int previousColumn = counterDataList[m_ActiveUnit]->getHexCol();
	counterDataList[m_ActiveUnit]->moveNorthWest(mapData, counterDataList, m_popSmokeWhileMoving, m_TimeOfDay, m_Weather);
	doUnitTracking(previousOPs, previousRow, previousColumn);
	moveMountedUnits();
	resetLaserDesignation();
	updateActiveUnit();
	if ( m_popSmokeWhileMoving )
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		m_vehicleSmokeHexList.Add( activeUnitHexRow, activeUnitHexColumn );
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionMoveSw()
{
	if (m_ActiveUnit < 0) return;
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isInfantry() && counterDataList[m_ActiveUnit]->getIsSuppressed() )
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	int previousOPs = counterDataList[m_ActiveUnit]->getOPs();
	int previousRow = counterDataList[m_ActiveUnit]->getHexRow();
	int previousColumn = counterDataList[m_ActiveUnit]->getHexCol();
	counterDataList[m_ActiveUnit]->moveSouthWest(mapData, counterDataList, m_popSmokeWhileMoving, m_TimeOfDay, m_Weather);
	doUnitTracking(previousOPs, previousRow, previousColumn);
	moveMountedUnits();
	resetLaserDesignation();
	updateActiveUnit();
	if ( m_popSmokeWhileMoving )
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		m_vehicleSmokeHexList.Add( activeUnitHexRow, activeUnitHexColumn );
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionMoveS()
{
	if (m_ActiveUnit < 0) return;
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isInfantry() && counterDataList[m_ActiveUnit]->getIsSuppressed() )
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	int previousOPs = counterDataList[m_ActiveUnit]->getOPs();
	int previousRow = counterDataList[m_ActiveUnit]->getHexRow();
	int previousColumn = counterDataList[m_ActiveUnit]->getHexCol();
	counterDataList[m_ActiveUnit]->moveSouth(mapData, counterDataList, m_popSmokeWhileMoving, m_TimeOfDay, m_Weather);
	doUnitTracking(previousOPs, previousRow, previousColumn);
	moveMountedUnits();
	resetLaserDesignation();
	updateActiveUnit();
	if ( m_popSmokeWhileMoving )
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		m_vehicleSmokeHexList.Add( activeUnitHexRow, activeUnitHexColumn );
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionMoveSe()
{
	if (m_ActiveUnit < 0) return;
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isInfantry() && counterDataList[m_ActiveUnit]->getIsSuppressed() )
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	int previousOPs = counterDataList[m_ActiveUnit]->getOPs();
	int previousRow = counterDataList[m_ActiveUnit]->getHexRow();
	int previousColumn = counterDataList[m_ActiveUnit]->getHexCol();
	counterDataList[m_ActiveUnit]->moveSouthEast(mapData, counterDataList, m_popSmokeWhileMoving, m_TimeOfDay, m_Weather);
	doUnitTracking(previousOPs, previousRow, previousColumn);
	moveMountedUnits();
	resetLaserDesignation();
	updateActiveUnit();
	if ( m_popSmokeWhileMoving )
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		m_vehicleSmokeHexList.Add( activeUnitHexRow, activeUnitHexColumn );
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionMoveNe()
{
	if (m_ActiveUnit < 0) return;
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isInfantry() && counterDataList[m_ActiveUnit]->getIsSuppressed() )
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	int previousOPs = counterDataList[m_ActiveUnit]->getOPs();
	int previousRow = counterDataList[m_ActiveUnit]->getHexRow();
	int previousColumn = counterDataList[m_ActiveUnit]->getHexCol();
	counterDataList[m_ActiveUnit]->moveNorthEast(mapData, counterDataList, m_popSmokeWhileMoving, m_TimeOfDay, m_Weather);
	doUnitTracking(previousOPs, previousRow, previousColumn);
	moveMountedUnits();
	resetLaserDesignation();
	updateActiveUnit();
	if ( m_popSmokeWhileMoving )
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		m_vehicleSmokeHexList.Add( activeUnitHexRow, activeUnitHexColumn );
	}
}

void CAirCavLOSDlg::OnLbnSelchangeListSightedUnits()
{
	UpdateData(FALSE);
	int indexSelectedUnit = m_SightedUnitsListBox.GetCurSel();
	if (indexSelectedUnit < 0 )
	{
		CString msgstr = (CString)"No sighted unit has been selected to fire upon";
		MessageBox((LPCTSTR)msgstr);
		return;
	}

	for ( int c=0; c<m_maxCounters; c++ )
	{
		CString thisUnitsName;
		char actualThisName[60], actualActiveName[60];
		m_SightedUnitsListBox.GetText(indexSelectedUnit, thisUnitsName);
		int rightBracket = thisUnitsName.Find(')');
		int j = 0;
		for ( int i=0; i<=rightBracket; i++ )
		{
			actualThisName[j] = thisUnitsName.GetAt(i);
			j++;
		}
		actualThisName[j] = '\0';
		CString thisUnitsActualName = (CString)actualThisName;

		CString activeUnitsName = counterDataList[c]->getName();
		rightBracket = activeUnitsName.Find(')');
		j = 0;
		for ( int i=0; i<=rightBracket; i++ )
		{
			actualActiveName[j] = activeUnitsName.GetAt(i);
			j++;
		}
		actualActiveName[j] = '\0';
		CString activeUnitsActualName = (CString)actualActiveName;

		if ( thisUnitsActualName == activeUnitsActualName )
		{
			counterDataList[m_ActiveUnit]->setTgtOppFiring(c);
			m_ActiveTarget = c;
			updateActiveUnit(FALSE);
			return;
		}
	}
}

void CAirCavLOSDlg::OnLbnSelchangeListUnits()
{
	OnBnClickedButtonMakeActive();
}

void CAirCavLOSDlg::OnEnChangeEditCurWeaponName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CAirCavLOSDlg::OnBnClickedButtonMakeActive()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide)
		return;

	int indexSelectedUnit = m_AvailableUnitsListBox.GetCurSel();
	if (indexSelectedUnit < 0 )
		return;

	// find the active unit based on its name
	for ( int c=0; c<m_maxCounters; c++ )
	{
		CString thisUnitsName = counterDataList[c]->getName();
		CString activeUnitsName = counterDataList[indexSelectedUnit]->getName();
		if ( thisUnitsName == activeUnitsName /* && counterDataList[c]->getIsAlive() == ALIVE */ )
		{
			if (s_thisActiveSide != BOTH)
			{
				// if networked, don't allow selection of unit from the other side
				SideType thisSide = counterDataList[c]->getSideType();
				if (thisSide != s_thisActiveSide)
				{
					m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
					return;
				}
				//if (m_ActiveUnit != c)
				{
					char buffer[MAX_BUF_SIZE];
					sprintf_s(buffer, "select %d", c);
					if (s_thisActiveSide == SIDE_BLUE)
						serverSend(buffer);
					else
						clientSend(buffer);
				}
			}

			m_ActiveUnit = c;
			break;
		}
	}

	// reset a couple of things
	m_activeUnitWeapon = 0;
	m_popSmokeWhileMoving = false;
	m_unitTracking.activeUnitChanged = true;

	// disable pop smoke button for Helicopters
	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isHelicopter() )
		GetDlgItem(IDC_CHECK_POP_SMOKE)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CHECK_POP_SMOKE)->EnableWindow(TRUE);

	// update the dialog
	UpdateData(FALSE);
	updateActiveUnit(TRUE, TRUE);
}

void CAirCavLOSDlg::OnBnClickedButtonResetActive()
{
	for ( int c=0; c<m_maxCounters; c++ )
	{
		counterDataList[c]->resetActive();
	}
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedButtonActionOppfire()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	int indexSelectedUnit = m_SightingUnitsListBox.GetCurSel();
	if (indexSelectedUnit < 0 )
		return;
	for ( int c=0; c<m_maxCounters; c++ )
	{
		CString thisUnitsName;
		char actualThisName[60], actualActiveName[60];
		m_SightingUnitsListBox.GetText(indexSelectedUnit, thisUnitsName);
		int rightBracket = thisUnitsName.Find(')');
		int j = 0;
		for ( int i=0; i<rightBracket-4; i++ )
		{
			actualThisName[j] = thisUnitsName.GetAt(i);
			j++;
		}
		actualThisName[j] = '\0';
		CString thisUnitsActualName = (CString)actualThisName;

		CString activeUnitsName = counterDataList[c]->getName();
		rightBracket = activeUnitsName.Find(')');
		j = 0;
		for ( int i=0; i<rightBracket-4; i++ )
		{
			actualActiveName[j] = activeUnitsName.GetAt(i);
			j++;
		}
		actualActiveName[j] = '\0';
		CString activeUnitsActualName = (CString)actualActiveName;

		// watch out for friendly fire!
		SideType activeSide = counterDataList[m_ActiveUnit]->getSideType();
		SideType thisSide = counterDataList[c]->getSideType();

		// found a target
		if (thisUnitsActualName == activeUnitsActualName && activeSide != thisSide)
		{
			// use popup to figure out which weapon is fired
			AirCavWeaponData *wpn1, *wpn2, *wpn3, *wpn4, *wpn5;
			int wpn1Type, wpn2Type, wpn3Type, wpn4Type, wpn5Type;
			wpn1 = counterDataList[c]->getUnitInfo()->getMainWeapon1();
			wpn2 = counterDataList[c]->getUnitInfo()->getMainWeapon2();
			wpn3 = counterDataList[c]->getUnitInfo()->getMainWeapon3();
			wpn4 = counterDataList[c]->getUnitInfo()->getSecondaryWeapon1();
			wpn5 = counterDataList[c]->getUnitInfo()->getSecondaryWeapon2();
			wpn1Type = wpn1->getType();
			wpn2Type = wpn2->getType();
			wpn3Type = wpn3->getType();
			wpn4Type = wpn4->getType();
			wpn5Type = wpn5->getType();
			int wpn1Ammo = counterDataList[c]->getAmmoMainWeapon1();
			int wpn2Ammo = counterDataList[c]->getAmmoMainWeapon2();
			int wpn3Ammo = counterDataList[c]->getAmmoMainWeapon3();
			int wpn4Ammo = counterDataList[c]->getAmmoSecondaryWeapon1();
			int wpn5Ammo = counterDataList[c]->getAmmoSecondaryWeapon2();

			CString wpnName1 = wpn1->getName();
			if (wpn1Type == ATGM || wpn1Type == SAM || wpn1Type == ROCKET)
			{
				if (wpn1Ammo < 1)
					wpnName1 += " [OUT]";
				else
				{
					std::string ammoStr = " [" + std::to_string(wpn1Ammo) + "]";
					wpnName1 += CString(ammoStr.c_str());
				}
			}
			
			CString wpnName2 = wpn2->getName();
			if (wpn2Type == ATGM || wpn2Type == SAM || wpn2Type == ROCKET)
			{
				if (wpn2Ammo < 1)
					wpnName2 += " [OUT]";
				else
				{
					std::string ammoStr = " [" + std::to_string(wpn2Ammo) + "]";
					wpnName2 += CString(ammoStr.c_str());
				}
			}

			CString wpnName3 = wpn3->getName();
			if (wpn3Type == ATGM || wpn3Type == SAM || wpn3Type == ROCKET)
			{
				if (wpn3Ammo < 1)
					wpnName3 += " [OUT]";
				else
				{
					std::string ammoStr = " [" + std::to_string(wpn3Ammo) + "]";
					wpnName3 += CString(ammoStr.c_str());
				}
			}

			CString wpnName4 = wpn4->getName();
			if (wpn4Type == ATGM || wpn4Type == SAM || wpn4Type == ROCKET)
			{
				if (wpn4Ammo < 1)
					wpnName4 += " [OUT]";
				else
				{
					std::string ammoStr = " [" + std::to_string(wpn4Ammo) + "]";
					wpnName4 += CString(ammoStr.c_str());
				}
			}

			CString wpnName5 = wpn5->getName();
			if (wpn5Type == ATGM || wpn5Type == SAM || wpn5Type == ROCKET)
			{
				if (wpn5Ammo < 1)
					wpnName5 += " [OUT]";
				else
				{
					std::string ammoStr = " [" + std::to_string(wpn5Ammo) + "]";
					wpnName5 += CString(ammoStr.c_str());
				}
			}

			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			OppFireDialog dlg;
			dlg.SetWeaponText(wpnName1, wpnName2, wpnName3, wpnName4, wpnName5);
			if ( dlg.DoModal() == IDOK )
			{
				AirCavWeaponData *wpn;
				int wpnType;
				int weapon = dlg.getSelectedWeapon();
				switch( weapon )
				{
					case 0:			// main weapon 1
						wpn = wpn1;
						wpnType = wpn1Type;
						break;
					case 1:			// main weapon 2
						wpn = wpn2;
						wpnType = wpn2Type;
						break;
					case 2:			// main weapon 3
						wpn = wpn3;
						wpnType = wpn3Type;
						break;
					case 3:			// secondary weapon 1
						wpn = wpn4;
						wpnType = wpn4Type;
						break;
					case 4:			// secondary weapon 2
						wpn = wpn5;
						wpnType = wpn5Type;
						break;
					default:
						return;
				}

				// allow the active unit to Evade
				int EVMmod = 0;
				if ( !counterDataList[m_ActiveUnit]->getEvading() && counterDataList[m_ActiveUnit]->enoughOPsForEvasiveManeuver() )
				{
					bool cannotEvade = ((counterDataList[m_ActiveUnit]->getUnitInfo()->isHelicopter() || counterDataList[m_ActiveUnit]->getUnitInfo()->isVehicle()) && wpnType == ROCKET);
					if (!cannotEvade)
					{
						CString tgtUnitName = counterDataList[m_ActiveUnit]->getName();

						char tName[32];
						int length = tgtUnitName.GetLength() + 1;
						for (int i = 0; i < length; i++)
							tName[i] = tgtUnitName.GetAt(i);

						// display the information
						char buffer1[MAX_BUF_SIZE];
						sprintf_s(buffer1, "Is the Unit \"%s\" going to Evade?", tName);
						if (MessageBox((CString)buffer1, _T("Evasive Maneuver"), MB_YESNO) == IDYES)
						{
							EVMmod = counterDataList[m_ActiveUnit]->evasiveManeuver(FALSE);
							if (EVMmod) sendUnitInfo(m_ActiveUnit);
						}
					}
				}

				switch( weapon )
				{
					case 0:			// main weapon 1
						counterDataList[c]->setWpnOppFiring(0);
						counterDataList[c]->setFKN1( counterDataList[c]->getFKN1() + EVMmod );
						break;
					case 1:			// main weapon 2
						counterDataList[c]->setWpnOppFiring(1);
						counterDataList[c]->setFKN2( counterDataList[c]->getFKN2() + EVMmod );
						break;
					case 2:			// main weapon 3
						counterDataList[c]->setWpnOppFiring(2);
						counterDataList[c]->setFKN3( counterDataList[c]->getFKN3() + EVMmod );
						break;
					case 3:			// secondary weapon 1
						counterDataList[c]->setWpnOppFiring(3);
						counterDataList[c]->setFKN4( counterDataList[c]->getFKN4() + EVMmod );
						break;
					case 4:			// secondary weapon 2
						counterDataList[c]->setWpnOppFiring(4);
						counterDataList[c]->setFKN5( counterDataList[c]->getFKN5() + EVMmod );
						break;
					default:
						return;
				}

				CString wpnName = wpn->getName();
				if ( wpnName == "None" )
					break;

				bool popup = false;
				CString strSightedUnit;
				CString popupStr("[POPUP]");
				m_SightingUnitsListBox.GetText(indexSelectedUnit, strSightedUnit);
				if ( strSightedUnit.Find( popupStr ) != -1 )
					popup = true;

				int canOppFire = false;
				if ( popup )
				{
					OnBnClickedButtonActionPopUp();
					canOppFire = counterDataList[c]->popupFire(true);
				}
				else if ( wpnType == GUN )
					canOppFire = counterDataList[c]->oppFire(GUN);
				else if (wpnType == ROCKET)
					canOppFire = counterDataList[c]->oppFire(ROCKET);
				else if ( wpnType == ATGM || wpnType == SAM )
					canOppFire = counterDataList[c]->oppFire(ATGM);

				// has enough OPs to fire
				if ( canOppFire )
				{
					// new Active unit is the one that fired, target is the current Active Unit
					counterDataList[c]->setTgtOppFiring(m_ActiveUnit);
					
					// remember the original Active unit
					if (m_FiringUnitsList.empty())
						m_originalActiveUnit = m_ActiveUnit;

					// set new active unit, so other units can opp fire on this one
					m_ActiveUnit = c;
					m_FiringUnitsList.push_back(c);
				}
				else
				{
					CString msgstr = (CString)"Not enough OPs for Opportunity Fire!";
					MessageBox((LPCTSTR)msgstr);
				}
				break;
			}
		}
	}
	updateActiveUnit();
	updateFiringUnits();

	m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
}

void CAirCavLOSDlg::OnBnClickedButtonActionLaser()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	int indexSelectedUnit = m_SightedUnitsListBox.GetCurSel();
	if (indexSelectedUnit < 0)
		return;
	for (int c = 0; c < m_maxCounters; c++)
	{
		CString thisUnitsName;
		char actualThisName[60], actualActiveName[60];
		m_SightedUnitsListBox.GetText(indexSelectedUnit, thisUnitsName);
		int rightBracket = thisUnitsName.Find(')');
		int j = 0;
		for (int i = 0; i < rightBracket - 4; i++)
		{
			actualThisName[j] = thisUnitsName.GetAt(i);
			j++;
		}
		actualThisName[j] = '\0';
		CString thisUnitsActualName = (CString)actualThisName;

		CString activeUnitsName = counterDataList[c]->getName();
		rightBracket = activeUnitsName.Find(')');
		j = 0;
		for (int i = 0; i < rightBracket - 4; i++)
		{
			actualActiveName[j] = activeUnitsName.GetAt(i);
			j++;
		}
		actualActiveName[j] = '\0';
		CString activeUnitsActualName = (CString)actualActiveName;

		SideType activeSide = counterDataList[m_ActiveUnit]->getSideType();
		SideType thisSide = counterDataList[c]->getSideType();

		// found a target
		if (thisUnitsActualName == activeUnitsActualName && activeSide != thisSide)
		{
			// if the active unit was painting this target unit, it is no longer
			if (counterDataList[m_ActiveUnit]->getLaserDesignatedUnit() == c)
			{
				counterDataList[m_ActiveUnit]->setLaserDesignatedUnit(-1);
				counterDataList[c]->setLaserDesignatingUnit(-1);
			}
			else
			{
				// this unit can only laser designate the target if it has an actual line of sight (not via another unit)
				bool hasNormalLOS = true;
				for (auto s : popupSightedUnits)
				{
					if (s.first == c)
					{
						hasNormalLOS = false;
						break;
					}
				}
				if (hasNormalLOS)
				{
					// whatever unit is being painted by the active unit, is no longer
					int unit = counterDataList[m_ActiveUnit]->getLaserDesignatedUnit();
					if (unit >= 0)
					{
						counterDataList[unit]->setLaserDesignatingUnit(-1);
					}
					// whatever unit was painting this target unit, is no longer
					unit = counterDataList[c]->getLaserDesignatingUnit();
					if (unit >= 0)
					{
						counterDataList[unit]->setLaserDesignatedUnit(-1);
					}
					// now the active unit is painting this target unit
					counterDataList[m_ActiveUnit]->setLaserDesignatedUnit(c);
					counterDataList[c]->setLaserDesignatingUnit(m_ActiveUnit);
				}
				else
				{
					CString msgstr = (CString)"Active unit must have direct LOS to a unit to laser designate";
					MessageBox((LPCTSTR)msgstr);
				}
			}
		}
	}
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedButtonActionRadar()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if (counterDataList[m_ActiveUnit]->getUnitInfo()->hasRadar())
	{
		if (counterDataList[m_ActiveUnit]->getRadarInUse())
		{
			counterDataList[m_ActiveUnit]->setRadarInUse(false);
		}
		else
		{
			counterDataList[m_ActiveUnit]->setRadarInUse(true);
		}
	}
	m_activeUnitRadarOn = counterDataList[m_ActiveUnit]->getRadarInUse();
	updateActiveUnit();
}

void CAirCavLOSDlg::updateFiringUnits()
{
	m_FiringUnitsListBox.ResetContent();

	size_t listSize = m_FiringUnitsList.size();
	for ( size_t c=0; c<listSize; c++ )
	{
		AirCavWeaponData *wpnData;
		char sName[32], sType[32];
		int length;
		int firingUnit = m_FiringUnitsList[c];
		CString sightingUnitName = counterDataList[firingUnit]->getName();
		CString sightingUnitType = counterDataList[firingUnit]->getUnitInfo()->getName();
		int wpnOppFiring = counterDataList[firingUnit]->getWpnOppFiring();
		switch( wpnOppFiring )
		{
			case 0:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getMainWeapon1();
				break;
			case 1:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getMainWeapon2();
				break;
			case 2:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getMainWeapon3();
				break;
			case 3:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getSecondaryWeapon1();
				break;
			case 4:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getSecondaryWeapon2();
				break;
		}
		CString wpnName = wpnData->getName();

		length = sightingUnitName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			sName[i] = sightingUnitName.GetAt(i);						
		length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			sType[i] = wpnName.GetAt(i);						
		char buffer[MAX_BUF_SIZE];
		sprintf_s( buffer, "%s [%s]", sName, sType );

		m_FiringUnitsListBox.AddString(CString(buffer));
	}
}

void CAirCavLOSDlg::updateActiveUnit(bool rebuildList, bool noNetworkUpdate)
{
	LPCTSTR STR_NOE = _T("Nap Of Earth");
	LPCTSTR STR_LL = _T("Low Level");
	LPCTSTR STR_MOUNT = _T("Mount");
	LPCTSTR STR_DISMOUNT = _T("Dismount");
	LPCTSTR STR_POPUP = _T("Pop Up");
	LPCTSTR STR_POPDOWN = _T("Pop Down");

	if ( m_ActiveUnit >= 0 )
	{
		if (s_thisActiveSide != BOTH && s_thisActiveSide == s_networkActiveSide)
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);

		// Active Unit info
		char buffer[MAX_BUF_SIZE];
		m_activeUnitName = counterDataList[m_ActiveUnit]->getName();
		m_activeUnitMounted = counterDataList[m_ActiveUnit]->getIsCarriedBy();
		m_activeUnitDismounted = counterDataList[m_ActiveUnit]->getIsDismounted();

		if (counterDataList[m_ActiveUnit]->getIsAlive() == DEAD)
		{
			// unit is dead
			m_activeUnitName += " [DEAD]";
			counterDataList[m_ActiveUnit]->setOPs(0.0);
		}
		else if (counterDataList[m_ActiveUnit]->getActionTaken())
		{
			// unit has been active this turn
			m_activeUnitName += " [ACTION]";
		}
		if (m_activeUnitMounted && !m_activeUnitDismounted)
		{
			// unit is currently mounted
			m_activeUnitName += " [MOUNT]";
		}
		sprintf_s( buffer, "%2.1f", counterDataList[m_ActiveUnit]->getOPs() );
		m_activeUnitOPs = (CString)buffer;

		// specific Unit Info
		m_activeUnitTypeName = counterDataList[m_ActiveUnit]->getUnitInfo()->getName();
		m_activeUnitFired = counterDataList[m_ActiveUnit]->getFired();
		m_activeUnitMoved = counterDataList[m_ActiveUnit]->getMoved();
		m_activeUnitEvading = counterDataList[m_ActiveUnit]->getEvading();
		m_activeUnitInDefilade = counterDataList[m_ActiveUnit]->getDefilade();
		m_activeUnitLowLevel = ( counterDataList[m_ActiveUnit]->getHeloOffset() > NAP_OF_EARTH_METERS) ? 1 : 0;
		m_activeUnitIsSuppressed = counterDataList[m_ActiveUnit]->getIsSuppressed();

		// reset the optics combo box (only want to display this unit's supported optics in the pulldown)
		m_currentOpticsComboBox.ResetContent();
		for (int o=0; o < m_currentOpticsComboBox.GetCount(); o++)
			m_currentOpticsComboBox.DeleteString(o);

		// populate the pulldown (OS is always 0, others will follow if supported)
		int OpticsIndex = 1; int TIindex = 0; int ALEindex = 0; int IRSLindex = 0; int WLSLindex = 0;
		m_currentOpticsComboBox.AddString(_T("OS"));
		if (counterDataList[m_ActiveUnit]->getUnitInfo()->hasTIsight())
		{
			TIindex = OpticsIndex++;
			m_currentOpticsComboBox.AddString(_T("TI"));
		}
		if (counterDataList[m_ActiveUnit]->getUnitInfo()->hasALEsight())
		{
			ALEindex = OpticsIndex++;
			m_currentOpticsComboBox.AddString(_T("ALE"));
		}
		if (counterDataList[m_ActiveUnit]->getUnitInfo()->hasIRSLsight())
		{
			IRSLindex = OpticsIndex++;
			m_currentOpticsComboBox.AddString(_T("IRSL"));
		}
		if (counterDataList[m_ActiveUnit]->getUnitInfo()->hasWLSLsight())
		{
			WLSLindex = OpticsIndex++;
			m_currentOpticsComboBox.AddString(_T("WLSL"));
		}

		// set current in-use optics
		m_Optics = counterDataList[m_ActiveUnit]->getOpticsInUse();
		if (m_Optics == OPTICS_THERMAL_IMAGER && counterDataList[m_ActiveUnit]->getUnitInfo()->hasTIsight())
			m_currentOpticsComboBox.SetCurSel(TIindex);
		else if (m_Optics == OPTICS_AMBIENT_LIGHT_ENHANCER && counterDataList[m_ActiveUnit]->getUnitInfo()->hasALEsight())
			m_currentOpticsComboBox.SetCurSel(ALEindex);
		else if (m_Optics == OPTICS_INFRARED_SEARCHLIGHT && counterDataList[m_ActiveUnit]->getUnitInfo()->hasIRSLsight())
			m_currentOpticsComboBox.SetCurSel(IRSLindex);
		else if (m_Optics == OPTICS_WHITELIGHT_SEARCHLIGHT && counterDataList[m_ActiveUnit]->getUnitInfo()->hasWLSLsight())
			m_currentOpticsComboBox.SetCurSel(WLSLindex);
		else
			m_currentOpticsComboBox.SetCurSel(OPTICS_OPTICAL_SIGHT);

		// laser designator, except during snow and heavy fog
		if (counterDataList[m_ActiveUnit]->getUnitInfo()->hasLaserDesignator() && m_Weather != WEATHER_HVY_FOG && m_Weather != WEATHER_SNOW)
			GetDlgItem(IDC_BUTTON_ACTION_LASER)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BUTTON_ACTION_LASER)->EnableWindow(FALSE);

		// radar, only if a unit has it
		if (counterDataList[m_ActiveUnit]->getUnitInfo()->hasRadar())
		{
			GetDlgItem(IDC_BUTTON_ACTION_RADAR)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_ACTIVE_RADAR_ON)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_ACTION_RADAR)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ACTIVE_RADAR_ON)->EnableWindow(FALSE);
		}

		// check for units that are mounted vs unmounted
		if ( m_activeUnitMounted >= 0 && !m_activeUnitDismounted )
		{
			SetDlgItemText(IDC_BUTTON_ACTION_MOUNT, STR_DISMOUNT);
			// mounted units cannot move themselves, so disable the move buttons
			if (m_normalMap)
			{
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_N)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_S)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW)->EnableWindow(FALSE);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_N2)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_S2)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE2)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW2)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE2)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW2)->EnableWindow(FALSE);
			}
			GetDlgItem(IDC_BUTTON_LAYSMOKE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ACTION_DEFILADE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ACTION_PREVIOUS_MOVE)->EnableWindow(FALSE);
		}
		else
		{
			SetDlgItemText(IDC_BUTTON_ACTION_MOUNT, STR_MOUNT);
			if (m_normalMap)
			{
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_N)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_S)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_N2)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_S2)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE2)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW2)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE2)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW2)->EnableWindow(TRUE);
			}
			GetDlgItem(IDC_BUTTON_LAYSMOKE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ACTION_DEFILADE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ACTION_PREVIOUS_MOVE)->EnableWindow(TRUE);
		}
		if ( m_activeUnitMounted >= 0 )
			GetDlgItem(IDC_BUTTON_ACTION_MOUNT)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BUTTON_ACTION_MOUNT)->EnableWindow(FALSE);

		// country of origin
		CountryType countryType = counterDataList[m_ActiveUnit]->getUnitInfo()->getCountryType();
		m_activeUnitCountry = counterDataList[m_ActiveUnit]->getUnitInfo()->getCountryTypeString(countryType);
		m_activeCountry = m_activeUnitCountry;

		// which side - blue or red
		SideType activeSideType = counterDataList[m_ActiveUnit]->getSideType();
		if (s_thisActiveSide != BOTH)
		{
			SideType activeSideType = s_networkActiveSide == SIDE_BLUE ? SIDE_BLUE : SIDE_RED;
			SideType mySideType = s_thisActiveSide == SIDE_RED ? SIDE_RED : SIDE_BLUE;
			m_activeSide = counterDataList[m_ActiveUnit]->getUnitInfo()->getSideTypeString(activeSideType);
			m_mySide = counterDataList[m_ActiveUnit]->getUnitInfo()->getSideTypeString(mySideType);
		}
		else
		{
			m_activeSide = counterDataList[m_ActiveUnit]->getUnitInfo()->getSideTypeString(activeSideType);
			m_mySide = (CString)"BOTH";
		}

		// unit type
		UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
		m_activeUnitUnitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitTypeString(unitType);

		// only helicopters have the option for low level
		if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isHelicopter() )
		{
			GetDlgItem(IDC_CHECK_ACTIVE_LOWLEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ACTION_LOWLEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_ACTIVE_SUPPRESSED)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_CHECK_ACTIVE_LOWLEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ACTION_LOWLEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ACTIVE_SUPPRESSED)->EnableWindow(TRUE);
		}

		// only attack/recon helicopters have the option for popups
		if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isAttackHelicopter() )
			GetDlgItem(IDC_BUTTON_ACTION_PASS)->EnableWindow(TRUE);
		else
			GetDlgItem(IDC_BUTTON_ACTION_PASS)->EnableWindow(FALSE);

		// target type
		TargetType targetType = counterDataList[m_ActiveUnit]->getUnitInfo()->getTargetType();
		m_activeUnitTargetType = counterDataList[m_ActiveUnit]->getUnitInfo()->getTargetTypeString(targetType);

		// if this unit is being carried and its not dismounted, get its carrier
		int isCarriedBy = counterDataList[m_ActiveUnit]->getIsCarriedBy();
		bool isCarried = (isCarriedBy != -1);
		bool isDismounted = (counterDataList[m_ActiveUnit]->getIsDismounted() == 1);

		// carrier unit if applicable
		m_activeUnitCarrierUnit = "";
		if (isCarried && !isDismounted)
		{
			// unit is being carried, display the name of the carrier unit
			m_activeUnitCarrierUnit = counterDataList[isCarriedBy]->getName();
		}

		// main weapon 1
 		AirCavWeaponData *mainWpn1 = counterDataList[m_ActiveUnit]->getUnitInfo()->getMainWeapon1();
		m_activeUnitMainWpn1 = mainWpn1->getName();
		if ( mainWpn1->getType() == ATGM || mainWpn1->getType() == SAM || mainWpn1->getType() == ROCKET )
		{
			int ammoRemaining = counterDataList[m_ActiveUnit]->getAmmoMainWeapon1();
			if ( ammoRemaining < 1 )
				m_activeUnitMainWpn1 += " [OUT]";
			else
			{
				std::string ammoStr = " [" + std::to_string(ammoRemaining) + "]";
				m_activeUnitMainWpn1 += CString(ammoStr.c_str());
			}
		}
		
		// main weapon 2
		AirCavWeaponData *mainWpn2 = counterDataList[m_ActiveUnit]->getUnitInfo()->getMainWeapon2();
		m_activeUnitMainWpn2 = mainWpn2->getName();
		if ( mainWpn2->getType() == ATGM || mainWpn2->getType() == SAM || mainWpn2->getType() == ROCKET )
		{
			int ammoRemaining = counterDataList[m_ActiveUnit]->getAmmoMainWeapon2();
			if ( ammoRemaining < 1 )
				m_activeUnitMainWpn2 += " [OUT]";
			else
			{
				std::string ammoStr = " [" + std::to_string(ammoRemaining) + "]";
				m_activeUnitMainWpn2 += CString(ammoStr.c_str());
			}
		}

		// main weapon 3
		AirCavWeaponData *mainWpn3 = counterDataList[m_ActiveUnit]->getUnitInfo()->getMainWeapon3();
		m_activeUnitMainWpn3 = mainWpn3->getName();
		if ( mainWpn3->getType() == ATGM || mainWpn3->getType() == SAM || mainWpn3->getType() == ROCKET )
		{
			int ammoRemaining = counterDataList[m_ActiveUnit]->getAmmoMainWeapon3();
			if ( ammoRemaining < 1 )
				m_activeUnitMainWpn3 += " [OUT]";
			else
			{
				std::string ammoStr = " [" + std::to_string(ammoRemaining) + "]";
				m_activeUnitMainWpn3 += CString(ammoStr.c_str());
			}
		}

		// secondary weapon 1
		AirCavWeaponData *secondaryWpn1 = counterDataList[m_ActiveUnit]->getUnitInfo()->getSecondaryWeapon1();
		m_activeUnitSecondaryWpn1 = secondaryWpn1->getName();
		if ( secondaryWpn1->getType() == ATGM || secondaryWpn1->getType() == SAM || secondaryWpn1->getType() == ROCKET )
		{
			int ammoRemaining = counterDataList[m_ActiveUnit]->getAmmoSecondaryWeapon1();
			if ( ammoRemaining < 1 )
				m_activeUnitSecondaryWpn1 += " [OUT]";
			else
			{
				std::string ammoStr = " [" + std::to_string(ammoRemaining) + "]";
				m_activeUnitSecondaryWpn1 += CString(ammoStr.c_str());
			}
		}

		// secondary weapon 2
		AirCavWeaponData *secondaryWpn2 = counterDataList[m_ActiveUnit]->getUnitInfo()->getSecondaryWeapon2();
		m_activeUnitSecondaryWpn2 = secondaryWpn2->getName();
		if ( secondaryWpn2->getType() == ATGM || secondaryWpn2->getType() == SAM || secondaryWpn2->getType() == ROCKET )
		{
			int ammoRemaining = counterDataList[m_ActiveUnit]->getAmmoSecondaryWeapon2();
			if ( ammoRemaining < 1 )
				m_activeUnitSecondaryWpn2 += " [OUT]";
			else
			{
				std::string ammoStr = " [" + std::to_string(ammoRemaining) + "]";
				m_activeUnitSecondaryWpn2 += CString(ammoStr.c_str());
			}
		}

		// position on the map
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		if ( m_activeUnitMounted >= 0 )
			sprintf_s( buffer, "%02d%02d", activeUnitHexColumn, activeUnitHexRow );
		else
			sprintf_s( buffer, "%02d%02d", activeUnitHexColumn, activeUnitHexRow );
		m_activeUnitPosition = (CString)buffer;

		// terrain in current hex
		int elev = mapData->getElevation( activeUnitHexRow, activeUnitHexColumn );
		_itoa_s(elev, buffer, 10);
		m_terrainElev = (CString)buffer;
		m_terrainStack = counterDataList[m_ActiveUnit]->getStackingString(mapData, counterDataList, activeUnitHexRow, activeUnitHexColumn);
		int activeTerrain = mapData->getTerrain( activeUnitHexRow, activeUnitHexColumn );
		m_terrainType = TerrainStr[activeTerrain];
		m_contourLines = mapData->getContour( activeUnitHexRow, activeUnitHexColumn );
		m_smoke = mapData->getSmoke( activeUnitHexRow, activeUnitHexColumn );
		m_wreck = mapData->getWreck( activeUnitHexRow, activeUnitHexColumn ) > 0 ? 1 : 0;
		int road = mapData->getRoad( activeUnitHexRow, activeUnitHexColumn );
		int autobahn = mapData->getAutobahn( activeUnitHexRow, activeUnitHexColumn );
		m_roadN  = mapData->getRoadHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NO);
		m_roadNW = mapData->getRoadHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NW);
		m_roadSW = mapData->getRoadHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SW);
		m_roadS  = mapData->getRoadHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SO);
		m_roadSE = mapData->getRoadHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SE);
		m_roadNE = mapData->getRoadHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NE);
		m_autobahnN  = mapData->getAutobahnHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NO);
		m_autobahnNW = mapData->getAutobahnHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NW);
		m_autobahnSW = mapData->getAutobahnHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SW);
		m_autobahnS  = mapData->getAutobahnHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SO);
		m_autobahnSE = mapData->getAutobahnHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SE);
		m_autobahnNE = mapData->getAutobahnHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NE);
		m_streamN  = mapData->getStreamHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NO);
		m_streamNW = mapData->getStreamHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NW);
		m_streamSW = mapData->getStreamHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SW);
		m_streamS  = mapData->getStreamHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SO);
		m_streamSE = mapData->getStreamHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_SE);
		m_streamNE = mapData->getStreamHex( activeUnitHexRow, activeUnitHexColumn, DIRECTION_NE);

		// for every other unit, calculate which have LOS to this unit
		if ( rebuildList )
		{
			m_SightedUnitsListBox.ResetContent();
			m_SightingUnitsListBox.ResetContent();
		}

		// a helicopter at low level is considered to be 30m above the terrain
		int activeUnitOffset = counterDataList[m_ActiveUnit]->getElevOffset();
		int activeUnitHeloOffset = counterDataList[m_ActiveUnit]->getHeloOffset();
		activeUnitOffset += activeUnitHeloOffset;
		if ( activeUnitHeloOffset > 0 )
		{
			if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isAttackHelicopter() )
				SetDlgItemText(IDC_BUTTON_ACTION_PASS, STR_POPDOWN);
			SetDlgItemText(IDC_BUTTON_ACTION_LOWLEVEL, STR_NOE);
		}
		else
		{
			if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isAttackHelicopter() )
				SetDlgItemText(IDC_BUTTON_ACTION_PASS, STR_POPUP);
			SetDlgItemText(IDC_BUTTON_ACTION_LOWLEVEL, STR_LL);
		}

		// actual elevation = elev of terrain + offset on contour + low level for helos
		int activeUnitElev = elev + activeUnitOffset;
		_itoa_s(counterDataList[m_ActiveUnit]->getElevOffset(), buffer, 10);
		m_activeUnitOffset = (CString)buffer;
		_itoa_s(activeUnitElev, buffer, 10);
		m_activeUnitActualElev = (CString)buffer;

		// list of units sighted by this unit, ONLY by popup
		popupSightedUnits.clear();

		// only bother with Line of Sight if the active unit is alive
		if (counterDataList[m_ActiveUnit]->getIsAlive())
		{
			// calculate range from active unit to all other enemy units (these are SIGHTED)
			int numPopupSightings = 0;
			for (int c = 0; c < m_maxCounters; c++)
			{
				// determine if sighting unit is using Thermal Imaging (to see through smoke)
				bool usingTI = counterDataList[m_ActiveUnit]->getOpticsInUse() == OPTICS_THERMAL_IMAGER;

				if (c != m_ActiveUnit)
				{
					bool skylined = false;
					int targetHexColumn = counterDataList[c]->getHexCol();
					int targetHexRow = counterDataList[c]->getHexRow();
					SideType thisSideType = counterDataList[c]->getSideType();
					int isAlive = counterDataList[c]->getIsAlive();
					int isDismounted = counterDataList[c]->getIsDismounted();
					if (activeSideType != thisSideType && isAlive && isDismounted)
					{
						bool popupRange = false;
						int targetUnitOffset = counterDataList[c]->getElevOffset();
						int targetUnitHeloOffset = counterDataList[c]->getHeloOffset();
						targetUnitOffset += targetUnitHeloOffset;

						//  calculate line-of-sight
						int range = mapData->CalculateLOS(activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, usingTI,
							targetHexRow, targetHexColumn, targetUnitOffset, skylined, false, buffer);

						CString unitName = counterDataList[m_ActiveUnit]->getName();
						CString targetName = counterDataList[c]->getName();
						if (m_debugLOSMessages)
						{
							CString msgstr = (CString)buffer;
							if (MessageBox((CString)msgstr, _T("Debug LOS from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
								m_debugLOSMessages = FALSE;
						}

						// for pop-ups, calculate Helo elev as if it is at low level
						if (range == 0)
						{
							// this calculation is only required for an active unit attack helicopter at nap-of-earth observing a target
							if ( activeUnitHeloOffset == 0 && counterDataList[m_ActiveUnit]->getUnitInfo()->isAttackHelicopter() )
							{
								//  recalculate calculate line-of-sight assuming a pop-up
								range = mapData->CalculateLOS(activeUnitHexRow, activeUnitHexColumn, activeUnitOffset + LOW_LEVEL_METERS, usingTI,
									targetHexRow, targetHexColumn, targetUnitOffset, skylined, false, buffer);

								if (range > 0)
									popupRange = true;

								if (m_debugLOSMessages)
								{
									CString msgstr = (CString)buffer;
									if (MessageBox((CString)msgstr, _T("Debug POPUP LOS from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
										m_debugLOSMessages = FALSE;
								}
							}
						}

						int activeSmoke = mapData->getSmoke(activeUnitHexRow, activeUnitHexColumn);
						int targetTerrain = mapData->getTerrain(targetHexRow, targetHexColumn);
						int targetSmoke = mapData->getSmoke(targetHexRow, targetHexColumn);
						int targetlowLevel = targetUnitHeloOffset > 0;
						int activelowLevel = activeUnitHeloOffset > 0;
						int targetFired = counterDataList[c]->getFired();
						int targetMoved = counterDataList[c]->getMoved();
						if (targetFired || targetMoved)
						{
							// if a unit is in smoke hex and not low level and it has fired, it can be observed only up to 5 hexes away
							// my errata: also include in this case if the unit has moved
							if (!targetlowLevel && targetSmoke && range > 5 && !usingTI)
								continue;
						}
						else
						{
							// a unit can only be observed from 1 hex away if it is in smoke hex and not low level and has not fired
							if (!targetlowLevel && targetSmoke && range > 1 && !usingTI)
								continue;
						}

						// ignore smoke in the active hex if it is a helo at low level or if using Thermal Imager
						if ((activelowLevel || usingTI) && activeSmoke)
							activeSmoke = 0;

						// ignore smoke in the target hex if it is a helo at low level
						if (targetlowLevel && targetSmoke)
							targetSmoke = 0;

						// skylining has some very specific requirements, check them here
						if (skylined && unitCannotBeSkylined(counterDataList[c]->getUnitInfo()->getUnitType(), targetTerrain, targetSmoke))
							skylined = false;

						// a skylined unit can be seen at 4 hexes greater than normal
						int skylinedRange = skylined ? range + 4 : range;

						// calculate if unit is visible given the terrain, range, weather, time of day, etc
#if BASIC_OBSERVATION
						bool isVisible = counterDataList[c]->isVisible(targetTerrain, skylinedRange, m_Weather, m_TimeOfDay, activeSmoke);
#else
						SideType activeSide = counterDataList[m_ActiveUnit]->getSideType();
						int activeOptics = counterDataList[m_ActiveUnit]->getOpticsInUse();
						bool activeRadar = (counterDataList[m_ActiveUnit]->getUnitInfo()->hasRadar() && counterDataList[m_ActiveUnit]->getRadarInUse());
						bool isVisible = counterDataList[c]->isVisibleAdvanced(targetTerrain, skylinedRange, m_Weather, m_TimeOfDay, activeSide, activeOptics, activeRadar, activeSmoke);
#endif
						// if in range, calculate FKN for each available weapon
						int ttModm1 = 0, ttModm2 = 0, ttModm3 = 0, ttMods1 = 0, ttMods2 = 0;
						if (range && isVisible)
						{
							//  calculate final kill number
							int oppfire = counterDataList[m_ActiveUnit]->getIsOppFiring();
							int supfire = counterDataList[m_ActiveUnit]->getIsSuppressed() ? counterDataList[m_ActiveUnit]->getUnitInfo()->getSM() : 0;
							int mFKN1 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								MAIN1, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, skylined, buffer, ttModm1);
							if (mFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN2 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								MAIN2, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, skylined, buffer, ttModm2);
							if (mFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN3 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								MAIN3, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, skylined, buffer, ttModm3);
							if (mFKN3 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN1 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								SECONDARY1, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, skylined, buffer, ttMods1);
							if (sFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN2 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								SECONDARY2, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, skylined, buffer, ttMods2);
							if (sFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							// only update FKN numbers for the active target
							if (m_ActiveTarget == c)
							{
								counterDataList[m_ActiveUnit]->setFinalKillNumbers(mFKN1, mFKN2, mFKN3, sFKN1, sFKN2, ttModm1, ttModm2, ttModm3, ttMods1, ttMods2);
							}

							// display the visible units their type, range and FKN values
							char sName[32], sType[32];
							int length;
							CString sightingUnitName = counterDataList[c]->getName();
							CString sightingUnitType = counterDataList[c]->getUnitInfo()->getName();
							length = sightingUnitName.GetLength() + 1;
							for (int i = 0; i < length; i++)
								sName[i] = sightingUnitName.GetAt(i);
							length = sightingUnitType.GetLength() + 1;
							for (int i = 0; i < length; i++)
								sType[i] = sightingUnitType.GetAt(i);

							// if another unit is laser designating this unit, display this
							std::string ld("");
							if (counterDataList[c]->getLaserDesignatingUnit() >= 0)
							{
								ld = "[LD]";
							}

							if (popupRange)
							{
								numPopupSightings++;
								if (skylined)
									sprintf_s(buffer, "%s [%s] [POPUP] %s Hex: %02d%02d  Range: %d   m1:(%d)  m2:(%d)  m3:(%d)  s1:(%d)  s2:(%d)",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								else
									sprintf_s(buffer, "%s [%s] [POPUP] %s Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								popupSightedUnits.insert(std::make_pair(c, buffer));
							}
							else
							{
								if (skylined)
									sprintf_s(buffer, "%s [%s] %s Hex: %02d%02d  Range: %d   m1:(%d)  m2:(%d)  m3:(%d)  s1:(%d)  s2:(%d)",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								else
									sprintf_s(buffer, "%s [%s] %s Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								if (rebuildList)
									m_SightedUnitsListBox.AddString(CString(buffer));
							}
						}
					}
				}
			}

			// check all other friendly units, if all sightings were popup sightings, then there are effectively no sightings (at least one must be non-popup)
			for (auto s : popupSightedUnits)
			{
				int numNormalSightings = 0;
				for (int c = 0; c < m_maxCounters; c++)
				{
					SideType thisSideType = counterDataList[c]->getSideType();
					int isAlive = counterDataList[c]->getIsAlive();
					int isDismounted = counterDataList[c]->getIsDismounted();
					if (activeSideType == thisSideType && isAlive && isDismounted)
					{
						bool skylined = false;
						int targetUnitHexColumn = counterDataList[s.first]->getHexCol();
						int targetUnitHexRow = counterDataList[s.first]->getHexRow();
						int targetUnitOffset = counterDataList[s.first]->getElevOffset();
						int targetUnitHeloOffset = counterDataList[s.first]->getHeloOffset();
						targetUnitOffset += targetUnitHeloOffset;

						// determine if sighting unit is using Thermal Imaging (to see through smoke)
						bool usingTI = counterDataList[c]->getOpticsInUse() == OPTICS_THERMAL_IMAGER;

						int friendlyUnitHexColumn = counterDataList[c]->getHexCol();
						int friendlyUnitHexRow = counterDataList[c]->getHexRow();
						int friendlyUnitOffset = counterDataList[c]->getElevOffset();
						int friendlyUnitHeloOffset = counterDataList[c]->getHeloOffset();
						friendlyUnitOffset += friendlyUnitHeloOffset;

						int range = mapData->CalculateLOS(friendlyUnitHexRow, friendlyUnitHexColumn, friendlyUnitOffset, usingTI,
							targetUnitHexRow, targetUnitHexColumn, targetUnitOffset, skylined, false, buffer);
						if (range > 0)
							numNormalSightings++;
					}
				}
				if (rebuildList && numNormalSightings)
				{
					m_SightedUnitsListBox.AddString(CString(s.second.c_str()));
				}
			}

			// calculate range from all other enemy units to active unit (these are SIGHTING)
			numPopupSightings = 0;
			std::map<int, std::string> popupSightingUnits;
			for (int c = 0; c < m_maxCounters; c++)
			{
				// determine if sighting unit is using Thermal Imaging (to see through smoke)
				bool usingTI = counterDataList[c]->getOpticsInUse() == OPTICS_THERMAL_IMAGER;

				if (c != m_ActiveUnit)
				{
					int targetHexColumn = counterDataList[c]->getHexCol();
					int targetHexRow = counterDataList[c]->getHexRow();
					SideType thisSideType = counterDataList[c]->getSideType();
					int isAlive = counterDataList[c]->getIsAlive();
					int isDismounted = counterDataList[c]->getIsDismounted();
					if (activeSideType != thisSideType && isAlive && isDismounted)
					{
						bool skylined = false;
						bool popupRange = false;
						int targetUnitOffset = counterDataList[c]->getElevOffset();
						int targetUnitHeloOffset = counterDataList[c]->getHeloOffset();
						targetUnitOffset += targetUnitHeloOffset;

						//  calculate line-of-sight
						int range = mapData->CalculateLOS(targetHexRow, targetHexColumn, targetUnitOffset, usingTI,
							activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, skylined, false, buffer);

						CString unitName = counterDataList[m_ActiveUnit]->getName();
						CString targetName = counterDataList[c]->getName();
						if (m_debugLOSMessages)
						{
							CString msgstr = (CString)buffer;
							if (MessageBox((CString)msgstr, _T("Debug LOS from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
								m_debugLOSMessages = FALSE;
						}

						// for pop-ups, calculate Helo elev as if it is at low level
						if (range == 0)
						{
							// this calculation is only required for attack helicopters at nap-of-earth observing the active unit
							if (targetUnitHeloOffset <= 0 && counterDataList[c]->getUnitInfo()->isAttackHelicopter() )
							{
								//  recalculate calculate line-of-sight assuming a pop-up
								range = mapData->CalculateLOS(targetHexRow, targetHexColumn, targetUnitOffset + LOW_LEVEL_METERS, usingTI,
									activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, skylined, false, buffer);

								if (range > 0)
									popupRange = true;

								if (m_debugLOSMessages)
								{
									CString msgstr = (CString)buffer;
									if (MessageBox((CString)msgstr, _T("Debug POPUP LOS from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
										m_debugLOSMessages = FALSE;
								}
							}
						}

						int activeSmoke = mapData->getSmoke(activeUnitHexRow, activeUnitHexColumn);
						int targetSmoke = mapData->getSmoke(targetHexRow, targetHexColumn);
						int activelowLevel = activeUnitHeloOffset > 0;
						int targetlowLevel = targetUnitHeloOffset > 0;

						int activeFired = counterDataList[m_ActiveUnit]->getFired();
						int activeMoved = counterDataList[m_ActiveUnit]->getMoved();
						if (activeFired || activeMoved)
						{
							// if a unit is in smoke hex and not low level and it has fired, it can be observed only up to 5 hexes away
							// my errata: also include in this case if the unit has moved
							if (!activelowLevel && activeSmoke && range > 5 && !usingTI)
								continue;
						}
						else
						{
							// a unit can only be observed from 1 hex away if it is in smoke hex and not low level and has not fired
							if (!activelowLevel && activeSmoke && range > 1 && !usingTI)
								continue;
						}

						// ignore smoke in the target hex if it is a helo at low level or if using Thermal Imager
						if ((targetlowLevel || usingTI) && targetSmoke)
							targetSmoke = 0;

						// ignore smoke in the active hex if it is a helo at low level
						if (activelowLevel && activeSmoke)
							activeSmoke = 0;

						// skylining has some very specific requirements, check them here
						if (skylined && unitCannotBeSkylined(counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType(), activeTerrain, activeSmoke))
							skylined = false;

						// a skylined unit can be seen at 4 hexes greater than normal
						int skylinedRange = skylined ? range + 4 : range;

						// calculate if unit is visible given the terrain, range, weather, time of day, etc
#if BASIC_OBSERVATION
						bool isVisible = counterDataList[m_ActiveUnit]->isVisible(activeTerrain, skylinedRange, m_Weather, m_TimeOfDay, targetSmoke);
#else
						SideType targetSide = counterDataList[c]->getSideType();
						int targetOptics = counterDataList[c]->getOpticsInUse();
						bool targetRadar = (counterDataList[c]->getUnitInfo()->hasRadar() && counterDataList[c]->getRadarInUse());
						bool isVisible = counterDataList[m_ActiveUnit]->isVisibleAdvanced(activeTerrain, skylinedRange, m_Weather, m_TimeOfDay, targetSide, targetOptics, targetRadar, targetSmoke);
#endif
						// if in range, calculate FKN for each available weapon
						int ttModm1 = 0, ttModm2 = 0, ttModm3 = 0, ttMods1 = 0, ttMods2 = 0;
						if (range && isVisible)
						{
							int oppfire = counterDataList[c]->getIsOppFiring();
							int supfire = counterDataList[c]->getIsSuppressed() ? counterDataList[c]->getUnitInfo()->getSM() : 0;

							//  calculate final kill number
							int mFKN1 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								MAIN1, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, skylined, buffer, ttModm1);
							if (mFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN2 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								MAIN2, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, skylined, buffer, ttModm2);
							if (mFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN3 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								MAIN3, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, skylined, buffer, ttModm3);
							if (mFKN3 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN1 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								SECONDARY1, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, skylined, buffer, ttMods1);
							if (sFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN2 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								SECONDARY2, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, skylined, buffer, ttMods2);
							if (sFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							counterDataList[c]->setFinalKillNumbers(mFKN1, mFKN2, mFKN3, sFKN1, sFKN2, ttModm1, ttModm2, ttModm3, ttMods1, ttMods2);

							// display the visible units their type, range and FKN values
							char sName[32], sType[32];
							int length;
							CString sightingUnitName = counterDataList[c]->getName();
							CString sightingUnitType = counterDataList[c]->getUnitInfo()->getName();
							length = sightingUnitName.GetLength() + 1;
							for (int i = 0; i < length; i++)
								sName[i] = sightingUnitName.GetAt(i);
							length = sightingUnitType.GetLength() + 1;
							for (int i = 0; i < length; i++)
								sType[i] = sightingUnitType.GetAt(i);

							// if the active unit is being laser designated by this unit, display this
							std::string ld("");
							if (counterDataList[m_ActiveUnit]->getLaserDesignatingUnit() == c)
							{
								ld = "[LD]";
							}

							if (popupRange)
							{
								numPopupSightings++;
								if (skylined)
									sprintf_s(buffer, "%s [%s] [POPUP] %s Hex: %02d%02d  Range: %d   m1:(%d)  m2:(%d)  m3:(%d)  s1:(%d)  s2:(%d)",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								else
									sprintf_s(buffer, "%s [%s] [POPUP] %s Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								popupSightingUnits.insert(std::make_pair(c, buffer));
							}
							else
							{
								if (skylined)
									sprintf_s(buffer, "%s [%s] %s Hex: %02d%02d  Range: %d   m1:(%d)  m2:(%d)  m3:(%d)  s1:(%d)  s2:(%d)",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								else
									sprintf_s(buffer, "%s [%s] %s Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
										sName, sType, ld.c_str(), targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								if (rebuildList)
									m_SightingUnitsListBox.AddString(CString(buffer));
							}
						}
					}
				}
			}

			// check all other enemy units, if all sightings were popup sightings, then there are effectively no sightings (at least one must be non-popup)
			for (auto s : popupSightingUnits)
			{
				int numNormalSightings = 0;
				for (int c = 0; c < m_maxCounters; c++)
				{
					if (c != m_ActiveUnit)
					{
						SideType thisSideType = counterDataList[c]->getSideType();
						int isAlive = counterDataList[c]->getIsAlive();
						int isDismounted = counterDataList[c]->getIsDismounted();
						if (activeSideType != thisSideType && isAlive && isDismounted)
						{
							bool skylined = false;
							int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
							int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();

							// determine if sighting unit is using Thermal Imaging (to see through smoke)
							bool usingTI = counterDataList[c]->getOpticsInUse() == OPTICS_THERMAL_IMAGER;

							int enemyUnitHexColumn = counterDataList[c]->getHexCol();
							int enemyUnitHexRow = counterDataList[c]->getHexRow();
							int enemyUnitOffset = counterDataList[c]->getElevOffset();
							int enemyUnitHeloOffset = counterDataList[c]->getHeloOffset();
							enemyUnitOffset += enemyUnitHeloOffset;

							int range = mapData->CalculateLOS(enemyUnitHexRow, enemyUnitHexColumn, enemyUnitOffset, usingTI,
								activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, skylined, false, buffer);
							if (range > 0)
								numNormalSightings++;
						}
					}
				}
				if (rebuildList && numNormalSightings)
				{
					m_SightingUnitsListBox.AddString(CString(s.second.c_str()));
				}
			}

			// override the hex position and sighted units if this is not the active side over the network
			if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide)
			{
				// do not display any units the active unit can sight
				m_SightedUnitsListBox.ResetContent();

				// if there are no units sighting this unit...
				if (m_SightingUnitsListBox.GetCount() == 0)
				{
					// ...obscure its actual position from the enemy
					m_activeUnitPosition = (CString)"XXXX";
				}
			}
		}

		if (!noNetworkUpdate && s_thisActiveSide == s_networkActiveSide)
		{
			sendUnitInfo(m_ActiveUnit);
		}

		UpdateData(FALSE);
	}
}

void CAirCavLOSDlg::sendUnitInfo(int unit)
{
	// only if networking is enabled
	if (s_thisActiveSide != BOTH)
	{
		char buffer[MAX_BUF_SIZE] = { 0 };;
		encodeUnitInfo(unit, buffer);
		if (s_thisActiveSide == SIDE_BLUE)
			serverSend(buffer);
		else
			clientSend(buffer);
		Sleep(1000);
	}
}

void CAirCavLOSDlg::encodeUnitInfo(int unit, char *outbuffer)
{
	int col = counterDataList[unit]->getHexCol();
	int row = counterDataList[unit]->getHexRow();
	double OPs = counterDataList[unit]->getOPs();
	int elevOffset = counterDataList[unit]->getElevOffset();
	int heloOffset = counterDataList[unit]->getHeloOffset();
	int fired = counterDataList[unit]->getFired();
	int moved = counterDataList[unit]->getMoved();
	int status = counterDataList[unit]->getIsAlive();
	int defilade = counterDataList[unit]->getDefilade();
	int evading = counterDataList[unit]->getEvading();
	int dismounted = counterDataList[unit]->getIsDismounted();
	int suppressed = counterDataList[unit]->getIsSuppressed();
	int optics = counterDataList[unit]->getOpticsInUse();
	int laserdesignated = counterDataList[unit]->getLaserDesignatedUnit();
	int laserdesignating = counterDataList[unit]->getLaserDesignatingUnit();
	int radar = counterDataList[unit]->getRadarInUse();

	char buffer[MAX_BUF_SIZE] = { 0 };;
	sprintf_s(buffer, "info %d %d %d %2.1f %d %d %d %d %d %d %d %d %d %d %d %d %d", unit, col, row, OPs, elevOffset, heloOffset, 
		fired, moved, status, defilade, evading, dismounted, suppressed, optics, laserdesignated, laserdesignating, radar);
	strncpy(outbuffer, buffer, strlen(buffer));
}

void CAirCavLOSDlg::decodeUnitInfo(char *inbuffer)
{
	char cmd[10];
	int unit, col, row, elevOffset, heloOffset, fired, moved, status, defilade, evading, dismounted, suppressed, optics, laserdesignated, laserdesignating, radar;
	float OPs;
	char buffer[MAX_BUF_SIZE] = { 0 };;
	strncpy(buffer, inbuffer, strlen(inbuffer));
	sscanf(buffer, "%s %d %d %d %f %d %d %d %d %d %d %d %d %d %d %d %d %d", cmd, &unit, &col, &row, &OPs, &elevOffset, &heloOffset,
		&fired, &moved, &status, &defilade, &evading, &dismounted, &suppressed, &optics, &laserdesignated, &laserdesignating, &radar);

	counterDataList[unit]->setHexCol(col);
	counterDataList[unit]->setHexRow(row);
	counterDataList[unit]->setOPs(OPs);
	counterDataList[unit]->setElevOffset(elevOffset);
	counterDataList[unit]->setHeloOffset(heloOffset);
	counterDataList[unit]->setFired(fired);
	counterDataList[unit]->setMoved(moved);
	counterDataList[unit]->setIsAlive(status);
	counterDataList[unit]->setDefilade(defilade);
	counterDataList[unit]->setEvading(evading);
	counterDataList[unit]->setIsDismounted(dismounted);
	counterDataList[unit]->setIsSuppressed(suppressed);
	counterDataList[unit]->setOpticsInUse(optics);
	counterDataList[unit]->setLaserDesignatedUnit(laserdesignated);
	counterDataList[unit]->setLaserDesignatingUnit(laserdesignating);
	counterDataList[unit]->setRadarInUse(radar);
}

void CAirCavLOSDlg::encodeMapInfo(int row, int col, char *outbuffer)
{
	int smoke = mapData->getSmoke(row, col);
	int wreck = mapData->getWreck(row, col);

	char buffer[MAX_BUF_SIZE] = { 0 };
	sprintf_s(buffer, "map %d %d %d %d", row, col, smoke, wreck);
	strncpy(outbuffer, buffer, strlen(buffer));
}

void CAirCavLOSDlg::decodeMapInfo(char *inbuffer)
{
	char cmd[10];
	int col, row, smoke, wreck;
	char buffer[MAX_BUF_SIZE] = { 0 };
	strncpy(buffer, inbuffer, strlen(inbuffer));
	sscanf(buffer, "%s %d %d %d %d", cmd, &row, &col, &smoke, &wreck);

	if (smoke == 2)
	{
		m_artillerySmokeHexList.Add(row, col);
		m_smoke = mapData->setSmoke(row, col, true, true);
	}
	else if (smoke == 1)
	{
		m_vehicleSmokeHexList.Add(row, col);
		m_smoke = mapData->setSmoke(row, col, false, true);
	}
	else
	{
		mapData->clearSmoke(row, col);
	}

	mapData->clearWreck(row, col);
	for (int i = 0; i < wreck; i++)
	{
		mapData->setWreck(row, col, false);
	}
}

void CAirCavLOSDlg::sendMapInfo(int row, int col)
{
	// only if networking is enabled
	if (s_thisActiveSide != BOTH)
	{
		char buffer[MAX_BUF_SIZE] = { 0 };
		encodeMapInfo(row, col, buffer);
		if (s_thisActiveSide == SIDE_BLUE)
			serverSend(buffer);
		else
			clientSend(buffer);
		Sleep(1000);
	}
}

void CAirCavLOSDlg::encodeWeather(char *outbuffer)
{
	char buffer[MAX_BUF_SIZE] = { 0 };
	sprintf_s(buffer, "weather %d", m_Weather);
	strncpy(outbuffer, buffer, strlen(buffer));
}

void CAirCavLOSDlg::decodeWeather(char *inbuffer)
{
	char cmd[10];
	char buffer[MAX_BUF_SIZE] = { 0 };
	strncpy(buffer, inbuffer, strlen(inbuffer));
	sscanf(buffer, "%s %d", cmd, &m_Weather);

	if (m_Weather == WEATHER_CLEAR)
		m_currentWeather = "Clear";
	else if (m_Weather == WEATHER_LT_FOG)
		m_currentWeather = "Light Fog";
	else if (m_Weather == WEATHER_HVY_FOG)
		m_currentWeather = "Heavy Fog";
	else if (m_Weather == WEATHER_OVERCAST)
		m_currentWeather = "Overcast";
	else if (m_Weather == WEATHER_RAIN)
		m_currentWeather = "Rain";
	else if (m_Weather == WEATHER_SNOW)
		m_currentWeather = "Snow";

	UpdateData(FALSE);
}

void CAirCavLOSDlg::sendWeather()
{
	// only if networking is enabled
	if (s_thisActiveSide != BOTH)
	{
		char buffer[MAX_BUF_SIZE] = { 0 };
		encodeWeather(buffer);
		if (s_thisActiveSide == SIDE_BLUE)
			serverSend(buffer);
		else
			clientSend(buffer);
		Sleep(1000);
	}
}

void CAirCavLOSDlg::encodeTimeOfDay(char *outbuffer)
{
	char buffer[MAX_BUF_SIZE] = { 0 };
	sprintf_s(buffer, "time %d", m_TimeOfDay);
	strncpy(outbuffer, buffer, strlen(buffer));
}

void CAirCavLOSDlg::decodeTimeOfDay(char *inbuffer)
{
	char cmd[10];
	char buffer[MAX_BUF_SIZE] = { 0 };
	strncpy(buffer, inbuffer, strlen(inbuffer));
	sscanf(buffer, "%s %d", cmd, &m_TimeOfDay);
	
	if (m_TimeOfDay == TIME_DAY)
		m_currentTimeOfDay = "Day";
	else if (m_TimeOfDay == TIME_NIGHT)
		m_currentTimeOfDay = "Night";

	UpdateData(FALSE);
}

void CAirCavLOSDlg::sendTimeOfDay()
{
	// only if networking is enabled
	if (s_thisActiveSide != BOTH)
	{
		char buffer[MAX_BUF_SIZE];
		encodeTimeOfDay(buffer);
		if (s_thisActiveSide == SIDE_BLUE)
			serverSend(buffer);
		else
			clientSend(buffer);
		Sleep(1000);
	}
}

void CAirCavLOSDlg::OnLbnSelchangeListLog()
{
	// TODO: Add your control notification handler code here
}

void CAirCavLOSDlg::writeToLog( char *buffer )
{
	m_OutputLogListBox.AddString((CString)buffer);
}

void CAirCavLOSDlg::OnBnClickedButtonEditTerrain()
{
	if ( m_ActiveUnit >= 0 )
	{
		int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
		int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
		mapData->editTerrainData( activeUnitHexRow, activeUnitHexColumn );
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedButtonFillTerrain()
{
	for (int r = MIN_ROWS; r <= MAX_ROWS; r++)
	{
		for (int c = MIN_COLUMNS; c <= MAX_COLUMNS; c++)
		{
			if (mapData->getElevation(r, c) < 0)
				return;
		}
	}
}

void CAirCavLOSDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (counterDataList[m_ActiveUnit]->getIsAlive())
	{
		LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

		if (pNMUpDown->iDelta < 0)
			counterDataList[m_ActiveUnit]->incrOPs((double)-((double)pNMUpDown->iDelta / (double)2.0));
		else
			counterDataList[m_ActiveUnit]->decrOPs((double)+((double)pNMUpDown->iDelta / (double)2.0));

		char buffer[10];
		sprintf_s(buffer, "%2.1f", counterDataList[m_ActiveUnit]->getOPs());
		m_activeUnitOPs = (CString)buffer;
	}
	else
	{
		m_activeUnitOPs = (CString)"0.0";
	}

	UpdateData(FALSE);

	*pResult = 0;
}

void CAirCavLOSDlg::OnBnClickedCheckActiveFired()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if (m_ActiveUnit >= 0)
	{
		if (counterDataList[m_ActiveUnit]->getFired())
			counterDataList[m_ActiveUnit]->nofire();
		else
			counterDataList[m_ActiveUnit]->fire();
		UpdateData(TRUE);
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedCheckActiveMoved()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if (m_ActiveUnit >= 0)
	{
		if (counterDataList[m_ActiveUnit]->getMoved())
			counterDataList[m_ActiveUnit]->nomove();
		else
			counterDataList[m_ActiveUnit]->move();
		UpdateData(TRUE);
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionReset()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( m_ActiveUnit >= 0 )
	{
		counterDataList[m_ActiveUnit]->reset();
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnEnChangeEditActiveLoc()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);

		char sCol[3], sRow[3];
		sCol[0] = m_activeUnitPosition.GetAt(0);
		sCol[1] = m_activeUnitPosition.GetAt(1);
		sCol[2] = '\0';
		sRow[0] = m_activeUnitPosition.GetAt(2);
		sRow[1] = m_activeUnitPosition.GetAt(3);
		sRow[2] = '\0';

		int hexColumn = atoi( sCol );
		int hexRow = atoi( sRow );

		counterDataList[m_ActiveUnit]->moveTo(mapData, counterDataList, hexColumn, hexRow);
		moveMountedUnits();
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedCheckDebug()
{
	UpdateData(TRUE);
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedCheckDebugPk()
{
	UpdateData(TRUE);
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedButtonActionEvade()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);
		counterDataList[m_ActiveUnit]->evasiveManeuver();
		if (counterDataList[m_ActiveUnit]->getEvading())
			counterDataList[m_ActiveUnit]->setActionTaken(true);
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionDefilade()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);
		int hexColActiveUnit = counterDataList[m_ActiveUnit]->getHexCol();
		int hexRowActiveUnit = counterDataList[m_ActiveUnit]->getHexRow();
		int terrain = mapData->getTerrain(hexRowActiveUnit, hexColActiveUnit);
		counterDataList[m_ActiveUnit]->enterDefilade(terrain);
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedCheckActiveEvading()
{
}

void CAirCavLOSDlg::OnBnClickedCheckActiveInDef()
{
}

void CAirCavLOSDlg::OnBnClickedButtonActiveSuppressed()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if (m_ActiveUnit >= 0)
	{
		UpdateData(TRUE);
		if (counterDataList[m_ActiveUnit]->getIsSuppressed())
		{
			bool successful = false;
			if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isTrackedVehicle() )
			{
				if (counterDataList[m_ActiveUnit]->decrOPs(2) != -1)
				{
					counterDataList[m_ActiveUnit]->setIsSuppressed(0);
					counterDataList[m_ActiveUnit]->setActionTaken(true);
					successful = true;
				}
			}
			else if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isWheeledVehicle() )
			{
				if (counterDataList[m_ActiveUnit]->decrOPs(3) != -1)
				{
					counterDataList[m_ActiveUnit]->setIsSuppressed(0);
					counterDataList[m_ActiveUnit]->setActionTaken(true);
					successful = true;
				}
			}
			else if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isInfantry() )
			{
				if (counterDataList[m_ActiveUnit]->decrOPs(3) != -1)
				{
					counterDataList[m_ActiveUnit]->setIsSuppressed(0);
					counterDataList[m_ActiveUnit]->setActionTaken(true);
					successful = true;
				}
			}
			if ( !successful )
			{
				CString msgstr = (CString)"Not enough OPs to Remove Suppression";
				MessageBox((LPCTSTR)msgstr);
			}
		}
		else
		{
			counterDataList[m_ActiveUnit]->setIsSuppressed(1);
		}
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionMount()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);
		// only care about mounting if this unit has a carrying unit
		int carryingUnit = counterDataList[m_ActiveUnit]->getIsCarriedBy();
		if ( carryingUnit >= 0 )
		{
			// must be in the same location as carrying unit
			int hexColActiveUnit = counterDataList[m_ActiveUnit]->getHexCol();
			int hexRowActiveUnit = counterDataList[m_ActiveUnit]->getHexRow();
			int hexColCarryingUnit = counterDataList[carryingUnit]->getHexCol();
			int hexRowCarryingUnit = counterDataList[carryingUnit]->getHexRow();

			if ( hexColActiveUnit == hexColCarryingUnit && hexRowActiveUnit == hexRowCarryingUnit )
			{
				// units always mount back to their originally mounted units
				if ( counterDataList[m_ActiveUnit]->getIsDismounted() )
				{
					// only if the both units can mount can the operation complete
					if ( counterDataList[m_ActiveUnit]->canMount() && counterDataList[carryingUnit]->canMount() )
					{
						counterDataList[m_ActiveUnit]->mountCarrier();
						counterDataList[carryingUnit]->mountUnit();
					}
				}
				else
				{
					// only if the both units can dismount can the operation complete
					if ( counterDataList[m_ActiveUnit]->canDismount() && counterDataList[carryingUnit]->canDismount() )
					{
						counterDataList[m_ActiveUnit]->dismountCarrier();
						counterDataList[carryingUnit]->dismountUnit();
					}
				}

				updateActiveUnit();
			}
			else
			{
				CString activeName = counterDataList[m_ActiveUnit]->getName();
				char aName[64];
				int rightBracket = activeName.Find(')');
				for (int i = 0; i < rightBracket-4; i++)
					aName[i] = activeName.GetAt(i);						
				aName[rightBracket-4] = '\0';
				CString carryingName = counterDataList[carryingUnit]->getName();
				char cName[64];
				rightBracket = carryingName.Find(')');
				for (int i = 0; i < rightBracket-4; i++)
					cName[i] = carryingName.GetAt(i);						
				cName[rightBracket-4] = '\0';

				char errorString[MAX_BUF_SIZE];
				sprintf_s( errorString, "\"%s\" must be in the same hex as \"%s\" in order to mount", aName, cName );
				MessageBox( (CString)errorString, (CString)"Cannot Mount Unit", MB_OK );
			}
		}
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionLowlevel()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);

		// only for Helicopters
		if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isHelicopter() )
		{
			if ( counterDataList[m_ActiveUnit]->getHeloOffset() > 0 )
				counterDataList[m_ActiveUnit]->actionHeloOffset(mapData, counterDataList, NAP_OF_EARTH_METERS);
			else
				counterDataList[m_ActiveUnit]->actionHeloOffset(mapData, counterDataList, LOW_LEVEL_METERS);
			updateActiveUnit();
		}
	}
}


void CAirCavLOSDlg::resolveOpportunityFire()
{
	int lastUnitWithKill = -1;

	// first pass - gun armed weapons
	int lastUnitToKillGuns = resolveFirePass(GUN);
	if (lastUnitToKillGuns >= 0)
		lastUnitWithKill = lastUnitToKillGuns;

	// second pass - non-ATGM missile-type weapons (ROCKET, SAM)
	int lastUnitToKillRocket= resolveFirePass(ROCKET);
	if (lastUnitToKillRocket >= 0)
		lastUnitWithKill = lastUnitToKillRocket;

	// third pass - ATGMs
	int lastUnitToKillATGM = resolveFirePass(ATGM);
	if (lastUnitToKillATGM >= 0)
		lastUnitWithKill = lastUnitToKillATGM;

	// clear the list
	size_t listSize = m_FiringUnitsList.size();
	for ( size_t c=0; c<listSize; c++ )
	{
		// clear opportunity fire
		int firingUnit = m_FiringUnitsList[c];
		counterDataList[firingUnit]->setIsOppFiring(FALSE);
		counterDataList[firingUnit]->setEvading(0);
	}
	m_FiringUnitsList.clear();

	// last unit with a kill is now the active unit, if any
	if (lastUnitWithKill >= 0)
		m_ActiveUnit = lastUnitWithKill;

	// in non-network mode, just (re)select the active unit
	if (s_thisActiveSide == BOTH)
	{
		updateActiveUnit();
		m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
	}
	else
	{
		// if the active unit is an enemy unit, might need to switch sides
		SideType thisSide = counterDataList[m_ActiveUnit]->getSideType();
		if (thisSide == s_networkActiveSide)
		{
			updateActiveUnit();
			m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
		}
		else
		{
			// display the last unit with a kill and which side we are switching to
			CString activeName = counterDataList[lastUnitWithKill]->getName();
			SideType activeSide = counterDataList[lastUnitWithKill]->getSideType();
			std::string newSide = "BLUE";
			if (activeSide == SIDE_RED)
				newSide = "RED";

			char buffer1[MAX_BUF_SIZE];
			sprintf_s(buffer1, "Last unit with a kill is \"%ls\". Switching active side to %s", activeName.GetString(), newSide.c_str());
			MessageBox((CString)buffer1, _T("Active Side Change"), MB_OK);
			OnBnClickedButtonConnect();
		}
	}
}

int CAirCavLOSDlg::resolveFirePass(int firePass)
{
	int lastActiveUnit = -1; // m_originalActiveUnit;
	std::vector<int> unitsKilledThisPass;

	size_t listSize = m_FiringUnitsList.size();
	for ( size_t c=0; c<listSize; c++ )
	{
		bool skylined = false;
		AirCavWeaponData *wpnData;
		char sName[32], sType[32], tName[32], tType[32];
		int length, FKN, FKNpercent, SUP, SUPpercent;

		// get the firing unit, make sure it is alive - it may have died in a previous pass
		int firingUnit = m_FiringUnitsList[c];
		if ( counterDataList[firingUnit]->getIsAlive() == DEAD )
		{
			// however, if the unit died in this pass, it can still fire
			bool killedThisPass = false;
			for ( auto u : unitsKilledThisPass )
			{
				if ( firingUnit == u )
					killedThisPass = true;
			}
			if ( killedThisPass == false )
				continue;
		}

		CString sightingUnitName = counterDataList[firingUnit]->getName();
		CString sightingUnitType = counterDataList[firingUnit]->getUnitInfo()->getName();
		int wpnOppFiring = counterDataList[firingUnit]->getWpnOppFiring();
		switch( wpnOppFiring )
		{
			case 0:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getMainWeapon1();
				FKN = counterDataList[firingUnit]->getFKN1() + wpnData->getOppFireMod();
				break;
			case 1:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getMainWeapon2();
				FKN = counterDataList[firingUnit]->getFKN2() + wpnData->getOppFireMod();
				break;
			case 2:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getMainWeapon3();
				FKN = counterDataList[firingUnit]->getFKN3() + wpnData->getOppFireMod();
				break;
			case 3:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getSecondaryWeapon1();
				FKN = counterDataList[firingUnit]->getFKN4() + wpnData->getOppFireMod();
				break;
			case 4:
				wpnData = counterDataList[firingUnit]->getUnitInfo()->getSecondaryWeapon2();
				FKN = counterDataList[firingUnit]->getFKN5() + wpnData->getOppFireMod();
				break;
		}

		// get the target information
		int tgt = counterDataList[firingUnit]->getTgtOppFiring();
		if ( tgt < 0 )
			continue;

		// cannot fire SAMs at nap-of-earth helicopters, only low level
		int wpnType = wpnData->getType();
		if ( wpnType == SAM )
		{
			if ( counterDataList[tgt]->getUnitInfo()->isHelicopter() )
			{
				if (counterDataList[tgt]->getHeloOffset() == NAP_OF_EARTH_METERS)
				{
					CString msgstr = (CString)"SAMs cannot be fired at nap-of-earth helicopters";
					MessageBox((LPCTSTR)msgstr);
					return lastActiveUnit;
				}
			}
		}

		// get target type modifier for guns and rockets only and only if not firing on helicopter (helicopters cannot be suppressed)
		int targetTypeModifier = 0;
		bool nonHelicopterTarget = !counterDataList[tgt]->getUnitInfo()->isHelicopter();
		if (nonHelicopterTarget)
		{
			switch (wpnOppFiring)
			{
			case 0:			// main weapon 1
				targetTypeModifier = counterDataList[firingUnit]->getTTModm1();
				break;
			case 1:			// main weapon 2
				targetTypeModifier = counterDataList[firingUnit]->getTTModm2();
				break;
			case 2:			// main weapon 3
				targetTypeModifier = counterDataList[firingUnit]->getTTModm3();
				break;
			case 3:			// secondary weapon 1
				targetTypeModifier = counterDataList[firingUnit]->getTTMods1();
				break;
			case 4:			// secondary weapon 2
				targetTypeModifier = counterDataList[firingUnit]->getTTMods2();
				break;
			}
		}

		// is helicopter firing
		bool isHelicopterFiring = counterDataList[firingUnit]->getUnitInfo()->isHelicopter();

		// is helicopter at nap of earth
		bool isNapOfEarth = (counterDataList[firingUnit]->getHeloOffset() == NAP_OF_EARTH_METERS);

		// determine if sighting unit is using Thermal Imaging (to see through smoke)
		bool usingTI = counterDataList[firingUnit]->getOpticsInUse() == OPTICS_THERMAL_IMAGER;

		// firing rocket from nap-of-earth helicopter is at half range
		if ( isHelicopterFiring && isNapOfEarth && wpnType == ROCKET )
		{
			// get the target unit's location
			int tgtCol = counterDataList[tgt]->getHexCol();
			int tgtRow = counterDataList[tgt]->getHexRow();

			// get the active unit's location
			int unitCol = counterDataList[firingUnit]->getHexCol();
			int unitRow = counterDataList[firingUnit]->getHexRow();

			char buffer[MAX_BUF_SIZE];
			int activeUnitOffset = counterDataList[firingUnit]->getElevOffset();
			int targetUnitOffset = counterDataList[tgt]->getElevOffset();
			int rocketRange = mapData->CalculateLOS( unitRow, unitCol, activeUnitOffset, usingTI, tgtRow, tgtCol, targetUnitOffset, skylined, false, buffer );
			int wpnAdjustedMaxRange = wpnData->getMaxRange() / 2;
			if ( rocketRange > wpnAdjustedMaxRange )
				return lastActiveUnit;
		}

		int nAmmoRemaining = 0;
		switch (wpnOppFiring)
		{
			case 0:			// main weapon 1
				nAmmoRemaining = counterDataList[firingUnit]->getAmmoMainWeapon1();
				break;
			case 1:			// main weapon 2
				nAmmoRemaining = counterDataList[firingUnit]->getAmmoMainWeapon2();
				break;
			case 2:			// main weapon 3
				nAmmoRemaining = counterDataList[firingUnit]->getAmmoMainWeapon3();
				break;
			case 3:			// secondary weapon 1
				nAmmoRemaining = counterDataList[firingUnit]->getAmmoSecondaryWeapon1();
				break;
			case 4:			// secondary weapon 2
				nAmmoRemaining = counterDataList[firingUnit]->getAmmoSecondaryWeapon2();
				break;
		}

		// Helicopter firing rockets at nap-of-earth are minus one
		if ( wpnType == ROCKET && isHelicopterFiring && isNapOfEarth )
			FKN--;

		// keep within 0 to 10
		if ( FKN < 0 ) FKN = 0;
		if ( FKN > 10 ) FKN = 10;

		// calculate final kill number, fudge it a little bit for randomness
		FKNpercent = FKN * 10;
		FKNpercent += ( rand() % 10 ) - 5.0;
		if ( FKNpercent < 0 ) FKNpercent = 0;
		if ( FKNpercent > 100 ) FKNpercent = 100;

		// suppression is FKN minus the target type modifier
		SUP = FKN - targetTypeModifier;

		// calculate suppression value
		SUPpercent = SUP * 10;
		SUPpercent += (rand() % 10) - 5.0;
		if (SUPpercent < 0) SUPpercent = 0;
		if (SUPpercent > 100) SUPpercent = 100;

		// only allow weapons of the selected pass to fire
		bool weaponCanSuppress = (wpnType != ATGM && wpnType != SAM);
		if ( wpnType == SAM ) wpnType = ROCKET;			// SAMs are treated as missiles
		if ( wpnType != firePass )
			continue;

		// decrement ammo
		int weaponType = wpnData->getType();
		if (weaponType == ATGM || weaponType == SAM || weaponType == ROCKET)
		{
			// expend the ammunition in the case of SAMS, Rockets and ATGMs
			if (nAmmoRemaining > 0)
			{
				switch (wpnOppFiring)
				{
				case 0:			// main weapon 1
					counterDataList[firingUnit]->setAmmoMainWeapon1(--nAmmoRemaining);
					break;
				case 1:			// main weapon 2
					counterDataList[firingUnit]->setAmmoMainWeapon2(--nAmmoRemaining);
					break;
				case 2:			// main weapon 3
					counterDataList[firingUnit]->setAmmoMainWeapon3(--nAmmoRemaining);
					break;
				case 3:			// secondary weapon 1
					counterDataList[firingUnit]->setAmmoSecondaryWeapon1(--nAmmoRemaining);
					break;
				case 4:			// secondary weapon 2
					counterDataList[firingUnit]->setAmmoSecondaryWeapon2(--nAmmoRemaining);
					break;
				}
			}
			else
			{
				CString msgstr = (CString)"The selected weapon is out of ammo!";
				MessageBox((LPCTSTR)msgstr);
			}
		}

		CString wpnName = wpnData->getName();

		length = sightingUnitName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			sName[i] = sightingUnitName.GetAt(i);						
		length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			sType[i] = wpnName.GetAt(i);

		CString tgtUnitName = counterDataList[tgt]->getName();
		CString tgtUnitType = counterDataList[tgt]->getUnitInfo()->getName();

		length = tgtUnitName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			tName[i] = tgtUnitName.GetAt(i);						
		length = tgtUnitType.GetLength() + 1;
		for (int i = 0; i < length; i++)
			tType[i] = tgtUnitType.GetAt(i);

		// display the information
		char buffer1[MAX_BUF_SIZE];
		sprintf_s( buffer1, "%s [%s] -> %s [%s]", sName, sType, tName, tType );
		char buffer2[MAX_BUF_SIZE];
		int result = IDCANCEL;
		if (nonHelicopterTarget && weaponCanSuppress)
		{
			sprintf_s(buffer2, "KILL: %d [%d%%]   SUP: %d [%d%%]", FKN, FKNpercent, SUP, SUPpercent);
			KillSuppressDialog dlg;
			dlg.setFKNText1((CString)buffer1);
			dlg.setFKNText2((CString)buffer2);
			result = dlg.DoModal();
		}
		else
		{
			sprintf_s(buffer2, "KILL: %d [%d%%]", FKN, FKNpercent);
			KillDialog dlg;
			dlg.setFKNText1((CString)buffer1);
			dlg.setFKNText2((CString)buffer2);
			result = dlg.DoModal();
		}

		if (result == IDOK2)
		{
			// suppress this unit
			counterDataList[tgt]->setIsSuppressed(TRUE);

			// infantry will go into defilade if suppressed
			if ( counterDataList[tgt]->getUnitInfo()->isInfantry() )
				counterDataList[tgt]->setDefilade(TRUE);
			sendUnitInfo(tgt);
		}
		else if ( result == IDOK )
		{
			// kill this unit
			counterDataList[tgt]->kill();
			sendUnitInfo(tgt);

			// kill any mounted units as well
			int numMountedUnits = counterDataList[tgt]->getNumberOfMountedUnits();
			for (int i=0; i<numMountedUnits; i++ )
			{
				int id = counterDataList[tgt]->getMountedUnit(i);
				if (!counterDataList[id]->getIsDismounted())
				{
					counterDataList[id]->kill();
					sendUnitInfo(id);
				}
			}

			// in the event of a kill, the firing unit becomes the active unit
			lastActiveUnit = firingUnit;

			// track killed units during this pass
			unitsKilledThisPass.push_back(tgt);
		}
	}
	return lastActiveUnit;
}

void CAirCavLOSDlg::OnBnClickedButtonResetFiring()
{
	// set Evasive Maneuver to false for all targets since combat has been resolved
	for ( int c=0; c<m_maxCounters; c++ )
		counterDataList[c]->setEvading(0);

	m_FiringUnitsList.clear();
	updateFiringUnits();
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedButtonResolveOppfire()
{
	resolveOpportunityFire();
	updateFiringUnits();
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedButtonLaysmoke()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;
	
	if (!counterDataList[m_ActiveUnit]->getIsDismounted())
	{
		MessageBox((CString)"A mounted unit cannot lay smoke", (CString)"Lay Smoke", MB_OK);
		return;
	}

	int col = counterDataList[m_ActiveUnit]->getHexCol();
	int row = counterDataList[m_ActiveUnit]->getHexRow();
	counterDataList[m_ActiveUnit]->laySmoke(mapData, col, row);
	m_vehicleSmokeHexList.Add( row, col );
	sendMapInfo(row, col);
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedCheckSmoke()
{
	int col = counterDataList[m_ActiveUnit]->getHexCol();
	int row = counterDataList[m_ActiveUnit]->getHexRow();
	int smokeExists = mapData->getSmoke(row, col);
	if (smokeExists)
	{
		m_artillerySmokeHexList.Remove(row, col);
		m_vehicleSmokeHexList.Remove(row, col);
		mapData->clearSmoke(row, col);
		sendMapInfo(row, col);
	}
	else
	{
		if (MessageBox((CString)"OK for Artillery Smoke, Cancel for Vehicle Smoke", (CString)"Smoke!", MB_OKCANCEL) == IDOK)
		{
			m_artillerySmokeHexList.Add(row, col);
			m_smoke = mapData->setSmoke(row, col, true, true);
		}
		else
		{
			m_vehicleSmokeHexList.Add(row, col);
			m_smoke = mapData->setSmoke(row, col, false, true);
		}
		sendMapInfo(row, col);
	}
	UpdateData(FALSE);
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedCheckWreck()
{
	int col = counterDataList[m_ActiveUnit]->getHexCol();
	int row = counterDataList[m_ActiveUnit]->getHexRow();
	int wreckExists = mapData->getWreck(row, col);
	if (wreckExists)
	{
		mapData->clearWreck(row, col);
	}
	else
	{
		m_wreck = mapData->setWreck(row, col, true);
	}
	sendMapInfo(row, col);
	UpdateData(FALSE);
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedButtonElevoffset()
{
	int col = counterDataList[m_ActiveUnit]->getHexCol();
	int row = counterDataList[m_ActiveUnit]->getHexRow();
	int curOffset = counterDataList[m_ActiveUnit]->getElevOffset() / -10;
	int activeUnitOffset = counterDataList[m_ActiveUnit]->checkContour(mapData, col, row, DIRECTION_UNDEF, curOffset);
	counterDataList[m_ActiveUnit]->setElevOffset(activeUnitOffset);
	UpdateData(FALSE);
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedButtonListUnits()
{
	int viewUnit = m_ActiveUnit < 0 ? 0 : m_ActiveUnit;

	UnitDetails dlg;
	dlg.setActiveUnit( viewUnit );
	dlg.setMaxUnits( m_maxCounters );
	dlg.setCounterData( counterDataList );

	dlg.setUnitNameText();
	dlg.setUnitTypeNameText();
	dlg.setUnitTypeText();
	dlg.setUnitCountryText();
	dlg.setUnitPositionText();
	dlg.setUnitStatusText();
	dlg.setActiveUnitPtr(&m_ActiveUnit);

	dlg.DoModal();

	UpdateData(FALSE);
	updateActiveUnit();
	updateFiringUnits();

	m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
}

void CAirCavLOSDlg::OnCbnSelchangeComboWeather()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	UpdateData(TRUE);

	if ( m_currentWeather == "Clear")
		m_Weather = WEATHER_CLEAR;
	else if ( m_currentWeather == "Light Fog")
		m_Weather = WEATHER_LT_FOG;
	else if ( m_currentWeather == "Heavy Fog")
		m_Weather = WEATHER_HVY_FOG;
	else if ( m_currentWeather == "Overcast")
		m_Weather = WEATHER_OVERCAST;
	else if ( m_currentWeather == "Rain")
		m_Weather = WEATHER_RAIN;
	else if ( m_currentWeather == "Snow")
		m_Weather = WEATHER_SNOW;

	sendWeather();
	updateActiveUnit();
}

void CAirCavLOSDlg::OnCbnSelchangeComboTimeOfDay()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	UpdateData(TRUE);

	if (m_currentTimeOfDay == "Day")
		m_TimeOfDay = TIME_DAY;
	else if (m_currentTimeOfDay == "Night")
		m_TimeOfDay = TIME_NIGHT;

	sendTimeOfDay();
	updateActiveUnit();
}

void CAirCavLOSDlg::OnCbnSelchangeComboOptics()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if (m_ActiveUnit >= 0)
	{
		UpdateData(TRUE);

		int sel = m_currentOpticsComboBox.GetCurSel();
		CString opticsName = _T("");
		m_currentOpticsComboBox.GetLBText(sel, opticsName);

		if (opticsName == "OS")
			m_Optics = OPTICS_OPTICAL_SIGHT;
		else if (opticsName == "TI" && counterDataList[m_ActiveUnit]->getUnitInfo()->hasTIsight())
			m_Optics = OPTICS_THERMAL_IMAGER;
		else if (opticsName == "ALE" && counterDataList[m_ActiveUnit]->getUnitInfo()->hasALEsight())
			m_Optics = OPTICS_AMBIENT_LIGHT_ENHANCER;
		else if (opticsName == "IRSL" && counterDataList[m_ActiveUnit]->getUnitInfo()->hasIRSLsight())
			m_Optics = OPTICS_INFRARED_SEARCHLIGHT;
		else if (opticsName == "WLSL" && counterDataList[m_ActiveUnit]->getUnitInfo()->hasWLSLsight())
			m_Optics = OPTICS_WHITELIGHT_SEARCHLIGHT;
		else
			m_Optics = OPTICS_OPTICAL_SIGHT;

		counterDataList[m_ActiveUnit]->setOpticsInUse(m_Optics);
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionIndfire()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	IndirectFire dlg;
	int smokeMission = 0;

	m_lastArtilleryRow = counterDataList[m_ActiveUnit]->getHexRow();
	m_lastArtilleryCol = counterDataList[m_ActiveUnit]->getHexCol();
	dlg.setIndirectFirePtr( &m_lastArtilleryUnit, &m_lastArtilleryRow, &m_lastArtilleryCol, &smokeMission );
	if ( dlg.DoModal() == IDOK )
	{
		int terrainModifier = 0;
		int thisTerrain = mapData->getTerrain( m_lastArtilleryRow, m_lastArtilleryCol );
		if ( thisTerrain == WOODS )
			terrainModifier = ArtilleryTypes[m_lastArtilleryUnit].Wds;
		else if ( thisTerrain == TOWN )
			terrainModifier = ArtilleryTypes[m_lastArtilleryUnit].Tn;

		// the type is also the number of hexes to spread (0, 1, or 2)
		int spread = ArtilleryTypes[m_lastArtilleryUnit].type;

		// scatter
		int scatterValue = ArtilleryTypes[m_lastArtilleryUnit].scatter_normal;
		bool didScatter = ( ( rand() % 6 ) <= scatterValue );
		if ( didScatter )
		{
			// calculate direction of scatter
			int sR, sC;
			int adjHex = ( rand() % 6 ) + 1;
			CalcAdj( adjHex, m_lastArtilleryRow, m_lastArtilleryCol, &sR, &sC );
			
			char scatterStr[128];
			sprintf_s( scatterStr, "Artillery Fire scattered to hex:  %02d%02d", sC, sR );
			MessageBox( (CString)scatterStr, (CString)"Scatter!", MB_OK );
			m_lastArtilleryRow = sR;
			m_lastArtilleryCol = sC;
		}

		// smoke mission, just set smoke in the hex(es) and bail
		if ( smokeMission )
		{
			char smokeStr[128];
			// set target hex
			mapData->setSmoke( m_lastArtilleryRow, m_lastArtilleryCol, true );
			sendMapInfo(m_lastArtilleryRow, m_lastArtilleryCol);
			m_artillerySmokeHexList.Add( m_lastArtilleryRow, m_lastArtilleryCol );
			sprintf_s( smokeStr, "Smoked hex:  %02d%02d", m_lastArtilleryCol, m_lastArtilleryRow );
			MessageBox( (CString)smokeStr, (CString)"Artillery Smoke", MB_OK );
			if ( spread )
			{
				int aR, aC;
				for ( int adjHex = DIRECTION_NO; adjHex <= DIRECTION_NW; adjHex++ )
				{
					// set adjacent hexes
					CalcAdj( adjHex, m_lastArtilleryRow, m_lastArtilleryCol, &aR, &aC );
					mapData->setSmoke( aR, aC, true);
					sendMapInfo(aR, aC);
					m_artillerySmokeHexList.Add( aR, aC );
					sprintf_s( smokeStr, "Smoked hex:  %02d%02d", aC, aR );
					MessageBox( (CString)smokeStr, (CString)"Artillery Smoke", MB_OK );
					if ( spread == 2 )
					{
						int aaR, aaC;
						for ( int nextAdjHex = DIRECTION_NO; nextAdjHex <= DIRECTION_NW; nextAdjHex++ )
						{
							// set adjacent hexes
							CalcAdj( nextAdjHex, aR, aC, &aaR, &aaC );
							mapData->setSmoke( aaR, aaC, true);
							sendMapInfo(aaR, aaC);
							m_artillerySmokeHexList.Add( aaR, aaC );
							sprintf_s( smokeStr, "Smoked hex:  %02d%02d", aaC, aaR );
							MessageBox( (CString)smokeStr, (CString)"Artillery Smoke", MB_OK );
						}
					}
				}
			}
			updateActiveUnit();
			return;
		}

		// build a list of hexes affected by this artillery strike
		RowCol hexList;
		hexList.Add( m_lastArtilleryRow, m_lastArtilleryCol );
		if ( spread )
		{
			int aR, aC;
			for ( int adjHex = DIRECTION_NO; adjHex <= DIRECTION_NW; adjHex++ )
			{
				// set adjacent hexes
				CalcAdj( adjHex, m_lastArtilleryRow, m_lastArtilleryCol, &aR, &aC );
				hexList.Add( aR, aC );
				if ( spread == 2 )
				{
					int aaR, aaC;
					for ( int nextAdjHex = DIRECTION_NO; nextAdjHex <= DIRECTION_NW; nextAdjHex++ )
					{
						// set adjacent hexes
						CalcAdj( nextAdjHex, aR, aC, &aaR, &aaC );
						hexList.Add( aaR, aaC );
					}
				}
			}
		}

		// loop through all hexes in the list
		for ( int n=0; n<hexList.GetNum(); n++ )
		{
			int targetRow, targetCol;
			hexList.Get( n, &targetRow, &targetCol );

			// apply adjacentHex modifier if this is not the target hex
			int adjModifier = 0;
			if ( !(m_lastArtilleryRow == targetRow && m_lastArtilleryCol == targetCol) )
				adjModifier = ArtilleryTypes[m_lastArtilleryUnit].AdjH;

			// loop through all counters, fire at those that are in this hex
			for ( int c=0; c<m_maxCounters; c++ )
			{
				int col = counterDataList[c]->getHexCol();
				int row = counterDataList[c]->getHexRow();
				if ( col == targetCol && row == targetRow )
				{
					// don't apply indirect fire to mounted units
					if (counterDataList[c]->getIsDismounted() == false)
						continue;

					int ttModifier = 0;

					AirCavUnitData *unitInfo = counterDataList[c]->getUnitInfo();
					TargetType tt = unitInfo->getTargetType();
					switch( tt )
					{
						case TT_A:
							ttModifier = ArtilleryTypes[m_lastArtilleryUnit].tt_A;
							break;
						case TT_B:
							ttModifier = ArtilleryTypes[m_lastArtilleryUnit].tt_B;
							break;
						case TT_C:
							ttModifier = ArtilleryTypes[m_lastArtilleryUnit].tt_C;
							break;
						case TT_D:
							ttModifier = ArtilleryTypes[m_lastArtilleryUnit].tt_D;
							break;
						case TT_E:
							ttModifier = ArtilleryTypes[m_lastArtilleryUnit].tt_E;
							break;
						case TT_F:
							ttModifier = ArtilleryTypes[m_lastArtilleryUnit].tt_F;
							break;
					}

					// targetModifer == 10 means apply -5 modifier, suppress instead of kill
					bool willSuppressNotKill = false;
					if ( ttModifier == 10 )
					{
						willSuppressNotKill = true;
						ttModifier = -5;
					}

					// terrainModifier == 10 means +1 on infantry, -1 vs all others
					if ( terrainModifier == 10 )
					{
						if ( unitInfo->isInfantry() )
							terrainModifier = 1;
						else
							terrainModifier = -1;
					}

					int FKN = 10 + terrainModifier + ttModifier + adjModifier;
					if ( FKN < 0 ) FKN = 0;
					if ( FKN > 10 ) FKN = 10;

					// calculate final kill number, fudge it a little bit for randomness
					int FKNpercent = FKN * 10;
					FKNpercent += ( rand() % 10 ) - 5.0;
					if ( FKNpercent < 0 ) FKNpercent = 0;
					if ( FKNpercent > 100 ) FKNpercent = 100;

					// get the target information
					CString tgtUnitName = counterDataList[c]->getName();
					CString tgtUnitType = counterDataList[c]->getUnitInfo()->getName();

					char tName[32], tType[32];
					int length = tgtUnitName.GetLength() + 1;
					for (int i = 0; i < length; i++)
						tName[i] = tgtUnitName.GetAt(i);						
					length = tgtUnitType.GetLength() + 1;
					for (int i = 0; i < length; i++)
						tType[i] = tgtUnitType.GetAt(i);

					// display the information
					char buffer1[MAX_BUF_SIZE];
					sprintf_s( buffer1, "%s -> %s [%s]", ArtilleryTypes[m_lastArtilleryUnit].name, tName, tType );
					char buffer2[MAX_BUF_SIZE];
					sprintf_s( buffer2, "FKN: %d [%d%%]", FKN, FKNpercent );

					// launch the appropriate dialog
					int result = IDCANCEL;
					if (willSuppressNotKill)
					{
						sprintf_s(buffer2, "KILL: %d [%d%%]   SUP: %d [%d%%]", 0, 0, FKN, FKNpercent);
						KillSuppressDialog dlg;
						dlg.setFKNText1((CString)buffer1);
						dlg.setFKNText2((CString)buffer2);
						result = dlg.DoModal();
					}
					else
					{
						sprintf_s(buffer2, "KILL: %d [%d%%]", FKN, FKNpercent);
						KillDialog dlg;
						dlg.setFKNText1((CString)buffer1);
						dlg.setFKNText2((CString)buffer2);
						result = dlg.DoModal();
					}

					// apply the results
					if (result == IDOK2)
					{
						// suppress this unit
						counterDataList[c]->setIsSuppressed(TRUE);

						// infantry will go into defilade if suppressed
						if ( counterDataList[c]->getUnitInfo()->isInfantry() )
							counterDataList[c]->setDefilade(TRUE);
						sendUnitInfo(c);

						// set suppressed any mounted units as well, but don't set them to defilade
						int numMountedUnits = counterDataList[c]->getNumberOfMountedUnits();
						for (int i = 0; i<numMountedUnits; i++)
						{
							int id = counterDataList[c]->getMountedUnit(i);
							if (counterDataList[id]->getIsDismounted() == false)
							{
								counterDataList[id]->setIsSuppressed(TRUE);
								sendUnitInfo(id);
							}
						}
					}
					else if ( result == IDOK )
					{
						// kill this unit
						counterDataList[c]->kill();
						sendUnitInfo(c);

						// kill any mounted units as well
						int numMountedUnits = counterDataList[c]->getNumberOfMountedUnits();
						for (int i=0; i<numMountedUnits; i++ )
						{
							int id = counterDataList[c]->getMountedUnit(i);
							if (!counterDataList[id]->getIsDismounted())
							{
								counterDataList[id]->kill();
								sendUnitInfo(id);
							}
						}
					}
					else
					{
						// its a miss, but check for automatic suppression
						if ( ArtilleryTypes[m_lastArtilleryUnit].auto_suppress == 1 )
						{
							// suppress this unit
							counterDataList[c]->setIsSuppressed(TRUE);

							// infantry will go into defilade if suppressed
							if ( counterDataList[c]->getUnitInfo()->isInfantry() )
								counterDataList[c]->setDefilade(TRUE);
							sendUnitInfo(c);

							// set suppressed any mounted units as well, but don't set them to defilade
							int numMountedUnits = counterDataList[c]->getNumberOfMountedUnits();
							for (int i = 0; i<numMountedUnits; i++)
							{
								int id = counterDataList[c]->getMountedUnit(i);
								if (counterDataList[id]->getIsDismounted() == false)
								{
									counterDataList[id]->setIsSuppressed(TRUE);
									sendUnitInfo(id);
								}
							}
						}
					}
				}
			}
		}

		UpdateData(FALSE);
		updateActiveUnit();
		updateFiringUnits();
	}
}

void CAirCavLOSDlg::OnBnClickedButtonRemoveSmoke()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	struct hex_loc {
		hex_loc(int r, int c) : row(r), col(c) {};
		int row;
		int col;
	};

	// remove vehicle smoke markers
	std::vector<hex_loc> removedVehicleSmokeHexes;
	for ( int n=0; n<m_vehicleSmokeHexList.GetNum(); n++ )
	{
		int smokeRow, smokeCol;
		m_vehicleSmokeHexList.Get( n, &smokeRow, &smokeCol );
		
		// 40% chance for vehicle smoke hex to be cleared
		char smokeStr[128];
		char titleStr[128];
		int chance = (rand() % 100);
		if ( chance < VEHICLE_SMOKE_DISPERSAL )
		{
			hex_loc hex(smokeRow, smokeCol);
			removedVehicleSmokeHexes.push_back(hex);
			sprintf_s(smokeStr, "*** Remove Smoke from hex:  %02d%02d ***", smokeCol, smokeRow);
			sprintf_s(titleStr, "Vehicle Smoke Removed [%02d]", chance);
			MessageBox( (CString)smokeStr, (CString)titleStr, MB_OK );
		}
		else
		{
			sprintf_s(smokeStr, "Smoke remains in hex:  %02d%02d", smokeCol, smokeRow);
			sprintf_s(titleStr, "Vehicle Smoke Remains [%02d]", chance);
			MessageBox((CString)smokeStr, (CString)titleStr, MB_OK);
		}
	}

	// clear out the vehicle smoke in each selected hex
	for (auto hex : removedVehicleSmokeHexes)
	{
		mapData->clearSmoke(hex.row, hex.col);
		m_vehicleSmokeHexList.Remove(hex.row, hex.col);
		sendMapInfo(hex.row, hex.col);
	}

	// remove artillery smoke markers
	std::vector<hex_loc> removedArtillerySmokeHexes;
	for (int n = 0; n<m_artillerySmokeHexList.GetNum(); n++)
	{
		int smokeRow, smokeCol;
		m_artillerySmokeHexList.Get(n, &smokeRow, &smokeCol);

		// 30% chance for artillery smoke hex to be cleared
		char smokeStr[128];
		char titleStr[128];
		int chance = (rand() % 100);
		if ( chance < ARTILLERY_SMOKE_DISPERSAL )
		{
			hex_loc hex(smokeRow, smokeCol);
			removedArtillerySmokeHexes.push_back(hex);
			sprintf_s(smokeStr, "*** Remove Smoke from hex:  %02d%02d ***", smokeCol, smokeRow);
			sprintf_s(titleStr, "Artillery Smoke Removed [%02d]", chance);
			MessageBox((CString)smokeStr, (CString)titleStr, MB_OK);
		}
		else
		{
			sprintf_s(smokeStr, "Smoke remains in hex:  %02d%02d", smokeCol, smokeRow);
			sprintf_s(titleStr, "Artillery Smoke Remains [%02d]", chance);
			MessageBox((CString)smokeStr, (CString)titleStr, MB_OK);
		}
	}

	// clear out the artillery smoke in each selected hex
	for (auto hex : removedArtillerySmokeHexes)
	{
		mapData->clearSmoke(hex.row, hex.col);
		m_artillerySmokeHexList.Remove(hex.row, hex.col);
		sendMapInfo(hex.row, hex.col);
	}
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedCheckPopSmoke()
{
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide) return;

	if ( counterDataList[m_ActiveUnit]->getUnitInfo()->isHelicopter() )
		MessageBox( (CString)"Helicopters cannot lay smoke", (CString)"Pop Smoke", MB_OK );
	else if (!counterDataList[m_ActiveUnit]->getIsDismounted())
		MessageBox((CString)"A mounted unit cannot pop smoke", (CString)"Pop Smoke", MB_OK);
	else
		UpdateData(TRUE);
}

bool CAirCavLOSDlg::readWeaponTextFile( FILE *fptr )
{
//  0                   1      2   3   4   5  6  7  8  9  10    11  12  13  14  15  16  17  18  19  20  21
//  name                type   M   M   0   5  10 15 20 25 30    A   B   C   D   E   F   W   T   O   M   A
//                             i   a   -   -   -  -  -  -  +                            d   n   p   v   e
//                             n   x   4   9  14 19 24 29                               s       F   T   T
	char 	name[32];
	char	wtype[10];
	int     min_range;
	int     max_range;
	int     range_1,range_2,range_3,range_4,range_5,range_6,range_7;
	int     target_A,target_B,target_C,target_D,target_E,target_F;
	int     wood_mod, town_mod, opp_fire_mod, moving_mod, aerial_mod;

	int w = 0;
	bool done = false;
	while( !done && !feof( fptr ) )
	{
		char lineStr[1024];
		char *bufPtr = fgets( lineStr, 1024, fptr );
		if ( lineStr[0] == '#' )
			continue;

		int ret = sscanf( lineStr, "%s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
						name, wtype, &min_range, &max_range,
						&range_1, &range_2, &range_3, &range_4, &range_5, &range_6, &range_7,
						&target_A, &target_B, &target_C, &target_D, &target_E, &target_F,
						&wood_mod, &town_mod, &opp_fire_mod, &moving_mod, &aerial_mod );

		if( ret == 22 )
		{
			replaceUnderscores( name );

			Weapon newWeapon;
			strncpy( newWeapon.name, name, 32 );

			if ( strstr( wtype, "GUN" ) ) newWeapon.type = GUN;
			else if ( strstr( wtype, "ATGM" ) ) newWeapon.type = ATGM;
			else if ( strstr( wtype, "SAM" ) ) newWeapon.type = SAM;
			else if ( strstr( wtype, "ROCKET" ) ) newWeapon.type = ROCKET;

			newWeapon.min_range = min_range;
			newWeapon.max_range = max_range;

			newWeapon.range_1 = range_1;
			newWeapon.range_2 = range_2;
			newWeapon.range_3 = range_3;
			newWeapon.range_4 = range_4;
			newWeapon.range_5 = range_5;
			newWeapon.range_6 = range_6;
			newWeapon.range_7 = range_7;

			newWeapon.target_A = target_A;
			newWeapon.target_B = target_B;
			newWeapon.target_C = target_C;
			newWeapon.target_D = target_D;
			newWeapon.target_E = target_E;
			newWeapon.target_F = target_F;

			newWeapon.wood_mod = wood_mod;
			newWeapon.town_mod = town_mod;
			newWeapon.opp_fire_mod = opp_fire_mod;
			newWeapon.moving_mod = moving_mod;
			newWeapon.aerial_mod = aerial_mod;

			weaponDataList[w++] = new AirCavWeaponData( newWeapon );

			if ( w == MAXWEAP )
				done = true;
		}
		else if ( ret == EOF )
			done = true;
	}
	m_maxWeapons = w;
	return true;
}

bool CAirCavLOSDlg::readUnitTextFile( FILE *fptr )
{
//  0         1       2        3        4        5        6      7    8    9   10  11  12  13  14  15  16 17  18   19   20 21
//  name      main 1  main 2   main 3   secn 1   secn 2   type   tt   evm  sm  dm  nm1 nm2 nm3 ns1 ns2 ti ale irsl wlsl ld r
	char		name[32];
	char		main1[32];
	char		main2[32];
	char		main3[32];
	char		secn1[32];
	char		secn2[32];
    char		type[32];
	char		armor[32];
	int			evm;
	int			sm;
	int			dm;
	int			nm1;
	int			nm2;
	int			nm3;
	int			ns1;
	int			ns2;
	int			ti;
	int			ale;
	int			irsl;
	int			wlsl;
	int			ld;
	int			radar;

	int u = 0;
	bool done = false;
	while( !done && !feof( fptr ) )
	{
		char lineStr[1024];
		char *bufPtr = fgets( lineStr, 1024, fptr );
		if ( lineStr[0] == '#' )
			continue;

		int ret = sscanf( lineStr, "%s %s %s %s %s %s %s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
							name, main1, main2, main3, secn1, secn2, type, armor,
							&evm, &sm, &dm, &nm1, &nm2, &nm3, &ns1, &ns2, &ti, &ale, &irsl, &wlsl, &ld, &radar);

		if( ret == 22 )
		{
			replaceUnderscores( name );
			replaceUnderscores( main1 );
			replaceUnderscores( main2 );
			replaceUnderscores( main3 );
			replaceUnderscores( secn1 );
			replaceUnderscores( secn2 );
			replaceUnderscores( type );

			Type newUnit;
			strncpy( newUnit.name, name, 32 );

			int main1idx = findWeaponIndex( main1 );
			int main2idx = findWeaponIndex( main2 );
			int main3idx = findWeaponIndex( main3 );
			int secn1idx = findWeaponIndex( secn1 );
			int secn2idx = findWeaponIndex( secn2 );

			if ( strstr( type, "TANK" ) ) newUnit.type = TANK;
			else if ( strstr( type, "TLAV" ) ) newUnit.type = TLAV;
			else if ( strstr( type, "WV" ) ) newUnit.type = WV;
			else if ( strstr( type, "UHH" ) ) newUnit.type = UHH;
			else if ( strstr( type, "UHM" ) ) newUnit.type = UHM;
			else if ( strstr( type, "ARH" ) ) newUnit.type = ARH;
			else if ( strstr( type, "LHX" ) ) newUnit.type = LHX;
			else if ( strstr( type, "INF" ) ) newUnit.type = INF;

			if ( strstr( armor, "TT_A" ) ) newUnit.armor = TT_A;
			else if ( strstr( armor, "TT_B" ) ) newUnit.armor = TT_B;
			else if ( strstr( armor, "TT_C" ) ) newUnit.armor = TT_C;
			else if ( strstr( armor, "TT_D" ) ) newUnit.armor = TT_D;
			else if ( strstr( armor, "TT_E" ) ) newUnit.armor = TT_E;
			else if ( strstr( armor, "TT_F" ) ) newUnit.armor = TT_F;

			newUnit.evm = evm;
			newUnit.sm = sm;
			newUnit.dm = dm;
			newUnit.ammo_m1 = nm1;
			newUnit.ammo_m2 = nm2;
			newUnit.ammo_m3 = nm3;
			newUnit.ammo_s1 = ns1;
			newUnit.ammo_s2 = ns2;
			newUnit.ti = ti;
			newUnit.ale = ale;
			newUnit.irsl = irsl;
			newUnit.wlsl = wlsl;
			newUnit.ld = ld;
			newUnit.radar = radar;

			unitDataList[u++] = new AirCavUnitData( newUnit, weaponDataList[main1idx], 
					weaponDataList[main2idx], weaponDataList[main3idx], 
					weaponDataList[secn1idx], weaponDataList[secn2idx]);

			if ( u == MAXTYPS )
				done = true;
		}
		else if ( ret == EOF )
			done = true;
	}
	m_maxUnits = u;
	return true;
}

FILE * CAirCavLOSDlg::openScenarioTextFile(std::string file_dir, int scen)
{
	char which[5];
	itoa(scen, which, 10);
	std::string whichFile = file_dir + (std::string)"scenario" + std::string(which) + (std::string)".txt";
	FILE *scenarioFile;
	errno_t err = fopen_s(&scenarioFile, whichFile.c_str(), "rt");
	if ( err != 0 || !scenarioFile )
	{
		CString msg("Unable to open ");
		msg += (CString)whichFile.c_str();
		MessageBox( msg, (CString)"Error", MB_OK );
		return NULL;
	}
	return scenarioFile;
}

bool CAirCavLOSDlg::readScenarioTextFile( FILE *fptr, int scen )
{
	const int numParameters = 10;

//  0         1       2        3        4     5     6      7       8       9
// name     side   country  unit type  col   row  elev     op   carrier  mounted
	char		name[32];		// unit name
	char		side[32];		// which side (0=BLUE, 1=RED)
	char		country[32];	// which country (0=US, 1=USSR)
	char		type[32];		// which type (0=T_72, etc)
	int			col;		 	// location on map
	int			row; 			// location on map
	int			elev;			// elevation offset
	float		op;				// OPs remaining
	char		carrier[32];	// unit mounting this one
	int			mounted;		// starts mounted

	int u = 0;
	bool done = false;
	bool first = true;
	while( !done && !feof( fptr ) )
	{
		char lineStr[1024];
		char *bufPtr = fgets( lineStr, 1024, fptr );
		if( ! bufPtr )
			done = true;

		if ( !done )
		{
			// first line must be name of scenario
			if ( first )
			{
				// remove the damn line feed
				char *lf = strrchr(lineStr, 0x0a);
				lineStr[lf-lineStr] = 0x00;
				strcpy(scenarioData->scenarioData[scen].scenarioname, lineStr);
				first = false;
				bufPtr = fgets( lineStr, 1024, fptr );
			}

			if ( lineStr[0] == '#' )
				continue;

			int ret = sscanf( lineStr, "%s %s %s %s %d %d %d %f %s %d", 
								name, side, country, type, &col, &row, &elev, &op, carrier, &mounted );

			if( ret == numParameters)
			{
				replaceUnderscores( name );
				replaceUnderscores( side );
				replaceUnderscores( country );
				replaceUnderscores( type );
				replaceUnderscores( carrier );

				UnitDef newUnit;
				newUnit.name = name;

				if ( strstr( side, "BLUE" ) ) newUnit.side = SIDE_BLUE;
				else if ( strstr( side, "RED" ) ) newUnit.side = SIDE_RED;
				else newUnit.side = SIDE_BLUE;
				
				if ( strstr( country, "US" ) ) newUnit.country = COUNTRY_US;
				else if ( strstr( country, "SOVIET" ) ) newUnit.country = COUNTRY_SOVIET;
				else if ( strstr( country, "GERMANY" ) ) newUnit.country = COUNTRY_GERMANY;
				else if ( strstr( country, "BRITAIN" ) ) newUnit.country = COUNTRY_BRITAIN;
				else if (strstr(country, "SYRIA")) newUnit.country = COUNTRY_SYRIA;
				else if (strstr(country, "ISRAEL")) newUnit.country = COUNTRY_ISRAEL;
				else newUnit.country = COUNTRY_US;

				newUnit.column = col;
				newUnit.row = row;
				newUnit.offset = elev;
				newUnit.op = op;
				newUnit.mounted = mounted;

				newUnit.type = findUnitIndex( type );
				if ( carrier != (CString)"none" )
					newUnit.mount = findScenarioUnitIndex( scen, carrier );
				else
					newUnit.mount = -1;

				if ( newUnit.country == COUNTRY_US )
					newUnit.name += " (US)";
				else if ( newUnit.country == COUNTRY_SOVIET )
					newUnit.name += " (SU)";
				else if ( newUnit.country == COUNTRY_GERMANY )
					newUnit.name += " (WG)";
				else if ( newUnit.country == COUNTRY_BRITAIN )
					newUnit.name += " (UK)";
				else if ( newUnit.country == COUNTRY_SYRIA )
					newUnit.name += " (SY)";
				else if ( newUnit.country == COUNTRY_ISRAEL )
					newUnit.name += " (IS)";

				scenarioData->scenarioData[scen].unit[u++] = newUnit;

				if ( u == MAXCOUNTERS )
					done = true;
			}
		}
	}
	scenarioData->scenarioData[scen].numunits = u;
	return true;
}

int CAirCavLOSDlg::findWeaponIndex( char *weaponName )
{
	for ( int w = 0; w < m_maxWeapons; w++ )
		if ( weaponDataList[w]->getName() == (CString)weaponName )
			return w;
	return -1;
}

int CAirCavLOSDlg::findUnitIndex( char *unitName )
{
	for ( int u = 0; u < m_maxUnits; u++ )
		if ( unitDataList[u]->getName() == (CString)unitName )
			return u;
	return -1;
}

int CAirCavLOSDlg::findScenarioUnitIndex( int scen, char *unitName )
{
	for (int u = 0; u < MAXCOUNTERS; u++)
	{
		CString name = (CString)unitName;
		if (scenarioData->scenarioData[scen].unit[u].country == COUNTRY_US)
			name += " (US)";
		else if (scenarioData->scenarioData[scen].unit[u].country == COUNTRY_SOVIET)
			name += " (SU)";
		else if (scenarioData->scenarioData[scen].unit[u].country == COUNTRY_GERMANY)
			name += " (WG)";
		else if (scenarioData->scenarioData[scen].unit[u].country == COUNTRY_BRITAIN)
			name += " (UK)";
		else if (scenarioData->scenarioData[scen].unit[u].country == COUNTRY_SYRIA)
			name += " (SY)";
		else if (scenarioData->scenarioData[scen].unit[u].country == COUNTRY_ISRAEL)
			name += " (IS)";
		if (scenarioData->scenarioData[scen].unit[u].name == name)
			return u;
	}
	return -1;
}

void CAirCavLOSDlg::replaceUnderscores( char *strPtr )
{
	for ( unsigned int i = 0; i < strlen(strPtr); i++ )
		if ( strPtr[i] == '_' ) strPtr[i] = ' ';
}

bool CAirCavLOSDlg::unitCannotBeSkylined(UnitType unitType, int terrain, int smoke)
{
	// skylined units requirements:
	// - helicopter or vehicle only (not infantry)
	// - in clear terrain
	// - not in smoke hex
	// - not during fog, rain or snow weather
	// - skylined unit must be equal or greater elevation (this is handled during LOS calculation)
	if (unitType == INF || terrain != CLEAR || smoke ||
		m_Weather == WEATHER_LT_FOG || m_Weather == WEATHER_HVY_FOG ||
		m_Weather == WEATHER_RAIN || m_Weather == WEATHER_SNOW)
	{
		return true;
	}
	return false;
}

void CAirCavLOSDlg::OnBnClickedButtonSaveProgress()
{
	if (scenarioData->saveScenario(file_dir, counterDataList))
	{
		MessageBox((CString)"In Progress scenario saved successfully", (CString)"Save", MB_OK);
	}
	else
	{
		MessageBox((CString)"In Progress scenario not saved", (CString)"Save", MB_OK);
	}
}

void CAirCavLOSDlg::OnBnClickedButtonAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CAirCavLOSDlg::OnBnClickedCheckIgnoreWater()
{
	mapData->setIgnoreWaterFeatures();
	updateActiveUnit();
}


void CAirCavLOSDlg::OnBnClickedCheckIgnoreAutobahn()
{
	mapData->setIgnoreAutobahn();
	updateActiveUnit();
}


void CAirCavLOSDlg::OnBnClickedCheckRotateMap()
{
	if (m_normalMap)
	{
		m_normalMap = false;
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_N)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_S)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_N2)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_S2)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE2)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW2)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE2)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW2)->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		m_normalMap = true;
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_N)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_S)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW)->ShowWindow(SW_SHOWNORMAL);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_N2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_S2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_NW2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SE2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_ACTION_MOVE_SW2)->ShowWindow(SW_HIDE);
	}
	updateActiveUnit();
}


void CAirCavLOSDlg::doUnitTracking(int previousOPs, int previousRow, int previousColumn)
{
	// reset if a different active unit or if the active unit has since changed
	if (m_unitTracking.unit != m_ActiveUnit || m_unitTracking.activeUnitChanged)
	{
		m_unitTracking.unit = m_ActiveUnit;
		int currentOPs = counterDataList[m_ActiveUnit]->getOPs();
		m_unitTracking.OPs = previousOPs - currentOPs;
		m_unitTracking.row = previousRow;
		m_unitTracking.column = previousColumn;
		m_unitTracking.activeUnitChanged = false;
	}
	else
	{
		// otherwise just update the number of OPs spent
		int currentOPs = counterDataList[m_ActiveUnit]->getOPs();
		m_unitTracking.OPs += (previousOPs - currentOPs);
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionPreviousMove()
{
	if ( m_unitTracking.unit > 0 && m_unitTracking.unit != m_ActiveUnit )
	{
		// check units are same side
		SideType followingSideType = counterDataList[m_unitTracking.unit]->getSideType();
		if (followingSideType != counterDataList[m_ActiveUnit]->getSideType())
		{
			char messageStr[128];
			CString sideTypeString = counterDataList[m_unitTracking.unit]->getUnitInfo()->getSideTypeString(followingSideType);
			sprintf_s(messageStr, "Following unit needs to be same side [%ls] as followed unit", sideTypeString.GetString());
			MessageBox((CString)messageStr, (CString)"Cannot Follow", MB_OK);
			return;
		}

		// check units are same type
		UnitType followingUnitType = counterDataList[m_unitTracking.unit]->getUnitInfo()->getUnitType();
		if (followingUnitType != counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType())
		{
			char messageStr[128];
			CString unitTypeString = counterDataList[m_unitTracking.unit]->getUnitInfo()->getUnitTypeString(followingUnitType);
			sprintf_s(messageStr, "Following unit needs to be same type [%ls] as followed unit", unitTypeString.GetString());
			MessageBox((CString)messageStr, (CString)"Cannot Follow", MB_OK);
			return;
		}

		// check units started from same location
		if (counterDataList[m_ActiveUnit]->getHexRow() != m_unitTracking.row ||	counterDataList[m_ActiveUnit]->getHexCol() != m_unitTracking.column )
		{
			char messageStr[128];
			sprintf_s(messageStr, "Following unit needs to start in same location [%02d%02d] as followed unit", m_unitTracking.column, m_unitTracking.row);
			MessageBox((CString)messageStr, (CString)"Cannot Follow", MB_OK);
			return;
		}

		// decrement the number of OPs that the tracked unit used
		if ( counterDataList[m_ActiveUnit]->decrOPs(m_unitTracking.OPs) != -1 )
		{
			// if successful, place the unit in the same position as the tracked unit
			int hexColumn = counterDataList[m_unitTracking.unit]->getHexCol();
			int hexRow = counterDataList[m_unitTracking.unit]->getHexRow();
			counterDataList[m_ActiveUnit]->moveTo(mapData, counterDataList, hexColumn, hexRow, false);

			// set same elevation offset
			int unitOffset = counterDataList[m_unitTracking.unit]->getElevOffset();
			counterDataList[m_ActiveUnit]->setElevOffset(unitOffset);

			// if the original unit has moved, so has this one
			if ( counterDataList[m_unitTracking.unit]->getMoved()) 
				counterDataList[m_ActiveUnit]->move();

			// and it it considered to have taken an action
			counterDataList[m_ActiveUnit]->setActionTaken(true);

			moveMountedUnits();
			updateActiveUnit();
		}
	}
}



void CAirCavLOSDlg::OnBnClickedCheckActiveRadarOn()
{
	OnBnClickedButtonActionRadar();
}


void CAirCavLOSDlg::OnBnClickedButtonConnect()
{
	// only the active side can switch
	if (s_thisActiveSide != BOTH && s_thisActiveSide != s_networkActiveSide)
	{
		MessageBox((CString)"Only Active side can switch sides", (CString)"Error", MB_OK);
		return;
	}

	// switch network sides
	if (s_networkActiveSide == SIDE_RED)
		s_networkActiveSide = SIDE_BLUE;
	else
		s_networkActiveSide = SIDE_RED;

	// tell the other side to switch
	char buffer[MAX_BUF_SIZE];
	sprintf_s(buffer, "switch");
	if (s_thisActiveSide == SIDE_BLUE)
		serverSend(buffer);
	else
		clientSend(buffer);

	// set active unit to the first one found on the active side
	for (int c = 0; c<m_maxCounters; c++)
	{
		SideType thisSide = counterDataList[c]->getSideType();
		if (thisSide == s_networkActiveSide)
		{
			m_ActiveUnit = c;
			break;
		}
	}
	m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
}


void CAirCavLOSDlg::setActiveUnit(int& id)
{
	m_ActiveUnit = id;
	m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
}


void CAirCavLOSDlg::switchSides()
{
	// switch active side
	if (s_networkActiveSide == SIDE_RED)
		s_networkActiveSide = SIDE_BLUE;
	else
		s_networkActiveSide = SIDE_RED;

	// set active unit to the first one found on the active side
	for (int c = 0; c<m_maxCounters; c++)
	{
		SideType thisSide = counterDataList[c]->getSideType();
		if (thisSide == s_networkActiveSide)
		{
			m_ActiveUnit = c;
			break;
		}
	}
	m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
}


