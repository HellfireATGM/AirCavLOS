// ElevOffsetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "ElevOffsetDlg.h"


// ElevOffsetDlg dialog

IMPLEMENT_DYNAMIC(ElevOffsetDlg, CDialog)

ElevOffsetDlg::ElevOffsetDlg(CWnd* pParent /*=NULL*/, int elev)
	: CDialog(ElevOffsetDlg::IDD, pParent), m_elevOffset(elev / -10)
{

}

ElevOffsetDlg::~ElevOffsetDlg()
{
}

void ElevOffsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ELEV_0, m_elevOffset);
}


BEGIN_MESSAGE_MAP(ElevOffsetDlg, CDialog)
END_MESSAGE_MAP()


// ElevOffsetDlg message handlers
int ElevOffsetDlg::getOffset()
{
	return ( m_elevOffset * -10 );
}
