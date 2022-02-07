#include "StdAfx.h"
#include "AirCavCounterData.h"
#include "ElevOffsetDlg.h"
#include "Observation.h"

double	US_OP_Cost[MAXUNITTYPES][MAXACTS] =
{
/*
 c   w  t  r   g   m   s  r   p   m  s   b   a   r  r  r   s  u  m  d  e  d  d
 l   o  o  i   u   i   a  o   o   a  a   r   u   o  u  u   t  p  o  i  v  e  e
 e   o  w  v   n   s   m  c   p   c  n   o   t   a  b  b   r  e  u  s  a  f  f
 a   d  n  e       s      k   u   r  d   k   o   d  1  2   e  l  n  m  s  1  2
 r   s     r       l      e   p   o      e   b             a  e  t  n  m
                   e      t              n   n             m  v     t  n
*/
{2,  3, 2, -1, 2, -1, -1, -1, -1, -1, 4, 3,  1,  1, 2, -1, 2, 2,-1,-1, 2, 5, 4}, /* TANK */
{2,  3, 2, -1, 2,  3,  3,  3, -1, -1, 4, 3,  1,  1, 1, -1, 2, 2, 4, 4, 2, 5, 3}, /* TLAV */
{1,  2, 2, -1, 1,  3,  3,  3, -1, -1, 2, 2, .5,  1, 1, -1, 1, 1, 4, 3, 2, 3, 2}, /* WV */
{2, -1, 5,  1, 1, -1, -1, -1, -1,  1, 4, 4,  1,  2, 3, -1, 0, 0, 1, 1, 4,-1,-1}, /* UHH */
{1,  2, 2,  1, 1,  4,  4,  4, -1, .5, 3, 2,  1,  1, 2,  4, 0, 0, 1, 1, 2,-1,-1}, /* UHM */
{.5, 2, 2, .5, 1,  3,  3,  3,  4, .5, 3, 1, .5, .5, 2,  3, 0, 0, 0, 0, 2,-1, 2}, /* ARH */
{.5, 2, 2, .5, 1,  1,  1,  1,  3, .5, 3, 1, .5, .5, 1,  2, 0, 0, 0, 0, 1,-1, 2}, /* LHX*/
{6,  8, 8, -1, 2,  3,  3,  3, -1, -1, 8, 8,  6,  6, 8, 10, 3, 2, 2, 1,-1, 0, 0}  /* INF */
};

double	Sov_OP_Cost[MAXUNITTYPES][MAXACTS] =
{
/*
 c   w  t  r   g   m  s   r   p   m  s  b   a   r  r  r   s  u  m  d  e  d  d
 l   o  o  i   u   i  a   o   o   a  a  r   u   o  u  u   t  p  o  i  v  e  e
 e   o  w  v   n   s  m   c   p   c  n  o   t   a  b  b   r  e  u  s  a  f  f
 a   d  n  e       s      k   u   r  d  k   o   d  1  2   e  l  n  m  s  1  2
 r   s     r       l      e   p   o     e   b             a  e  t  n  m
                   e      t             n   n             m  v     t  n
*/
{3,  4, 3, -1, 3, -1, -1,-1, -1, -1, 4, 4,  1,  2, 2, -1, 2, 2,-1,-1, 3, 6, 5}, /* TANK */
{3,  3, 2, -1, 3,  3,  3, 3, -1, -1, 4, 3,  1,  1, 1, -1, 2, 2, 4, 4, 3, 6, 4}, /* TLAV */
{1,  2, 2, -1, 1,  3,  3, 3, -1, -1, 2, 2, .5,  1, 1, -1, 1, 1, 4, 3, 2, 4, 3}, /* WV */
{3, -1,-1,  2, 1, -1, -1,-1, -1,  1, 4, 4,  2,  3, 3, -1, 0, 0, 1, 1, 4,-1,-1}, /* UHH */
{1,  3, 3,  1, 1,  4,  4, 4, -1, .5, 3, 3,  1,  2, 2,  4, 0, 0, 1, 1, 3,-1,-1}, /* UHM */
{.5, 3, 3,  1, 1,  3,  3, 3,  4, .5, 3, 1,  1,  1, 2,  4, 0, 0, 0, 0, 3,-1,-1}, /* ARH */
{.5, 2, 2, .5, 1,  1,  1, 1,  3, .5, 3, 1, .5, .5, 1,  2, 0, 0, 0, 0, 1,-1, 2}, /* LHX*/
{6,  8, 8, -1, 2,  3,  3, 3, -1, -1, 8, 8,  6,  6, 8, 10, 3, 2, 2, 1,-1, 0, 0}  /* INF */
};

int ClearObservationTable[MAXUNITTYPES][MAXACTS] = 
{
/*
 c   w  t  r  g   m  s  r  p  m  s   b  a  r  r  r  s  u  m  d  e  d  d
 l   o  o  i  u   i  a  o  o  a  a   r  u  o  u  u  t  p  o  i  v  e  e
 e   o  w  v  n   s  m  c  p  c  n   o  t  a  b  b  r  e  u  s  a  f  f
 a   d  n  e      s     k  u  r  d   k  o  d  1  2  e  l  n  m  s  1  2
 r   s     r      l     e  p  o      e  b           a  e  t  n  m
                  e     t            n  n           m  v     t  n
*/
{30, 4, 4, 0, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4}, /* TANK */
{30, 4, 4, 0, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4}, /* TLAV */
{30, 4, 4, 0, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4}, /* WV */
{35, 8, 8,35, 0,  0, 0, 0, 0, 0, 35,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5}, /* UHH */
{30, 5, 5,30, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5}, /* UHM */
{30, 5, 5,30, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5}, /* ARH */
{30, 5, 5,30, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5}, /* LHX*/
{10, 3, 3, 3, 0,  0, 0, 0, 0, 0, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3}  /* INF */
};

