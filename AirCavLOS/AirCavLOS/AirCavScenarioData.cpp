#include "StdAfx.h"
#include "AirCavScenarioData.h"



AirCavScenarioData::AirCavScenarioData(void)
{
}

AirCavScenarioData::~AirCavScenarioData(void)
{
}

int AirCavScenarioData::initScenarios(void)
{
	return MAXSCENARIOS;
}

int AirCavScenarioData::getScenario( int which, AirCavCounterData *counterDataList[MAXCOUNTERS], AirCavUnitData *unitDataList[MAXTYPS] )
{
	int mC = 0;

	for ( int i=0; i<scenarioData[which].numunits; i++ )
	{
		UnitDef unit = scenarioData[which].unit[i];

		counterDataList[mC++] = new AirCavCounterData(
										unit.name,					// unit name
										(SideType)unit.side,		// which side (0=BLUE, 1=RED)
										(CountryType)unit.country,	// which country (0=US, 1=USSR)
										unitDataList[unit.type],	// which type (0=T_72, etc)
										unit.column,				// location on map
										unit.row,					// location on map
										unit.offset,				// elevation offset
										unit.mount,					// carried by
										unit.mounted);				// starts mounted or not
	}

	// assign the carried units to the carrying unit
	for ( int c=0; c<mC; c++ )
	{
		int parentUnit = counterDataList[c]->getIsCarriedBy();
		if ( parentUnit > 0 )
		{
			int numMounted = counterDataList[parentUnit]->getNumberOfMountedUnits();
			counterDataList[parentUnit]->setMountedUnit(numMounted, c);
		}
	}

	return mC;
}

