#pragma once

#include "AirCavCommon.h"
#include "AirCavCounterData.h"
#include "AirCavUnitData.h"

#define MAXSCENARIOS 11


class AirCavScenarioData
{
public:
	AirCavScenarioData(void);
	~AirCavScenarioData(void);

	int initScenarios(void);
	int getScenario( int which, AirCavCounterData *counterData[MAXCOUNTERS], AirCavUnitData *unitDataList[MAXTYPS] );
	CString getName( int which ) { return (CString)scenarioData[which].scenarioname; }

	Scenario scenarioData[MAXSCENARIOS];

};
