// KillSuppressDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "KillSuppressDialog.h"


// KillSuppressDialog dialog

IMPLEMENT_DYNAMIC(KillSuppressDialog, CDialog)

KillSuppressDialog::KillSuppressDialog(CWnd* pParent /*=NULL*/)
	: CDialog(KillSuppressDialog::IDD, pParent)
{

}

KillSuppressDialog::~KillSuppressDialog()
{
}

void KillSuppressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FKN, m_FKN1);
	DDX_Text(pDX, IDC_EDIT_FKN2, m_FKN2);
}


BEGIN_MESSAGE_MAP(KillSuppressDialog, CDialog)
	ON_BN_CLICKED(IDOK2, &KillSuppressDialog::OnBnClickedOk2)
END_MESSAGE_MAP()


// KillSuppressDialog message handlers


void KillSuppressDialog::OnBnClickedOk2()
{
	EndDialog(IDOK2);
}
