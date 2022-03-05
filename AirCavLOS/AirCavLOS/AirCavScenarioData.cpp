#include "StdAfx.h"
#include "AirCavScenarioData.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>


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
										unit.op,					// OPs remaining
										unit.mount,					// carried by
										unit.mounted,				// starts mounted or not
										unit.defilade);				// starts in defilade or not
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

	m_currentScenario = which;

	return mC;
}

static bool replaceString(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

int AirCavScenarioData::saveScenario(std::string& file_dir, AirCavCounterData *counterDataList[MAXCOUNTERS])
{
	__time64_t long_time;
	_time64(&long_time);

	struct tm newtime;
	errno_t err = _localtime64_s(&newtime, &long_time);

	std::ostringstream oss;
	oss << std::put_time(&newtime, "%m-%d-%Y__%H-%M-%S");
	auto str = oss.str();

	std::string whichFile = file_dir + (std::string)"scenario" + std::string("11__") + str + (std::string)".txt";
	FILE *scenarioFile;
	err = fopen_s(&scenarioFile, whichFile.c_str(), "wt");
	if (err != 0 || !scenarioFile)
	{
		return 0;
	}

	std::string scenarioName(scenarioData[m_currentScenario].scenarioname);
	replaceString(scenarioName, "In-Progress [", "");
	replaceString(scenarioName, "]", "");
	fprintf(scenarioFile, "In-Progress [%s]\n", scenarioName.c_str());
	fprintf(scenarioFile, "#\n");
	fprintf(scenarioFile, "# name     side   country  unit type           col  row  elev  op   carrier  mounted  defilade\n");
	fprintf(scenarioFile, "#\n");

	for (int i = 0; i < scenarioData[m_currentScenario].numunits; i++)
	{
		UnitDef unit = scenarioData[m_currentScenario].unit[i];

		//name       side   country  unit type           col  row  elev  op   carrier    mounted\n");
		CT2CA unitNameConvertedAnsiString(unit.name);
		std::string name(unitNameConvertedAnsiString);
		std::replace(name.begin(), name.end(), ' ', '_');
		replaceString(name, "_(US)", " ");
		replaceString(name, "_(SU)", " ");
		replaceString(name, "_(WG)", " ");
		replaceString(name, "_(UK)", " ");
		replaceString(name, "_(SY)", " ");
		replaceString(name, "_(IS)", " ");
		fprintf(scenarioFile, "%s", name.c_str());

		fprintf(scenarioFile, "  %s", unit.side == SIDE_BLUE ? "BLUE" : "RED");
		if (unit.country == COUNTRY_US)
			fprintf(scenarioFile, "  %s", "US");
		else if(unit.country == COUNTRY_SOVIET)
			fprintf(scenarioFile, "  %s", "SOVIET");
		else if (unit.country == COUNTRY_GERMANY)
			fprintf(scenarioFile, "  %s", "GERMANY");
		else if (unit.country == COUNTRY_BRITAIN)
			fprintf(scenarioFile, "  %s", "BRITAIN");
		else if (unit.country == COUNTRY_SYRIA)
			fprintf(scenarioFile, "  %s", "SYRIA");
		else if (unit.country == COUNTRY_ISRAEL)
			fprintf(scenarioFile, "  %s", "ISRAEL");

		AirCavUnitData *unitData = counterDataList[i]->getUnitInfo();
		CT2CA unitTypeConvertedAnsiString(unitData->getName());
		std::string type(unitTypeConvertedAnsiString);
		std::replace(type.begin(), type.end(), ' ', '_');
		fprintf(scenarioFile, "  %s", type.c_str());

		fprintf(scenarioFile, "  %d", counterDataList[i]->getHexCol());
		fprintf(scenarioFile, "  %d", counterDataList[i]->getHexRow());
		fprintf(scenarioFile, "  %d", counterDataList[i]->getElevOffset());
		if (counterDataList[i]->getIsAlive())
			fprintf(scenarioFile, "  %3.1f", counterDataList[i]->getOPs());
		else
			fprintf(scenarioFile, "  %3.1f", -1.0);		// a dead unit has negative OPs remaining

		int parentUnit = counterDataList[i]->getIsCarriedBy();
		if (parentUnit > 0)
		{
			CString parentName = counterDataList[parentUnit]->getName();
			CT2CA parentNameConvertedAnsiString(parentName);
			std::string parent(parentNameConvertedAnsiString);
			std::replace(parent.begin(), parent.end(), ' ', '_');
			replaceString(parent, "_(US)", " ");
			replaceString(parent, "_(SU)", " ");
			replaceString(parent, "_(WG)", " ");
			replaceString(parent, "_(UK)", " ");
			replaceString(parent, "_(SY)", " ");
			replaceString(parent, "_(IS)", " ");
			fprintf(scenarioFile, "  %s", parent.c_str());
		}
		else
		{
			fprintf(scenarioFile, "  %s", "none");
		}
		fprintf(scenarioFile, "  %d", counterDataList[i]->getIsDismounted() == 1 ? 0 : 1);
		fprintf(scenarioFile, "  %d\n", counterDataList[i]->getDefilade() == TRUE ? 1 : 0);
	}
	fclose(scenarioFile);

   // copy this file to the scenario11.txt file so that file represents the last saved scenario
   std::string latestFile = file_dir + (std::string)"scenario" + std::string("11") + (std::string)".txt";
   std::wstring wFileName(latestFile.begin(), latestFile.end());
   auto deleted = DeleteFile(wFileName.c_str());

   std::wstring wSourceFileName(whichFile.begin(), whichFile.end());
   auto copied = CopyFile(wSourceFileName.c_str(), wFileName.c_str(), 0);

	return 1;
}
