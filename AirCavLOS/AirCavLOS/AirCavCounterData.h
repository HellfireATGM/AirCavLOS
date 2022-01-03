#pragma once

#include "AirCavCommon.h"
#include "AirCavUnitData.h"
#include "AirCavMapData.h"


class AirCavCounterData
{
public:
	AirCavCounterData(void);
	AirCavCounterData(CString name, SideType side, CountryType country, 
					AirCavUnitData *unitinfo, int column, int row, int elev, float op, int mount, int mounted);
public:
	~AirCavCounterData(void);

// data
private:
	CString				m_name;				// name of counter
	SideType			m_side;				// which side its on
	CountryType			m_country;			// which country its fighting for
	AirCavUnitData		*m_unitInfo;		// info about this counter
	int					m_hexCol;			// column location on map
	int					m_hexRow;			// row location on map
	double				m_OPs;				// operation points (1-12)
	double				m_OPsSpentMoving;	// operation points spent moving
	int			        m_isActive;			// NOT_ACTIVE, ACTIVE or WAS_ACTIVE
	bool		        m_isOppFiring;		// true or false
	bool		        m_isPopUp;			// true or false
	int			        m_wpnOppFiring;		// which weapon used for OppFire (1-5)
	int			        m_tgtOppFiring;		// which unit is the target for OppFire
	int					m_status;			// DEAD or ALIVE
	int					m_mode;				// 
	int					m_fired;			// 
	int					m_moved;			// expended OPs in moving
	int					m_rangeToActiveUnit;
	int					m_activeUnitRange;
	int					m_inDefilade;		// TRUE or FALSE
	int					m_evasiveManeuver;	// TRUE or FALSE
	int                 m_elevOffset;       // if not at highest elevation, offset (-10, -20, etc)
	int                 m_nextElevOffset;   // if not at highest elevation, offset (-10, -20, etc)
	int					m_heloOffset;		// 0==nap of the earth, 30==low level
	int					m_numMountedUnits;	// number of units mounted on this one
	int					m_mountedUnits[6];	// units mounted on this one
	int					m_isDismounted;		// this unit has dismounted from its carrying unit
	int					m_carriedByUnit;	// unit carrying this one
	int					m_FKNm1;			// Final Kill Number Main Weapon 1
	int					m_FKNm2;			// Final Kill Number Main Weapon 2
	int					m_FKNm3;			// Final Kill Number Main Weapon 3
	int					m_FKNs1;			// Final Kill Number Secondary Weapon 1
	int					m_FKNs2;			// Final Kill Number Secondary Weapon 2
	int					m_macroMove;		// tally of hexes moved when low level
	bool				m_actionTaken;		// unit has been activated (opportunity fire does not count)

// methods
public:
	CString getName() { return m_name; }
	SideType getSideType() { return m_side; }
	CountryType getCountryType() { return m_country; }
	AirCavUnitData *getUnitInfo() { return m_unitInfo; }

	void setHexCol(int col) { m_hexCol = col; }
	void setHexRow(int row) { m_hexRow = row; }
	int getHexCol() { return m_hexCol; }
	int getHexRow() { return m_hexRow; }
	void setElevOffset(int elev) { m_elevOffset = elev; }
	int getElevOffset() { return m_elevOffset; }
	void setHeloOffset(AirCavMapData *mapData, int elev, bool noCost = false);
	int getHeloOffset() { return m_heloOffset; }
	int enterDefilade(int terrain, int toggle = TRUE);
	int evasiveManeuver(int toggle = TRUE);
	int getDefilade() { return m_inDefilade; }
	int getEvading() { return m_evasiveManeuver; }
	int getIsDismounted() { return m_isDismounted; }
	void setIsDismounted( int u ) { m_isDismounted = u; }
	int getIsCarriedBy() { return m_carriedByUnit; }
	void setIsCarriedBy( int u ) { m_carriedByUnit = u; }
	int getNumberOfMountedUnits() { return m_numMountedUnits; }
	int getMountedUnit( int u ) { return m_mountedUnits[u]; }
	void setMountedUnit( int u, int m ) { m_mountedUnits[u] = m; m_numMountedUnits++; }
	void setFinalKillNumbers( int FKNm1, int FKNm2, int FKNm3, int FKNs1, int FKNs2 );
	void setWpnOppFiring( int wpn ) { m_wpnOppFiring = wpn; } 
	void setTgtOppFiring( int tgt ) { m_tgtOppFiring = tgt; } 
	void setDefilade(int def) { m_inDefilade = def; }
	void setEvading(int evade) { m_evasiveManeuver = evade; }
	bool getActionTaken() { return m_actionTaken; }

