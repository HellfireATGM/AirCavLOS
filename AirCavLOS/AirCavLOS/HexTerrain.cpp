// HexTerrain.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "HexTerrain.h"


// HexTerrain dialog

IMPLEMENT_DYNAMIC(HexTerrain, CDialog)

HexTerrain::HexTerrain(CWnd* pParent /*=NULL*/)
	: CDialog(HexTerrain::IDD, pParent)
	, m_terrainType(0), m_elevType(0)
{

	EnableAutomation();

}

HexTerrain::HexTerrain(int x, int y)
	: CDialog(HexTerrain::IDD, NULL)
	, m_terrainType(0), m_elevType(0)
{
	m_row = x;
	m_col = y;
	m_edit = false;

	// MAP data
	m_road = 0;
	m_autobahn = 0;
	m_river = 0;
	m_contour = 0;
	m_roadN = 0;
	m_roadNE = 0;
	m_roadNW = 0;
	m_roadS = 0;
	m_roadSE = 0;
	m_roadSW = 0;
	m_autobahnN = 0;
	m_autobahnNE = 0;
	m_autobahnNW = 0;
	m_autobahnS = 0;
	m_autobahnSE = 0;
	m_autobahnSW = 0;
	m_riverN = 0;
	m_riverNE = 0;
	m_riverNW = 0;
	m_riverS = 0;
	m_riverSE = 0;
	m_riverSW = 0;
}

HexTerrain::HexTerrain(int x, int y, MAP map)
	: CDialog(HexTerrain::IDD, NULL)
	, m_terrainType(0), m_elevType(0)
{
	m_row = x;
	m_col = y;
	m_edit = true;

	// MAP data
	m_terrainType = map.terrain;
	m_elevType = map.elevation / 10;
	m_road = map.road;
	m_autobahn = map.autobahn;
	m_river = map.river;
	m_contour = map.contour;
	m_roadN = map.rn;
	m_roadNE = map.rne;
	m_roadNW = map.rnw;
	m_roadS = map.rs;
	m_roadSE = map.rse;
	m_roadSW = map.rsw;
	m_autobahnN = map.an;
	m_autobahnNE = map.ane;
	m_autobahnNW = map.anw;
	m_autobahnS = map.as;
	m_autobahnSE = map.ase;
	m_autobahnSW = map.asw;
	m_riverN = map.vn;
	m_riverNE = map.vne;
	m_riverNW = map.vnw;
	m_riverS = map.vs;
	m_riverSE = map.vse;
	m_riverSW = map.vsw;
}

HexTerrain::~HexTerrain()
{
}

void HexTerrain::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void HexTerrain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_TERRAIN_CLEAR, m_terrainType);
	DDX_Radio(pDX, IDC_RADIO_ELEV_0, m_elevType);
	DDX_Check(pDX, IDC_CHECK_ROAD, m_road);
	DDX_Check(pDX, IDC_CHECK_AUTOBAHN, m_autobahn);
	DDX_Check(pDX, IDC_CHECK_RIVER, m_river);
	DDX_Check(pDX, IDC_CHECK_CONTOUR, m_contour);
	DDX_Check(pDX, IDC_CHECK_R_N, m_roadN);
	DDX_Check(pDX, IDC_CHECK_R_NE, m_roadNE);
	DDX_Check(pDX, IDC_CHECK_R_NW, m_roadNW);
	DDX_Check(pDX, IDC_CHECK_R_S, m_roadS);
	DDX_Check(pDX, IDC_CHECK_R_SE, m_roadSE);
	DDX_Check(pDX, IDC_CHECK_R_SW, m_roadSW);
	DDX_Check(pDX, IDC_CHECK_A_N, m_autobahnN);
	DDX_Check(pDX, IDC_CHECK_A_NE, m_autobahnNE);
	DDX_Check(pDX, IDC_CHECK_A_NW, m_autobahnNW);
	DDX_Check(pDX, IDC_CHECK_A_S, m_autobahnS);
	DDX_Check(pDX, IDC_CHECK_A_SE, m_autobahnSE);
	DDX_Check(pDX, IDC_CHECK_A_SW, m_autobahnSW);
	DDX_Check(pDX, IDC_CHECK_V_N, m_riverN);
	DDX_Check(pDX, IDC_CHECK_V_NE, m_riverNE);
	DDX_Check(pDX, IDC_CHECK_V_NW, m_riverNW);
	DDX_Check(pDX, IDC_CHECK_V_S, m_riverS);
	DDX_Check(pDX, IDC_CHECK_V_SE, m_riverSE);
	DDX_Check(pDX, IDC_CHECK_V_SW, m_riverSW);
}

