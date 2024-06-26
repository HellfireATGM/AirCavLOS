#ifndef AIRCAVCOMMON_H
#define AIRCAVCOMMON_H

#include <afxstr.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

#define MAX_OPS 12
#define MAXCOUNTERS 100
#define MAXTYPS   	100
#define MAXWEAP 	100

#define VEHICLE_STACKING_LIMIT 6
#define INFANTRY_STACKING_LIMIT 20
#define HELO_STACKING_LIMIT 2
#define LIGHT_HELO_STACKING 2
#define HEAVY_HELO_STACKING 3

#define MAX_BUF_SIZE 8192

// unit types
#define MAXUNITTYPES 8
enum UnitType
{
	TANK = 0,	// main battle tank
	TLAV,		// tracked light armored vehicle
	WV,			// wheeled vehicle
	UHH,		// utility helicoper - heavy
	UHM,		// utility helicoper - medium
	ARH,		// attack-recon helicopter
	LHX,		// light helicopter experimental
	INF			// poor, bloody infantry
};

// target types
#define MAXTARGETTYPES 6
enum TargetType
{
	TT_A = 0,	// very heavy armor (advanced tank)
	TT_B,		// heavy armor (standard tank)
	TT_C,		// heavy medium armor (armored vehicle)
	TT_D,		// light medium armor (light armored vehicle)
	TT_E,		// light armor (armored vehicle)
	TT_F		// very light armor (helicopter)
};

// side types
enum SideType
{
	SIDE_BLUE = 0,	// blue
	SIDE_RED		// red
};

// country types
enum CountryType
{
	COUNTRY_US = 0,		// United States
	COUNTRY_SOVIET,		// Soviet Union
	COUNTRY_GERMANY,	// West Germany
	COUNTRY_BRITAIN,	// Great Britain
	COUNTRY_SYRIA,		// Syria
	COUNTRY_ISRAEL		// Isreal
};

enum RoadType
{
	RD = 0,		// road
	AB,			// autobahn
	BOTH		// road & autobahn
};

#define CLEAR		0	/* clear */
#define WOODS		1	/* woods */
#define TOWN		2	/* town */
#define RIVER		3	/* river */
#define MAXTERRAIN	4
#define UNKNOWN		-1

#define GUN			4	/* gun, cannon, grenade launcher, rifle */
#define ATGM		5	/* ATGM */
#define SAM			6	/* SAM */
#define ROCKET		7	/* Rocket */
#define POPUP		8	/* pop-up */
#define MACRO		9	/* macrohex */
#define SAND		10	/* sand */
#define BROKEN		11	/* broken/wadi */
#define AUTOBAHN	12	/* autobahn */
#define ROAD		13	/* road */
#define RUBBLE1		14	/* rubble 1 */
#define RUBBLE2		15	/* rubble 2 */
#define STREAM		16	/* stream hexside */
#define UPELEV		17	/* up elevation */
#define MOUNT		18	/* mount */
#define DISMOUNT	19	/* dismount */
#define EVM			20	/* evasive manuever */
#define DEFCLR		21	/* enter defilade (clear) */
#define DEFBRK		22	/* enter defilade (broken) */
#define MAXACTS 	23

#define US81MM_MORTAR		0
#define US42INCH_MORTAR		1
#define US160MM_MORTAR		2
#define US105MM_HOWITZ		3
#define US155MM_HOWITZ		4
#define US203MM_ROCKET		5
#define SOVIET120MM_MORTAR	6
#define SOVIET122MM_HOWITZ	7
#define SOVIET152MM_HOWITZ	8
#define SOVIET122MM_ROCKET	9
#define MAXINDIRECT			10

/* definitions for LOS */
#define MIN_ROWS    01
#define MIN_COLUMNS 10
#define MAX_ROWS    53
#define MAX_COLUMNS 48
#define NROW    	(MAX_ROWS+1)
#define NCOL		(MAX_COLUMNS+1)
#define SCALE       100        /* meters per hex */

#define Deg2Rad  0.017453292

#define DIRECTION_UNDEF 0                   /* undefined */
#define DIRECTION_NO    1                   /* north */
#define DIRECTION_NE    2                   /* northeast */
#define DIRECTION_SE    3                   /* southeast */
#define DIRECTION_SO    4                   /* south */
#define DIRECTION_SW    5                   /* southwest */
#define DIRECTION_NW    6                   /* northwest */

#define NOT_ACTIVE 	0
#define ACTIVE		1
#define WAS_ACTIVE	2

