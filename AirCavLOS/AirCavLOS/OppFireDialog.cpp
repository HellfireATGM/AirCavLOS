// OppFireDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "OppFireDialog.h"


// OppFireDialog dialog

IMPLEMENT_DYNAMIC(OppFireDialog, CDialog)

OppFireDialog::OppFireDialog(CWnd* pParent /*=NULL*/)
	: CDialog(OppFireDialog::IDD, pParent),
	m_wpn1Name(_T("None")), m_wpn2Name(_T("None")), m_wpn3Name(_T("None")), 
	m_wpn4Name(_T("None")), m_wpn5Name(_T("None"))
{
	m_weaponSelection = 0;
}

OppFireDialog::~OppFireDialog()
{
}

void OppFireDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_OPP_MAIN1, m_weaponSelection);
	DDX_Text(pDX, IDC_EDIT_MAINWPN1, m_wpn1Name);
	DDX_Text(pDX, IDC_EDIT_MAINWPN2, m_wpn2Name);
	DDX_Text(pDX, IDC_EDIT_MAINWPN3, m_wpn3Name);
	DDX_Text(pDX, IDC_EDIT_MAINWPN4, m_wpn4Name);
	DDX_Text(pDX, IDC_EDIT_MAINWPN5, m_wpn5Name);
}


BEGIN_MESSAGE_MAP(OppFireDialog, CDialog)
	ON_BN_CLICKED(IDC_RADIO_OPP_MAIN1, &OppFireDialog::OnBnClickedRadioMain1)
END_MESSAGE_MAP()


// OppFireDialog message handlers
void OppFireDialog::OnBnClickedRadioMain1()
{
	UpdateData(TRUE);
}

void OppFireDialog::SetWeaponText(CString m1, CString m2, CString m3, CString s1, CString s2)
{
	m_wpn1Name = m1;
	m_wpn2Name = m2;
	m_wpn3Name = m3;
	m_wpn4Name = s1;
	m_wpn5Name = s2;

//	SetDlgItemText(IDC_TEXT_OPP_MAIN1, m_wpn1Name);
//	SetDlgItemText(IDC_RADIO_OPP_MAIN2, m_wpn2Name);
//	SetDlgItemText(IDC_RADIO_OPP_MAIN3, m_wpn3Name);
//	SetDlgItemText(IDC_RADIO_OPP_SECOND1, m_wpn4Name);
//	SetDlgItemText(IDC_RADIO_OPP_SECOND2, m_wpn5Name);

//	UpdateData(TRUE);
}


