#pragma once
#include "afxwin.h"


// IndirectFire dialog

class IndirectFire : public CDialog
{
	DECLARE_DYNAMIC(IndirectFire)

public:
	IndirectFire(CWnd* pParent = NULL);   // standard constructor
	virtual ~IndirectFire();

	void setIndirectFirePtr( int *typePtr, int *rowPtr, int *colPtr, int *smokePtr );

// Dialog Data
	enum { IDD = IDD_DIALOG_INDIRECT_FIRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboIndirect();
	afx_msg void OnEnChangeEditActiveLoc();

	CString m_indirectType;
	CString m_targetHex;
	int m_smokeMission;

	int *m_type;
	int *m_row;
	int *m_col;
	int *m_smoke;
	afx_msg void OnBnClickedCheckSmokeMission();
};
