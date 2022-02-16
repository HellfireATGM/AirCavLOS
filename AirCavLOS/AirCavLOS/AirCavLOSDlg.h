// AirCavLOSDlg.h : header file
//
#include "AirCavUnitData.h"
#include "AirCavWeaponData.h"
#include "AirCavCounterData.h"
#include "AirCavMapData.h"
#include "AirCavScenarioData.h"
#include <vector>
#include <map>

#pragma once


// CAirCavLOSDlg dialog
class CAirCavLOSDlg : public CDialog
{
// Construction
public:
	CAirCavLOSDlg(CWnd* pParent = NULL);	// standard constructor
	~CAirCavLOSDlg();
	void OnOK();
	void OnCloseDialog();

// Dialog Data
	enum { IDD = IDD_AIRCAVLOS_DIALOG };

	void updateActiveUnit(bool rebuildList = TRUE, bool noNetworkUpdate = FALSE);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void updateFiringUnits();

// AirCav Data
public:
	AirCavUnitData		*unitDataList[MAXTYPS];
	AirCavWeaponData	*weaponDataList[MAXWEAP];
	AirCavCounterData	*counterDataList[MAXCOUNTERS];
	AirCavMapData		*mapData;
	AirCavScenarioData	*scenarioData;

	int					m_maxCounters;
	int					m_maxUnits;
	int					m_maxWeapons;
	int					m_ActiveUnit;
	int					m_originalActiveUnit;
	int					m_ActiveTarget;
	int					m_Weather;
	int					m_TimeOfDay;
	int					m_Optics;
	CComboBox			m_currentOpticsComboBox;
	CListBox			m_AvailableUnitsListBox;
	CListBox			m_SightedUnitsListBox;
	CListBox			m_SightingUnitsListBox;
	CListBox			m_FiringUnitsListBox;
	CListBox			m_OutputLogListBox;

	std::vector<int>	m_FiringUnitsList;
	RowCol				m_vehicleSmokeHexList;
	RowCol				m_artillerySmokeHexList;

	UnitTracking		m_unitTracking;

	bool				m_shutdown;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeEditActiveName();
	afx_msg void OnCbnSelchangeComboActiveType();
	afx_msg void OnCbnSelchangeComboActiveTt();
	afx_msg void OnEnChangeEditActiveOp();
	afx_msg void OnBnClickedRadioActiveMain1();
	afx_msg void OnBnClickedRadioActiveMain2();
	afx_msg void OnBnClickedRadioActiveMain3();
	afx_msg void OnBnClickedRadioActiveSecondary1();
	afx_msg void OnBnClickedRadioActiveSecondary2();
	afx_msg void OnCbnSelchangeComboActiveMain();
	afx_msg void OnCbnSelchangeComboActiveSecondary();
	afx_msg void OnBnClickedButtonActionPopUp();
	afx_msg void OnBnClickedButtonActionFireGun();
	afx_msg void OnBnClickedButtonActionMoveN();
	afx_msg void OnBnClickedButtonActionMoveNw();
	afx_msg void OnBnClickedButtonActionMoveSw();
	afx_msg void OnBnClickedButtonActionMoveS();
	afx_msg void OnBnClickedButtonActionMoveSe();
	afx_msg void OnBnClickedButtonActionMoveNe();
	afx_msg void OnLbnSelchangeListSightedUnits();
	afx_msg void OnLbnSelchangeListUnits();
	afx_msg void OnEnChangeEditCurWeaponName();
	afx_msg void OnBnClickedButtonMakeActive();
	afx_msg void OnBnClickedButtonResetActive();

	CString m_activeUnitName;
	CString m_activeUnitTypeName;
	CString m_activeUnitUnitType;
	CString m_activeUnitCountry;
	CString m_activeUnitTargetType;
	CString m_activeUnitCarrierUnit;
	CString m_activeUnitOPs;
	CString m_activeUnitMainWpn1;
	CString m_activeUnitMainWpn2;
	CString m_activeUnitMainWpn3;
	CString m_activeUnitSecondaryWpn1;
	CString m_activeUnitSecondaryWpn2;
	CString m_activeUnitPosition;
	CString m_activeUnitOffset;
	CString m_activeUnitActualElev;
	CString m_activeCountry;
	CString m_activeSide;
	CString m_mySide;
	CString m_activeUnitOptics;

	CString m_currentWeather;
	CString m_currentTimeOfDay;

	int m_lastArtilleryUnit;
	int m_lastArtilleryRow;
	int m_lastArtilleryCol;

	int m_popSmokeWhileMoving;

	bool m_normalMap;

