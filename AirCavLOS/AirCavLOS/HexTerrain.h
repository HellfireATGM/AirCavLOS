#pragma once
#include "AirCavCommon.h"
#include "resource.h"
#include "afxwin.h"


// HexTerrain dialog

class HexTerrain : public CDialog
{
	DECLARE_DYNAMIC(HexTerrain)

public:
	HexTerrain(CWnd* pParent = NULL);   // standard constructor
	HexTerrain(int x, int y);			// constructor for unknown hex
	HexTerrain(int x, int y, MAP map);	// constructor for known hex
	virtual ~HexTerrain();
	BOOL OnInitDialog();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_DIALOG_UNKNOWN_HEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	int m_terrainType;
	int m_elevType;
	int m_road;
	int m_autobahn;
	int m_river;
	int m_contour;
	int m_roadN;
	int m_roadNE;
	int m_roadNW;
	int m_roadS;
	int m_roadSE;
	int m_roadSW;
	int m_autobahnN;
	int m_autobahnNE;
	int m_autobahnNW;
	int m_autobahnS;
	int m_autobahnSE;
	int m_autobahnSW;
	int m_riverN;
	int m_riverNE;
	int m_riverNW;
	int m_riverS;
	int m_riverSE;
	int m_riverSW;

	int getTerrain();
	int getElevation();
	int getRoad();
	int getAutobahn();
	int getRiver();
	int getContour();
	int getRoadHex(int which);
	int getAutobahnHex(int which);
	int getRiverHex(int which);

	int m_col;
	int m_row;
	int m_edit;

	afx_msg void OnBnClickedRadioElev0();
	afx_msg void OnBnClickedCheckRoad();
	afx_msg void OnBnClickedCheckAutobahn();
	afx_msg void OnBnClickedCheckRiver();
	afx_msg void OnBnClickedCheckContour();
	afx_msg void OnBnClickedCheckRN();
	afx_msg void OnBnClickedCheckRNw();
	afx_msg void OnBnClickedCheckRSw();
	afx_msg void OnBnClickedCheckRS();
	afx_msg void OnBnClickedCheckRSe();
	afx_msg void OnBnClickedCheckRNe();
	afx_msg void OnBnClickedCheckAN();
	afx_msg void OnBnClickedCheckANw();
	afx_msg void OnBnClickedCheckASw();
	afx_msg void OnBnClickedCheckAS();
	afx_msg void OnBnClickedCheckASe();
	afx_msg void OnBnClickedCheckANe();
	afx_msg void OnBnClickedCheckVN();
	afx_msg void OnBnClickedCheckVNw();
	afx_msg void OnBnClickedCheckVSw();
	afx_msg void OnBnClickedCheckVS();
	afx_msg void OnBnClickedCheckVSe();
	afx_msg void OnBnClickedCheckVNe();
};
