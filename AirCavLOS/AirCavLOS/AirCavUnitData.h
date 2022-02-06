#pragma once

#include "AirCavCommon.h"
#include "AirCavWeaponData.h"

class AirCavCounterData;

class AirCavUnitData
{
public:
	AirCavUnitData(void);
	AirCavUnitData(Type unit, AirCavWeaponData *mwpn1, AirCavWeaponData *mwpn2, AirCavWeaponData *mwpn3,
					AirCavWeaponData *swpn1, AirCavWeaponData *swpn2);
	AirCavUnitData(CString name, UnitType type, TargetType TT, 
					AirCavWeaponData *mwpn1, AirCavWeaponData *mwpn2, AirCavWeaponData *mwpn3,
					AirCavWeaponData *swpn1, AirCavWeaponData *swpn2, int evm, int sm, int dm, 
							int ammo_m1, int ammo_m2, int ammo_m3, int ammo_s1, int ammo_s2,
							int ti, int ale, int irsl, int wlsl, int ld, int radar);
public:
	~AirCavUnitData(void);

// data
private:
	CString				m_name;				// name of unit
	UnitType			m_type;				// type
	TargetType			m_TT;				// target type
	CountryType			m_country;			// which country
	int					m_EVM;				// evasive maneuver modifier
	int					m_SM;				// suppression modifier
	int					m_DM;				// defilade modifier
	AirCavWeaponData	*m_mainWpn1;		// main weapon 1
	AirCavWeaponData	*m_mainWpn2;		// main weapon 2
	AirCavWeaponData	*m_mainWpn3;		// main weapon 3
	AirCavWeaponData	*m_secondaryWpn1;	// secondary weapon 1
	AirCavWeaponData	*m_secondaryWpn2;	// secondary weapon 2
	int					m_nAmmoMainWpn1;		// ammo count for main weapon 1
	int					m_nAmmoMainWpn2;		// ammo count for main weapon 2
	int					m_nAmmoMainWpn3;		// ammo count for main weapon 3
	int					m_nAmmoSecondaryWpn1;	// ammo count for secondary weapon 1
	int					m_nAmmoSecondaryWpn2;	// ammo count for secondary weapon 2
	int					m_ti;					// thermal imager
	int					m_ale;					// ambient light enhancer
	int					m_irsl;					// infrared searchlight
	int					m_wlsl;					// white light searchlight
	int					m_ld;					// laser designator
	int					m_radar;				// radar

// methods
public:
	CString getName() { return m_name; }
	UnitType getUnitType() { return m_type; }
	void setUnitType(UnitType unitType) { m_type = unitType; }
	TargetType getTargetType() { return m_TT; }
	void setTargetType(TargetType targetType) { m_TT = targetType; }
	CountryType getCountryType() { return m_country; }
	void setCountryType(CountryType countryType) { m_country = countryType; }

	AirCavWeaponData *getMainWeapon1() { return m_mainWpn1; }
	AirCavWeaponData *getMainWeapon2() { return m_mainWpn2; }
	AirCavWeaponData *getMainWeapon3() { return m_mainWpn3; }
	AirCavWeaponData *getSecondaryWeapon1() { return m_secondaryWpn1; }
	AirCavWeaponData *getSecondaryWeapon2() { return m_secondaryWpn2; }

	int getEVM() { return m_EVM; }
	int getSM() { return m_SM; }
	int getDM() { return m_DM; }

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

	CString getUnitTypeString( UnitType unitType );
	CString getTargetTypeString( TargetType targetType );
	CString getCountryTypeString( CountryType countryType );
	CString getSideTypeString(SideType sideType);

	bool isHelicopter();
	bool isHeavyHelicopter();
	bool isLightHelicopter();
	bool isAttackHelicopter();
	bool isVehicle();
	bool isTrackedVehicle();
	bool isWheeledVehicle();
	bool isInfantry();

	bool hasTIsight() { return m_ti == 1; }
	bool hasALEsight() { return m_ale == 1; }
	bool hasIRSLsight() { return m_irsl == 1; }
	bool hasWLSLsight() { return m_wlsl == 1; }

	bool hasLaserDesignator() { return m_ld == 1; }
	bool hasRadar() { return m_radar == 1; }

	bool isWeaponNotIRGuided(int which);
	bool isWeaponLaserGuided(int which);

	int CalculateFKN( int which, AirCavCounterData *tgt, int terr, int smoke, 
							int range, int opp, int sup, int skylining, char *log, int &ttMod);
};
