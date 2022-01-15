// AirCavLOSDlg.cpp : implementation file
//

#include <stdio.h>
#include <stdlib.h>
#include <io.h>

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
#pragma warning(disable: 4244)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void CalcAdj (int dir, int cur_y, int cur_x, int *y, int *x);

char *TerrainStr[4] =
{
	"Clear", "Woods", "Town", "River"
};

static std::string file_dir = "C:\\AirCavLOS\\";
static std::string weapons_file = "weapons.txt";
static std::string units_file = "units.txt";

// --------------------------------------------------------------------------------------
#if 0
Weapon  Weapons [MAXWEAP] = 
{
// name                 t    M   M   0  5 10 15 20 25 30    A   B  C  D  E  F   W  T  O  M  A
//                      y    i   a   -  -  -  -  -  -  +                        d  n  p  v  e
//                      p    n   x   4  9 14 19 24 29                           s     F  T  T

"None",                 GUN, 0,  0,  0, 0, 0, 0, 0, 0, 0,   0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,		// None
"Soviet 125mm Gun",     GUN, 0, 30, 10,10,10, 9, 8, 6, 4,  -2, -1, 0, 0, 0, 0,  0,-1,-2,-3,-3,		// Sov125mm
"Soviet 12.7mm MG",     GUN, 0, 13, 10, 8, 6, 0, 0, 0, 0, -10,-10,-8,-7,-2,-1, -1,-2,-2,-1,-1,		// Sov12mm
"Soviet 73mm Gun",      GUN, 0, 13, 10, 8, 6, 0, 0, 0, 0,  -6, -4,-2,-1, 0, 0, -2,-2,-2,-3,-7,		// Sov73mm
"Soviet Sagger ATGM",  ATGM, 5, 30,  0, 9, 9, 9, 9, 8, 8,  -3, -2, 0, 0, 0, 0, -4,-2,-2,-2,-5,		// SaggerATGM
"Soviet RPG16 LAW",    ATGM, 0,  8, 10, 8, 0, 0, 0, 0, 0,  -6, -5,-3,-2, 0, 0, -1,-1,-1,-1,-3,		// SovRPG16
"Soviet 7.62mm MG",     GUN, 0,  8, 10, 7, 0, 0, 0, 0, 0, -10,-10,-9,-8,-2,-2, -2,-3,-2,-2,-2,		// Sov762mm
"Soviet 14.5mm MG",     GUN, 0, 13, 10, 8, 6, 0, 0, 0, 0, -10, -9,-5,-4,-2, 0, -1,-2,-1,-1,-1,		// Sov145mm
"Soviet 4x23mm Cannon", GUN, 0, 25, 10,10, 8, 7, 7, 6, 0,  -8, -6,-4,-3,-1, 0, -2,-2,-1,-2, 0,		// Sov23mm
"U.S. 120mm Gun",       GUN, 0, 32, 10,10,10,10, 9, 8, 6,  -2, -1, 0, 0, 0, 0, -1,-2,-1,-1,-3,		// US120mm
"U.S. 12.7mm MG",       GUN, 0, 13, 10, 8, 6, 0, 0, 0, 0, -10,-10,-8,-7,-2,-1, -1,-2,-2,-1,-1,		// US12mm
"U.S. 20mm Cannon",     GUN, 0, 20, 10,10,10, 8, 6, 0, 0,  -6, -5,-3,-2, 0, 0, -1,-2,-1, 0,-1,		// US20mm
"U.S. TOW ATGM",       ATGM, 1, 37, 8,10,10,10, 10, 9, 8,  -3, -1, 0, 0, 0, 0, -3,-2,-1,-1,-4,		// USTOW
"U.S. RGL",             GUN, 0,  3, 9, 0,  0, 0, 0, 0, 0, -10, -9,-7,-5,-2,-2, -2,-2,-2,-2,-5,		// USRGL
"U.S. 7.62mm MG",       GUN, 0,  8, 10, 7, 0, 0, 0, 0, 0, -10,-10,-9,-8,-2,-2, -2,-3,-2,-2,-2,		// US762mm
"Soviet 115mm Gun",     GUN, 0, 30, 10,10, 8, 5, 3, 2, 1,  -3, -1, 0, 0, 0, 0, -1,-1,-2,-4,-4,		// Sov115mm
"Minigun MG",           GUN, 0, 10, 10, 9, 7, 0, 0, 0, 0,  -9, -9,-6,-4,-1, 0, -1,-3,-1,-1,-2,		// Minigun
"Stinger AAM",         ATGM, 1, 39, 10,10,10,10, 9, 8, 8,   0,  0, 0,-3,-2, 0,  0, 0,-1, 0, 0,		// Stinger
"Soviet 30mm GL",       GUN, 0, 15, 10, 9, 7, 5, 0, 0, 0,  -9, -7,-3,-2,-1, 0, -1,-3,-2,-1,-3,		// Sov30mm
"U.S. 30mm Cannon",     GUN, 0, 20, 10,10,10, 9, 7, 0, 0,  -6, -5,-3,-1,-1, 0, -1,-2,-1, 0,-1,		// US30mm
"Hellfire ATGM",       ATGM, 1, 50,  9,10,10,10,10,10, 9,  -2, -1, 0, 0, 0, 0, -2,-2,-1, 0,-4,		// Hellfire
"ECAS Rockets",      ROCKET, 4, 40,  0, 9,10,10,10, 9, 9,  -4, -3,-2,-1, 0, 0, -1,-2,-2, 0, 0,		// ECAS
"U.S. M-16 AR",         GUN, 0,  3,  9, 0, 0, 0, 0, 0, 0, -10,-10,-9,-6,-2,-2, -3,-4,-1,-2, 0,		// USM16AR
"U.S. M72 LAW",        ATGM, 0,  3,  9, 0, 0, 0, 0, 0, 0,  -9, -7,-5,-4,-2,-1, -2,-2,-1,-2,-3,		// M72LAW
"Soviet AKS AR",        GUN, 0,  3,  9, 0, 0, 0, 0, 0, 0, -10,-10,-9,-6,-2,-2, -3,-4,-1,-2, 0,		// SovAKSAR
"Soviet SA-7 SAM",      SAM, 2, 35,  8,10,10, 9, 8, 7, 7,   0,  0, 0,-3,-2, 0,  0, 0,-2, 0, 0,		// SovSA7SAM
"Soviet SA-14 SAM",     SAM, 3, 60,  4, 8,10,10,10, 9, 9,   0,  0, 0,-1, 0, 0,  0, 0,-1, 0, 0,		// SovSA14SAM
"Soviet RPG18 LAW",    ATGM, 0,  3,  9, 0, 0, 0, 0, 0, 0,  -9, -7,-5,-4,-2,-1, -2,-2,-1,-2,-3,		// SovRPG18
"U.S. Dragon ATGM",    ATGM, 0,  3,  9, 0, 0, 0, 0, 0, 0,  -9, -7,-5,-4,-2,-1, -2,-2,-1,-2,-3,		// Dragon
"Soviet SA-9 SAM",      SAM, 1, 70,  8,10,10,10,10, 9, 8,   0,  0, 0,-1, 0, 0,  0, 0,-1, 0, 0,		// SovSA9SAM
"Soviet SA-13 SAM",     SAM, 1, 80,  8,10,10,10,10,10, 9,   0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0,		// SovSA13SAM
"U.S. 6x23mm Cannon",   GUN, 0, 16, 10, 9, 8, 7, 0, 0, 0,  -8, -6,-3,-1, 0, 0, -1,-1,-2,-2, 0		// US6x23mmCan
};
#endif