BOOL HexTerrain::OnInitDialog()
{
	CDialog::OnInitDialog();

	char buffer[32];
	if ( m_edit )
		sprintf_s( buffer, "Edit Hex: %02d%02d", m_col, m_row );
	else
		sprintf_s( buffer, "Unknown Hex: %02d%02d", m_col, m_row );

	CString msgstr = (CString)buffer;
	SetWindowText((LPCTSTR)msgstr);

	UpdateData(FALSE);

	return TRUE;
}

BEGIN_MESSAGE_MAP(HexTerrain, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ELEV_0, &HexTerrain::OnBnClickedRadioElev0)
	ON_BN_CLICKED(IDC_CHECK_ROAD, &HexTerrain::OnBnClickedCheckRoad)
	ON_BN_CLICKED(IDC_CHECK_AUTOBAHN, &HexTerrain::OnBnClickedCheckAutobahn)
	ON_BN_CLICKED(IDC_CHECK_RIVER, &HexTerrain::OnBnClickedCheckRiver)
	ON_BN_CLICKED(IDC_CHECK_CONTOUR, &HexTerrain::OnBnClickedCheckContour)
	ON_BN_CLICKED(IDC_CHECK_R_N, &HexTerrain::OnBnClickedCheckRN)
	ON_BN_CLICKED(IDC_CHECK_R_NW, &HexTerrain::OnBnClickedCheckRNw)
	ON_BN_CLICKED(IDC_CHECK_R_SW, &HexTerrain::OnBnClickedCheckRSw)
	ON_BN_CLICKED(IDC_CHECK_R_S, &HexTerrain::OnBnClickedCheckRS)
	ON_BN_CLICKED(IDC_CHECK_R_SE, &HexTerrain::OnBnClickedCheckRSe)
	ON_BN_CLICKED(IDC_CHECK_R_NE, &HexTerrain::OnBnClickedCheckRNe)
	ON_BN_CLICKED(IDC_CHECK_A_N, &HexTerrain::OnBnClickedCheckAN)
	ON_BN_CLICKED(IDC_CHECK_A_NW, &HexTerrain::OnBnClickedCheckANw)
	ON_BN_CLICKED(IDC_CHECK_A_SW, &HexTerrain::OnBnClickedCheckASw)
	ON_BN_CLICKED(IDC_CHECK_A_S, &HexTerrain::OnBnClickedCheckAS)
	ON_BN_CLICKED(IDC_CHECK_A_SE, &HexTerrain::OnBnClickedCheckASe)
	ON_BN_CLICKED(IDC_CHECK_A_NE, &HexTerrain::OnBnClickedCheckANe)
	ON_BN_CLICKED(IDC_CHECK_V_N, &HexTerrain::OnBnClickedCheckVN)
	ON_BN_CLICKED(IDC_CHECK_V_NW, &HexTerrain::OnBnClickedCheckVNw)
	ON_BN_CLICKED(IDC_CHECK_V_SW, &HexTerrain::OnBnClickedCheckVSw)
	ON_BN_CLICKED(IDC_CHECK_V_S, &HexTerrain::OnBnClickedCheckVS)
	ON_BN_CLICKED(IDC_CHECK_V_SE, &HexTerrain::OnBnClickedCheckVSe)
	ON_BN_CLICKED(IDC_CHECK_V_NE, &HexTerrain::OnBnClickedCheckVNe)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(HexTerrain, CDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_IHexTerrain to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {3729D868-BE48-4A46-BB4E-4B97808C1A75}
static const IID IID_IHexTerrain =
{ 0x3729D868, 0xBE48, 0x4A46, { 0xBB, 0x4E, 0x4B, 0x97, 0x80, 0x8C, 0x1A, 0x75 } };

BEGIN_INTERFACE_MAP(HexTerrain, CDialog)
	INTERFACE_PART(HexTerrain, IID_IHexTerrain, Dispatch)
END_INTERFACE_MAP()


// HexTerrain message handlers

int HexTerrain::getTerrain()
{
	return m_terrainType;
}
int HexTerrain::getElevation()
{
	return m_elevType * 10;
}

int HexTerrain::getRoad()
{
	return m_road;
}

int HexTerrain::getAutobahn()
{
	return m_autobahn;
}

int HexTerrain::getRiver()
{
	return m_river;
}

int HexTerrain::getContour()
{
	return m_contour;
}

int HexTerrain::getRoadHex(int which)
{
	switch( which )
	{
		case DIRECTION_NO: return m_roadN;
			break;
		case DIRECTION_NW: return m_roadNW;
			break;
		case DIRECTION_SW: return m_roadSW;
			break;
		case DIRECTION_SO: return m_roadS;
			break;
		case DIRECTION_SE: return m_roadSE;
			break;
		case DIRECTION_NE: return m_roadNE;
			break;
	}
	return 0;
}

int HexTerrain::getAutobahnHex(int which)
{
	switch( which )
	{
		case DIRECTION_NO: return m_autobahnN;
			break;
		case DIRECTION_NW: return m_autobahnNW;
			break;
		case DIRECTION_SW: return m_autobahnSW;
			break;
		case DIRECTION_SO: return m_autobahnS;
			break;
		case DIRECTION_SE: return m_autobahnSE;
			break;
		case DIRECTION_NE: return m_autobahnNE;
			break;
	}
	return 0;
}

int HexTerrain::getRiverHex(int which)
{
	switch( which )
	{
		case DIRECTION_NO: return m_riverN;
			break;
		case DIRECTION_NW: return m_riverNW;
			break;
		case DIRECTION_SW: return m_riverSW;
			break;
		case DIRECTION_SO: return m_riverS;
			break;
		case DIRECTION_SE: return m_riverSE;
			break;
		case DIRECTION_NE: return m_riverNE;
			break;
	}
	return 0;
}


void HexTerrain::OnBnClickedRadioElev0()
{
	// TODO: Add your control notification handler code here
}

void HexTerrain::OnBnClickedCheckRoad()
{
	UpdateData(TRUE);
}

void HexTerrain::OnBnClickedCheckAutobahn()
{
	UpdateData(TRUE);
}

void HexTerrain::OnBnClickedCheckRiver()
{
	UpdateData(TRUE);
}

void HexTerrain::OnBnClickedCheckContour()
{
	UpdateData(TRUE);
}

void HexTerrain::OnBnClickedCheckRN()
{
	UpdateData(TRUE);
	if ( m_roadN )
		m_road = 1;
	else if ( !m_roadN && !m_roadNW && !m_roadNE && !m_roadS && !m_roadSW && !m_roadSE )
		m_road = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckRNw()
{
	UpdateData(TRUE);
	if ( m_roadNW )
		m_road = 1;
	else if ( !m_roadN && !m_roadNW && !m_roadNE && !m_roadS && !m_roadSW && !m_roadSE )
		m_road = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckRSw()
{
	UpdateData(TRUE);
	if ( m_roadSW )
		m_road = 1;
	else if ( !m_roadN && !m_roadNW && !m_roadNE && !m_roadS && !m_roadSW && !m_roadSE )
		m_road = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckRS()
{
	UpdateData(TRUE);
	if ( m_roadS )
		m_road = 1;
	else if ( !m_roadN && !m_roadNW && !m_roadNE && !m_roadS && !m_roadSW && !m_roadSE )
		m_road = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckRSe()
{
	UpdateData(TRUE);
	if ( m_roadSE )
		m_road = 1;
	else if ( !m_roadN && !m_roadNW && !m_roadNE && !m_roadS && !m_roadSW && !m_roadSE )
		m_road = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckRNe()
{
	UpdateData(TRUE);
	if ( m_roadNE )
		m_road = 1;
	else if ( !m_roadN && !m_roadNW && !m_roadNE && !m_roadS && !m_roadSW && !m_roadSE )
		m_road = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckAN()
{
	UpdateData(TRUE);
	if ( m_autobahnN )
		m_autobahn = 1;
	else if ( !m_autobahnN && !m_autobahnNW && !m_autobahnNE && !m_autobahnS && !m_autobahnSW && !m_autobahnSE )
		m_autobahn = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckANw()
{
	UpdateData(TRUE);
	if ( m_autobahnNW )
		m_autobahn = 1;
	else if ( !m_autobahnN && !m_autobahnNW && !m_autobahnNE && !m_autobahnS && !m_autobahnSW && !m_autobahnSE )
		m_autobahn = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckASw()
{
	UpdateData(TRUE);
	if ( m_autobahnSW )
		m_autobahn = 1;
	else if ( !m_autobahnN && !m_autobahnNW && !m_autobahnNE && !m_autobahnS && !m_autobahnSW && !m_autobahnSE )
		m_autobahn = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckAS()
{
	UpdateData(TRUE);
	if ( m_autobahnS )
		m_autobahn = 1;
	else if ( !m_autobahnN && !m_autobahnNW && !m_autobahnNE && !m_autobahnS && !m_autobahnSW && !m_autobahnSE )
		m_autobahn = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckASe()
{
	UpdateData(TRUE);
	if ( m_autobahnSE )
		m_autobahn = 1;
	else if ( !m_autobahnN && !m_autobahnNW && !m_autobahnNE && !m_autobahnS && !m_autobahnSW && !m_autobahnSE )
		m_autobahn = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckANe()
{
	UpdateData(TRUE);
	if ( m_autobahnNE )
		m_autobahn = 1;
	else if ( !m_autobahnN && !m_autobahnNW && !m_autobahnNE && !m_autobahnS && !m_autobahnSW && !m_autobahnSE )
		m_autobahn = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckVN()
{
	UpdateData(TRUE);
	if ( m_riverN )
		m_river = 1;
	else if ( !m_riverN && !m_riverNW && !m_riverNE && !m_riverS && !m_autobahnSW && !m_riverSE )
		m_river = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckVNw()
{
	UpdateData(TRUE);
	if ( m_riverNW )
		m_river = 1;
	else if ( !m_riverN && !m_riverNW && !m_riverNE && !m_riverS && !m_riverSW && !m_riverSE )
		m_river = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckVSw()
{
	UpdateData(TRUE);
	if ( m_riverSW )
		m_river = 1;
	else if ( !m_riverN && !m_riverNW && !m_riverNE && !m_riverS && !m_riverSW && !m_riverSE )
		m_river = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckVS()
{
	UpdateData(TRUE);
	if ( m_riverS )
		m_river = 1;
	else if ( !m_riverN && !m_riverNW && !m_riverNE && !m_riverS && !m_riverSW && !m_riverSE )
		m_river = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckVSe()
{
	UpdateData(TRUE);
	if ( m_riverSE )
		m_river = 1;
	else if ( !m_riverN && !m_riverNW && !m_riverNE && !m_riverS && !m_riverSW && !m_riverSE )
		m_river = 0;
	UpdateData(FALSE);
}

void HexTerrain::OnBnClickedCheckVNe()
{
	UpdateData(TRUE);
	if ( m_riverNE )
		m_river = 1;
	else if ( !m_riverN && !m_riverNW && !m_riverNE && !m_riverS && !m_riverSW && !m_riverSE )
		m_river = 0;
	UpdateData(FALSE);
}
