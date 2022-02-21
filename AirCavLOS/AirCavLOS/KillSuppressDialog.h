#pragma once
#include "afxwin.h"


// KillSuppressDialog dialog

class KillSuppressDialog : public CDialog
{
	DECLARE_DYNAMIC(KillSuppressDialog)

public:
	KillSuppressDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~KillSuppressDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_KILL_SUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_FKN1;
	CString m_FKN2;
	CString m_FKN3;

	void setFKNText1( CString fkn ) { m_FKN1 = fkn; }
	void setFKNText2( CString fkn ) { m_FKN2 = fkn; }
	void setFKNText3( CString fkn ) { m_FKN3 = fkn; }
	afx_msg void OnBnClickedOk2();
};