int RainObservationTable[MAXUNITTYPES][MAXACTS] = 
{
/*
 c   w  t  r  g   m  s  r  p  m  s   b  a  r  r  r  s  u  m  d  e  d  d
 l   o  o  i  u   i  a  o  o  a  a   r  u  o  u  u  t  p  o  i  v  e  e
 e   o  w  v  n   s  m  c  p  c  n   o  t  a  b  b  r  e  u  s  a  f  f
 a   d  n  e      s     k  u  r  d   k  o  d  1  2  e  l  n  m  s  1  2
 r   s     r      l     e  p  o      e  b           a  e  t  n  m
                  e     t            n  n           m  v     t  n
*/
{12, 2, 2, 0, 0,  0, 0, 0, 0, 0, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TANK */
{12, 2, 2, 0, 0,  0, 0, 0, 0, 0, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TLAV */
{12, 2, 2, 0, 0,  0, 0, 0, 0, 0, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* WV */
{18, 4, 4,18, 0,  0, 0, 0, 0, 0, 18, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHH */
{12, 3, 3,12, 0,  0, 0, 0, 0, 0, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHM */
{12, 3, 3,12, 0,  0, 0, 0, 0, 0, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ARH */
{12, 3, 3,12, 0,  0, 0, 0, 0, 0, 12, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* LHX*/
{ 2, 2, 2, 0, 0,  0, 0, 0, 0, 0,  2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  /* INF */
};

int OvercastObservationTable[MAXUNITTYPES][MAXACTS] = 
{
/*
 c   w  t  r  g   m  s  r  p  m  s   b   a   r  r  r  s  u  m  d  e  d  d
 l   o  o  i  u   i  a  o  o  a  a   r   u   o  u  u  t  p  o  i  v  e  e
 e   o  w  v  n   s  m  c  p  c  n   o   t   a  b  b  r  e  u  s  a  f  f
 a   d  n  e      s     k  u  r  d   k   o   d  1  2  e  l  n  m  s  1  2
 r   s     r      l     e  p  o      e   b            a  e  t  n  m
                  e     t            n   n            m  v     t  n
*/
{25, 2, 2, 0, 0,  0, 0, 0, 0, 0, 25, 3,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TANK */
{25, 2, 2, 0, 0,  0, 0, 0, 0, 0, 25, 3,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TLAV */
{25, 2, 2, 0, 0,  0, 0, 0, 0, 0, 25, 3,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* WV */
{30, 4, 4,30, 0,  0, 0, 0, 0, 0, 30, 5,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHH */
{25, 3, 3,25, 0,  0, 0, 0, 0, 0, 25, 3,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHM */
{25, 3, 3,25, 0,  0, 0, 0, 0, 0, 25, 3,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ARH */
{25, 3, 3,25, 0,  0, 0, 0, 0, 0, 25, 3,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* LHX*/
{ 5, 2, 2, 0, 0,  0, 0, 0, 0, 0,  5, 2,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  /* INF */
};

int HeavyFogObservationTable[MAXUNITTYPES][MAXACTS] = 
{
/*
 c   w  t  r  g   m  s  r  p  m  s  b   a   r  r  r  s  u  m  d  e  d  d
 l   o  o  i  u   i  a  o  o  a  a  r   u   o  u  u  t  p  o  i  v  e  e
 e   o  w  v  n   s  m  c  p  c  n  o   t   a  b  b  r  e  u  s  a  f  f
 a   d  n  e      s     k  u  r  d  k   o   d  1  2  e  l  n  m  s  1  2
 r   s     r      l     e  p  o     e   b            a  e  t  n  m
                  e     t           n   n            m  v     t  n
*/
{ 2, 1, 1, 0, 0, 0, 0, 0, 0,  0, 2, 1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TANK */
{ 2, 1, 1, 0, 0, 0, 0, 0, 0,  0, 2, 1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TLAV */
{ 2, 1, 1, 0, 0, 0, 0, 0, 0,  0, 2, 1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* WV */
{ 4, 2, 2, 4, 0, 0, 0, 0, 0,  0, 4, 2,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHH */
{ 2, 1, 1, 2, 0, 0, 0, 0, 0,  0, 2, 1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHM */
{ 2, 1, 1, 2, 0, 0, 0, 0, 0,  0, 2, 1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ARH */
{ 2, 1, 1, 2, 0, 0, 0, 0, 0,  0, 2, 1,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* LHX*/
{ 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 1, 0,  0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  /* INF */
};

AirCavCounterData::AirCavCounterData(void)
{
}

AirCavCounterData::AirCavCounterData(CString name, SideType side, CountryType country, 
					AirCavUnitData *unitinfo, int column, int row, int elev, float op, int mount, int mounted)
{
	unitinfo->setCountryType(country);
	m_name = name;
	m_side = side;
	m_country = country;
	m_unitInfo = unitinfo;
	m_hexCol = column;
	m_hexRow = row;
	m_OPs = op < 0 ? 0.0 : op;
	m_OPsSpentMoving = 0;
	m_isActive = NOT_ACTIVE;
	m_isOppFiring = FALSE;
	m_isPopUp = NOT_IN_POPUP;
    m_wpnOppFiring = 0;
    m_tgtOppFiring = 0;
	m_status = op < 0 ? DEAD : ALIVE;		// a dead unit has negative OPs remaining
	m_mode = 0;
	m_fired = 0;
	m_moved = 0;
	m_rangeToActiveUnit = 0;
	m_activeUnitRange = 0;
	m_inDefilade = FALSE;
	m_evasiveManeuver = FALSE;
	m_isOppFiring = FALSE;
	m_tgtOppFiring = -1;
	m_elevOffset = elev;
	m_nextElevOffset = 0;
	m_heloOffset = 0;
	m_numMountedUnits = 0;
	m_mountedUnits[0] = 0;
	m_mountedUnits[1] = 0;
	m_mountedUnits[2] = 0;
	m_mountedUnits[3] = 0;
	m_mountedUnits[4] = 0;
	m_mountedUnits[5] = 0;
	m_carriedByUnit = mount;
	m_isDismounted = !mounted;
	m_FKNm1 = 0;
	m_FKNm2 = 0;
	m_FKNm3 = 0;
	m_FKNs1 = 0;
	m_FKNs2 = 0;
	m_ttModm1 = 0;
	m_ttModm2 = 0;
	m_ttModm3 = 0;
	m_ttMods1 = 0;
	m_ttMods2 = 0;
	m_nAmmoMainWpn1 = m_nFullAmmoMainWpn1 = unitinfo->getAmmoMainWeapon1();
	m_nAmmoMainWpn2 = m_nFullAmmoMainWpn2 = unitinfo->getAmmoMainWeapon2();
	m_nAmmoMainWpn3 = m_nFullAmmoMainWpn3 = unitinfo->getAmmoMainWeapon3();
	m_nAmmoSecondaryWpn1 = m_nFullAmmoSecondaryWpn1 = unitinfo->getAmmoSecondaryWeapon1();
	m_nAmmoSecondaryWpn2 = m_nFullAmmoSecondaryWpn2 = unitinfo->getAmmoSecondaryWeapon2();
	m_macroMove = 0;
	m_actionTaken = false;
	m_isSuppressed = FALSE;
	m_opticsInUse = OPTICS_OPTICAL_SIGHT;
	m_laserDesignated = -1;
	m_laserDesignating = -1;
	m_radarInUse = FALSE;
}

AirCavCounterData::~AirCavCounterData(void)
{
}

void AirCavCounterData::reset()
{
	// this reset is for bringing units back to initial state
	m_status = ALIVE;
	m_fired = FALSE;
	m_moved = FALSE;
	m_OPs = MAX_OPS;
	m_OPsSpentMoving = 0;
	m_inDefilade = FALSE;
	m_evasiveManeuver = FALSE;
	m_isOppFiring = FALSE;
	m_isPopUp = NOT_IN_POPUP;
    m_wpnOppFiring = 0;
    m_tgtOppFiring = -1;
	m_macroMove = 0;
	m_actionTaken = false;
	m_isSuppressed = FALSE;
	m_nAmmoMainWpn1 = m_nFullAmmoMainWpn1;
	m_nAmmoMainWpn2 = m_nFullAmmoMainWpn2;
	m_nAmmoMainWpn3 = m_nFullAmmoMainWpn3;
	m_nAmmoSecondaryWpn1 = m_nFullAmmoSecondaryWpn1;
	m_nAmmoSecondaryWpn2 = m_nFullAmmoSecondaryWpn2;
	m_opticsInUse = OPTICS_OPTICAL_SIGHT;
	m_laserDesignated = -1;
	m_laserDesignating = -1;
	m_radarInUse = FALSE;
}

void AirCavCounterData::resetActive()
{
	// don't want to reset back to ALIVE - this is meant for reset between turns
	//m_status = ALIVE;	
	m_fired = FALSE;
	m_moved = FALSE;
	m_OPs = MAX_OPS;
	m_OPsSpentMoving = 0;
	//m_inDefilade = FALSE;
	m_evasiveManeuver = FALSE;
	m_isOppFiring = FALSE;
	m_isPopUp = NOT_IN_POPUP;
    m_wpnOppFiring = 0;
    m_tgtOppFiring = -1;
	m_macroMove = 0;
	m_actionTaken = false;
	m_isSuppressed = FALSE;
	m_laserDesignated = -1;
	m_laserDesignating = -1;
}

double AirCavCounterData::incrOPs(double op)
{
	m_OPs += op;
	if ( m_OPs > MAX_OPS ) m_OPs = MAX_OPS;
	return m_OPs;
}

double AirCavCounterData::decrOPs(double op, bool oppCost)
{
	if ( op > m_OPs )
		return -1;

	m_OPs -= op;
	if ( m_OPs < 0 ) m_OPs = 0;
	return m_OPs;
}

void AirCavCounterData::setIsPopUp(int popup)
{
	// if "in popup" but have not fired and going to "not popup", expend OPs for the popup only
	if (m_isPopUp == POPUP_NOT_FIRED && popup == NOT_IN_POPUP)
	{
		UnitType utype = m_unitInfo->getUnitType();
		CountryType side = m_unitInfo->getCountryType();

		double OPcost;
		if (isNATO(side))
			OPcost = US_OP_Cost[utype][POPUP];
		else
			OPcost = Sov_OP_Cost[utype][POPUP];
		if (OPcost > 0)
		{
			if (decrOPs(OPcost, false) != -1)
			{
				nofire();
				move();
				m_isPopUp = popup;
			}
		}
		return;
	}

	// otherwise, if "not popup", go to "in popup" (Fired and not Moving)
	if (popup == NOT_IN_POPUP)
	{
		fire();
		nomove();
	}
	else
	{
		// and if "in popup", go back to "not popup" (not Fired and Moving)
		nofire();
		move();
	}
	m_isPopUp = popup;
}

int AirCavCounterData::moveAction( AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int col, int row, int from, int popSmoke, int timeOfDay, int weather, bool goingToNOE )
{
	double OPcost;

	// check if within bounds of map
	if ( row < MIN_ROWS || col < MIN_COLUMNS || row > MAX_ROWS || col > MAX_COLUMNS )
		return 0;

	CountryType side = m_unitInfo->getCountryType();
	UnitType unitType = m_unitInfo->getUnitType();

	// determine if this unit moving into this hex would overstack
	if (hexStackingFull(mapData, counterData, row, col, false, goingToNOE))
		return 0;

	bool isInfantryUnit = m_unitInfo->isInfantry();
	bool isHeavyHeloUnit = m_unitInfo->isHeavyHelicopter();

	int nextTerr = mapData->getTerrain( row, col );
	int currRoad = mapData->getRoad( m_hexRow, m_hexCol );
	int nextRoad = mapData->getRoad( row, col );
	int currAutobahn = mapData->getAutobahn( m_hexRow, m_hexCol );
	int nextAutobahn = mapData->getAutobahn( row, col );
	int currRiver = mapData->getRiver( m_hexRow, m_hexCol );
	int nextRiver = mapData->getRiver( row, col );
	int currHexElevation = mapData->getElevation( m_hexRow, m_hexCol );
	int nextHexElevation = mapData->getElevation( row, col );
	int currElevation = currHexElevation + m_elevOffset;
	int nextElevation = nextHexElevation + m_nextElevOffset;
	int smokeHex = mapData->getSmoke( row, col );
	int wreckHex = mapData->getWreck( row, col );	// Wreck x2 marker or more does not benefit from Road
	if ( wreckHex > 1 )	nextRoad = 0;

	// determine whether crossing a road hex
	if ( from == DIRECTION_NO && mapData->getRoadHex(row, col, DIRECTION_NO) && wreckHex < 2)
		nextTerr = ROAD;
	else if ( from == DIRECTION_NW && mapData->getRoadHex(row, col, DIRECTION_NW) && wreckHex < 2)
		nextTerr = ROAD;
	else if ( from == DIRECTION_SW && mapData->getRoadHex(row, col, DIRECTION_SW) && wreckHex < 2)
		nextTerr = ROAD;
	else if ( from == DIRECTION_SO && mapData->getRoadHex(row, col, DIRECTION_SO) && wreckHex < 2)
		nextTerr = ROAD;
	else if ( from == DIRECTION_SE && mapData->getRoadHex(row, col, DIRECTION_SE) && wreckHex < 2)
		nextTerr = ROAD;
	else if ( from == DIRECTION_NE && mapData->getRoadHex(row, col, DIRECTION_NE) && wreckHex < 2)
		nextTerr = ROAD;

	// determine whether crossing an autobahn hex
	if ( from == DIRECTION_NO && mapData->getAutobahnHex(row, col, DIRECTION_NO) )
		nextTerr = AUTOBAHN;
	else if ( from == DIRECTION_NW && mapData->getAutobahnHex(row, col, DIRECTION_NW) )
		nextTerr = AUTOBAHN;
	else if ( from == DIRECTION_SW && mapData->getAutobahnHex(row, col, DIRECTION_SW) )
		nextTerr = AUTOBAHN;
	else if ( from == DIRECTION_SO && mapData->getAutobahnHex(row, col, DIRECTION_SO) )
		nextTerr = AUTOBAHN;
	else if ( from == DIRECTION_SE && mapData->getAutobahnHex(row, col, DIRECTION_SE) )
		nextTerr = AUTOBAHN;
	else if ( from == DIRECTION_NE && mapData->getAutobahnHex(row, col, DIRECTION_NE) )
		nextTerr = AUTOBAHN;

	// determine if there is a hexside that will cost extra
	bool stream = false;
	if ( from == DIRECTION_NO && mapData->getStreamHex(row, col, 0) )
		stream = true;
	else if ( from == DIRECTION_NW && mapData->getStreamHex(row, col, 1) )
		stream = true;
	else if ( from == DIRECTION_SW && mapData->getStreamHex(row, col, 2) )
		stream = true;
	else if ( from == DIRECTION_SO && mapData->getStreamHex(row, col, 3) )
		stream = true;
	else if ( from == DIRECTION_SE && mapData->getStreamHex(row, col, 4) )
		stream = true;
	else if ( from == DIRECTION_NE && mapData->getStreamHex(row, col, 5) )
		stream = true;

	// for helicopters at low level or moving along road/autobahn or river, the only cost is the macro cost
	bool isHeloUnit = m_unitInfo->isHelicopter();
	bool heloAtLowLevel = (isHeloUnit && m_heloOffset > 0);
	bool alongRoadOrRiver = (currRoad && nextRoad || currAutobahn && nextAutobahn || currRiver && nextRiver);
	bool heloAtNOEalongRoadOrRiver = (isHeloUnit && !heloAtLowLevel && alongRoadOrRiver);
	if ( isHeloUnit )
	{
		// entering Nap of the earth from Low Level
		if ( from == -1 )
		{
			heloAtLowLevel = true;
			m_macroMove = 0;
		}
		// moving at Low Level, or along a road/river - cost is macrohex
		else if ( heloAtLowLevel || heloAtNOEalongRoadOrRiver )
		{
			nextTerr = MACRO;

			// check if moved a macroHex, if so, reset the tally
			m_macroMove++;
			if ( m_macroMove > 2 )		// macrohex is basically 3 hexes
				m_macroMove = 0;
		}
	}

	// calculate basic OP cost based on nationality
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[unitType][nextTerr];
	else
		OPcost = Sov_OP_Cost[unitType][nextTerr];

	// determine if this is a valid maneuver and what the cost is
	if ( OPcost > 0 )
	{
		// a helicopter at low level ignores the following terrain features
		if  ( heloAtLowLevel )
		{
			// the cost for a macroHex is zero until the tally is reset back to zero
			if ( m_macroMove != 0 )
				OPcost = 0;
		}
		else
		{
			// the cost for a macroHex is zero until the tally is reset back to zero
			if ( heloAtNOEalongRoadOrRiver && m_macroMove != 0 )
				OPcost = 0;

			// nap-of-earth helicopters cannnot enter a smoke hex
			if ( smokeHex && isHeloUnit )
				return 0;

			// crossing a stream
			if ( stream )
			{
				if ( isNATO( side ) )
					OPcost += US_OP_Cost[unitType][STREAM];
				else
					OPcost += Sov_OP_Cost[unitType][STREAM];
			}

			// figure in changes in elevation (only if not on road or autobahn)
			if ( !(currRoad && nextRoad) && !(currAutobahn && nextAutobahn) )
			{
				if ( nextElevation > currElevation )
				{
					if ( isNATO( side ) )
						OPcost += US_OP_Cost[unitType][UPELEV];
					else
						OPcost += Sov_OP_Cost[unitType][UPELEV];
				}
			}
			// creating smoke hex costs extra
			if ( popSmoke )
				OPcost += 1.0;
			// being suppressed costs extra
			if (m_isSuppressed)
				OPcost += 1.0;
			// smoke hex costs extra for all other units
			if ( smokeHex )
				OPcost += 1.0;
			// night costs extra (NOTE: tables don't mention towns, presumably because they have lights at night)
			if ( timeOfDay == TIME_NIGHT )
				OPcost += 1.0;
			// bad weather costs extra (NOTE: presumably operations in towns is not hindered by bad weather)
			if ( weather == WEATHER_HVY_FOG || weather == WEATHER_RAIN || weather == WEATHER_SNOW )
				OPcost += ((nextTerr == TOWN) ? 0.0 : 2.0);
		}

		// determine if there are enough operation points
		if ( decrOPs(OPcost) != -1 )
		{
			m_OPsSpentMoving += OPcost;
			if ( m_OPsSpentMoving >= 6 )
				m_moved = 1;
			setActionTaken(true);
			setDefilade(FALSE);
			m_elevOffset = m_nextElevOffset;
			if ( popSmoke )
				mapData->setSmoke( row, col, false );
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::fireGun(bool oppFire)
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][GUN];
	else
		OPcost = Sov_OP_Cost[utype][GUN];
	if ( oppFire )
		OPcost += 1;
	// being suppressed costs extra
	if (m_isSuppressed)
		OPcost += 1.0;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
			setActionTaken(true);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::fireMissile(bool oppFire)
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][ATGM];
	else
		OPcost = Sov_OP_Cost[utype][ATGM];
	if ( oppFire )
		OPcost += 1;
	// being suppressed costs extra
	if (m_isSuppressed)
		OPcost += 1.0;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
			setActionTaken(true);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::fireRocket(bool oppFire)
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][ROCKET];
	else
		OPcost = Sov_OP_Cost[utype][ROCKET];
	if ( oppFire )
		OPcost += 1;
	// being suppressed costs extra
	if (m_isSuppressed)
		OPcost += 1.0;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
			setActionTaken(true);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::fireSAM(bool oppFire)
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][SAM];
	else
		OPcost = Sov_OP_Cost[utype][SAM];
	if ( oppFire )
		OPcost += 1;
	// being suppressed costs extra
	if (m_isSuppressed)
		OPcost += 1.0;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
			setActionTaken(true);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::popupFire(bool oppFire)
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][POPUP];
	else
		OPcost = Sov_OP_Cost[utype][POPUP];
	if ( oppFire )
		OPcost += 1;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
			setActionTaken(true);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::oppFire(int weaponType)
{
	if ( weaponType == GUN && fireGun(true) )
	{
		m_isOppFiring = TRUE;
		m_fired = TRUE;
		return true;
	}
	else if ( weaponType == ATGM && fireMissile(true) )
	{
		m_isOppFiring = TRUE;
		m_fired = TRUE;
		return true;
	}
	else if ( weaponType == ROCKET && fireRocket(true) )
	{
		m_isOppFiring = TRUE;
		m_fired = TRUE;
		return true;
	}
	else if ( weaponType == SAM && fireSAM(true) )
	{
		m_isOppFiring = TRUE;
		m_fired = TRUE;
		return true;
	}
	return false;
}

int AirCavCounterData::checkContour(AirCavMapData *mapData, int col, int row, int from, int curOffset)
{
	int elevOffset = 0;
	bool heloAtLowLevel = false;
	if ( m_unitInfo->isHelicopter() && m_heloOffset > 0 )
	{
		heloAtLowLevel = true;
	}

	// get this elevation - this will bring up a popup if not known
	int nextElev = mapData->getElevation(row, col);
	int contour = mapData->getContour( row, col );

	// don't check contours for helicopters at low level
	if ( contour > 0 && !heloAtLowLevel )
	{
		bool moveViaRoad = false;
		if (from > 0)
		{
			// determine whether crossing a road or autohex
			if ((from == DIRECTION_NO && mapData->getRoadHex(row, col, DIRECTION_NO)) ||
				(from == DIRECTION_NW && mapData->getRoadHex(row, col, DIRECTION_NW)) ||
				(from == DIRECTION_SW && mapData->getRoadHex(row, col, DIRECTION_SW)) ||
				(from == DIRECTION_SO && mapData->getRoadHex(row, col, DIRECTION_SO)) ||
				(from == DIRECTION_SE && mapData->getRoadHex(row, col, DIRECTION_SE)) ||
				(from == DIRECTION_NE && mapData->getRoadHex(row, col, DIRECTION_NE)))
			{
				moveViaRoad = true;
			}

			if ((from == DIRECTION_NO && mapData->getAutobahnHex(row, col, DIRECTION_NO)) ||
				(from == DIRECTION_NW && mapData->getAutobahnHex(row, col, DIRECTION_NW)) ||
				(from == DIRECTION_SW && mapData->getAutobahnHex(row, col, DIRECTION_SW)) ||
				(from == DIRECTION_SO && mapData->getAutobahnHex(row, col, DIRECTION_SO)) ||
				(from == DIRECTION_SE && mapData->getAutobahnHex(row, col, DIRECTION_SE)) ||
				(from == DIRECTION_NE && mapData->getAutobahnHex(row, col, DIRECTION_NE)))
			{
				moveViaRoad = true;
			}
		}

		// don't bring up the dialog if moving via a road - always stay at the lowest elevation
		if (moveViaRoad)
		{
			elevOffset = -20;
			if ((elevOffset + nextElev) < 0)
				elevOffset += 10;
		}
		else
		{
			ElevOffsetDlg dlg(NULL, m_elevOffset);
			if (dlg.DoModal() == IDOK)
			{
				elevOffset = dlg.getOffset();
				if ((elevOffset + nextElev) < 0)
					elevOffset += 10;
			}
		}
	}
	return elevOffset;
}

void AirCavCounterData::setHeloOffset(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int elev, bool noCost)
{
	double OPcost;

	if ( m_heloOffset > 0 )
	{
		if ( noCost )
			m_heloOffset = elev;
		// changing from Low Level to Nap of the Earth (cost is terrain)
		else if ( moveAction( mapData, counterData, m_hexCol, m_hexRow, -1, false, TIME_DAY, WEATHER_CLEAR, true ) )
			m_heloOffset = elev;
	}
	else
	{
		// changing from Nap of the Earth to Low Level (cost is macrohex)
		CountryType side = m_unitInfo->getCountryType();
		UnitType utype = m_unitInfo->getUnitType();
		if ( isNATO( side ) )
			OPcost = US_OP_Cost[utype][MACRO];
		else
			OPcost = Sov_OP_Cost[utype][MACRO];

		if ( OPcost > 0 )
		{
			// first check to see if its possible to go to low level
			if (hexStackingFull(mapData, counterData, m_hexRow, m_hexCol, true))
			{
				return;
			}

			if ( noCost )
				m_heloOffset = elev;
			else if ( decrOPs(OPcost) != -1 )
			{
				m_heloOffset = elev;
				setActionTaken(true);
			}
		}
	}
}

void AirCavCounterData::moveTo(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int col, int row, bool doCheckContour)
{
	if ( mapData->validHex(col, row) )
	{
		if (hexStackingFull(mapData, counterData, row, col))
			return;

		m_hexRow = row;
		m_hexCol = col;
		m_elevOffset = doCheckContour ? checkContour(mapData, col, row, DIRECTION_UNDEF) : 0;
	}
}

void AirCavCounterData::moveNorth(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather)
{
	int nextRow = m_hexRow-1;
	int nextCol = m_hexCol;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow, DIRECTION_SO);
		if ( moveAction( mapData, counterData, nextCol, nextRow, DIRECTION_SO, popSmoke, timeOfDay, weather ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveNorthWest(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow-1 : m_hexRow;
	int nextCol = m_hexCol - 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow, DIRECTION_SE);
		if ( moveAction( mapData, counterData, nextCol, nextRow, DIRECTION_SE, popSmoke, timeOfDay, weather ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveNorthEast(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow-1 : m_hexRow;
	int nextCol = m_hexCol + 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow, DIRECTION_SW);
		if ( moveAction( mapData, counterData, nextCol, nextRow, DIRECTION_SW, popSmoke, timeOfDay, weather ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveSouth(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather)
{
	int nextRow = m_hexRow + 1;
	int nextCol = m_hexCol;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow, DIRECTION_NO);
		if ( moveAction( mapData, counterData, nextCol, nextRow, DIRECTION_NO, popSmoke, timeOfDay, weather ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveSouthWest(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow : m_hexRow + 1;
	int nextCol = m_hexCol - 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow, DIRECTION_NE);
		if ( moveAction( mapData, counterData, nextCol, nextRow, DIRECTION_NE, popSmoke, timeOfDay, weather ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveSouthEast(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow : m_hexRow + 1;
	int nextCol = m_hexCol + 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow, DIRECTION_NW);
		if ( moveAction( mapData, counterData, nextCol, nextRow, DIRECTION_NW, popSmoke, timeOfDay, weather ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

bool AirCavCounterData::isVisible(int terrain, int range, int weather, int timeofday, int smoke)
{
	int maxFired = 40;
	int maxLowLevelHeavyHelo = 40;
	int maxLowLevelLightHelo = 30;
	if ( weather == WEATHER_RAIN || weather == WEATHER_SNOW || weather == WEATHER_LT_FOG )
	{
		maxFired = 25;
		maxLowLevelHeavyHelo = 25;
		maxLowLevelLightHelo = 20;
	}
	else if ( weather == WEATHER_OVERCAST )
	{
		maxFired = 35;
		maxLowLevelHeavyHelo = 35;
		maxLowLevelLightHelo = 30;
	}
	else if ( weather == WEATHER_HVY_FOG )
	{
		maxFired = 6;
		maxLowLevelHeavyHelo = 6;
		maxLowLevelLightHelo = 4;
	}

	bool lowlevel = m_heloOffset > 0;
	if ( m_fired && range < maxFired )
		return 1;
	else if (m_unitInfo->isHeavyHelicopter() && lowlevel && range < maxLowLevelHeavyHelo )
		return 1;
	else if (m_unitInfo->isLightHelicopter() && lowlevel && range < maxLowLevelLightHelo )
		return 1;
	else
	{
		if (m_inDefilade && terrain == CLEAR)
			terrain = DEFCLR;
		else if (m_inDefilade)
			terrain = DEFBRK;

		UnitType unitType = m_unitInfo->getUnitType();
		int maxRange = ClearObservationTable[unitType][terrain];
		if ( weather == WEATHER_RAIN || weather == WEATHER_SNOW || weather == WEATHER_LT_FOG )
			maxRange = RainObservationTable[unitType][terrain];
		else if ( weather == WEATHER_OVERCAST )
			maxRange = OvercastObservationTable[unitType][terrain];
		else if ( weather == WEATHER_HVY_FOG )
			maxRange = HeavyFogObservationTable[unitType][terrain];
		
		if ( m_moved )
			maxRange += 2;
		if ( smoke && !lowlevel )
			maxRange /= 2;
		if ( range <= maxRange )
			return 1;
		else
			return 0;
	}
}

// note: in this code, 'this' is the unit being observed (aka the 'target'), the passed in values are for the 'observer'
bool AirCavCounterData::isVisibleAdvanced(int terrain, int range, int weather, int timeofday, SideType side, int optics, bool radar, int smoke)
{
	// if this unit is using a WLSL at night - visibility is automatic at any range
	if (timeofday == TIME_NIGHT && m_opticsInUse == OPTICS_WHITELIGHT_SEARCHLIGHT)
		return true;

	// if this unit is using a IRSL - visibility is automatic at any range if observer is using TI
	if (m_opticsInUse == OPTICS_INFRARED_SEARCHLIGHT && optics == OPTICS_THERMAL_IMAGER)
		return true;

	// low level helicopter
	bool lowlevelHelo = (m_unitInfo->isHelicopter() && m_heloOffset > 0);
	UnitType unitType = m_unitInfo->getUnitType();

	// low level helicopter is visible at any range if observer is using radar
	if (lowlevelHelo && radar)
		return true;

	// get the max range at which the target can be seen
	ObservationData observationData;
	int maxRange = observationData.getObservationRange(unitType, side, optics, terrain, lowlevelHelo, m_inDefilade, m_fired, timeofday, weather);

	// if target is moving, range is increased by 2
	if (m_moved)
		maxRange += 2;

	// if using Thermal Imager, a unit is unaffected by smoke in its own hex
	bool usingTI = (optics == OPTICS_THERMAL_IMAGER);

	// if sighting out of a smoke hex, range is halved, unless at low level or using Thermal Imager
	if (smoke && !lowlevelHelo && !usingTI)
		maxRange /= 2;

	// if range is beyond max range, target is not visible
	if (range <= maxRange)
		return true;
	else
		return false;
}

void AirCavCounterData::setFinalKillNumbers( int FKNm1, int FKNm2, int FKNm3, int FKNs1, int FKNs2, int ttm1, int ttm2, int ttm3, int tts1, int tts2 )
{
	m_FKNm1 = FKNm1;
	m_FKNm2 = FKNm2;
	m_FKNm3 = FKNm3;
	m_FKNs1 = FKNs1;
	m_FKNs2 = FKNs2;
	m_ttModm1 = ttm1;
	m_ttModm2 = ttm2;
	m_ttModm3 = ttm3;
	m_ttMods1 = tts1;
	m_ttMods2 = tts2;
}

int AirCavCounterData::enterDefilade(int terrain, int toggle)
{
	double OPcost;

	if ( m_inDefilade && toggle )
	{
		m_inDefilade = FALSE;
		return 0;
	}

	// no cost for infantry to enter defilade
	if ( m_unitInfo->isInfantry() )
	{
		m_inDefilade = TRUE;
		return 1;
	}

	int terrainType = (terrain == CLEAR) ? DEFCLR : DEFBRK;
	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][terrainType];
	else
		OPcost = Sov_OP_Cost[utype][terrainType];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			m_inDefilade = TRUE;
			setActionTaken(true);
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::enoughOPsForEvasiveManeuver()
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if (isNATO(side))
		OPcost = US_OP_Cost[utype][EVM];
	else
		OPcost = Sov_OP_Cost[utype][EVM];

	if (m_OPs >= OPcost)
		return true;
	return false;
}

int AirCavCounterData::evasiveManeuver(int toggle)
{
	double OPcost;

	if ( m_evasiveManeuver && toggle )
	{
		m_evasiveManeuver = FALSE;
		return 0;
	}

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][EVM];
	else
		OPcost = Sov_OP_Cost[utype][EVM];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			m_evasiveManeuver = TRUE;
			return m_unitInfo->getEVM();
		}
		else
			return 0;
	}
	else
	{
		return 0;
	}
}

int AirCavCounterData::laySmoke( AirCavMapData *mapData, int col, int row )
{
	double OPcost = 1;

	if ( decrOPs(OPcost) != -1 )
	{
		mapData->setSmoke( row, col );
		setActionTaken(true);
		return 1;
	}
	else
		return 0;
}

int AirCavCounterData::mountCarrier( void )
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][MOUNT];
	else
		OPcost = Sov_OP_Cost[utype][MOUNT];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			m_isDismounted = FALSE;
			setActionTaken(true);
			return 1;
		}
	}
	return 0;
}

int AirCavCounterData::dismountCarrier( void )
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][DISMOUNT];
	else
		OPcost = Sov_OP_Cost[utype][DISMOUNT];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			m_isDismounted = TRUE;
			setActionTaken(true);
			return 1;
		}
	}
	return 0;
}

bool AirCavCounterData::canMount( void )
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][MOUNT];
	else
		OPcost = Sov_OP_Cost[utype][MOUNT];

	if ( OPcost > 0 )
	{
		if ( getOPs() >= OPcost )
			return true;
	}
	return false;
}

bool AirCavCounterData::canDismount( void )
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][DISMOUNT];
	else
		OPcost = Sov_OP_Cost[utype][DISMOUNT];

	if ( OPcost > 0 )
	{
		if ( getOPs() >= OPcost )
			return true;
	}
	return false;
}

int AirCavCounterData::mountUnit( void )
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][MOUNT];
	else
		OPcost = Sov_OP_Cost[utype][MOUNT];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			setActionTaken(true);
			return 1;
		}
	}
	return 0;
}

int AirCavCounterData::dismountUnit( void )
{
	double OPcost;

	UnitType utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][DISMOUNT];
	else
		OPcost = Sov_OP_Cost[utype][DISMOUNT];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			setActionTaken(true);
			return 1;
		}
	}
	return 0;
}

bool AirCavCounterData::hexStackingFull(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int row, int col, bool checkLowLevel, bool isGoingtoNOE)
{
	// stacking limits:
	// - 6 ground vehicles
	// - 20 infantry teams (1 squad = 2 teams)
	// - at NOE, helicopter is equal to 2 vehicles, 3 for heavy helo
	// - at Low Level, limit is 2 helicopters
	// - ignoring wrecks for now
	bool isInfantryUnit = m_unitInfo->isInfantry();
	bool isHeloUnit = m_unitInfo->isHelicopter();
	bool heloAtLowLevel = false;
	if (m_heloOffset > 0)
	{
		heloAtLowLevel = true;
	}

	// start the stacking value with any wrecks in the hex (for vehicle stacking only)
	int vehicleStackingCount = mapData->getWreck(row, col);
	int infantryStackingCount = 0;
	int lowLevelHeloStackingCount = 0;

	// iterate across all counters and apply each stacking value to its appropriate limit
	for (int c = 0; c < MAXCOUNTERS; c++)
	{
		if (counterData[c] && counterData[c]->getHexCol() == col && counterData[c]->getHexRow() == row)
		{
			// don't count mounted units
			if (counterData[c]->getIsDismounted() == false)
				continue;

			// don't count dead units (these will be counted as wrecks)
			if (counterData[c]->getIsAlive() == false)
				continue;

			AirCavUnitData *unitInfo = counterData[c]->getUnitInfo();
			CString unitName = unitInfo->getName();
			bool isAtLowLevel = (counterData[c]->getHeloOffset() > 0);
			int unitStackingValue = unitInfo->isHeavyHelicopter() ? 3 : unitInfo->isHelicopter() ? 2 : 1; // helo at NOE depends on type
			
			if ( unitInfo->isHelicopter() && isAtLowLevel )
				lowLevelHeloStackingCount += 1;	// helo at low level is just one
			else if (unitInfo->isInfantry() && unitName.Find(_T("Squad")) > 0 )
				infantryStackingCount += unitStackingValue * 2;
			else if (unitInfo->isInfantry() )
				infantryStackingCount += unitStackingValue;
			else if (unitInfo->isVehicle())
				vehicleStackingCount += unitStackingValue;
		}
	}

	// determine if hex is already at maximum
	if (isHeloUnit && (heloAtLowLevel || checkLowLevel) && !isGoingtoNOE)
		return (lowLevelHeloStackingCount >= HELO_STACKING_LIMIT);
	else if (isInfantryUnit)
		return (infantryStackingCount >= INFANTRY_STACKING_LIMIT);
	else
		return (vehicleStackingCount >= VEHICLE_STACKING_LIMIT);
}

CString AirCavCounterData::getStackingString(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int row, int col)
{
	int vehicleStackingCount = 0;
	int infantryStackingCount = 0;
	int lowLevelHeloStackingCount = 0;
	for (int c = 0; c < MAXCOUNTERS; c++)
	{
		if (counterData[c] && counterData[c]->getHexCol() == col && counterData[c]->getHexRow() == row)
		{
			// don't count mounted units
			if (counterData[c]->getIsDismounted() == false)
				continue;

			AirCavUnitData *unitInfo = counterData[c]->getUnitInfo();
			CString unitName = unitInfo->getName();
			bool isInfantryUnit = unitInfo->isInfantry();
			bool isAtLowLevel = (counterData[c]->getHeloOffset() > 0);
			int unitStackingValue = unitInfo->isHeavyHelicopter() ? 3 : unitInfo->isHelicopter() ? 2 : 1; // helo at NOE depends on type

			if ( unitInfo->isHelicopter() && isAtLowLevel )
				lowLevelHeloStackingCount += 1;	// helo at low level is just one
			else if ( unitInfo->isInfantry() && unitName.Find(_T("Squad")) > 0 )
				infantryStackingCount += unitStackingValue * 2;
			else if ( unitInfo->isInfantry() )
				infantryStackingCount += unitStackingValue;
			else if ( unitInfo->isVehicle() )
				vehicleStackingCount += unitStackingValue;
		}
	}

	// construct string
	char buffer[MAX_BUF_SIZE];
	sprintf_s(buffer, "%d/%d/%d", vehicleStackingCount, infantryStackingCount, lowLevelHeloStackingCount);
	return (CString)buffer;
}