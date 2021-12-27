#include "StdAfx.h"
#include "AirCavCounterData.h"
#include "ElevOffsetDlg.h"

double	US_OP_Cost[MAXUNITTYPES][MAXACTS] =
{
/*
 c   w  t  r  g   m  s  r  p  m  s  b   a   r  r  r  s  u  m  d  e  d  d
 l   o  o  i  u   i  a  o  o  a  a  r   u   o  u  u  t  p  o  i  v  e  e
 e   o  w  v  n   s  m  c  p  c  n  o   t   a  b  b  r  e  u  s  a  f  f
 a   d  n  e      s     k  u  r  d  k   o   d  1  2  e  l  n  m  s  1  2
 r   s     r      l     e  p  o     e   b            a  e  t  n  m
                  e     t           n   n            m  v     t  n
*/
{2,  3, 2, -1, 2,-1,-1.-1,-1,-1, 4, 3,  1,  1, 2,-1, 2, 2,-1,-1, 2, 5, 4}, /* TANK */
{2,  3, 2, -1, 2, 3, 3, 3,-1,-1, 4, 3,  1,  1, 1,-1, 2, 2, 4, 4, 2, 5, 3}, /* TLAV */
{1,  2, 2, -1, 1, 3, 3, 3,-1,-1, 2, 2, .5,  1, 1,-1, 1, 1, 4, 3, 2, 3, 2}, /* WV */
{2, -1, 5,  1, 1,-1,-1,-1,-1, 1, 4, 4,  1,  2, 3,-1, 0, 0, 1, 1, 4,-1,-1}, /* UHH */
{1,  2, 2,  1, 1, 4, 4, 4,-1,.5, 3, 2,  1,  1, 2, 4, 0, 0, 1, 1, 2,-1,-1}, /* UHM */
{.5, 2, 2, .5, 1, 3, 3, 3, 4,.5, 3, 1, .5, .5, 2, 3, 0, 0, 0, 0, 2,-1, 2}, /* ARH */
{.5, 2, 2, .5, 1, 1, 1, 1, 3,.5, 3, 1, .5, .5, 1, 2, 0, 0, 0, 0, 1,-1, 2}, /* LHX*/
{6,  8, 8, -1, 2, 3, 3, 3,-1,-1, 8, 8,  6,  6, 8,10, 3, 2, 2, 1,-1, 0, 0}  /* INF */
};

