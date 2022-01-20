#include "StdAfx.h"
#include "AirCavUnitData.h"
#include "AirCavCounterData.h"


#pragma warning(disable:4996)
#pragma warning(disable:4244)

char *UnitTypeString[8] =
{
	"Tank", "TLAV", "WV", "UHH", "UHM", "A-RH", "LHX", "INF"
};

char *TargetTypeString[6] =
{
	"A", "B", "C", "D", "E", "F"
};

char *TerrStr[4] =
{
	"Clear", "Woods", "Town", "River"
};

char *CountryTypeString[6] =
{
	"United States", "Soviet Union", "West Germany", "Great Britain", "Syria", "Israel"
};

char *SideTypeString[2] =
{
	"BLUE", "RED"
};

char *IRGuidedWeapons[11] =
{
	"SA-14", "AT-6M", "SA-13", "Chaparral SAM", "Roland SAM", "Stinger AAM", "Stinger POST", "Hellfire POST", "SA-7", "SA-9", "ADSM"
};

AirCavUnitData::AirCavUnitData(void)
{
}

AirCavUnitData::AirCavUnitData(CString name, UnitType type, TargetType TT, 
					AirCavWeaponData *mwpn1, AirCavWeaponData *mwpn2, AirCavWeaponData *mwpn3,
					AirCavWeaponData *swpn1, AirCavWeaponData *swpn2, int evm, int sm, int dm, 
					int ammo_m1, int ammo_m2, int ammo_m3, int ammo_s1, int ammo_s2)
{
	m_name = name;
	m_type = type;
	m_TT = TT;
	m_EVM = evm;
	m_SM = sm;
	m_DM = dm;
	m_mainWpn1 = mwpn1;
	m_mainWpn2 = mwpn2;
	m_mainWpn3 = mwpn3;
	m_secondaryWpn1 = swpn1;
	m_secondaryWpn2 = swpn2;
	m_nAmmoMainWpn1 = ammo_m1;
	m_nAmmoMainWpn2 = ammo_m2;
	m_nAmmoMainWpn3 = ammo_m3;
	m_nAmmoSecondaryWpn1 = ammo_s1;
	m_nAmmoSecondaryWpn2 = ammo_s2;
}

AirCavUnitData::AirCavUnitData(Type unit, AirCavWeaponData *mwpn1, AirCavWeaponData *mwpn2, 
							   AirCavWeaponData *mwpn3,	AirCavWeaponData *swpn1, AirCavWeaponData *swpn2)
{
	m_name = unit.name;
	m_type = unit.type;
	m_TT = unit.armor;
	m_EVM = unit.evm;
	m_SM = unit.sm;
	m_DM = unit.dm;
	m_mainWpn1 = mwpn1;
	m_mainWpn2 = mwpn2;
	m_mainWpn3 = mwpn3;
	m_secondaryWpn1 = swpn1;
	m_secondaryWpn2 = swpn2;
	m_nAmmoMainWpn1 = unit.ammo_m1;
	m_nAmmoMainWpn2 = unit.ammo_m2;
	m_nAmmoMainWpn3 = unit.ammo_m3;
	m_nAmmoSecondaryWpn1 = unit.ammo_s1;
	m_nAmmoSecondaryWpn2 = unit.ammo_s2;
}

AirCavUnitData::~AirCavUnitData(void)
{
}

CString AirCavUnitData::getUnitTypeString( UnitType unitType )
{
	return( (CString)UnitTypeString[unitType] );
}

CString AirCavUnitData::getTargetTypeString( TargetType targetType )
{
	return( (CString)TargetTypeString[targetType] );
}

CString AirCavUnitData::getCountryTypeString( CountryType countryType )
{
	return( (CString)CountryTypeString[countryType] );
}

CString AirCavUnitData::getSideTypeString(SideType sideType)
{
	return((CString)SideTypeString[sideType]);
}

bool AirCavUnitData::isHelicopter()
{
	if ( m_type == ARH || m_type == UHH || m_type == UHM || m_type == LHX )
		return true;
	return false;
}

bool AirCavUnitData::isAttackHelicopter()
{
	if ( m_type == ARH || m_type == LHX )
		return true;
	return false;
}

bool AirCavUnitData::isTrackedVehicle()
{
	if ( m_type == TANK || m_type == TLAV )
		return true;
	return false;
}

bool AirCavUnitData::isWheeledVehicle()
{
	if ( m_type == WV )
		return true;
	return false;
}

bool AirCavUnitData::isInfantry()
{
	if ( m_type == INF )
		return true;
	return false;
}

bool AirCavUnitData::isWeaponNotIRGuided(int which)
{
	AirCavWeaponData *wpn;
	if (which == SECONDARY1)
	{
		wpn = getSecondaryWeapon1();
	}
	else if (which == SECONDARY2)
	{
		wpn = getSecondaryWeapon2();
	}
	CT2CA unitTypeConvertedAnsiString(wpn->getName());
	std::string wpnName(unitTypeConvertedAnsiString);
	for (auto w : IRGuidedWeapons)
	{
		if (wpnName.find(w) != std::string::npos)
			return false;
	}

	return true;
}

