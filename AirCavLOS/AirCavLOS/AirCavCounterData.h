#pragma once

#include "AirCavCommon.h"
#include "AirCavUnitData.h"
#include "AirCavMapData.h"


class AirCavCounterData
{
public:
	AirCavCounterData(void);
	AirCavCounterData(CString name, SideType side, CountryType country, 
					AirCavUnitData *unitinfo, int column, int row, int elev, float op, int mount, int mounted, int defilade);
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
	int			        m_isPopUp;			// 0 = not in popup; 1 = in popup, not yet fired; 2 = in popup, has fired
	int			        m_wpnOppFiring;		// which weapon used for OppFire (1-5)
	int			        m_tgtOppFiring;		// which unit is the target for OppFire
	int					m_status;			// DEAD or ALIVE
	int					m_mode;				// 
	int					m_fired;			// 
	int					m_moved;			// expended OPs in moving
	int					m_rangeToActiveUnit;
	int					m_activeUnitRange;
	int					m_inDefilade;		// TRUE or FALSE
	int					m_isSuppressed;		// TRUE or FALSE
	int					m_evasiveManeuver;	// TRUE or FALSE
	int                 m_elevOffset;       // if not at highest elevation, offset (-10, -20, etc)
	int                 m_nextElevOffset;   // if not at highest elevation, offset (-10, -20, etc)
	int					m_heloOffset;		// 0==nap of the earth, 30==low level
	int					m_numMountedUnits;	// number of units mounted on this one
	int					m_mountedUnits[6];	// units mounted on this one
	int					m_isDismounted;		// this unit has dismounted from its carrying unit
	int					m_carriedByUnit;	// unit carrying this one
	int					m_opticsInUse;		// OPTICS_*
	int					m_laserDesignated;	// ID of unit that is laser designated by this unit
	int					m_laserDesignating;	// ID of unit that is laser designating this unit
	int					m_radarInUse;		// TRUE or FALSE

	int					m_FKNm1;			// Final Kill Number Main Weapon 1
	int					m_FKNm2;			// Final Kill Number Main Weapon 2
	int					m_FKNm3;			// Final Kill Number Main Weapon 3
	int					m_FKNs1;			// Final Kill Number Secondary Weapon 1
	int					m_FKNs2;			// Final Kill Number Secondary Weapon 2

	int					m_ttModm1;			// Target Type modifier Main Weapon 1
	int					m_ttModm2;			// Target Type modifier Main Weapon 2
	int					m_ttModm3;			// Target Type modifier Main Weapon 3
	int					m_ttMods1;			// Target Type modifier Secondary Weapon 1
	int					m_ttMods2;			// Target Type modifier Secondary Weapon 2

	int					m_nAmmoMainWpn1;			// current ammo count for main weapon 1
	int					m_nAmmoMainWpn2;			// current ammo count for main weapon 2
	int					m_nAmmoMainWpn3;			// current ammo count for main weapon 3
	int					m_nAmmoSecondaryWpn1;		// current ammo count for secondary weapon 1
	int					m_nAmmoSecondaryWpn2;		// current ammo count for secondary weapon 2

	int					m_nFullAmmoMainWpn1;		// full ammo count for main weapon 1
	int					m_nFullAmmoMainWpn2;		// full ammo count for main weapon 2
	int					m_nFullAmmoMainWpn3;		// full ammo count for main weapon 3
	int					m_nFullAmmoSecondaryWpn1;	// full ammo count for secondary weapon 1
	int					m_nFullAmmoSecondaryWpn2;	// full ammo count for secondary weapon 2

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
	void actionHeloOffset(AirCavMapData *mapData, AirCavCounterData *counterDataList[MAXCOUNTERS], int elev, bool noCost = false);
	void setHeloOffset(int elev) { m_heloOffset = elev; }
	int getHeloOffset() { return m_heloOffset; }
	int enterDefilade(int terrain, int toggle = TRUE);
	int evasiveManeuver(int toggle = TRUE);
	int enoughOPsForEvasiveManeuver();
	int getDefilade() { return m_inDefilade; }
	int getEvading() { return m_evasiveManeuver; }
	int getIsDismounted() { return m_isDismounted; }
	void setIsDismounted( int u ) { m_isDismounted = u; }
	int getIsCarriedBy() { return m_carriedByUnit; }
	void setIsCarriedBy( int u ) { m_carriedByUnit = u; }
	int getNumberOfMountedUnits() { return m_numMountedUnits; }
	int getMountedUnit( int u ) { return m_mountedUnits[u]; }
	void setMountedUnit( int u, int m ) { m_mountedUnits[u] = m; m_numMountedUnits++; }
	void setFinalKillNumbers( int FKNm1, int FKNm2, int FKNm3, int FKNs1, int FKNs2, int ttm1, int ttm2, int ttm3, int tts1, int tts2 );
	void setWpnOppFiring( int wpn ) { m_wpnOppFiring = wpn; } 
	void setTgtOppFiring( int tgt ) { m_tgtOppFiring = tgt; } 
	void setDefilade(int def) { m_inDefilade = def; }
	void setEvading(int evade) { m_evasiveManeuver = evade; }
	bool getActionTaken() { return m_actionTaken; }
	void setActionTaken(bool act) { m_actionTaken = act; }
	int getIsSuppressed() { return m_isSuppressed; }
	void setIsSuppressed(int sup) { m_isSuppressed = sup; }
	int getOpticsInUse() { return m_opticsInUse; }
	void setOpticsInUse(int opt) { m_opticsInUse = opt; }
	int getLaserDesignatedUnit() { return m_laserDesignated; }
	void setLaserDesignatedUnit(int unit) { m_laserDesignated = unit; }
	int getLaserDesignatingUnit() { return m_laserDesignating; }
	void setLaserDesignatingUnit(int unit) { m_laserDesignating = unit; }
	int getRadarInUse() { return m_radarInUse; }
	void setRadarInUse(int radar) { m_radarInUse = radar; }

