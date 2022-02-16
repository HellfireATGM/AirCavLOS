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
	DDX_Radio(pDX, IDC_RADIO_SIDE_0, m_side);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_ip);
}


BEGIN_MESSAGE_MAP(ScenarioDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_SCENARIO_0, &ScenarioDlg::OnBnClickedRadioScenario0)
	ON_BN_CLICKED(IDC_RADIO_SIDE_0, &ScenarioDlg::OnBnClickedRadioSide)
	ON_BN_CLICKED(IDC_RADIO_SIDE_1, &ScenarioDlg::OnBnClickedRadioSide)
	ON_BN_CLICKED(IDC_RADIO_SIDE_2, &ScenarioDlg::OnBnClickedRadioSide)
	ON_EN_CHANGE(IDC_EDIT_ADDRESS, &ScenarioDlg::OnEnChangeEditAddress)
END_MESSAGE_MAP()


// ScenarioDlg message handlers

void ScenarioDlg::OnBnClickedRadioScenario0()
{
	// TODO: Add your control notification handler code here
}

void ScenarioDlg::OnBnClickedRadioSide()
{
	// host ip is only enable for RED (client) side
	UpdateData(TRUE);
	if (m_side == 1)
		GetDlgItem(IDC_EDIT_ADDRESS)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_EDIT_ADDRESS)->EnableWindow(FALSE);
}

void ScenarioDlg::OnEnChangeEditAddress()
{
	// TODO:  Add your control notification handler code here
}

int ScenarioDlg::getScenario()
{
	return m_scenario;
}
	
int ScenarioDlg::getSide()
{
	return m_side;
}

CString ScenarioDlg::getHostIP()
{
	return m_ip;
}

BOOL ScenarioDlg::OnInitDialog()
{
	m_scenario = 0;
	m_side = 2;
	m_ip = "192.168.1.154";

	GetDlgItem(IDC_EDIT_ADDRESS)->EnableWindow(FALSE);

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
	SetDlgItemText(IDC_RADIO_SCENARIO_11, scenarioNames[11]);

	UpdateData(FALSE);
	return TRUE;
}

void ScenarioDlg::setScenarioTitle(int which, CString name)
{
	scenarioNames[which] = name;
}


