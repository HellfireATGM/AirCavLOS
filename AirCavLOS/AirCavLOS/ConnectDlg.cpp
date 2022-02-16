// ConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "AirCavCommon.h"
#include "ConnectDlg.h"
#include "afxdialogex.h"


// ConnectDlg dialog

IMPLEMENT_DYNAMIC(ConnectDlg, CDialogEx)

ConnectDlg::ConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CONNECT, pParent)
{

}

ConnectDlg::~ConnectDlg()
{
}

void ConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_ip);
	DDX_CBString(pDX, IDC_COMBO_NETWORK_SIDE, (m_side == SIDE_BLUE) ? (CString)"BLUE" : (CString)"RED");
}


BEGIN_MESSAGE_MAP(ConnectDlg, CDialogEx)
END_MESSAGE_MAP()

void ConnectDlg::setSide(int side)
{
	m_side = side;
}

void ConnectDlg::setIP(CString ip)
{
	m_ip = ip;
}
