#pragma once
#include "afxwin.h"

// OppFireDialog dialog

class OppFireDialog : public CDialog
{
	DECLARE_DYNAMIC(OppFireDialog)

public:
	OppFireDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~OppFireDialog();

	int m_weaponSelection;

	CString m_wpn1Name;
	CString m_wpn2Name;
	CString m_wpn3Name;
	CString m_wpn4Name;
	CString m_wpn5Name;

	afx_msg void OnBnClickedRadioMain1();

	int getSelectedWeapon() { return m_weaponSelection; }
	void SetWeaponText(CString m1, CString m2, CString m3, CString s1, CString s2);

// Dialog Data
	enum { IDD = IDD_DIALOG_OPPFIRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