int AirCavUnitData::CalculateFKN( int which, AirCavCounterData *tgt, int terr, int smoke,
								 int range, int opp, int sup, int skylined, char *logString, int &ttMod )
{
	char logBuffer[256];
	AirCavWeaponData *mainWpn;

	char sName[32];
	CString tgtName = tgt->getName();
	int length = tgtName.GetLength() + 1;
	for (int i = 0; i < length; i++)
		sName[i] = tgtName.GetAt(i);

	bool isSecondaryWeapon = false;
	bool isNotIRGuided = true;
	if ( which == MAIN1 )
	{
		char wName[32];
		mainWpn = getMainWeapon1();
		CString wpnName = mainWpn->getName();
		if ( wpnName == "None" )
			return -1;
		int length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			wName[i] = wpnName.GetAt(i);
		sprintf_s( logBuffer, "Calculating Main Weapon 1 [%s]\nTarget: \"%s\"\nRange: %d  Terrain: %s\n", wName, sName, range, TerrStr[terr]);
	}
	else if ( which == MAIN2 )
	{
		char wName[32];
		mainWpn = getMainWeapon2();
		CString wpnName = mainWpn->getName();
		if ( wpnName == "None" )
			return -1;
		int length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			wName[i] = wpnName.GetAt(i);
		sprintf_s( logBuffer, "Calculating Main Weapon 2 [%s]\nTarget: \"%s\"\nRange: %d  Terrain: %s\n", wName, sName, range, TerrStr[terr]);
	}
	else if ( which == MAIN3 )
	{
		char wName[32];
		mainWpn = getMainWeapon3();
		CString wpnName = mainWpn->getName();
		if ( wpnName == "None" )
			return -1;
		int length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			wName[i] = wpnName.GetAt(i);
		sprintf_s( logBuffer, "Calculating Main Weapon 3 [%s]\nTarget: \"%s\"\nRange: %d  Terrain: %s\n", wName, sName, range, TerrStr[terr]);
	}
	else if ( which == SECONDARY1 )
	{
		char wName[32];
		mainWpn = getSecondaryWeapon1();
		CString wpnName = mainWpn->getName();
		if ( wpnName == "None" )
			return -1;
		int length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			wName[i] = wpnName.GetAt(i);
		sprintf_s( logBuffer, "Calculating Secondary Weapon 1 [%s]\nTarget: \"%s\"\n Range: %d  Terrain: %s\n", wName, sName, range, TerrStr[terr]);
		isSecondaryWeapon = true;
		isNotIRGuided = isWeaponNotIRGuided(which);
	}
	else if ( which == SECONDARY2 )
	{
		char wName[32];
		mainWpn = getSecondaryWeapon2();
		CString wpnName = mainWpn->getName();
		if ( wpnName == "None" )
			return -1;
		int length = wpnName.GetLength() + 1;
		for (int i = 0; i < length; i++)
			wName[i] = wpnName.GetAt(i);
		sprintf_s( logBuffer, "Calculating Secondary Weapon 2 [%s]\nTarget: \"%s\"\n Range: %d  Terrain: %s\n", wName, sName, range, TerrStr[terr]);
		isSecondaryWeapon = true;
		isNotIRGuided = isWeaponNotIRGuided(which);
	}
	strcpy( logString, logBuffer );

	int min_range = mainWpn->getMinRange();
	int max_range = mainWpn->getMaxRange();
	int woodsMod = mainWpn->getWoodsMod();
	int smokeMod = mainWpn->getWoodsMod();		// smoke is the same as woods
	int townMod = mainWpn->getTownMod();
	int oppMod = mainWpn->getOppFireMod();
	int moveMod = mainWpn->getMoveMod();
	int aerMod = mainWpn->getAerialMod();
	int evmMod = tgt->getUnitInfo()->getEVM();
	int defMod = tgt->getUnitInfo()->getDM();
	int supMod = tgt->getUnitInfo()->getSM();
	if ( range < min_range || range > max_range )
	{
		sprintf( logBuffer, "Out of range (Min Range: %d, Max Range: %d)\n", min_range, max_range );
		strcat( logString, logBuffer );
		return 0;
	}

	// base kill number
	rangeTable rt = mainWpn->getRangeTable();
	int BKN;
	if ( range >= 30 )
		BKN = rt.GetRangeData(7);
	else if ( range >= 25 )
		BKN = rt.GetRangeData(6);
	else if ( range >= 20 )
		BKN = rt.GetRangeData(5);
	else if ( range >= 15 )
		BKN = rt.GetRangeData(4);
	else if ( range >= 10 )
		BKN = rt.GetRangeData(3);
	else if ( range >= 5 )
		BKN = rt.GetRangeData(2);
	else
		BKN = rt.GetRangeData(1);
	sprintf( logBuffer, "Base Kill Number = %d (Range: %d)\n", BKN, range );
	strcat( logString, logBuffer );
	if ( BKN == 0 )
		return 0;

	// suppression modifier
	sprintf(logBuffer, "Suppression Modifier = %d\n", sup);
	strcat(logString, logBuffer);

	// secondary weapon modifier
	int secMod = 0;
	if (!isHelicopter() && isSecondaryWeapon && isNotIRGuided)
		secMod = -2;
	sprintf(logBuffer, "Secondary Weapon Modifier = %d\n", secMod);
	strcat(logString, logBuffer);

	// target type modifier
	targetTable tt = mainWpn->getTargetTable();
	int targetType = tgt->getUnitInfo()->getTargetType();
	switch( targetType )
	{
		case TT_A:
			ttMod = tt.GetTargetData(1);
			sprintf( logBuffer, "Target Type Modifier = %d (Type A)\n", ttMod );
			break;
		case TT_B:
			ttMod = tt.GetTargetData(2);
			sprintf( logBuffer, "Target Type Modifier = %d (Type B)\n", ttMod );
			break;
		case TT_C:
			ttMod = tt.GetTargetData(3);
			sprintf( logBuffer, "Target Type Modifier = %d (Type C)\n", ttMod );
			break;
		case TT_D:
			ttMod = tt.GetTargetData(4);
			sprintf( logBuffer, "Target Type Modifier = %d (Type D)\n", ttMod );
			break;
		case TT_E:
			ttMod = tt.GetTargetData(5);
			sprintf( logBuffer, "Target Type Modifier = %d (Type E)\n", ttMod );
			break;
		case TT_F:
			ttMod = tt.GetTargetData(6);
			sprintf( logBuffer, "Target Type Modifier = %d (Type F)\n", ttMod );
			break;
	}
	strcat( logString, logBuffer );

	// terrain modifier
	int terrainMod = 0;
	if ( terr == WOODS )
	{
		terrainMod = woodsMod;
		sprintf( logBuffer, "Terrain Modifier = %d (Woods)\n", terrainMod );
		strcat( logString, logBuffer );
	}
	else if ( terr == TOWN )
	{
		terrainMod = townMod;
		sprintf( logBuffer, "Terrain Modifier = %d (Town)\n", terrainMod );
		strcat( logString, logBuffer );
	}
	else
	{
		sprintf( logBuffer, "Terrain Modifier = %d (None)\n", terrainMod );
		strcat( logString, logBuffer );
	}

	// smoke modifier
	int lowLevel = tgt->getHeloOffset() > 0;
	if ( smoke &&  !lowLevel )
	{
		terrainMod += smokeMod;
		sprintf( logBuffer, "Smoke Modifier = %d\n", smokeMod );
		strcat( logString, logBuffer );
	}

	// moving modifier
	int movingMod = 0;
	int targetMoving = tgt->getMoved();
	if ( targetMoving )
		movingMod = moveMod;
	sprintf( logBuffer, "Moving Modifier = %d\n", movingMod );
	strcat( logString, logBuffer );

	// aerial modifier
	int aerialMod = 0;
	int unitType = tgt->getUnitInfo()->getUnitType();
	if ( unitType == UHH || unitType == UHM || unitType == ARH || unitType == LHX )
		aerialMod = aerMod;
	sprintf( logBuffer, "Aerial Modifier = %d\n", aerialMod );
	strcat( logString, logBuffer );

	// opportunity fire modifier
	int oppFireMod = 0;
	if ( opp )
		oppFireMod = oppMod;
	sprintf( logBuffer, "Opportunity Fire Modifier = %d\n", oppFireMod );
	strcat( logString, logBuffer );

	// defilade modifier
	int defiladeMod = 0;
	int targetInDefilade = tgt->getDefilade();
	if ( targetInDefilade )
		defiladeMod = defMod;
	sprintf( logBuffer, "In Defilade Modifier = %d\n", defiladeMod );
	strcat( logString, logBuffer );

	// evasive manuever modifier
	int evasiveMod = 0;
	int targetEvading = tgt->getEvading();
	if ( targetEvading )
		evasiveMod = evmMod;
	sprintf( logBuffer, "Evasive Maneuver Modifier = %d\n", evasiveMod );
	strcat( logString, logBuffer );

	// skylined modifier
	int skylineMod = 0;
	if ( skylined )
		skylineMod = 1;
	sprintf(logBuffer, "Skylining Modifier = %d\n", skylineMod);
	strcat(logString, logBuffer);

	// final kill number
	int FKN = BKN + ttMod + sup + secMod + terrainMod + oppFireMod + movingMod + aerialMod + defiladeMod + evasiveMod + skylineMod;
	sprintf( logBuffer, "Final Kill Number = %d\n", FKN );
	strcat( logString, logBuffer );

	// anything less than zero is essentially zero
	if ( FKN < 0 )
		return 0;

	return FKN;	
}

