#include "StdAfx.h"
#include "AirCavWeaponData.h"


AirCavWeaponData::AirCavWeaponData(void)
{
}

AirCavWeaponData::AirCavWeaponData(Weapon weapon)
{
	m_name = weapon.name;
	m_type = weapon.type;
	m_rmin = weapon.min_range;
	m_rmax = weapon.max_range;
	m_rt.SetRangeData(1, weapon.range_1);
	m_rt.SetRangeData(2, weapon.range_2);
	m_rt.SetRangeData(3, weapon.range_3);
	m_rt.SetRangeData(4, weapon.range_4);
	m_rt.SetRangeData(5, weapon.range_5);
	m_rt.SetRangeData(6, weapon.range_6);
	m_rt.SetRangeData(7, weapon.range_7);
	m_tt.SetTargetData(1, weapon.target_A);
	m_tt.SetTargetData(2, weapon.target_B);
	m_tt.SetTargetData(3, weapon.target_C);
	m_tt.SetTargetData(4, weapon.target_D);
	m_tt.SetTargetData(5, weapon.target_E);
	m_tt.SetTargetData(6, weapon.target_F);
	m_woods_mod = weapon.wood_mod;
	m_town_mod = weapon.town_mod;
	m_oppfire_mod = weapon.opp_fire_mod;
	m_move_mod = weapon.moving_mod;
	m_aerial_mod = weapon.aerial_mod;
}

AirCavWeaponData::AirCavWeaponData(CString name, int type, int rmin, int rmax, rangeTable rt, targetTable tt, 
					int w_mod, int t_mod, int o_mod, int m_mod, int a_mod)
{
	m_name = name;
	m_type = type;
	m_rmin = rmin;
	m_rmax = rmax;
	m_rt = rt;
	m_tt = tt;
	m_woods_mod = w_mod;
	m_town_mod = t_mod;
	m_oppfire_mod = o_mod;
	m_move_mod = m_mod;
	m_aerial_mod = a_mod;
}

AirCavWeaponData::~AirCavWeaponData(void)
{
}

CString AirCavWeaponData::getName()
{
	return m_name;
}


int rangeTable::GetRangeData(int band)
{
	switch( band )
	{
		case 1:
			return band_1;
			break;
		case 2:
			return band_2;
			break;
		case 3:
			return band_3;
			break;
		case 4:
			return band_4;
			break;
		case 5:
			return band_5;
			break;
		case 6:
			return band_6;
			break;
		case 7:
			return band_7;
			break;
		default:
			return 0;
	}
}
void rangeTable::SetRangeData(int band, int rangedata)
{
	switch( band )
	{
		case 1:
			band_1 = rangedata;
			break;
		case 2:
			band_2 = rangedata;
			break;
		case 3:
			band_3 = rangedata;
			break;
		case 4:
			band_4 = rangedata;
			break;
		case 5:
			band_5 = rangedata;
			break;
		case 6:
			band_6 = rangedata;
			break;
		case 7:
			band_7 = rangedata;
			break;
		default:
			return;
	}
}

int targetTable::GetTargetData(int type)
{
	switch( type )
	{
		case 1:
			return type_A;
			break;
		case 2:
			return type_B;
			break;
		case 3:
			return type_C;
			break;
		case 4:
			return type_D;
			break;
		case 5:
			return type_E;
			break;
		case 6:
			return type_F;
			break;
		default:
			return 0;
	}
}

void targetTable::SetTargetData(int band, int targetdata)
{
	switch( band )
	{
		case 1:
			type_A = targetdata;
			break;
		case 2:
			type_B = targetdata;
			break;
		case 3:
			type_C = targetdata;
			break;
		case 4:
			type_D = targetdata;
			break;
		case 5:
			type_E = targetdata;
			break;
		case 6:
			type_F = targetdata;
			break;
		default:
			return;
	}
}
