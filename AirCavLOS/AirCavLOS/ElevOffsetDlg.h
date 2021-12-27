#pragma once
#include "resource.h"


// ElevOffsetDlg dialog

class ElevOffsetDlg : public CDialog
{
	DECLARE_DYNAMIC(ElevOffsetDlg)

public:
	ElevOffsetDlg(CWnd* pParent = NULL, int elev = 0);   // standard constructor
	virtual ~ElevOffsetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ELEV_OFFSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int getOffset();

	int m_elevOffset;

};
