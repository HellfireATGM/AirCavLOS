#pragma once
#include "afxwin.h"


// KillDialog dialog

class KillDialog : public CDialog
{
	DECLARE_DYNAMIC(KillDialog)

public:
	KillDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~KillDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_KILL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_FKN1;
	CString m_FKN2;

	void setFKNText1( CString fkn ) { m_FKN1 = fkn; }
	void setFKNText2( CString fkn ) { m_FKN2 = fkn; }
};
