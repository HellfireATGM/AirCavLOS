#pragma once

#include "AirCavCommon.h"


class rangeTable 
{
public:
	rangeTable(void){}
	rangeTable(int b1, int b2, int b3, int b4, int b5, int b6, int b7)
	{
		band_1 = b1;
		band_2 = b2;
		band_3 = b3;
		band_4 = b4;
		band_5 = b5;
		band_6 = b6;
		band_7 = b7;
	}

	~rangeTable(void){}

	int GetRangeData(int band);
	void SetRangeData(int band, int rangedata);

private:
	int band_1;			// 0-4 hexes
	int band_2;			// 5-9 hexes
	int band_3;			// 10-14 hexes
	int band_4;			// 15-19 hexes
	int band_5;			// 20-24 hexes
	int band_6;			// 25-29 hexes
	int band_7;			// 30+ hexes
};

class targetTable 
{
public:
	targetTable(void){}
	targetTable(int a, int b, int c, int d, int e, int f)
	{
		type_A = a;
		type_B = b;
		type_C = c;
		type_D = d;
		type_E = e;
		type_F = f;
	}

	~targetTable(void){}

	int GetTargetData(int type);
	void SetTargetData(int type, int targetdata);

private:
	int type_A;			// target type A
	int type_B;			// target type B
	int type_C;			// target type C
	int type_D;			// target type D
	int type_E;			// target type E
	int type_F;			// target type F
};


class AirCavWeaponData
{
public:
	AirCavWeaponData(void);
	AirCavWeaponData(Weapon weapon);
	AirCavWeaponData(CString name, int type, int rmin, int rmax, rangeTable rt, targetTable tt, 
						int w_mod, int t_mod, int o_mod, int m_mod, int a_mod);
public:
	~AirCavWeaponData(void);

	int getType() { return m_type; }
	int getMinRange() { return m_rmin; }
	int getMaxRange() { return m_rmax; }
	int getWoodsMod() { return m_woods_mod; }
	int getTownMod() { return m_town_mod; }
	int getOppFireMod() { return m_oppfire_mod; }
	int getMoveMod() { return m_move_mod; }
	int getAerialMod() { return m_aerial_mod; }
	CString getName();
	rangeTable getRangeTable() { return m_rt; }
	targetTable getTargetTable() { return m_tt; }

private:
	CString m_name;			// name of weapon
	int m_type;				// type = GUN or MSLE or ROCKET
	int m_rmin;				// minimum range
	int m_rmax;				// maximum range
	int m_woods_mod;		// woods/smoke modifier
	int m_town_mod;			// town modifier
	int m_oppfire_mod;		// opportunity fire modifier
	int m_move_mod;			// moving modifier
	int m_aerial_mod;		// aerial target modifier
	rangeTable m_rt;		// table of to hit numbers at each range
	targetTable m_tt;		// table of to hit modifiers for each target type
};

