// IndirectFire.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavCommon.h"
#include "AirCavLOS.h"
#include "IndirectFire.h"


// IndirectFire dialog

IMPLEMENT_DYNAMIC(IndirectFire, CDialog)

IndirectFire::IndirectFire(CWnd* pParent /*=NULL*/)
	: CDialog(IndirectFire::IDD, pParent)
	, m_targetHex(_T("1744"))
	, m_indirectType(_T("U.S. 81mm Mortar"))
{
	m_type = 0;
	m_row = 0;
	m_col = 0;
}

IndirectFire::~IndirectFire()
{
}

void IndirectFire::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_INDIRECT, m_indirectType);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_LOC, m_targetHex);
	DDX_Check(pDX, IDC_CHECK_SMOKE_MISSION, m_smokeMission);
}


BEGIN_MESSAGE_MAP(IndirectFire, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_INDIRECT, &IndirectFire::OnCbnSelchangeComboIndirect)
	ON_EN_KILLFOCUS(IDC_EDIT_ACTIVE_LOC, &IndirectFire::OnEnChangeEditActiveLoc)
	ON_BN_CLICKED(IDC_CHECK_SMOKE_MISSION, &IndirectFire::OnBnClickedCheckSmokeMission)
END_MESSAGE_MAP()


// IndirectFire message handlers

void IndirectFire::OnCbnSelchangeComboIndirect()
{
	UpdateData(TRUE);

	//U.S. 81mm Mortar;U.S. 4.2" Mortar;U.S. 160mm Mortar;U.S. 105mm Howitzer;U.S. 155mm Howitzer;U.S. 203mm Rocket;Soviet 120mm Mortar;Soviet 122mm Howitzer;Soviet 152mm Howitzer;Soviet 122mm Rocket

	int indirectType = 0;
	if ( m_indirectType == "U.S. 81mm Mortar")
		indirectType = US81MM_MORTAR;
	else if ( m_indirectType == "U.S. 4.2\" Mortar")
		indirectType = US42INCH_MORTAR;
	else if ( m_indirectType == "U.S. 160mm Mortar")
		indirectType = US160MM_MORTAR;
	else if ( m_indirectType == "U.S. 105mm Howitzer")
		indirectType = US105MM_HOWITZ;
	else if ( m_indirectType == "U.S. 155mm Howitzer")
		indirectType = US155MM_HOWITZ;
	else if ( m_indirectType == "U.S. 203mm Rocket")
		indirectType = US203MM_ROCKET;
	else if ( m_indirectType == "Soviet 120mm Mortar")
		indirectType = SOVIET120MM_MORTAR;
	else if ( m_indirectType == "Soviet 122mm Howitzer")
		indirectType = SOVIET122MM_HOWITZ;
	else if ( m_indirectType == "Soviet 152mm Howitzer")
		indirectType = SOVIET152MM_HOWITZ;
	else if ( m_indirectType == "Soviet 122mm Rocket")
		indirectType = SOVIET122MM_ROCKET;

	if ( m_type )
		*m_type = indirectType;
}

void IndirectFire::OnEnChangeEditActiveLoc()
{
	UpdateData(TRUE);

	char sCol[3], sRow[3];
	sCol[0] = (char)m_targetHex.GetAt(0);
	sCol[1] = (char)m_targetHex.GetAt(1);
	sCol[2] = '\0';
	sRow[0] = (char)m_targetHex.GetAt(2);
	sRow[1] = (char)m_targetHex.GetAt(3);
	sRow[2] = '\0';

	if ( m_col )
		*m_col = atoi( sCol );
	if ( m_row )
		*m_row = atoi( sRow );
}

void IndirectFire::setIndirectFirePtr( int *typePtr, int *rowPtr, int *colPtr, int *smokePtr )
{
	m_type = typePtr;
	m_row = rowPtr;
	m_col = colPtr;
	m_smoke = smokePtr;

	char buffer[10];
	sprintf_s( buffer, "%02d%02d", *colPtr, *rowPtr );
	m_targetHex = (CString)buffer;

	if ( *typePtr == US81MM_MORTAR )
		m_indirectType = "U.S. 81mm Mortar";
	else if ( *typePtr == US42INCH_MORTAR )
		m_indirectType = "U.S. 4.2\" Mortar";
	else if ( *typePtr == US160MM_MORTAR )
		m_indirectType = "U.S. 160mm Mortar";
	else if ( *typePtr == US105MM_HOWITZ )
		m_indirectType = "U.S. 105mm Howitzer";
	else if ( *typePtr == US155MM_HOWITZ )
		m_indirectType = "U.S. 155mm Howitzer";
	else if ( *typePtr == US203MM_ROCKET )
		m_indirectType = "U.S. 203mm Rocket";
	else if ( *typePtr == SOVIET120MM_MORTAR )
		m_indirectType = "Soviet 120mm Mortar";
	else if ( *typePtr == SOVIET122MM_HOWITZ )
		m_indirectType = "Soviet 122mm Howitzer";
	else if ( *typePtr == SOVIET152MM_HOWITZ )
		m_indirectType = "Soviet 152mm Howitzer";
	else if ( *typePtr == SOVIET122MM_ROCKET )
		m_indirectType = "Soviet 122mm Rocket";
}

void IndirectFire::OnBnClickedCheckSmokeMission()
{
	UpdateData(TRUE);
	*m_smoke = m_smokeMission;
}