double	Sov_OP_Cost[MAXUNITTYPES][MAXACTS] =
{
/*
 c   w  t  r  g   m  s  r  p  m  s  b   a   r  r  r  s  u  m  d  e  d  d
 l   o  o  i  u   i  a  o  o  a  a  r   u   o  u  u  t  p  o  i  v  e  e
 e   o  w  v  n   s  m  c  p  c  n  o   t   a  b  b  r  e  u  s  a  f  f
 a   d  n  e      s     k  u  r  d  k   o   d  1  2  e  l  n  m  s  1  2
 r   s     r      l     e  p  o     e   b            a  e  t  n  m
                  e     t           n   n            m  v     t  n
*/
{3,  4, 3, -1, 3,-1,-1,-1,-1,-1, 4, 4,  1,  2, 2,-1, 2, 2,-1,-1, 3, 6, 5}, /* TANK */
{3,  3, 2, -1, 3, 3, 3, 3,-1,-1, 4, 3,  1,  1, 1,-1, 2, 2, 4, 4, 3, 6, 4}, /* TLAV */
{1,  2, 2, -1, 1, 3, 3, 3,-1,-1, 2, 2, .5,  1, 1,-1, 1, 1, 4, 3, 2, 4, 3}, /* WV */
{3, -1,-1,  2, 1,-1,-1,-1,-1, 1, 4, 4,  2,  3, 3,-1, 0, 0, 1, 1, 4,-1,-1}, /* UHH */
{1,  3, 3,  1, 1, 4, 4, 4,-1,.5, 3, 3,  1,  2, 2, 4, 0, 0, 1, 1, 3,-1,-1}, /* UHM */
{.5, 3, 3,  1, 1, 3, 3, 3, 4,.5, 3, 1,  1,  1, 2, 4, 0, 0, 0, 0, 3,-1,-1}, /* ARH */
{.5, 2, 2, .5, 1, 1, 1, 1, 3,.5, 3, 1, .5, .5, 1, 2, 0, 0, 0, 0, 1,-1, 2}, /* LHX*/
{6,  8, 8, -1, 2, 3, 3, 3,-1,-1, 8, 8,  6,  6, 8,10, 3, 2, 2, 1,-1, 0, 0}  /* INF */
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
{30, 4, 4, 0, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TANK */
{30, 4, 4, 0, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* TLAV */
{30, 4, 4, 0, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* WV */
{35, 8, 8,35, 0,  0, 0, 0, 0, 0, 35,10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHH */
{30, 5, 5,30, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* UHM */
{30, 5, 5,30, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* ARH */
{30, 5, 5,30, 0,  0, 0, 0, 0, 0, 30, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* LHX*/
{10, 3, 3, 3, 0,  0, 0, 0, 0, 0, 10, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}  /* INF */
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
					AirCavUnitData *unitinfo, int column, int row, int elev, int mount, int mounted)
{
	unitinfo->setCountryType(country);
	m_name = name;
	m_side = side;
	m_country = country;
	m_unitInfo = unitinfo;
	m_hexCol = column;
	m_hexRow = row;
	m_OPs = MAX_OPS;
	m_OPsSpentMoving = 0;
	m_isActive = NOT_ACTIVE;
	m_isOppFiring = FALSE;
	m_isPopUp = FALSE;
    m_wpnOppFiring = 0;
    m_tgtOppFiring = 0;
	m_status = ALIVE;
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
	m_macroMove = 0;
	m_actionTaken = false;
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
	m_isPopUp = FALSE;
    m_wpnOppFiring = 0;
    m_tgtOppFiring = -1;
	m_macroMove = 0;
	m_actionTaken = false;
}

void AirCavCounterData::resetActive()
{
	// don't want to reset back to ALIVE - this is meant for reset between turns
	//m_status = ALIVE;	
	m_fired = FALSE;
	m_moved = FALSE;
	m_OPs = MAX_OPS;
	m_OPsSpentMoving = 0;
	m_inDefilade = FALSE;
	m_evasiveManeuver = FALSE;
	m_isOppFiring = FALSE;
	m_isPopUp = FALSE;
    m_wpnOppFiring = 0;
    m_tgtOppFiring = -1;
	m_macroMove = 0;
	m_actionTaken = false;
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
	if ( !oppCost )
		m_actionTaken = true;
	return m_OPs;
}

int AirCavCounterData::moveAction( AirCavMapData *mapData, int col, int row, int from, int popSmoke )
{
	double OPcost;

	// check if within bounds of map
	if ( row < MIN_ROWS || col < MIN_COLUMNS || row > MAX_ROWS || col > MAX_COLUMNS )
		return 0;

	CountryType side = m_unitInfo->getCountryType();
	int unitType = m_unitInfo->getUnitType();
	int nextTerr = mapData->getTerrain( row, col );
	int currRoad = mapData->getRoad( m_hexRow, m_hexCol );
	int nextRoad = mapData->getRoad( row, col );
	int currAutobahn = mapData->getAutobahn( m_hexRow, m_hexCol );
	int nextAutobahn = mapData->getAutobahn( row, col );
	int currHexElevation = mapData->getElevation( m_hexRow, m_hexCol );
	int nextHexElevation = mapData->getElevation( row, col );
	int currElevation = currHexElevation + m_elevOffset;
	int nextElevation = nextHexElevation + m_nextElevOffset;
	int smokeHex = mapData->getSmoke( row, col );

	// determine whether crossing a road hex
	if ( from == NO && mapData->getRoadHex(row, col, 0) )
		nextTerr = ROAD;
	else if ( from == NW && mapData->getRoadHex(row, col, 1) )
		nextTerr = ROAD;
	else if ( from == SW && mapData->getRoadHex(row, col, 2) )
		nextTerr = ROAD;
	else if ( from == SO && mapData->getRoadHex(row, col, 3) )
		nextTerr = ROAD;
	else if ( from == SE && mapData->getRoadHex(row, col, 4) )
		nextTerr = ROAD;
	else if ( from == NE && mapData->getRoadHex(row, col, 5) )
		nextTerr = ROAD;

	// determine whether crossing an autobahn hex
	if ( from == NO && mapData->getAutobahnHex(row, col, 0) )
		nextTerr = AUTOBAHN;
	else if ( from == NW && mapData->getAutobahnHex(row, col, 1) )
		nextTerr = AUTOBAHN;
	else if ( from == SW && mapData->getAutobahnHex(row, col, 2) )
		nextTerr = AUTOBAHN;
	else if ( from == SO && mapData->getAutobahnHex(row, col, 3) )
		nextTerr = AUTOBAHN;
	else if ( from == SE && mapData->getAutobahnHex(row, col, 4) )
		nextTerr = AUTOBAHN;
	else if ( from == NE && mapData->getAutobahnHex(row, col, 5) )
		nextTerr = AUTOBAHN;

	// determine if there is a hexside that will cost extra
	bool stream = false;
	if ( from == NO && mapData->getStreamHex(row, col, 0) )
		stream = true;
	else if ( from == NW && mapData->getStreamHex(row, col, 1) )
		stream = true;
	else if ( from == SW && mapData->getStreamHex(row, col, 2) )
		stream = true;
	else if ( from == SO && mapData->getStreamHex(row, col, 3) )
		stream = true;
	else if ( from == SE && mapData->getStreamHex(row, col, 4) )
		stream = true;
	else if ( from == NE && mapData->getStreamHex(row, col, 5) )
		stream = true;

	// for helicopters at low level, the only cost is the macro cost
	bool isHelo = false;
	bool heloAtLowLevel = false;
	if ( unitType == ARH || unitType == UHM || unitType == UHH || unitType == LHX )
	{
		// this is a helicopter
		isHelo = true;

		// entering Nap of the earth from Low Level
		if ( from == -1 )
		{
			heloAtLowLevel = true;
			m_macroMove = 0;
		}
		// moving at Low Level
		else if ( m_heloOffset > 0 )
		{
			heloAtLowLevel = true;
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
			// helicopters cannnot enter a smoke hex
			if ( smokeHex && isHelo )
				return 0;
			// smoke hex costs extra for all other units
			if ( smokeHex )
				OPcost += 1.0;
		}

		// determine if there are enough operation points
		if ( decrOPs(OPcost) != -1 )
		{
			m_OPsSpentMoving += OPcost;
			if ( m_OPsSpentMoving >= 6 )
				m_moved = 1;
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

	int utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][GUN];
	else
		OPcost = Sov_OP_Cost[utype][GUN];
	if ( oppFire )
		OPcost += 1;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
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

	int utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][ATGM];
	else
		OPcost = Sov_OP_Cost[utype][ATGM];
	if ( oppFire )
		OPcost += 1;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
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

	int utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][ROCKET];
	else
		OPcost = Sov_OP_Cost[utype][ROCKET];
	if ( oppFire )
		OPcost += 1;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
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

	int utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][SAM];
	else
		OPcost = Sov_OP_Cost[utype][SAM];
	if ( oppFire )
		OPcost += 1;
	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost, oppFire) != -1 )
		{
			fire();
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

	int utype = m_unitInfo->getUnitType();
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

int AirCavCounterData::checkContour(AirCavMapData *mapData, int col, int row, int curOffset)
{
	int elevOffset = 0;
	int unitType = m_unitInfo->getUnitType();
	bool heloAtLowLevel = false;
	if ( unitType == ARH || unitType == UHM || unitType == UHH || unitType == LHX )
	{
		if ( m_heloOffset > 0 )
		{
			heloAtLowLevel = true;
		}
	}

	// get this elevation - this will bring up a popup if not known
	int nextElev = mapData->getElevation(row, col);
	int contour = mapData->getContour( row, col );

	// don't check contours for helicopters at low level
	if ( contour > 0 && !heloAtLowLevel )
	{
		ElevOffsetDlg dlg(NULL, m_elevOffset);
		if ( dlg.DoModal() == IDOK )
		{
			elevOffset = dlg.getOffset();
			if ( ( elevOffset + nextElev ) < 0 )
				elevOffset += 10;
		}
	}
	return elevOffset;
}

void AirCavCounterData::setHeloOffset(AirCavMapData *mapData, int elev, bool noCost)
{
	double OPcost;

	if ( m_heloOffset > 0 )
	{
		if ( noCost )
			m_heloOffset = elev;
		// changing from Low Level to Nap of the Earth (cost is terrain)
		else if ( moveAction( mapData, m_hexCol, m_hexRow, -1, false ) )
			m_heloOffset = elev;
	}
	else
	{
		// changing from Nap of the Earth to Low Level (cost is macrohex)
		CountryType side = m_unitInfo->getCountryType();
		int utype = m_unitInfo->getUnitType();
		if ( isNATO( side ) )
			OPcost = US_OP_Cost[utype][MACRO];
		else
			OPcost = Sov_OP_Cost[utype][MACRO];

		if ( OPcost > 0 )
		{
			if ( noCost )
				m_heloOffset = elev;
			else if ( decrOPs(OPcost) != -1 )
			{
				m_heloOffset = elev;
			}
		}
	}
}

void AirCavCounterData::moveTo(AirCavMapData *mapData, int col, int row, bool doCheckContour)
{
	if ( mapData->validHex(col, row) )
	{
		m_hexRow = row;
		m_hexCol = col;
		m_elevOffset = doCheckContour ? checkContour(mapData, col, row) : 0;
	}
}

void AirCavCounterData::moveNorth(AirCavMapData *mapData, int popSmoke)
{
	int nextRow = m_hexRow-1;
	int nextCol = m_hexCol;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow);
		if ( moveAction( mapData, nextCol, nextRow, SO, popSmoke ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveNorthWest(AirCavMapData *mapData, int popSmoke)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow-1 : m_hexRow;
	int nextCol = m_hexCol - 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow);
		if ( moveAction( mapData, nextCol, nextRow, SE, popSmoke ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveNorthEast(AirCavMapData *mapData, int popSmoke)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow-1 : m_hexRow;
	int nextCol = m_hexCol + 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow);
		if ( moveAction( mapData, nextCol, nextRow, SW, popSmoke ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveSouth(AirCavMapData *mapData, int popSmoke)
{
	int nextRow = m_hexRow + 1;
	int nextCol = m_hexCol;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow);
		if ( moveAction( mapData, nextCol, nextRow, NO, popSmoke ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveSouthWest(AirCavMapData *mapData, int popSmoke)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow : m_hexRow + 1;
	int nextCol = m_hexCol - 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow);
		if ( moveAction( mapData, nextCol, nextRow, NE, popSmoke ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

void AirCavCounterData::moveSouthEast(AirCavMapData *mapData, int popSmoke)
{
	int nextRow = EVEN(m_hexCol) ? m_hexRow : m_hexRow + 1;
	int nextCol = m_hexCol + 1;
	if ( mapData->validHex(nextCol, nextRow) )
	{
		m_nextElevOffset = checkContour(mapData, nextCol, nextRow);
		if ( moveAction( mapData, nextCol, nextRow, NW, popSmoke ) )
		{
			m_hexRow = nextRow;
			m_hexCol = nextCol;
		}
	}
}

int AirCavCounterData::isVisible(int terrain, int range, int lowlevel, int weather, int smoke)
{
	int unittype = getUnitInfo()->getUnitType();
	bool lightHelo = false;
	if ( unittype == ARH || unittype == UHM || unittype == LHX )
		lightHelo = true;
	bool heavyHelo = false;
	if ( unittype == UHH )
		heavyHelo = true;

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

	if ( m_fired && range < maxFired )
		return 1;
	else if ( heavyHelo && lowlevel && range < maxLowLevelHeavyHelo )
		return 1;
	else if ( lightHelo && lowlevel && range < maxLowLevelLightHelo )
		return 1;
	else
	{
		int utype = m_unitInfo->getUnitType();
		int maxRange = ClearObservationTable[utype][terrain];
		if ( weather == WEATHER_RAIN || weather == WEATHER_SNOW || weather == WEATHER_LT_FOG )
			maxRange = RainObservationTable[utype][terrain];
		else if ( weather == WEATHER_OVERCAST )
			maxRange = OvercastObservationTable[utype][terrain];
		else if ( weather == WEATHER_HVY_FOG )
			maxRange = HeavyFogObservationTable[utype][terrain];
		
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

void AirCavCounterData::setFinalKillNumbers( int FKNm1, int FKNm2, int FKNm3, int FKNs1, int FKNs2 )
{
	m_FKNm1 = FKNm1;
	m_FKNm2 = FKNm2;
	m_FKNm3 = FKNm3;
	m_FKNs1 = FKNs1;
	m_FKNs2 = FKNs2;
}

int AirCavCounterData::enterDefilade(int toggle)
{
	double OPcost;

	if ( m_inDefilade && toggle )
	{
		m_inDefilade = FALSE;
		return 0;
	}

	int utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][DEFBRK];
	else
		OPcost = Sov_OP_Cost[utype][DEFBRK];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			m_inDefilade = TRUE;
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

int AirCavCounterData::evasiveManeuver(int toggle)
{
	double OPcost;

	if ( m_evasiveManeuver && toggle )
	{
		m_evasiveManeuver = FALSE;
		return 0;
	}

	int utype = m_unitInfo->getUnitType();
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
		return 1;
	}
	else
		return 0;
}

int AirCavCounterData::mountCarrier( void )
{
	double OPcost;

	int utype = m_unitInfo->getUnitType();
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
			return 1;
		}
	}
	return 0;
}

int AirCavCounterData::dismountCarrier( void )
{
	double OPcost;

	int utype = m_unitInfo->getUnitType();
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
			return 1;
		}
	}
	return 0;
}

bool AirCavCounterData::canMount( void )
{
	double OPcost;

	int utype = m_unitInfo->getUnitType();
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

	int utype = m_unitInfo->getUnitType();
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

	int utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][MOUNT];
	else
		OPcost = Sov_OP_Cost[utype][MOUNT];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			return 1;
		}
	}
	return 0;
}

int AirCavCounterData::dismountUnit( void )
{
	double OPcost;

	int utype = m_unitInfo->getUnitType();
	CountryType side = m_unitInfo->getCountryType();
	if ( isNATO( side ) )
		OPcost = US_OP_Cost[utype][DISMOUNT];
	else
		OPcost = Sov_OP_Cost[utype][DISMOUNT];

	if ( OPcost > 0 )
	{
		if ( decrOPs(OPcost) != -1 )
		{
			return 1;
		}
	}
	return 0;
}