#define FALSE		0
#define TRUE		1

#define DEAD 		0
#define ALIVE		1

#define NOT_MOVING	0
#define MOVING		1

#define MAIN1		0
#define MAIN2		1
#define MAIN3		2
#define SECONDARY1	3
#define SECONDARY2	4

#define NOT_IN_POPUP 0
#define POPUP_NOT_FIRED 1
#define POPUP_HAS_FIRED 2

#define LOS_UNBLOCKED 						0
#define LOS_BLOCKED_BY_TERRAIN				1
#define LOS_BLOCKED_BY_TERRAIN_INTERSECT	2
#define LOS_BLOCKED_BY_SMOKE				3

#define WEATHER_CLEAR		0
#define WEATHER_LT_FOG		1
#define WEATHER_HVY_FOG		2
#define WEATHER_OVERCAST	3
#define WEATHER_RAIN		4
#define WEATHER_SNOW		5

#define TIME_DAY		0
#define TIME_NIGHT		1

#define NAP_OF_EARTH_METERS			0	// nap of earth is 0m above terrain
#define LOW_LEVEL_METERS			30	// low level is 30m above terrain
#define ARTILLERY_SMOKE_DISPERSAL	30	// 30% chance
#define VEHICLE_SMOKE_DISPERSAL		40	// 40% chance

#define OBS_VEHICLE			0
#define OBS_INFANTRY		1
#define OBS_LIGHT_HELO		2
#define OBS_HEAVY_HELO		3
#define OBS_FIRING			4
#define OBS_MAX				5

#define TER_CLEAR		0	/* clear */
#define TER_BROKEN		1	/* broken */
#define TER_DEFILADE	2	/* defilade */
#define TER_WOODS		3	/* woods */
#define TER_TOWN		4	/* town */
#define TER_LIGHT_LOW	5	/* light or medium helo at low-level */
#define TER_HEAVY_LOW	6	/* heavy helo at low-level */
#define TER_MAX			7
#define TER_MAX_2		TER_MAX * 2	// includes day and night values for observation

#define OPTICS_OPTICAL_SIGHT				0
#define OPTICS_THERMAL_IMAGER				1
#define OPTICS_AMBIENT_LIGHT_ENHANCER		2
#define OPTICS_INFRARED_SEARCHLIGHT			3
#define OPTICS_WHITELIGHT_SEARCHLIGHT		4

#define EVEN(a) ((a) % 2 ? 0 : 1)

/*  unit definitions  */
typedef struct
{
    char            name[10];
    int             x;						// row
    int             y;						// column
    int             type;
    int             nation;
    int             OPs;
    int             active;
    int             status;
    int             mode;
    int             moved;
    int             range_to_active_unit;
    int             active_unit_range;
    int             in_defilade;
	int             height_offset;
	int				num_mounted_units;
	int             mounted_units[6];		// units mounted on this one
	int				carried_by_unit;		// unit mounting this one
} Unit;

/*  vehicle definitions  */
typedef struct
{
	char			name[32];
	int				main1;			// main weapon 1
	int				main2;			// main weapon 2
	int				main3;			// main weapon 3
	int				secn1;			// secondary weapon 1
	int				secn2;			// secondary weapon 2
    UnitType		type;			// unit type
	TargetType		armor;			// target type
	int				evm;			// evasive manuever modifier
	int				sm;				// suppression modifier
	int				dm;				// defilade modifier
    int             ammo_m1;		// ammo - main weapon 1
	int             ammo_m2;		// ammo - main weapon 2
	int             ammo_m3;		// ammo - main weapon 3
	int             ammo_s1;		// ammo - secondary weapon 1
	int             ammo_s2;		// ammo - secondary weapon 2
	int				ti;				// thermal imager
	int				ale;			// ambient light enhancer
	int				irsl;			// infrared searchlight
	int				wlsl;			// white light searchlight
	int				ld;				// laser designator
	int				radar;			// radar
} Type;

/*  weapon definitions  */
typedef struct
{
	char 	name[32];
	int     type;  		/* 4=GUN, 5=ATGM, 6=SAM, 7=RKT */
	int     min_range;
	int     max_range;
	int     range_1,range_2,range_3,range_4,range_5,range_6,range_7;
	int     target_A,target_B,target_C,target_D,target_E,target_F;
	int     wood_mod, town_mod, opp_fire_mod, moving_mod, aerial_mod;
} Weapon;

