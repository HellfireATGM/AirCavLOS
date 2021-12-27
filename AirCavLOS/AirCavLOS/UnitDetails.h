#pragma once
#include "afxwin.h"
#include "AirCavCounterData.h"

// UnitDetails dialog

class UnitDetails : public CDialog
{
	DECLARE_DYNAMIC(UnitDetails)

public:
	UnitDetails(CWnd* pParent = NULL);   // standard constructor
	virtual ~UnitDetails();

	void setCounterData( AirCavCounterData *counterDataList[] );
	void setActiveUnit( int activeUnit );
	void setMaxUnits ( int maxUnits );
	void setActiveUnitPtr( int *activeUnit );

	void setUnitNameText( void );
	void setUnitTypeNameText( void );
	void setUnitTypeText( void );
	void setUnitCountryText( void );
	void setUnitPositionText( void );
	void setUnitStatusText( void );

// Dialog Data
	enum { IDD = IDD_DIALOG_UNIT_DETAILS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	AirCavCounterData *m_counterDataList[100];
	int m_activeUnit;
	int m_maxUnits;
	int *m_activeUnitPtr;

public:
	CString m_activeUnitName;
	CString m_activeUnitTypeName;
	CString m_activeUnitUnitType;
	CString m_activeUnitCountry;
	CString m_activeUnitPosition;
	CString m_activeUnitStatus;

	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedGoto();
};
