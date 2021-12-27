// KillDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "KillDialog.h"


// KillDialog dialog

IMPLEMENT_DYNAMIC(KillDialog, CDialog)

KillDialog::KillDialog(CWnd* pParent /*=NULL*/)
	: CDialog(KillDialog::IDD, pParent)
{

}

KillDialog::~KillDialog()
{
}

void KillDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FKN, m_FKN1);
	DDX_Text(pDX, IDC_EDIT_FKN2, m_FKN2);
}


BEGIN_MESSAGE_MAP(KillDialog, CDialog)
END_MESSAGE_MAP()


// KillDialog message handlers
