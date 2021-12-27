// ScenarioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AirCavLOS.h"
#include "ScenarioDlg.h"


// ScenarioDlg dialog

IMPLEMENT_DYNAMIC(ScenarioDlg, CDialog)

ScenarioDlg::ScenarioDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ScenarioDlg::IDD, pParent), m_scenario(0)
{

}

ScenarioDlg::~ScenarioDlg()
{
}

void ScenarioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SCENARIO_0, m_scenario);
}


BEGIN_MESSAGE_MAP(ScenarioDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SCENARIO_0, &ScenarioDlg::OnBnClickedRadioScenario0)
END_MESSAGE_MAP()


// ScenarioDlg message handlers

void ScenarioDlg::OnBnClickedRadioScenario0()
{
	// TODO: Add your control notification handler code here
}

int ScenarioDlg::getScenario()
{
	return m_scenario;
}
	
BOOL ScenarioDlg::OnInitDialog()
{
	m_scenario = 0;

	SetDlgItemText(IDC_RADIO_SCENARIO_0, scenarioNames[0]);
	SetDlgItemText(IDC_RADIO_SCENARIO_1, scenarioNames[1]);
	SetDlgItemText(IDC_RADIO_SCENARIO_2, scenarioNames[2]);
	SetDlgItemText(IDC_RADIO_SCENARIO_3, scenarioNames[3]);
	SetDlgItemText(IDC_RADIO_SCENARIO_4, scenarioNames[4]);
	SetDlgItemText(IDC_RADIO_SCENARIO_5, scenarioNames[5]);
	SetDlgItemText(IDC_RADIO_SCENARIO_6, scenarioNames[6]);
	SetDlgItemText(IDC_RADIO_SCENARIO_7, scenarioNames[7]);
	SetDlgItemText(IDC_RADIO_SCENARIO_8, scenarioNames[8]);
	SetDlgItemText(IDC_RADIO_SCENARIO_9, scenarioNames[9]);
	SetDlgItemText(IDC_RADIO_SCENARIO_10, scenarioNames[10]);

	UpdateData(FALSE);
	return TRUE;
}

void ScenarioDlg::setScenarioTitle(int which, CString name)
{
	scenarioNames[which] = name;
}