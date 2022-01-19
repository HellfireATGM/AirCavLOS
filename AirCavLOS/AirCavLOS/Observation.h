#pragma once

#include "AirCavCommon.h"
#include "AirCavUnitData.h"

class ObservationData
{
public:
	ObservationData(void) {};

public:
	~ObservationData(void) {};

	// data
private:

	// methods
public:
	int getObservationRange(UnitType unitType, int side, int terrain, int lowlevel, int defilade, int fired, int optics, int timeofday, int weather);

};