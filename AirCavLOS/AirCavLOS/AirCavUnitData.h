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
					AirCavWeaponData *swpn1, AirCavWeaponData *swpn2, int evm, int sm, int dm, int nr, int nm);
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
	int					m_nRockets;			// number of Rockets carried
	int					m_nMissiles;		// number of Missiles carried
	AirCavWeaponData	*m_mainWpn1;		// main weapon 1
	AirCavWeaponData	*m_mainWpn2;		// main weapon 2
	AirCavWeaponData	*m_mainWpn3;		// main weapon 3
	AirCavWeaponData	*m_secondaryWpn1;	// secondary weapon 1
	AirCavWeaponData	*m_secondaryWpn2;	// secondary weapon 2

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
	void setNumberRockets( int nr ) { m_nRockets = nr; }
	void setNumberMissiles( int nm ) { m_nMissiles = nm; }
	int getNumberRockets() { return m_nRockets; }
	int getNumberMissiles() { return m_nMissiles; }

	CString getUnitTypeString( UnitType unitType );
	CString getTargetTypeString( TargetType targetType );
	CString getCountryTypeString( CountryType countryType );

	int CalculateFKN( int which, AirCavCounterData *tgt, int terr, int smoke, 
							int range, int opp, char *log );
};
