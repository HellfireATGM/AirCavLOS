// UnitDetails.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "UnitDetails.h"


// UnitDetails dialog

IMPLEMENT_DYNAMIC(UnitDetails, CDialog)

UnitDetails::UnitDetails(CWnd* pParent /*=NULL*/)
	: CDialog(UnitDetails::IDD, pParent)
{

}

UnitDetails::~UnitDetails()
{
}

void UnitDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_NAME, m_activeUnitName);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_UNIT_TYPE, m_activeUnitUnitType);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_UNIT_COUNTRY, m_activeUnitCountry);	
	DDX_Text(pDX, IDC_EDIT_ACTIVE_UNIT_TYPE_NAME, m_activeUnitTypeName);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_STATUS, m_activeUnitStatus);
	DDX_Text(pDX, IDC_EDIT_ACTIVE_LOC, m_activeUnitPosition);
}


BEGIN_MESSAGE_MAP(UnitDetails, CDialog)
	ON_BN_CLICKED(IDC_PREVIOUS, &UnitDetails::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_NEXT, &UnitDetails::OnBnClickedNext)
	ON_BN_CLICKED(IDC_GOTO, &UnitDetails::OnBnClickedGoto)
END_MESSAGE_MAP()

void UnitDetails::setCounterData( AirCavCounterData *counterDataList[] )
{
	for ( int c=0; c<m_maxUnits; c++ )
		m_counterDataList[c] = counterDataList[c];
}
void UnitDetails::setActiveUnit( int activeUnit )
{
	m_activeUnit = activeUnit;
}
void UnitDetails::setMaxUnits ( int maxUnits )
{
	m_maxUnits = maxUnits;
}
void UnitDetails::setActiveUnitPtr ( int *activeUnitPtr )
{
	m_activeUnitPtr = activeUnitPtr;
}

void UnitDetails::setUnitNameText( void )
{
	m_activeUnitName = m_counterDataList[m_activeUnit]->getName();
}
void UnitDetails::setUnitTypeNameText( void )
{
	// Unit Type Name
	m_activeUnitTypeName = m_counterDataList[m_activeUnit]->getUnitInfo()->getName();
}
void UnitDetails::setUnitTypeText( void )
{
	// Unit Type 
	UnitType unitType = m_counterDataList[m_activeUnit]->getUnitInfo()->getUnitType();
	m_activeUnitUnitType = m_counterDataList[m_activeUnit]->getUnitInfo()->getUnitTypeString(unitType);
}
void UnitDetails::setUnitCountryText( void )
{
	// Unit Country
	CountryType countryType = m_counterDataList[m_activeUnit]->getUnitInfo()->getCountryType();
	m_activeUnitCountry = m_counterDataList[m_activeUnit]->getUnitInfo()->getCountryTypeString(countryType);
}
void UnitDetails::setUnitPositionText( void )
{
	// Unit Location
	int col = m_counterDataList[m_activeUnit]->getHexCol();
	int row = m_counterDataList[m_activeUnit]->getHexRow();
	char buffer1[1024];
	sprintf_s( buffer1, "%02d%02d", col, row );
	m_activeUnitPosition = (CString) buffer1;
}

void UnitDetails::setUnitStatusText( void )
{
	double OPsRemaining = m_counterDataList[m_activeUnit]->getOPs();
	bool actionTaken = m_counterDataList[m_activeUnit]->getActionTaken();
   char buffer1[1024];
   if (m_counterDataList[m_activeUnit]->getIsAlive())
      sprintf_s(buffer1, actionTaken ? "Active [%2.1f]" : "Not Active [%2.1f]", OPsRemaining);
   else
      sprintf_s( buffer1, "DEAD");
	m_activeUnitStatus = (CString) buffer1;
}

void UnitDetails::OnBnClickedPrevious()
{
	m_activeUnit--;
	if ( m_activeUnit < 0 )
		m_activeUnit = m_maxUnits - 1;

	setUnitNameText();
	setUnitTypeNameText();
	setUnitTypeText();
	setUnitCountryText();
	setUnitPositionText();
	setUnitStatusText();

	UpdateData(FALSE);
}

void UnitDetails::OnBnClickedNext()
{
	m_activeUnit++;
	if ( m_activeUnit >= m_maxUnits )
		m_activeUnit = 0;

	setUnitNameText();
	setUnitTypeNameText();
	setUnitTypeText();
	setUnitCountryText();
	setUnitPositionText();
	setUnitStatusText();

	UpdateData(FALSE);
}

void UnitDetails::OnBnClickedGoto()
{
	*m_activeUnitPtr = m_activeUnit;
	OnOK();
}