	double getOPs() { return m_OPs; }
	void setOPs(double op) { m_OPs = op; }
	double incrOPs(double op);
	double decrOPs(double op, bool oppAction=false);

	int moveAction( AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int col, int row, int from, int popSmoke, int timeOfDay, int weather, bool goingToNOE = false);
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
	void setFired(int fired) { m_fired = fired; }
	int getMoved() { return m_moved; }
	void setMoved(int moved) { m_moved = moved; }
	int getIsAlive() { return m_status; }
	void setIsAlive(int status) { m_status = status; }
	bool getIsOppFiring() { return m_isOppFiring; }
	void setIsOppFiring(bool opp) { m_isOppFiring = opp; }
	int getIsPopUp() { return m_isPopUp; }
	void setIsPopUp(int p);
	int getWpnOppFiring() { return m_wpnOppFiring; }
	int getTgtOppFiring() { return m_tgtOppFiring; }

	int getFKN1() { return m_FKNm1; }
	int getFKN2() { return m_FKNm2; }
	int getFKN3() { return m_FKNm3; }
	int getFKN4() { return m_FKNs1; }
	int getFKN5() { return m_FKNs2; }

	int getTTModm1() { return m_ttModm1; }
	int getTTModm2() { return m_ttModm2; }
	int getTTModm3() { return m_ttModm3; }
	int getTTMods1() { return m_ttMods1; }
	int getTTMods2() { return m_ttMods2; }

	void setFKN1(int FKN) { m_FKNm1 = FKN; }
	void setFKN2(int FKN) { m_FKNm2 = FKN; }
	void setFKN3(int FKN) { m_FKNm3 = FKN; }
	void setFKN4(int FKN) { m_FKNs1 = FKN; }
	void setFKN5(int FKN) { m_FKNs2 = FKN; }

	void setAmmoMainWeapon1(int n) { m_nAmmoMainWpn1 = n; }
	void setAmmoMainWeapon2(int n) { m_nAmmoMainWpn2 = n; }
	void setAmmoMainWeapon3(int n) { m_nAmmoMainWpn3 = n; }
	void setAmmoSecondaryWeapon1(int n) { m_nAmmoSecondaryWpn1 = n; }
	void setAmmoSecondaryWeapon2(int n) { m_nAmmoSecondaryWpn2 = n; }

	int getAmmoMainWeapon1() { return m_nAmmoMainWpn1; }
	int getAmmoMainWeapon2() { return m_nAmmoMainWpn2; }
	int getAmmoMainWeapon3() { return m_nAmmoMainWpn3; }
	int getAmmoSecondaryWeapon1() { return m_nAmmoSecondaryWpn1; }
	int getAmmoSecondaryWeapon2() { return m_nAmmoSecondaryWpn2; }

	void moveTo(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int col, int row, bool doCheckContour = true);
	void moveNorth( AirCavMapData *mapData, AirCavCounterData *counterDataList[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather );
	void moveNorthWest( AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather );
	void moveNorthEast( AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather );
	void moveSouth( AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather );
	void moveSouthWest( AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather );
	void moveSouthEast( AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int popSmoke, int timeOfDay, int weather );

	int checkContour(AirCavMapData *mapData, int col, int row, int from, int curOffset = 0);
	bool isVisible(int terrain, int range, int weather, int timeofday, int smoke = FALSE);
	bool isVisibleAdvanced(int terrain, int range, int weather, int timeofday, SideType side, int optics, bool radar, int smoke = FALSE);

	bool hexStackingFull(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int row, int col, bool checkLowLevel = false, bool isGoingtoNOE = false);
	CString AirCavCounterData::getStackingString(AirCavMapData *mapData, AirCavCounterData *counterData[MAXCOUNTERS], int row, int col);

	bool isNATO( CountryType side )
	{
		return ( side == COUNTRY_US || side == COUNTRY_GERMANY || side == COUNTRY_BRITAIN || side == COUNTRY_ISRAEL );
	}
};