#if 0
Type    Types[MAXTYPS] =
{
//     name               main 1       main 2      main 3    secn 1      secn 2    type   tt   evm  sm  dm rkt atgm

	{ "T-72 MBT",         Sov125mm,    None,       None,     Sov12mm,    None,     TANK,  TT_A, -2, -2, -3, 0,  0 },	// T_72
    { "BMP-1 APC",        Sov73mm,     SaggerATGM, None,     Sov762mm,   None,     TLAV,  TT_D, -2, -3, -3, 0,  8 },	// BMP1
	{ "Soviet Inf Squad", SovAKSAR,    SovRPG16,   Sov762mm, SovSA7SAM,  SovRPG18, INF,   TT_E,  0, -5, -5, 0,  8 },	// SINF
	{ "BRDM Recon",       Sov145mm,    None,       None,     Sov762mm,   None,     WV,    TT_E, -3, -2, -3, 0,  0 },	// BRDM
	{ "ZSU-23 SPAAG",     Sov23mm,     None,       None,     Sov762mm,   None,     TLAV,  TT_D, -2,  0, -2, 0,  0 },	// ZSU23
	{ "M1A1 MBT",         US120mm,     None,       None,     US12mm,     None,     TANK,  TT_A, -3, -1, -4, 0,  0 },	// M1A1
	{ "AH-1S Cobra",      USTOW,       ECAS,       None,     US20mm,     None,     ARH,   TT_E, -3,  0,  0, 4,  4 },	// AH1S
	{ "UH-60 Blackhawk",  US762mm,     None,       None,     None,       None,     UHM,   TT_E, -3,  0,  0, 0,  0 },	// UH60
	{ "U.S. Inf Squad",   USM16AR,     USRGL,      US762mm,  M72LAW,     None,     INF,   TT_E,  0, -5, -5, 0,  8 },	// UINF
	{ "OH-58 Kiowa",      US762mm,     None,       None,     None,       None,     ARH,   TT_E, -4,  0,  0, 0,  0 },	// OH58
	{ "T-62 MBT",         Sov115mm,    None,       None,     Sov12mm,    None,     TANK,  TT_B, -1, -3, -3, 0,  0 },	// T_62
	{ "M2 Bradley APC",   US20mm,      None,       None,     USTOW,      None,     TLAV,  TT_C, -3, -2, -4, 0,  8 },	// M2CV
	{ "M3 Bradley APC",   US20mm,      None,       None,     USTOW,      None,     TLAV,  TT_C, -3, -2, -4, 0,  8 },	// M3CV
	{ "AHIP Scout",       Minigun,     None,       None,     Stinger,    None,     ARH,   TT_E, -4,  0,  0, 0,  2 },	// AHIP
	{ "Mi-6 Hook",        None,        None,       None,     Sov12mm,    None,     UHH,   TT_E, -1,  0,  0, 0,  0 },	// MI_6
	{ "Mi-8 Hip",         SaggerATGM,  None,       None,     Sov30mm,    None,     UHM,   TT_E, -1,  0,  0, 0,  4 },	// MI_8
	{ "Mi-24 Hind",       SaggerATGM,  None,       None,     Sov30mm,    None,     ARH,   TT_D, -2,  0,  0, 4,  4 },	// MI_24
    { "BMD APC",          Sov73mm,     None,       None,     SaggerATGM, None,     TLAV,  TT_D, -3, -3, -3, 0,  8 },	// BMD
	{ "AH-64 Apache",     Hellfire,    ECAS,       None,     US30mm,     None,     ARH,   TT_D, -3,  0,  0, 4,  4 },	// AH64
	{ "U.S. Inf Team",    Dragon,      Stinger,    None,     USM16AR,    None,     INF,   TT_F,  0, -5, -6, 0,  0 },	// USTM
	{ "Soviet Inf Team",  SaggerATGM,  SovSA14SAM, None,     SovAKSAR,   None,     INF,   TT_F,  0, -6, -5, 0,  8 },	// SVTM
	{ "BRDM SPSAM",       SovSA9SAM,   None,       None,     Sov762mm,   None,     WV,    TT_E, -2, -1, -3, 0,  8 },	// SPSAM
	{ "BRDM ATGM Carrier",SaggerATGM,  None,       None,     Sov762mm,   None,     WV,    TT_E, -3, -1, -3, 0,  8 },	// SPATGM
	{ "Vulcan SPAAG",     US6x23mmCan, None,       None,     US762mm,    None,     TLAV,  TT_D, -3, -3, -4, 0,  0 }		// VULCAN
};
#endif

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
	m_lastArtilleryUnit = 0;
	m_lastArtilleryRow = 0;
	m_lastArtilleryCol = 0;

	m_shutdown = false;
}

void CAirCavLOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNITS, m_AvailableUnitsListBox);
	DDX_Control(pDX, IDC_LIST_SIGHTED_UNITS, m_SightedUnitsListBox);
	DDX_Control(pDX, IDC_LIST_SIGHTING_UNITS, m_SightingUnitsListBox);
	DDX_Control(pDX, IDC_LIST_FIRING_UNITS, m_FiringUnitsListBox);
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
	DDX_Check(pDX, IDC_CHECK_DEBUG, m_debugLOSMessages);
	DDX_Check(pDX, IDC_CHECK_DEBUG_PK, m_debugFKNMessages);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_LOWLEVEL, m_activeUnitLowLevel);
	DDX_Check(pDX, IDC_CHECK_ACTIVE_SUPPRESSED, m_activeUnitIsSuppressed);
	DDX_CBString(pDX, IDC_COMBO_WEATHER, m_currentWeather);
	DDX_Check(pDX, IDC_CHECK_POP_SMOKE, m_popSmokeWhileMoving);
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
	ON_BN_CLICKED(IDC_BUTTON_ACTION_INDFIRE, &CAirCavLOSDlg::OnBnClickedButtonActionIndfire)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_SMOKE, &CAirCavLOSDlg::OnBnClickedButtonRemoveSmoke)
	ON_BN_CLICKED(IDC_CHECK_POP_SMOKE, &CAirCavLOSDlg::OnBnClickedCheckPopSmoke)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PROGRESS, &CAirCavLOSDlg::OnBnClickedButtonSaveProgress)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CAirCavLOSDlg::OnBnClickedButtonAbout)
	ON_BN_CLICKED(IDC_CHECK_IGNORE_WATER, &CAirCavLOSDlg::OnBnClickedCheckIgnoreWater)
	ON_BN_CLICKED(IDC_CHECK_IGNORE_AUTOBAHN, &CAirCavLOSDlg::OnBnClickedCheckIgnoreAutobahn)
	ON_BN_CLICKED(IDC_CHECK_ROTATE_MAP, &CAirCavLOSDlg::OnBnClickedCheckRotateMap)
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
	if ( dlg.DoModal() == IDOK )
		scenarioToPlay = dlg.getScenario();

	m_maxCounters = scenarioData->getScenario( scenarioToPlay, counterDataList, unitDataList );
	SetWindowText((LPCTSTR)scenarioData->getName( scenarioToPlay ));

	// initialization of available counters
	m_ActiveUnit = -1;
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
			UnitType unitType = counterDataList[c]->getUnitInfo()->getUnitType();
			if (unitType != INF)
			{
				int hexColumn = counterDataList[c]->getHexCol();
				int hexRow = counterDataList[c]->getHexRow();
				mapData->setWreck(hexRow, hexColumn);
			}
		}
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
		char msgbox[128];
		if ( mapData )
		{
			mapData->SaveAndCloseMapDataFile(msgbox);
			MessageBox( (CString)msgbox, (CString)"Map Progress", MB_OK );
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
	// pre-perform Popup to get other units opportunity to fire
	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);

		// only for Attack Helicopters
		UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
		if ( unitType == ARH || unitType == LHX )
		{
			if ( counterDataList[m_ActiveUnit]->getHeloOffset() > 0 )
			{
				counterDataList[m_ActiveUnit]->setHeloOffset(mapData, counterDataList, 0, true);
				counterDataList[m_ActiveUnit]->setIsPopUp(false);
			}
			else
			{
				counterDataList[m_ActiveUnit]->setHeloOffset(mapData, counterDataList, 30, true);
				counterDataList[m_ActiveUnit]->setIsPopUp(true);
			}
			updateActiveUnit();
		}
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionFireGun()
{
	int wpnType, FKN, FKNpercent, SUP, SUPpercent;
	bool popup = false;
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
	bool isHelicopterFiring = false;
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if ( unitType == ARH || unitType == UHH || unitType == UHM || unitType == LHX )
		isHelicopterFiring = true;

	// helicopter at low level
	bool isNapOfEarth = false;
	if ( counterDataList[m_ActiveUnit]->getHeloOffset() != 0 )
		isNapOfEarth = true;

	// get target type modifier for guns and rockets only and only if not firing on helicopter (helicopters cannot be suppressed)
	int targetTypeModifier = 0;
	UnitType targetUnitType = counterDataList[tgt]->getUnitInfo()->getUnitType();
	bool nonHelicopterTarget = targetUnitType != ARH && targetUnitType != UHH && targetUnitType != UHM && targetUnitType != LHX;
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

	// make sure there is a line of sight for helicopters - it might need to do a popup first
	if (isHelicopterFiring && !isNapOfEarth)
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
		int range = mapData->CalculateLOS(activeUnitHexRow, activeUnitHexColumn, activeUnitOffset,
			tgtRow, tgtCol, targetUnitOffset, buffer);

		if (range == 0)
		{
			CString msgstr = (CString)"Active unit cannot fire on target without first doing a popup!";
			MessageBox((LPCTSTR)msgstr);
			return;
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
		int rocketRange = mapData->CalculateLOS( unitRow, unitCol, targetUnitOffset, tgtRow, tgtCol, activeUnitOffset, buffer );
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
		UnitType tgtunitType = counterDataList[tgt]->getUnitInfo()->getUnitType();
		if ( tgtunitType == ARH || tgtunitType == UHH || tgtunitType == UHM || tgtunitType == LHX )
			if ( counterDataList[tgt]->getHeloOffset() == 0 )
			{
				CString msgstr = (CString)"Cannot fire SAM at a nap-of-earth helicopter!";
				MessageBox((LPCTSTR)msgstr);
				return;
			}
	}

	// figure out if there enough OPs and enough ammo to fire the selected weapon
	int succeeded = 0;
	if ( popup )
		succeeded = counterDataList[m_ActiveUnit]->popupFire();
	else if ( wpnType == GUN )
		succeeded = counterDataList[m_ActiveUnit]->fireGun();
	else if ( wpnType == ATGM || wpnType == SAM || wpnType == ROCKET )
	{
		// expend the ammunition in the case of SAMS, Rockets and ATGMs
		if (nAmmoRemaining > 0 )
		{
			if ( succeeded = counterDataList[m_ActiveUnit]->fireMissile() )
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
		}
	}
	if ( succeeded == 0 )
		return;

	// fire at all units in this hex
	size_t listSize = tgtUnitsList.size();
	for ( size_t u=0; u<listSize; u++ )
	{
		int t = tgtUnitsList[u];

		// cannot fire Missiles on Infantry
		UnitType tgtUnitClass = counterDataList[t]->getUnitInfo()->getUnitType();
		if ( tgtUnitClass == INF && wpnType == ATGM )
		{
			CString msgstr = (CString)"Cannot fire missiles on infantry!";
			MessageBox((LPCTSTR)msgstr);
			continue;
		}

		// allow the target unit to Evade
		int EVMmod = 0;
		if ( !counterDataList[t]->getEvading() && wpnType != ROCKET )
		{
			CString tgtUnitName = counterDataList[t]->getName();

			char tName[32];
			int length = tgtUnitName.GetLength() + 1;
			for (int i = 0; i < length; i++)
				tName[i] = tgtUnitName.GetAt(i);						

			// display the information
			char buffer1[MAX_BUF_SIZE];
			sprintf_s( buffer1, "Is the Unit \"%s\" going to Evade?", tName );
			if ( MessageBox((CString)buffer1, _T("Evasive Maneuver"), MB_YESNO) == IDYES )
				EVMmod = counterDataList[t]->evasiveManeuver(FALSE);
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
			if (targetUnitType == INF)
				counterDataList[tgt]->setDefilade(TRUE);
		}
		else if (result == IDOK)
		{
			// kill this unit
			counterDataList[t]->kill();
			// kill any mounted units as well
			int numMountedUnits = counterDataList[t]->getNumberOfMountedUnits();
			for (int i=0; i<numMountedUnits; i++ )
			{
				int id = counterDataList[t]->getMountedUnit(i);
				if ( !counterDataList[id]->getIsDismounted() )
					counterDataList[id]->kill();
			}
			// if the target was a vehicle, add a wreck marker to the target hex
			UnitType tgtUnitType = counterDataList[t]->getUnitInfo()->getUnitType();
			if (tgtUnitType != INF)
			{
				m_wreck = mapData->setWreck(tgtRow, tgtCol);
			}
		}

		// set Evasive Maneuver to false
		if ( EVMmod )
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

void CAirCavLOSDlg::OnBnClickedButtonActionMoveN()
{
	if (m_ActiveUnit < 0) return;
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if (unitType == INF && counterDataList[m_ActiveUnit]->getIsSuppressed())
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}
	counterDataList[m_ActiveUnit]->moveNorth(mapData, counterDataList, m_popSmokeWhileMoving);
	moveMountedUnits();
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
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if (unitType == INF && counterDataList[m_ActiveUnit]->getIsSuppressed())
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}
	counterDataList[m_ActiveUnit]->moveNorthWest(mapData, counterDataList, m_popSmokeWhileMoving);
	moveMountedUnits();
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
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if (unitType == INF && counterDataList[m_ActiveUnit]->getIsSuppressed())
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}
	counterDataList[m_ActiveUnit]->moveSouthWest(mapData, counterDataList, m_popSmokeWhileMoving);
	moveMountedUnits();
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
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if (unitType == INF && counterDataList[m_ActiveUnit]->getIsSuppressed())
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}
	counterDataList[m_ActiveUnit]->moveSouth(mapData, counterDataList, m_popSmokeWhileMoving);
	moveMountedUnits();
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
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if (unitType == INF && counterDataList[m_ActiveUnit]->getIsSuppressed())
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}
	counterDataList[m_ActiveUnit]->moveSouthEast(mapData, counterDataList, m_popSmokeWhileMoving);
	moveMountedUnits();
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
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if (unitType == INF && counterDataList[m_ActiveUnit]->getIsSuppressed())
	{
		CString msgstr = (CString)"Suppressed infantry cannot move!";
		MessageBox((LPCTSTR)msgstr);
		return;
	}
	counterDataList[m_ActiveUnit]->moveNorthEast(mapData, counterDataList, m_popSmokeWhileMoving);
	moveMountedUnits();
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
			m_ActiveUnit = c;
			break;
		}
	}
	
	// reset a couple of things
	m_activeUnitWeapon = 0;
	m_popSmokeWhileMoving = false;

	// disable pop smoke button for Helicopters
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if ( unitType == ARH || unitType == UHH || unitType == UHM || unitType == LHX )
		GetDlgItem(IDC_CHECK_POP_SMOKE)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CHECK_POP_SMOKE)->EnableWindow(TRUE);

	// update the dialog
	UpdateData(FALSE);
	updateActiveUnit();
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
				if ( !counterDataList[m_ActiveUnit]->getEvading() && wpnType != GUN )
				{
					CString tgtUnitName = counterDataList[m_ActiveUnit]->getName();

					char tName[32];
					int length = tgtUnitName.GetLength() + 1;
					for (int i = 0; i < length; i++)
						tName[i] = tgtUnitName.GetAt(i);						

					// display the information
					char buffer1[MAX_BUF_SIZE];
					sprintf_s( buffer1, "Is the Unit \"%s\" going to Evade?", tName );
					if ( MessageBox((CString)buffer1, _T("Evasive Maneuver"), MB_YESNO) == IDYES )
						EVMmod = counterDataList[m_ActiveUnit]->evasiveManeuver(FALSE);
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
					m_ActiveUnit = c;

					m_FiringUnitsList.push_back(m_ActiveUnit);
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

void CAirCavLOSDlg::updateActiveUnit(bool rebuildList)
{
	LPCTSTR STR_NOE = _T("Nap Of Earth");
	LPCTSTR STR_LL = _T("Low Level");
	LPCTSTR STR_MOUNT = _T("Mount");
	LPCTSTR STR_DISMOUNT = _T("Dismount");
	LPCTSTR STR_POPUP = _T("Pop Up");
	LPCTSTR STR_POPDOWN = _T("Pop Down");

	if ( m_ActiveUnit >= 0 )
	{
		// Active Unit info
		char buffer[MAX_BUF_SIZE];
		m_activeUnitName = counterDataList[m_ActiveUnit]->getName();

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
		sprintf_s( buffer, "%2.1f", counterDataList[m_ActiveUnit]->getOPs() );
		m_activeUnitOPs = (CString)buffer;

		// specific Unit Info
		m_activeUnitTypeName = counterDataList[m_ActiveUnit]->getUnitInfo()->getName();
		m_activeUnitFired = counterDataList[m_ActiveUnit]->getFired();
		m_activeUnitMoved = counterDataList[m_ActiveUnit]->getMoved();
		m_activeUnitEvading = counterDataList[m_ActiveUnit]->getEvading();
		m_activeUnitInDefilade = counterDataList[m_ActiveUnit]->getDefilade();
		m_activeUnitMounted = counterDataList[m_ActiveUnit]->getIsCarriedBy();
		m_activeUnitDismounted = counterDataList[m_ActiveUnit]->getIsDismounted();
		m_activeUnitLowLevel = ( counterDataList[m_ActiveUnit]->getHeloOffset() > 0 ) ? 1 : 0;
		m_activeUnitLowLevel = (counterDataList[m_ActiveUnit]->getHeloOffset() > 0) ? 1 : 0;
		m_activeUnitIsSuppressed = counterDataList[m_ActiveUnit]->getIsSuppressed();

		if ( m_activeUnitMounted >= 0 && !m_activeUnitDismounted )
		{
			SetDlgItemText(IDC_BUTTON_ACTION_MOUNT, STR_DISMOUNT);
			// mounted units cannot move themselves
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
		m_activeSide = counterDataList[m_ActiveUnit]->getUnitInfo()->getSideTypeString(activeSideType);;

		// unit type
		UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
		m_activeUnitUnitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitTypeString(unitType);

		// only helicopters have the option for low level
		if ( unitType != ARH && unitType != UHH && unitType != UHM && unitType != LHX )
		{
			GetDlgItem(IDC_CHECK_ACTIVE_LOWLEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ACTION_LOWLEVEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ACTIVE_SUPPRESSED)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CHECK_ACTIVE_LOWLEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ACTION_LOWLEVEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_ACTIVE_SUPPRESSED)->EnableWindow(FALSE);
		}

		// only attack/recon helicopters have the option for popups
		if ( unitType != ARH && unitType != LHX )
			GetDlgItem(IDC_BUTTON_ACTION_PASS)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_BUTTON_ACTION_PASS)->EnableWindow(TRUE);

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
			if (unitType == ARH || unitType == LHX)
				SetDlgItemText(IDC_BUTTON_ACTION_PASS, STR_POPDOWN);
			SetDlgItemText(IDC_BUTTON_ACTION_LOWLEVEL, STR_NOE);
		}
		else
		{
			if (unitType == ARH || unitType == LHX)
				SetDlgItemText(IDC_BUTTON_ACTION_PASS, STR_POPUP);
			SetDlgItemText(IDC_BUTTON_ACTION_LOWLEVEL, STR_LL);
		}

		// actual elevation = elev of terrain + offset on contour + low level for helos
		int activeUnitElev = elev + activeUnitOffset;
		_itoa_s(counterDataList[m_ActiveUnit]->getElevOffset(), buffer, 10);
		m_activeUnitOffset = (CString)buffer;
		_itoa_s(activeUnitElev, buffer, 10);
		m_activeUnitActualElev = (CString)buffer;

		// only bother with Line of Sight if the active unit is alive
		if (counterDataList[m_ActiveUnit]->getIsAlive())
		{
			// calculate range from active unit to all other enemy units (these are SIGHTED)
			int numPopupSightings = 0;
			std::map<int, std::string> popupSightedUnits;
			for (int c = 0; c < m_maxCounters; c++)
			{
				if (c != m_ActiveUnit)
				{
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
						int range = mapData->CalculateLOS(activeUnitHexRow, activeUnitHexColumn, activeUnitOffset,
							targetHexRow, targetHexColumn, targetUnitOffset, buffer);

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
							UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();

							// this calculation is only required for an active unit attack helicopter at nap-of-earth observing a target
							if (activeUnitHeloOffset == 0 && (unitType == ARH || unitType == LHX))
							{
								//  recalculate calculate line-of-sight assuming a pop-up
								range = mapData->CalculateLOS(activeUnitHexRow, activeUnitHexColumn, activeUnitOffset + 30,
									targetHexRow, targetHexColumn, targetUnitOffset, buffer);

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
						if (targetFired)
						{
							// a unit can only be observed from 5 hexes away if it is in smoke hex and not low level and it has fired
							if (!targetlowLevel && targetSmoke && range > 5)
							continue;
						}
						else
						{
							// if it has not fired, a unit can only be observed from 1 hex away if it is in smoke hex and not low level
							if (!targetlowLevel && targetSmoke && range > 1)
							continue;
						}

						// ignore smoke in the active hex if it is a helo at low level
						if (activelowLevel && activeSmoke)
							activeSmoke = 0;
						
						int ttModm1 = 0, ttModm2 = 0, ttModm3 = 0, ttMods1 = 0, ttMods2 = 0;
						if (range && counterDataList[c]->isVisible(targetTerrain, range, targetlowLevel, m_Weather, activeSmoke))
						{
							//  calculate final kill number
							int oppfire = counterDataList[m_ActiveUnit]->getIsOppFiring();
							int supfire = counterDataList[m_ActiveUnit]->getIsSuppressed() ? counterDataList[m_ActiveUnit]->getUnitInfo()->getSM() : 0;
							int mFKN1 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								MAIN1, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, buffer, ttModm1);
							if (mFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN2 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								MAIN2, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, buffer, ttModm2);
							if (mFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN3 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								MAIN3, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, buffer, ttModm3);
							if (mFKN3 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN1 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								SECONDARY1, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, buffer, ttMods1);
							if (sFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN2 = counterDataList[m_ActiveUnit]->getUnitInfo()->CalculateFKN(
								SECONDARY2, counterDataList[c], targetTerrain, targetSmoke, range, oppfire, supfire, buffer, ttMods2);
							if (sFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + unitName + _T(" to ") + targetName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							// only update FKN numbers for the active target
							if (m_ActiveTarget == c)
								counterDataList[m_ActiveUnit]->setFinalKillNumbers(mFKN1, mFKN2, mFKN3, sFKN1, sFKN2, ttModm1, ttModm2, ttModm3, ttMods1, ttMods2);

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
							if (popupRange)
							{
								numPopupSightings++;
								sprintf_s(buffer, "%s [%s] [POPUP]  Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
									sName, sType, targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								popupSightedUnits.insert(std::make_pair(c, buffer));
							}
							else
							{
								sprintf_s(buffer, "%s [%s]  Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
									sName, sType, targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
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
						int targetUnitHexColumn = counterDataList[s.first]->getHexCol();
						int targetUnitHexRow = counterDataList[s.first]->getHexRow();
						int targetUnitOffset = counterDataList[s.first]->getElevOffset();
						int targetUnitHeloOffset = counterDataList[s.first]->getHeloOffset();
						targetUnitOffset += targetUnitHeloOffset;
						int friendlyUnitHexColumn = counterDataList[c]->getHexCol();
						int friendlyUnitHexRow = counterDataList[c]->getHexRow();
						int friendlyUnitOffset = counterDataList[c]->getElevOffset();
						int friendlyUnitHeloOffset = counterDataList[c]->getHeloOffset();
						friendlyUnitOffset += friendlyUnitHeloOffset;

						int range = mapData->CalculateLOS(friendlyUnitHexRow, friendlyUnitHexColumn, friendlyUnitOffset,
							targetUnitHexRow, targetUnitHexColumn, targetUnitOffset, buffer);
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
				if (c != m_ActiveUnit)
				{
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
						int range = mapData->CalculateLOS(targetHexRow, targetHexColumn, targetUnitOffset, 
							activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, buffer);

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
							UnitType unitType = counterDataList[c]->getUnitInfo()->getUnitType();

							// this calculation is only required for attack helicopters at nap-of-earth observing the active unit
							if (targetUnitHeloOffset <= 0 && (unitType == ARH || unitType == LHX))
							{
								//  recalculate calculate line-of-sight assuming a pop-up
								range = mapData->CalculateLOS(targetHexRow, targetHexColumn, targetUnitOffset + 30,
									activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, buffer);

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
						if (activeFired)
						{
							// a unit can only be observed from 5 hexes away if it is in smoke hex and not low level and it has fired
							if (!activelowLevel && activeSmoke && range > 5)
							continue;
						}
						else
						{
							// if it has not fired, a unit can only be observed from 1 hex away if it is in smoke hex and not low level
							if (!activelowLevel && activeSmoke && range > 1)
							continue;
						}

						// ignore smoke in the target hex if it is a helo at low level
						if (targetlowLevel && targetSmoke)
							targetSmoke = 0;

						int ttModm1 = 0, ttModm2 = 0, ttModm3 = 0, ttMods1 = 0, ttMods2 = 0;
						int oppfire = counterDataList[c]->getIsOppFiring();
						int supfire = counterDataList[c]->getIsSuppressed() ? counterDataList[c]->getUnitInfo()->getSM() : 0;
						if (range && counterDataList[m_ActiveUnit]->isVisible(activeTerrain, range, activelowLevel, m_Weather, activeSmoke))
						{
							//  calculate final kill number
							int mFKN1 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								MAIN1, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, buffer, ttModm1);
							if (mFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN2 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								MAIN2, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, buffer, ttModm2);
							if (mFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int mFKN3 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								MAIN3, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, buffer, ttModm3);
							if (mFKN3 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN1 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								SECONDARY1, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, buffer, ttMods1);
							if (sFKN1 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							int sFKN2 = counterDataList[c]->getUnitInfo()->CalculateFKN(
								SECONDARY2, counterDataList[m_ActiveUnit], activeTerrain, activeSmoke, range, oppfire, supfire, buffer, ttMods2);
							if (sFKN2 >= 0 && m_debugFKNMessages)
							{
								CString msgstr = (CString)buffer;
								if (MessageBox((CString)msgstr, _T("Debug FKN from ") + targetName + _T(" to ") + unitName, MB_OKCANCEL) == IDCANCEL)
									m_debugFKNMessages = FALSE;
							}

							counterDataList[c]->setFinalKillNumbers(mFKN1, mFKN2, mFKN3, sFKN1, sFKN2, ttModm1, ttModm2, ttModm3, ttMods1, ttMods2);

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
							if (popupRange)
							{
								numPopupSightings++;
								sprintf_s(buffer, "%s [%s] [POPUP]  Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
									sName, sType, targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
								popupSightingUnits.insert(std::make_pair(c, buffer));
							}
							else
							{
								sprintf_s(buffer, "%s [%s]  Hex: %02d%02d  Range: %d   m1: %d   m2: %d   m3: %d   s1: %d   s2: %d",
									sName, sType, targetHexColumn, targetHexRow, range, mFKN1, mFKN2, mFKN3, sFKN1, sFKN2);
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
							int activeUnitHexColumn = counterDataList[m_ActiveUnit]->getHexCol();
							int activeUnitHexRow = counterDataList[m_ActiveUnit]->getHexRow();
							int enemyUnitHexColumn = counterDataList[c]->getHexCol();
							int enemyUnitHexRow = counterDataList[c]->getHexRow();
							int enemyUnitOffset = counterDataList[c]->getElevOffset();
							int enemyUnitHeloOffset = counterDataList[c]->getHeloOffset();
							enemyUnitOffset += enemyUnitHeloOffset;

							int range = mapData->CalculateLOS(enemyUnitHexRow, enemyUnitHexColumn, enemyUnitOffset,
								activeUnitHexRow, activeUnitHexColumn, activeUnitOffset, buffer);
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
		}

		UpdateData(FALSE);
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
	if ( m_ActiveUnit >= 0 )
	{
		counterDataList[m_ActiveUnit]->reset();
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnEnChangeEditActiveLoc()
{
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
	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);
		counterDataList[m_ActiveUnit]->evasiveManeuver();
		updateActiveUnit();
	}
}

void CAirCavLOSDlg::OnBnClickedButtonActionDefilade()
{
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
	if (m_ActiveUnit >= 0)
	{
		UpdateData(TRUE);
		if (counterDataList[m_ActiveUnit]->getIsSuppressed())
		{
			UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
			if (unitType == TANK || unitType == TLAV)
			{
				if (counterDataList[m_ActiveUnit]->decrOPs(2) != -1)
					counterDataList[m_ActiveUnit]->setIsSuppressed(0);
			}
			else if (unitType == WV || unitType == INF)
			{
				if (counterDataList[m_ActiveUnit]->decrOPs(3) != -1)
					counterDataList[m_ActiveUnit]->setIsSuppressed(0);
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
	if ( m_ActiveUnit >= 0 )
	{
		UpdateData(TRUE);

		// only for Helicopters
		UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
		if ( unitType == ARH || unitType == UHH || unitType == UHM || unitType == LHX )
		{
			if ( counterDataList[m_ActiveUnit]->getHeloOffset() > 0 )
				counterDataList[m_ActiveUnit]->setHeloOffset(mapData, counterDataList, 0);
			else
				counterDataList[m_ActiveUnit]->setHeloOffset(mapData, counterDataList, 30);
			updateActiveUnit();
		}
	}
}


void CAirCavLOSDlg::resolveOpportunityFire()
{
	// first gun armed weapons, then ATGMs
	resolveFirePass(GUN);
	resolveFirePass(ATGM);

	size_t listSize = m_FiringUnitsList.size();
	for ( size_t c=0; c<listSize; c++ )
	{
		// clear opportunity fire
		int firingUnit = m_FiringUnitsList[c];
		counterDataList[firingUnit]->setIsOppFiring(FALSE);
		counterDataList[firingUnit]->setEvading(0);
	}

	updateActiveUnit();
	m_AvailableUnitsListBox.SetCurSel(m_ActiveUnit);
}

void CAirCavLOSDlg::resolveFirePass(int firePass)
{
	size_t listSize = m_FiringUnitsList.size();
	for ( size_t c=0; c<listSize; c++ )
	{
		AirCavWeaponData *wpnData;
		char sName[32], sType[32], tName[32], tType[32];
		int length, FKN, FKNpercent, SUP, SUPpercent;

		// get the firing unit, make sure it is alive - it may have died in a previous pass
		int firingUnit = m_FiringUnitsList[c];
		if ( counterDataList[firingUnit]->getIsAlive() == DEAD && firePass == ATGM )
			continue;

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
			UnitType tgtunitType = counterDataList[tgt]->getUnitInfo()->getUnitType();
			if ( tgtunitType == ARH || tgtunitType == UHH || tgtunitType == UHM || tgtunitType == LHX )
				if (counterDataList[tgt]->getHeloOffset() == 0 && firePass == ATGM)
				{
					CString msgstr = (CString)"SAMs cannot be fired at nap-of-earth helicopters";
					MessageBox((LPCTSTR)msgstr);
					return;
				}
		}

		// get target type modifier for guns and rockets only and only if not firing on helicopter (helicopters cannot be suppressed)
		int targetTypeModifier = 0;
		UnitType targetUnitType = counterDataList[tgt]->getUnitInfo()->getUnitType();
		bool nonHelicopterTarget = targetUnitType != ARH && targetUnitType != UHH && targetUnitType != UHM && targetUnitType != LHX;
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
		bool isHelicopterFiring = false;
		UnitType unitType = counterDataList[firingUnit]->getUnitInfo()->getUnitType();
		if ( unitType == ARH || unitType == UHH || unitType == UHM || unitType == LHX )
			isHelicopterFiring = true;

		// helicopter at low level
		bool isNapOfEarth = false;
		if ( counterDataList[firingUnit]->getHeloOffset() == 0 )
			isNapOfEarth = true;

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
			int rocketRange = mapData->CalculateLOS( unitRow, unitCol, targetUnitOffset, tgtRow, tgtCol, activeUnitOffset, buffer );
			int wpnAdjustedMaxRange = wpnData->getMaxRange() / 2;
			if ( rocketRange > wpnAdjustedMaxRange )
				return;
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
		if ( wpnType == ROCKET ) wpnType = ATGM;		// ROCKETs are treated as missiles
		if ( wpnType == SAM ) wpnType = ATGM;			// SAMs are treated as missiles
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
			if (targetUnitType == INF)
				counterDataList[tgt]->setDefilade(TRUE);
		}
		else if ( result == IDOK )
		{
			// kill this unit
			counterDataList[tgt]->kill();
			// kill any mounted units as well
			int numMountedUnits = counterDataList[tgt]->getNumberOfMountedUnits();
			for (int i=0; i<numMountedUnits; i++ )
			{
				int id = counterDataList[tgt]->getMountedUnit(i);
				if ( !counterDataList[id]->getIsDismounted() )
					counterDataList[id]->kill();
			}
			// firing unit becomes the active unit
			m_ActiveUnit = firingUnit;
		}
	}
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
   if (!counterDataList[m_ActiveUnit]->getIsDismounted())
   {
      MessageBox((CString)"A mounted unit cannot lay smoke", (CString)"Lay Smoke", MB_OK);
      return;
   }

	int col = counterDataList[m_ActiveUnit]->getHexCol();
	int row = counterDataList[m_ActiveUnit]->getHexRow();
	counterDataList[m_ActiveUnit]->laySmoke(mapData, col, row);
	m_vehicleSmokeHexList.Add( row, col );
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedCheckSmoke()
{
	int col = counterDataList[m_ActiveUnit]->getHexCol();
	int row = counterDataList[m_ActiveUnit]->getHexRow();
	m_smoke = mapData->setSmoke(row, col, true);

	if (MessageBox((CString)"OK for Artillery Smoke, Cancel for Vehicle Smoke", (CString)"Smoke!", MB_OKCANCEL) == IDOK)
		m_artillerySmokeHexList.Add( row, col );
	else
		m_vehicleSmokeHexList.Add( row, col );

	UpdateData(FALSE);
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedCheckWreck()
{
	int col = counterDataList[m_ActiveUnit]->getHexCol();
	int row = counterDataList[m_ActiveUnit]->getHexRow();
	m_wreck = mapData->setWreck(row, col, true);
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

	updateActiveUnit();

}

void CAirCavLOSDlg::OnBnClickedButtonActionIndfire()
{
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
			mapData->setSmoke( m_lastArtilleryRow, m_lastArtilleryCol );
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
					mapData->setSmoke( aR, aC );
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
							mapData->setSmoke( aaR, aaC );
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
						UnitType unitType = unitInfo->getUnitType();
						if ( unitType == INF )
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
						UnitType targetUnitType = counterDataList[c]->getUnitInfo()->getUnitType();
						if (targetUnitType == INF)
							counterDataList[c]->setDefilade(TRUE);

						// set suppressed any mounted units as well, but don't set them to defilade
						int numMountedUnits = counterDataList[c]->getNumberOfMountedUnits();
						for (int i = 0; i<numMountedUnits; i++)
						{
							int id = counterDataList[c]->getMountedUnit(i);
							if (counterDataList[id]->getIsDismounted() == false)
								counterDataList[id]->setIsSuppressed(TRUE);
						}
					}
					else if ( result == IDOK )
					{
						// kill this unit
						counterDataList[c]->kill();

						// kill any mounted units as well
						int numMountedUnits = counterDataList[c]->getNumberOfMountedUnits();
						for (int i=0; i<numMountedUnits; i++ )
						{
							int id = counterDataList[c]->getMountedUnit(i);
							if ( !counterDataList[id]->getIsDismounted() )
								counterDataList[id]->kill();
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
							UnitType targetUnitType = counterDataList[c]->getUnitInfo()->getUnitType();
							if (targetUnitType == INF)
								counterDataList[c]->setDefilade(TRUE);

							// set suppressed any mounted units as well, but don't set them to defilade
							int numMountedUnits = counterDataList[c]->getNumberOfMountedUnits();
							for (int i = 0; i<numMountedUnits; i++)
							{
								int id = counterDataList[c]->getMountedUnit(i);
								if (counterDataList[id]->getIsDismounted() == false)
									counterDataList[id]->setIsSuppressed(TRUE);
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
		if ( chance < 40 )
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
		if ( chance < 30)
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
	}
	updateActiveUnit();
}

void CAirCavLOSDlg::OnBnClickedCheckPopSmoke()
{
	UnitType unitType = counterDataList[m_ActiveUnit]->getUnitInfo()->getUnitType();
	if ( unitType == ARH || unitType == UHH || unitType == UHM || unitType == LHX )
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
//  0         1       2        3        4        5        6      7    8    9   10  11  12  13  14  15
//  name      main 1  main 2   main 3   secn 1   secn 2   type   tt   evm  sm  dm  nm1 nm2 nm3 ns1 ns2
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

	int u = 0;
	bool done = false;
	while( !done && !feof( fptr ) )
	{
		char lineStr[1024];
		char *bufPtr = fgets( lineStr, 1024, fptr );
		if ( lineStr[0] == '#' )
			continue;

		int ret = sscanf( lineStr, "%s %s %s %s %s %s %s %s %d %d %d %d %d %d %d %d", 
							name, main1, main2, main3, secn1, secn2, type, armor,
							&evm, &sm, &dm, &nm1, &nm2, &nm3, &ns1, &ns2);

		if( ret == 16 )
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