	CString m_terrainType;
	CString m_terrainElev;
	CString m_terrainStack;
	int m_contourLines;
	int m_smoke;
	int m_wreck;
	int m_roadN;
	int m_roadNW;
	int m_roadSW;
	int m_roadS;
	int m_roadSE;
	int m_roadNE;
	int m_autobahnN;
	int m_autobahnNW;
	int m_autobahnSW;
	int m_autobahnS;
	int m_autobahnSE;
	int m_autobahnNE;
	int m_streamN;
	int m_streamNW;
	int m_streamSW;
	int m_streamS;
	int m_streamSE;
	int m_streamNE;

	int m_activeUnitWeapon;		// 0==main1, 1==main2, 2==main3, 3=secn1, 4=secn2
	int m_activeUnitFired;
	int m_activeUnitMoved;
	int m_activeUnitEvading;
	int m_activeUnitInDefilade;
	int m_activeUnitMounted;
	int m_activeUnitDismounted;
	int m_activeUnitLowLevel;
	int m_activeUnitIsSuppressed;
	int m_activeUnitRadarOn;
	int m_debugLOSMessages;
	int m_debugFKNMessages;

	std::map<int, std::string> popupSightedUnits;

	afx_msg void OnLbnSelchangeListLog();

	void writeToLog( char *buffer );
	void moveMountedUnits();
	void resolveOpportunityFire();
	int resolveFirePass(int firePass);
	bool readWeaponTextFile( FILE * );
	bool readUnitTextFile( FILE * );

	FILE * openScenarioTextFile(std::string f, int c);
	bool readScenarioTextFile( FILE *fptr, int scen );
	int findWeaponIndex( char *weaponName );
	int findUnitIndex( char *unitName );
	int findScenarioUnitIndex( int scen, char *unitName );
	void replaceUnderscores( char *strPtr );
	bool unitCannotBeSkylined(UnitType unitType, int terrain, int smoke);

	void doUnitTracking(int previousOPs, int previousRow, int previousColumn);
	void resetLaserDesignation();

	void setActiveUnit(int& id);
	void switchSides();

	void sendUnitInfo(int unit);
	void encodeUnitInfo(int unit, char *outbuffer);
	void decodeUnitInfo(char *inbuffer);
	
	void sendMapInfo(int row, int col);
	void encodeMapInfo(int col, int row, char *outbuffer);
	void decodeMapInfo(char *inbuffer);
	
	void sendWeather();
	void encodeWeather(char *outbuffer);
	void decodeWeather(char *inbuffer);

	void sendTimeOfDay();
	void encodeTimeOfDay(char *outbuffer);
	void decodeTimeOfDay(char *inbuffer);

	afx_msg void OnBnClickedButtonEditTerrain();
	afx_msg void OnBnClickedButtonFillTerrain();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckActiveFired();
	afx_msg void OnBnClickedCheckActiveMoved();
	afx_msg void OnBnClickedButtonActionReset();
	afx_msg void OnEnChangeEditActiveLoc();
	afx_msg void OnBnClickedCheckDebug();
	afx_msg void OnBnClickedCheckDebugPk();
	afx_msg void OnBnClickedButtonActionEvade();
	afx_msg void OnBnClickedButtonActionDefilade();
	afx_msg void OnBnClickedCheckActiveEvading();
	afx_msg void OnBnClickedCheckActiveInDef();
	afx_msg void OnBnClickedButtonActiveSuppressed();
	afx_msg void OnBnClickedButtonActionMount();
	afx_msg void OnBnClickedButtonActionLowlevel();
	afx_msg void OnBnClickedButtonActionOppfire();
	afx_msg void OnBnClickedButtonResetFiring();
	afx_msg void OnBnClickedButtonResolveOppfire();
	afx_msg void OnBnClickedButtonLaysmoke();
	afx_msg void OnBnClickedCheckSmoke();
	afx_msg void OnBnClickedCheckWreck();
	afx_msg void OnBnClickedButtonElevoffset();
	afx_msg void OnBnClickedButtonListUnits();
	afx_msg void OnCbnSelchangeComboWeather();
	afx_msg void OnBnClickedButtonActionIndfire();
	afx_msg void OnBnClickedButtonRemoveSmoke();
	afx_msg void OnBnClickedCheckPopSmoke();
	afx_msg void OnBnClickedButtonSaveProgress();
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnBnClickedCheckIgnoreWater();
	afx_msg void OnBnClickedCheckIgnoreAutobahn();
	afx_msg void OnBnClickedCheckRotateMap();
	afx_msg void OnBnClickedButtonActionPreviousMove();
	afx_msg void OnCbnSelchangeComboTimeOfDay();
	afx_msg void OnCbnSelchangeComboOptics();
	afx_msg void OnBnClickedButtonActionLaser();
	afx_msg void OnBnClickedButtonActionRadar();
	afx_msg void OnBnClickedCheckActiveRadarOn();
	afx_msg void OnBnClickedButtonConnect();
};
