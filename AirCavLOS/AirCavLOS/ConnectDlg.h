#pragma once


// ConnectDlg dialog

class ConnectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConnectDlg)

public:
	ConnectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ConnectDlg();

	void setSide(int side);
	int getSide() { return m_side; }

	void setIP(CString ip);
	CString getIP() { return m_ip; }

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CONNECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int	m_side;
	CString m_ip;

};
