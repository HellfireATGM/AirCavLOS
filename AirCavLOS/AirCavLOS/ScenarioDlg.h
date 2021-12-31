#pragma once


// ScenarioDlg dialog

class ScenarioDlg : public CDialog
{
	DECLARE_DYNAMIC(ScenarioDlg)

public:
	ScenarioDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ScenarioDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SCENARIO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioScenario0();

	int getScenario();

	int m_scenario;

	void setScenarioTitle(int which, CString name);

	CString scenarioNames[12];
};