typedef struct {
	char	name[32];			// unit name
	int		type;				// 0=Mortar, 1=Artillery, 2=Rocket, number of spread hexes = 0, 1, 2 (for SMOKE ONLY)
	int		scatter_normal;
	int		scatter_dedicated;
	int		min_range;
	int		max_range;
	int		tt_A;				// NOTES (see Tables 15.21 and 15.22): 
	int		tt_B;				// 10 = "S"  "Use -5 as modifier; a successfull roll suppressed instead of kills"
	int		tt_C;
	int		tt_D;
	int		tt_E;
	int		tt_F;
	int		Wds;				// 10 = "+"  "+1 vs infantry, -1 vs all others"
	int		Tn;
	int		AdjH;				// units in adjacent hexes are attacked, apply this modifier (10 means No Effect)
	int		auto_suppress;		// automatic suppression
} Artillery;
	
/*  map definitions  */
typedef struct {
	double		x;              /* row */
	double		y;              /* column */
	int			elevation;      /* -1 means hex is uninitialized */
	int			terrain;        /* 0=clear, 1=woods, 2=town, 3=river */
	int			contour;        /* 0 = no contour lines, 1 = has contour lines */
	int			road;		    /* quick and dirty check for road */
	int			autobahn;		/* quick and dirty check for autobahn */
	int			river;			/* quick and dirty check for river */
	int			rnw, rn, rne;	/* northern hexsides for road */
	int			rsw, rs, rse;	/* southern hexsides for road */
	int			anw, an, ane;	/* northern hexsides for autobahn */
	int			asw, as, ase;	/* southern hexsides for autobahn */
	int			vnw, vn, vne;	/* northern hexsides for river */
	int			vsw, vs, vse;	/* southern hexsides for river */
	int			unit;		    /* id of unit */
	int			smoke;			/* 0 = no smoke, 1 = smoke */
	int			wreck;			/* 0 = no wreck, 1 = wreck */
} MAP;

/* scenario set up information  */
typedef struct {
	CString	name;		// unit name
	int		side;		// which side (0=BLUE, 1=RED)
	int		country;	// which country (0=US, 1=USSR)
	int		type;		// which type (0=T_72, etc)
	int		column;		// location on map
	int		row;		// location on map
	int		offset;		// elevation offset
	float	op;			// OPs remaining
	int		mount;		// unit mounting this one
	int		mounted;	// starts mounted
	int		defilade;	// starts in defilade
} UnitDef;

typedef struct {
	char		scenarioname[80];		// name of the scenario
	int			numunits;				// number of units
	UnitDef		unit[MAXCOUNTERS];		// the units in the scenario
} Scenario;

typedef struct {
	int		unit;					// the unit that is moving
	int		OPs;					// number of OPs spent moving
	int		row;					// start row
	int		column;					// start column
	bool	activeUnitChanged;		// set when active unit has changed
} UnitTracking;

class RowCol {
public:
	RowCol() { rowcol.clear(); }
	~RowCol() {}

	void Add(int r, int c)
	{
		if ( Exists(r, c) == rowcol.end() )
		{
			int rc = ( c * 100 ) + r;
			rowcol.push_back( rc );
		}
	}
	void Remove(int r, int c)
	{
		vector<int>::iterator foundIter = Exists(r, c);
		if ( foundIter != rowcol.end() )
		{
			rowcol.erase( foundIter );
		}
	}
	void Get(int n, int *r, int *c)
	{
		*r = rowcol[n] - ( (rowcol[n] / 100) * 100 );
		*c = rowcol[n] / 100;
	}
	vector<int>::iterator Exists(int r, int c)
	{
		vector<int>::iterator Iter;
		for (Iter = rowcol.begin(); Iter != rowcol.end(); Iter++ )
		{
			int rc = ( c * 100 ) + r;
			if ( (*Iter) == rc )
				return Iter;
		}
		return rowcol.end();
	}
	int GetNum() { return (int)rowcol.size(); }

private:
	vector<int> rowcol;
};

// messaging
static const char *set_active = "select";
static const char *set_info = "info";
static const char *update_map = "map";
static const char *set_time = "time";
static const char *set_weather = "weather";
static const char *switch_sides = "switch";
static const char *opp_fire = "oppfire";

#define MSG_UPDATE_SET_ACTIVE 1
#define MSG_UPDATE_UNIT_INFO 2
#define MSG_UPDATE_MAP_INFO 3
#define MSG_CHANGE_TIMEOFDAY 4
#define MSG_CHANGE_WEATHER 5
#define MSG_SWITCH_SIDES 6
#define MSG_OPP_FIRE 7

#endif