	double getOPs() { return m_OPs; }
	void setOPs(double op) { m_OPs = op; }
	double incrOPs(double op);
	double decrOPs(double op, bool oppAction=false);

	int moveAction( AirCavMapData *mapData, int col, int row, int from, int popSmoke );
	int fireGun( bool oppFire = FALSE );
	int fireMissile( bool oppFire = FALSE );
	int fireRocket( bool oppFire = FALSE );
	int fireSAM( bool oppFire = FALSE );
	int popupFire(bool oppFire = FALSE);
	int oppFire( int weaponType );
	int laySmoke( AirCavMapData *mapData, int col, int row );

	int mountCarrier( void );
	int dismountCarrier( void );
	bool canMount( void );
	bool canDismount( void );
	int mountUnit( void );
	int dismountUnit( void );

	void reset();
	void resetActive();
	void kill() { m_status = DEAD; setOPs(0); }
	void fire() { m_fired = TRUE; }
	void nofire() { m_fired = FALSE; }
	void move() { m_moved = TRUE; }
	void nomove() { m_moved = FALSE; }
	int getFired() { return m_fired; }
	int getMoved() { return m_moved; }
	int getIsAlive() { return m_status; }
	bool getIsOppFiring() { return m_isOppFiring; }
	void setIsOppFiring(bool opp) { m_isOppFiring = opp; }
	bool getIsPopUp() { return m_isPopUp; }
	void setIsPopUp(bool p) { if (p) {fire();nomove();} else {nofire(); move();} m_isPopUp = p; }
	int getWpnOppFiring() { return m_wpnOppFiring; }
	int getTgtOppFiring() { return m_tgtOppFiring; }
	int getFKN1() { return m_FKNm1; }
	int getFKN2() { return m_FKNm2; }
	int getFKN3() { return m_FKNm3; }
	int getFKN4() { return m_FKNs1; }
	int getFKN5() { return m_FKNs2; }
	void setFKN1(int FKN) { m_FKNm1 = FKN; }
	void setFKN2(int FKN) { m_FKNm2 = FKN; }
	void setFKN3(int FKN) { m_FKNm3 = FKN; }
	void setFKN4(int FKN) { m_FKNs1 = FKN; }
	void setFKN5(int FKN) { m_FKNs2 = FKN; }

	void moveTo(AirCavMapData *mapData, int col, int row, bool doCheckContour = true);
	void moveNorth( AirCavMapData *mapData, int popSmoke );
	void moveNorthWest( AirCavMapData *mapData, int popSmoke );
	void moveNorthEast( AirCavMapData *mapData, int popSmoke );
	void moveSouth( AirCavMapData *mapData, int popSmoke );
	void moveSouthWest( AirCavMapData *mapData, int popSmoke );
	void moveSouthEast( AirCavMapData *mapData, int popSmoke );

	int checkContour(AirCavMapData *mapData, int col, int row, int curOffset = 0);
	int isVisible(int terrain, int range, int lowlevel, int weather, int smoke = FALSE);

	bool isNATO( CountryType side )
	{
		return ( side == COUNTRY_US || side == COUNTRY_GERMANY || side == COUNTRY_BRITAIN );
	}

};
