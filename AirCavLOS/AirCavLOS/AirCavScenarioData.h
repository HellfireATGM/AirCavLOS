#pragma once

#include "AirCavCommon.h"
#include "AirCavCounterData.h"
#include "AirCavUnitData.h"

#define MAXSCENARIOS 12


class AirCavScenarioData
{
public:
	AirCavScenarioData(void);
	~AirCavScenarioData(void);

	int initScenarios(void);
	int getScenario( int which, AirCavCounterData *counterData[MAXCOUNTERS], AirCavUnitData *unitDataList[MAXTYPS] );
	CString getName( int which ) { return (CString)scenarioData[which].scenarioname; }

	Scenario scenarioData[MAXSCENARIOS];

	int saveScenario(std::string& file_dir, AirCavCounterData *counterDataList[MAXCOUNTERS]);
	int m_currentScenario;
};
