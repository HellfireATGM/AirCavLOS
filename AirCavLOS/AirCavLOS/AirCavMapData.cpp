#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <io.h>
#include "AirCavMapData.h"
#include "HexTerrain.h"

#pragma warning(disable:4996)

#define EPSILON 0.00001f
#define DBLEQUAL(a1, a2) (fabs(a1 - a2) < EPSILON)
#define EPSILONTAN 1.0f
#define DBLEQUALTAN(a1, a2) (fabs(a1 - a2) < EPSILONTAN)

char *TerrainString[4] =
{
	"Clear", "Woods", "Town", "River"
};



static int ar_empty(char *s)
{
	while (*s)
		if (!isspace(*s++))
			return (0);

	return (1);
}

extern void CalcAdj (int dir, int cur_y, int cur_x, int *y, int *x)
{
	switch (dir)
	{
		case DIRECTION_NO:
			*x = cur_x;
			*y = cur_y - 1;
			break;
		case DIRECTION_NE:
			if (EVEN(cur_x))
				*y = cur_y - 1;
			else
				*y = cur_y;
			*x = cur_x + 1;
			break;
		case DIRECTION_SE:
			if (EVEN(cur_x))
				*y = cur_y;
			else
				*y = cur_y + 1;
			*x = cur_x + 1;
			break;
		case DIRECTION_SO:
			*x = cur_x;
			*y = cur_y + 1;
			break;
		case DIRECTION_SW:
			if (EVEN(cur_x))
				*y = cur_y;
			else
				*y = cur_y + 1;
			*x = cur_x - 1;
			break;
		case DIRECTION_NW:
			if (EVEN(cur_x))
				*y = cur_y - 1;
			else
				*y = cur_y;
			*x = cur_x - 1;
			break;
	}
	return;
}



AirCavMapData::AirCavMapData(void)
{
	map_file_pointer = 0;
	m_ignoreWaterFeatures = false;
	m_ignoreAutobahn = false;
	m_mapDataEdited = false;
}

AirCavMapData::~AirCavMapData(void)
{
}

int AirCavMapData::OpenMapDataFile(std::string& file_dir)
{
	int i, j; 
	double horizontal_distance, a, b;

	/* open data file */
	map = file_dir + "map.dat";
	dem = file_dir + "dem.txt";

	errno_t err = fopen_s(&map_file_pointer, map.c_str(), "rb");
	if (err != 0 || map_file_pointer == NULL) 
	{
		return 0;
	}

	a = (SCALE/2.0) * 0.577350269;       /* length of 1/2 hexside */
	b = a*a + (SCALE/2.0)*(SCALE/2.0);   /* square of distance to farthest edge */
	horizontal_distance = a + sqrt(b);   /* distance between columns */

	/* read old data file and setup x and y positions */
	for (i=0; i < NROW; i++)
	{
		for (j=0; j < NCOL; j++)
		{
			Map[j][i].x = j * horizontal_distance;
			if (EVEN(j))
				Map[j][i].y = i * SCALE;
			else
				Map[j][i].y = i * SCALE + (SCALE/2.0);

			if (map_file_pointer)
			{
				fread (&Map[j][i].terrain,   sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].elevation, sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].contour,   sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].road,      sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].autobahn,  sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].river,     sizeof(int), 1, map_file_pointer);

				fread (&Map[j][i].rnw,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].rn,         sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].rne,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].rsw,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].rs,         sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].rse,        sizeof(int), 1, map_file_pointer);

				fread (&Map[j][i].anw,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].an,         sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].ane,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].asw,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].as,         sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].ase,        sizeof(int), 1, map_file_pointer);

				fread (&Map[j][i].vnw,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].vn,         sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].vne,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].vsw,        sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].vs,         sizeof(int), 1, map_file_pointer);
				fread (&Map[j][i].vse,        sizeof(int), 1, map_file_pointer);

				fread (&Map[j][i].unit,      sizeof(int), 1, map_file_pointer);

				if ( Map[j][i].elevation == 0xcdcdcdcd )
				{
					Map[j][i].terrain = 0;
					Map[j][i].elevation = -1;
					Map[j][i].contour = 0;
					Map[j][i].road = 0;
					Map[j][i].autobahn = 0;
					Map[j][i].river = 0;
					Map[j][i].smoke = 0;
					Map[j][i].rnw = 0;
					Map[j][i].rn  = 0;
					Map[j][i].rne = 0;
					Map[j][i].rsw = 0;
					Map[j][i].rs  = 0;
					Map[j][i].rse = 0;
					Map[j][i].anw = 0;
					Map[j][i].an  = 0;
					Map[j][i].ane = 0;
					Map[j][i].asw = 0;
					Map[j][i].as  = 0;
					Map[j][i].ase = 0;
					Map[j][i].vnw = 0;
					Map[j][i].vn  = 0;
					Map[j][i].vne = 0;
					Map[j][i].vsw = 0;
					Map[j][i].vs  = 0;
					Map[j][i].vse = 0;
					Map[j][i].unit = 0;
				}
				Map[j][i].smoke = 0;	// smoke is never persisted, always set to false
				Map[j][i].wreck = 0;	// wreck is never persisted, always set to false
			}
			else
			{
				Map[j][i].terrain = 0;
				Map[j][i].elevation = -1;
				Map[j][i].contour = 0;
				Map[j][i].road = 0;
				Map[j][i].autobahn = 0;
				Map[j][i].river = 0;
				Map[j][i].smoke = 0;
				Map[j][i].wreck = 0;
				Map[j][i].rnw = 0;
				Map[j][i].rn  = 0;
				Map[j][i].rne = 0;
				Map[j][i].rsw = 0;
				Map[j][i].rs  = 0;
				Map[j][i].rse = 0;
				Map[j][i].anw = 0;
				Map[j][i].an  = 0;
				Map[j][i].ane = 0;
				Map[j][i].asw = 0;
				Map[j][i].as  = 0;
				Map[j][i].ase = 0;
				Map[j][i].vnw = 0;
				Map[j][i].vn  = 0;
				Map[j][i].vne = 0;
				Map[j][i].vsw = 0;
				Map[j][i].vs  = 0;
				Map[j][i].vse = 0;
				Map[j][i].unit = 0;
			}
		}
	}
    return 1;
}

int AirCavMapData::CloseMapDataFile( )
{
	if ( map_file_pointer )
		fclose( map_file_pointer );
	map_file_pointer = NULL;
	return 1;
}

int AirCavMapData::SaveAndCloseMapDataFile( char *msgbox )
{
	/* reopen the old file, write new data and close data file */
	errno_t err;
	if (map_file_pointer)
		err = freopen_s (&map_file_pointer, map.c_str(), "wb", map_file_pointer);
	else
		err = fopen_s (&map_file_pointer, map.c_str(), "wb");

	if (err != 0 || map_file_pointer == NULL)
		return 0;

	/* count the number of unknown hexes */
	int numDefinedHexes = 0;
	int numValidHexes = 0;
	for (int r = 0; r < NROW; r++)
	{
		for (int c = 0; c < NCOL; c++)
		{
			fwrite (&Map[c][r].terrain,   sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].elevation, sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].contour,   sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].road,      sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].autobahn,  sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].river,     sizeof(int), 1, map_file_pointer);

			fwrite (&Map[c][r].rnw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].rn,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].rne,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].rsw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].rs,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].rse,        sizeof(int), 1, map_file_pointer);

			fwrite (&Map[c][r].anw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].an,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].ane,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].asw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].as,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].ase,        sizeof(int), 1, map_file_pointer);

			fwrite (&Map[c][r].vnw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].vn,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].vne,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].vsw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].vs,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[c][r].vse,        sizeof(int), 1, map_file_pointer);

			fwrite (&Map[c][r].unit,       sizeof(int), 1, map_file_pointer);

			int maxRow;
			if (EVEN(r))
				maxRow = MAX_ROWS;
			else
				maxRow = MAX_ROWS - 1;
			bool rowValid = (r >= MIN_ROWS && r <= maxRow);
			bool colValid = (c >= MIN_COLUMNS && c <= MAX_COLUMNS);

			if ( rowValid && colValid )
			{
				if (Map[c][r].elevation >= 0)
					numDefinedHexes++;
				numValidHexes++;
			}
		}
	}

	int f = fclose (map_file_pointer);
	if (f != 0)
		return 0;

	// write out the DEM
	FILE *dem_file_pointer;
	err = fopen_s (&dem_file_pointer, dem.c_str(), "w");

	if (err != 0 || dem_file_pointer == NULL) 
		return 0;

	double demScale = SCALE;
	double a = (demScale/2.0) * 0.577350269;			/* length of 1/2 hexside */
	double b = a*a + (demScale/2.0)*(demScale/2.0);		/* square of distance to farthest edge */
	double horizontal_distance = a + sqrt(b);			/* distance between columns */

	for (int r=MAX_ROWS; r >= MIN_ROWS; r--)
	{
		for (int c=MIN_COLUMNS; c <= MAX_COLUMNS ; c++)
		{
			double e, x, y;

			x = c * horizontal_distance;
			if (EVEN(c))
				y = (MAX_ROWS-r) * demScale;
			else
				y = (MAX_ROWS-r) * demScale + (demScale/2.0);

			if (Map[c][r].elevation < 0)
				e = (double)Map[c][r].elevation;
			else if (Map[c][r].terrain == RIVER)
				e = (double)Map[c][r].elevation - 10.0f;
			else if (Map[c][r].terrain == TOWN)
				e = (double)Map[c][r].elevation + 20.0f;
			else
			{
				e = (double)Map[c][r].elevation * 2.0f;
				if (Map[c][r].contour && e > 0.0f)
					e -= 10.0f;
			}

			fprintf (dem_file_pointer, "%f, %f, %f\n", x, y, e);
		}
	}
	fclose(dem_file_pointer);

	sprintf( msgbox, "Number of hexes known: %d out of %d (%3.1f percent)\n", numDefinedHexes, numValidHexes, (double)numDefinedHexes /(double)(numValidHexes)*(double)100.0 );

    return 1;
}


int AirCavMapData::CalculateLOS( int org_x, int org_y, int org_elev, bool usingTI, int tgt_x, int tgt_y, int tgt_elev, bool &skylined, bool laser, char *logString )
{
	char logBuffer[256];
	int Fdir, Ldir, Rdir, Rx, Ry, Fx, Fy, Lx, Ly, cur_x, cur_y;
	int clear_LOS, blk1, blk2, range, targetRange = 0;
	double LOS_vec_x, LOS_vec_y, angle;
	double front_x, front_y, left_x, left_y, right_x, right_y;
	double Ftan, Ltan, Rtan, Fangle, Langle, Rangle, SmallTan;
	double Flen, Llen, Rlen, LOS_len;
	double Fvec_x, Fvec_y, Lvec_x, Lvec_y, Rvec_x, Rvec_y;
	double Fvec, Lvec, Rvec;
	bool checkingSkylining = false;
	bool hitSmokeHex = false;
	bool hitBlockingHex = false;

	sprintf( logBuffer, "Calculating LOS from %02d%02d [elev: %d] to %02d%02d [elev: %d]\n", org_y, org_x, org_elev, tgt_y, tgt_x, tgt_elev);
	strcpy( logString, logBuffer );

	/* setup initial values */
	clear_LOS = 1;
	range = 0;
	cur_x = org_x;
	cur_y = org_y;

	/* calculate line of sight vector */
	LOS_vec_x = Map[tgt_y][tgt_x].x - Map[org_y][org_x].x;
	LOS_vec_y = Map[tgt_y][tgt_x].y - Map[org_y][org_x].y;

	/* determine which direction the vector is facing */
	if (LOS_vec_y > 0 && LOS_vec_x > 0)
		{ Ldir = DIRECTION_NE;   Fdir = DIRECTION_SE;   Rdir = DIRECTION_SO; }
	else if (LOS_vec_y > 0 && LOS_vec_x < 0)
		{ Ldir = DIRECTION_SO;   Fdir = DIRECTION_SW;   Rdir = DIRECTION_NW; }
	else if (LOS_vec_y > 0 && LOS_vec_x == 0)
		{ Ldir = DIRECTION_SE;   Fdir = DIRECTION_SO;   Rdir = DIRECTION_SW; }
	else if (LOS_vec_y <= 0 && LOS_vec_x > 0)
		{ Ldir = DIRECTION_NO;   Fdir = DIRECTION_NE;   Rdir = DIRECTION_SE; }
	else if (LOS_vec_y <= 0 && LOS_vec_x < 0)
		{ Ldir = DIRECTION_SW;   Fdir = DIRECTION_NW;   Rdir = DIRECTION_NO; }
	else if (LOS_vec_y < 0 && LOS_vec_x == 0)
		{ Ldir = DIRECTION_NW;   Fdir = DIRECTION_NO;   Rdir = DIRECTION_NE; }
	else
	   return(0);		// same hex, just return 0

	getElevation( org_x, org_y );
	getElevation( tgt_x, tgt_y );
	sprintf( logBuffer, "Origin Hex %02d%02d   ", org_y, org_x );
	strcat( logString, logBuffer );
	sprintf( logBuffer, "Terrain: %s  Elevation: %d [%d]\n",TerrainString[Map[org_y][org_x].terrain],Map[org_y][org_x].elevation, org_elev );
	strcat( logString, logBuffer );
	sprintf( logBuffer, "Target Hex %02d%02d   ", tgt_y, tgt_x );
	strcat( logString, logBuffer );
	sprintf( logBuffer, "Terrain: %s  Elevation: %d [%d]\n",TerrainString[Map[tgt_y][tgt_x].terrain],Map[tgt_y][tgt_x].elevation, tgt_elev );
	strcat( logString, logBuffer );

top:
	/* left */
	CalcAdj (Ldir, cur_x, cur_y, &Lx, &Ly);
	left_x = Map[tgt_y][tgt_x].x - Map[Ly][Lx].x;
	left_y = Map[tgt_y][tgt_x].y - Map[Ly][Lx].y;
	Lvec_x = Map[org_y][org_x].x - Map[Ly][Lx].x;
	Lvec_y = Map[org_y][org_x].y - Map[Ly][Lx].y;
	Llen = sqrt(left_x*left_x + left_y*left_y);
	Lvec = sqrt(Lvec_x*Lvec_x + Lvec_y*Lvec_y);

	/* front */
	CalcAdj (Fdir, cur_x, cur_y, &Fx, &Fy);
	front_x = Map[tgt_y][tgt_x].x - Map[Fy][Fx].x;
	front_y = Map[tgt_y][tgt_x].y - Map[Fy][Fx].y;
	Fvec_x = Map[org_y][org_x].x - Map[Fy][Fx].x;
	Fvec_y = Map[org_y][org_x].y - Map[Fy][Fx].y;
	Flen = sqrt(front_x*front_x + front_y*front_y);
	Fvec = sqrt(Fvec_x*Fvec_x + Fvec_y*Fvec_y);

	/* right */
	CalcAdj (Rdir, cur_x, cur_y, &Rx, &Ry);
	right_x = Map[tgt_y][tgt_x].x - Map[Ry][Rx].x;
	right_y = Map[tgt_y][tgt_x].y - Map[Ry][Rx].y;
	Rvec_x = Map[org_y][org_x].x - Map[Ry][Rx].x;
	Rvec_y = Map[org_y][org_x].y - Map[Ry][Rx].y;
	Rlen = sqrt(right_x*right_x + right_y*right_y);
	Rvec = sqrt(Rvec_x*Rvec_x + Rvec_y*Rvec_y);

	/* calculate angles */
	LOS_len = sqrt(LOS_vec_x*LOS_vec_x + LOS_vec_y*LOS_vec_y);
	angle = (Llen*Llen - LOS_len*LOS_len - Lvec*Lvec)/(-2*LOS_len*Lvec);
	if ( DBLEQUAL(angle, -1.0 ) )
		angle = -1.0;
	if ( DBLEQUAL(angle, 1.0 ) )
		angle = 1.0;
	if ( angle < -1.0 || angle > 1.0 )
	{
		sprintf( logBuffer, "Angle is out of range! (%f)...\n", angle );
		strcat( logString, logBuffer );
		return 0;
	}
	Langle = acos(angle);
	Ltan = Lvec*sin(Langle);
	SmallTan = Ltan;

	angle = (Flen*Flen - LOS_len*LOS_len - Fvec*Fvec)/(-2*LOS_len*Fvec);
	if ( DBLEQUAL(angle, -1.0 ) )
		angle = -1.0;
	if ( DBLEQUAL(angle, 1.0 ) )
		angle = 1.0;
	if ( angle < -1.0 || angle > 1.0 )
	{
		sprintf( logBuffer, "Angle is out of range! (%f)...\n", angle );
		strcat( logString, logBuffer );
		return 0;
	}
	Fangle = acos(angle);
	Ftan = Fvec*sin(Fangle);
	if (Ftan < SmallTan) SmallTan = Ftan;

	angle = (Rlen*Rlen - LOS_len*LOS_len - Rvec*Rvec)/(-2*LOS_len*Rvec);
	if ( DBLEQUAL(angle, -1.0 ) )
		angle = -1.0;
	if ( DBLEQUAL(angle, 1.0 ) )
		angle = 1.0;
	if ( angle < -1.0 || angle > 1.0 )
	{
		sprintf( logBuffer, "Angle is out of range! (%f)...\n", angle );
		strcat( logString, logBuffer );
		return 0;
	}
	Rangle = acos(angle);
	Rtan = Lvec*sin(Rangle);
	if (Rtan < SmallTan) SmallTan = Rtan;

	/* find direction with smallest tangent */
	int hgt;
	if ( (DBLEQUALTAN(SmallTan, Ftan)) && (DBLEQUALTAN(SmallTan, Rtan)) )
	{
		if (!validHex(Fy, Fx))
		{
			blk1 = LOS_BLOCKED_BY_TERRAIN;
			sprintf(logBuffer, "Hex: %02d%02d result: Invalid  ", Fy, Fx);
			strcat(logString, logBuffer);
		}
		else
		{
			blk1 = Check_Block(org_x, org_y, org_elev, Fx, Fy, tgt_x, tgt_y, tgt_elev, hgt);
			sprintf(logBuffer, "Hex 1: %02d%02d result: %s  ", Fy, Fx, blk1 ? "Blocked" : "Clear");
			strcat(logString, logBuffer);
			sprintf(logBuffer, "Terrain: %s  Elevation: %d\n", TerrainString[Map[Fy][Fx].terrain], hgt);
			strcat(logString, logBuffer);
		}
		if (!validHex(Ry, Rx))
		{
			blk1 = LOS_BLOCKED_BY_TERRAIN;
			sprintf(logBuffer, "Hex: %02d%02d result: Invalid  ", Ry, Rx);
			strcat(logString, logBuffer);
		}
		else
		{
			blk2 = Check_Block(org_x, org_y, org_elev, Rx, Ry, tgt_x, tgt_y, tgt_elev, hgt);
			sprintf(logBuffer, "Hex 2: %02d%02d result: %s  ", Ry, Rx, blk2 ? "Blocked" : "Clear");
			strcat(logString, logBuffer);
			sprintf(logBuffer, "Terrain: %s  Elevation: %d\n", TerrainString[Map[Ry][Rx].terrain], hgt);
			strcat(logString, logBuffer);
		}
		if (blk1 && blk2)
		{
			bool smoke = (blk1 == LOS_BLOCKED_BY_SMOKE || blk2 == LOS_BLOCKED_BY_SMOKE);
			if (smoke && usingTI && !laser && hitSmokeHex == false)
			{
				// if using thermal imaging, ignore the first smoke hex (only)
				hitSmokeHex = true;
			}
			else
			{
				if (laser && hitBlockingHex == false)
					// if using laser designator, ignore the first blocking hex (only)
					hitBlockingHex = true;
				else
				{
					clear_LOS = 0;
					blk_x = Fx;
					blk_y = Fy;
				}
			}
		}
		cur_x = Fx;
		cur_y = Fy;
	}
	else if ( (DBLEQUALTAN(SmallTan, Ftan)) && (DBLEQUALTAN(SmallTan, Ltan)) )
	{
		if (!validHex(Fy, Fx))
		{
			blk1 = LOS_BLOCKED_BY_TERRAIN;
			sprintf(logBuffer, "Hex: %02d%02d result: Invalid  ", Fy, Fx);
			strcat(logString, logBuffer);
		}
		else
		{
			blk1 = Check_Block(org_x, org_y, org_elev, Fx, Fy, tgt_x, tgt_y, tgt_elev, hgt);
			sprintf(logBuffer, "Hex 1: %02d%02d result: %s  ", Fy, Fx, blk1 ? "Blocked" : "Clear");
			strcat(logString, logBuffer);
			sprintf(logBuffer, "Terrain: %s  Elevation: %d\n", TerrainString[Map[Fy][Fx].terrain], hgt);
			strcat(logString, logBuffer);
		}
		if (!validHex(Ly, Lx))
		{
			blk1 = LOS_BLOCKED_BY_TERRAIN;
			sprintf(logBuffer, "Hex: %02d%02d result: Invalid  ", Ly, Lx);
			strcat(logString, logBuffer);
		}
		else
		{
			blk2 = Check_Block(org_x, org_y, org_elev, Lx, Ly, tgt_x, tgt_y, tgt_elev, hgt);
			sprintf(logBuffer, "Hex 2: %02d%02d result: %s  ", Ly, Lx, blk2 ? "Blocked" : "Clear");
			strcat(logString, logBuffer);
			sprintf(logBuffer, "Terrain: %s  Elevation: %d\n", TerrainString[Map[Ly][Lx].terrain], hgt);
			strcat(logString, logBuffer);
		}
		if (blk1 && blk2)
		{
			bool smoke = (blk1 == LOS_BLOCKED_BY_SMOKE || blk2 == LOS_BLOCKED_BY_SMOKE);
			if (smoke && usingTI && !laser && hitSmokeHex == false)
			{
				// if using thermal imaging, ignore the first smoke hex (only)
				hitSmokeHex = true;
			}
			else
			{
				if (laser && hitBlockingHex == false)
					// if using laser designator, ignore the first blocking hex (only)
					hitBlockingHex = true;
				else
				{
					clear_LOS = 0;
					blk_x = Fx;
					blk_y = Fy;
				}
			}
		}
		cur_x = Fx;
		cur_y = Fy;
	}
	else if ( DBLEQUAL(SmallTan, Ltan) )
	{
		if (!validHex(Ly, Lx))
		{
			blk1 = LOS_BLOCKED_BY_TERRAIN;
			sprintf(logBuffer, "Hex: %02d%02d result: Invalid  ", Ly, Lx);
			strcat(logString, logBuffer);
		}
		else
		{
			blk1 = Check_Block(org_x, org_y, org_elev, Lx, Ly, tgt_x, tgt_y, tgt_elev, hgt);
			sprintf(logBuffer, "Hex: %02d%02d result: %s  ", Ly, Lx, blk1 ? "Blocked" : "Clear");
			strcat(logString, logBuffer);
			sprintf(logBuffer, "Terrain: %s  Elevation: %d\n", TerrainString[Map[Ly][Lx].terrain], hgt);
			strcat(logString, logBuffer);
		}
		if (blk1)
		{
			bool smoke = (blk1 == LOS_BLOCKED_BY_SMOKE);
			if (smoke && usingTI && !laser && hitSmokeHex == false)
			{
				// if using thermal imaging, ignore the first smoke hex (only)
				hitSmokeHex = true;
			}
			else
			{
				if (laser && hitBlockingHex == false)
					// if using laser designator, ignore the first blocking hex (only)
					hitBlockingHex = true;
				else
				{
					clear_LOS = 0;
					blk_x = Lx;
					blk_y = Ly;
				}
			}
		}
		cur_x = Lx;
		cur_y = Ly;
	}
	else if ( DBLEQUAL(SmallTan, Ftan) )
	{
		if (!validHex(Fy, Fx))
		{
			blk1 = LOS_BLOCKED_BY_TERRAIN;
			sprintf(logBuffer, "Hex: %02d%02d result: Invalid  ", Fy, Fx);
			strcat(logString, logBuffer);
		}
		else
		{
			blk1 = Check_Block(org_x, org_y, org_elev, Fx, Fy, tgt_x, tgt_y, tgt_elev, hgt);
			sprintf(logBuffer, "Hex: %02d%02d result: %s  ", Fy, Fx, blk1 ? "Blocked" : "Clear");
			strcat(logString, logBuffer);
			sprintf(logBuffer, "Terrain: %s  Elevation: %d\n", TerrainString[Map[Fy][Fx].terrain], hgt);
			strcat(logString, logBuffer);
		}
		if (blk1)
		{
			bool smoke = (blk1 == LOS_BLOCKED_BY_SMOKE);
			if (smoke && usingTI && !laser && hitSmokeHex == false)
			{
				// if using thermal imaging, ignore the first smoke hex (only)
				hitSmokeHex = true;
			}
			else
			{
				if (laser && hitBlockingHex == false)
					// if using laser designator, ignore the first blocking hex (only)
					hitBlockingHex = true;
				else
				{
					clear_LOS = 0;
					blk_x = Fx;
					blk_y = Fy;
				}
			}
		}
		cur_x = Fx;
		cur_y = Fy;
	}
	else if ( DBLEQUAL(SmallTan, Rtan) )
	{
		if (!validHex(Ry, Rx))
		{
			blk1 = LOS_BLOCKED_BY_TERRAIN;
			sprintf(logBuffer, "Hex: %02d%02d result: Invalid  ", Ry, Rx);
			strcat(logString, logBuffer);
		}
		else
		{
			blk1 = Check_Block(org_x, org_y, org_elev, Rx, Ry, tgt_x, tgt_y, tgt_elev, hgt);
			sprintf(logBuffer, "Hex: %02d%02d result: %s  ", Ry, Rx, blk1 ? "Blocked" : "Clear");
			strcat(logString, logBuffer);
			sprintf(logBuffer, "Terrain: %s  Elevation: %d\n", TerrainString[Map[Ry][Rx].terrain], hgt);
			strcat(logString, logBuffer);
		}
		if (blk1)
		{
			bool smoke = (blk1 == LOS_BLOCKED_BY_SMOKE);
			if (smoke && usingTI && !laser && hitSmokeHex == false)
			{
				// if using thermal imaging, ignore the first smoke hex (only)
				hitSmokeHex = true;
			}
			else
			{
				if (laser && hitBlockingHex == false)
					// if using laser designator, ignore the first blocking hex (only)
					hitBlockingHex = true;
				else
				{
					clear_LOS = 0;
					blk_x = Rx;
					blk_y = Ry;
				}
			}
		}
		cur_x = Rx;
		cur_y = Ry;
	}
	else
	{
		sprintf( logBuffer, "ERROR: could not find next hex...\n" );
		strcat( logString, logBuffer );
	}

	range += 1;
	if (!clear_LOS)
	{
		if ( !validHex(cur_y, cur_x) && checkingSkylining )
		{
			// hit a border hex while checking skylining, skylining check is done
			skylined = true;
			sprintf(logBuffer, "Skylining check unblocked, range = %d\n", range);
			strcat(logString, logBuffer);
			return(targetRange);
		}
		else if (checkingSkylining)
		{
			sprintf(logBuffer, "Skylining Blocked\n");
			strcat(logString, logBuffer);
			return (targetRange);
		}
		else if (cur_x == tgt_x && cur_y == tgt_y && !checkingSkylining)
		{
			// reached the target hex, the LOS is clear
			clear_LOS = 1;
			sprintf(logBuffer, "LOS Clear, range = %d\n", range);
			strcat(logString, logBuffer);

			// only check skylining if target elevation is equal to or greater than origin elevation
			if (tgt_elev >= org_elev)
			{
				sprintf(logBuffer, "Check Skylining (Target Elev >= Origin Elev)\n");
				strcat(logString, logBuffer);
				checkingSkylining = true;
				targetRange = range;
				goto top;
			}
			else
			{
				return (range);
			}
		}
		else
		{
			range = 0;
			sprintf(logBuffer, "LOS Blocked\n");
			strcat(logString, logBuffer);
			return (range);
		}
	}
	else
	{
		if ( borderHex(cur_y, cur_x) && checkingSkylining )
		{
			// reached the bounds of the map without hitting a blocked hex, skylining check is done
			skylined = true;
			sprintf(logBuffer, "Skylining check done, range = %d\n", range);
			strcat(logString, logBuffer);
			return(targetRange);
		}
		else if ( cur_x == tgt_x && cur_y == tgt_y && !checkingSkylining )
		{
			// reached the target hex, the LOS is clear
			sprintf(logBuffer, "LOS Clear, range = %d\n", range);
			strcat(logString, logBuffer);

			// only check skylining if target elevation is equal to or greater than origin elevation
			if (tgt_elev >= org_elev)
			{
				sprintf(logBuffer, "Check Skylining (Target Elev >= Origin Elev)\n");
				strcat(logString, logBuffer);
				checkingSkylining = true;
				targetRange = range;
				goto top;
			}
			else
			{
				return (range);
			}
		}
		else
		{
			// continue the traversal
			goto top;
		}
	}
	return 0;
}

int AirCavMapData::Check_Block (int org_x, int org_y, int org_elev, int x, int y, int tgt_x, int tgt_y, int tgt_elev, int &int_hgt)
{
	int blk = LOS_UNBLOCKED;
	int_hgt = CalculateAverageHeight(x, y);
	int org_hgt = org_elev;
	int tgt_hgt = tgt_elev;

	if (Map[y][x].terrain == WOODS || Map[y][x].terrain == TOWN)
	{
		int_hgt += 10;
	}

	if (int_hgt > org_hgt && int_hgt > tgt_hgt)
	{
		blk = LOS_BLOCKED_BY_TERRAIN;
	}
	else if (Map[y][x].smoke == 1)
	{
		blk = LOS_BLOCKED_BY_SMOKE;
	}
	else if (org_hgt > tgt_hgt)
	{
		double dist2tgt, dist2org;
		if (int_hgt >= org_hgt)
		{
			blk = LOS_BLOCKED_BY_TERRAIN;
		}
		else if (int_hgt < org_hgt && int_hgt > tgt_hgt)
		{
			double d2o = (x-org_x)*(x-org_x) + (y-org_y)*(y-org_y);
			dist2org = sqrt(d2o);
			double d2t = (x-tgt_x)*(x-tgt_x) + (y-tgt_y)*(y-tgt_y);
			dist2tgt = sqrt(d2t);
			if (dist2tgt < dist2org)
			{
				blk = LOS_BLOCKED_BY_TERRAIN;
			}
		}
	}
	else if (org_hgt < tgt_hgt)
	{
		double dist2tgt, dist2org;
		if (int_hgt >= tgt_hgt)
		{
			blk = LOS_BLOCKED_BY_TERRAIN;
		}
		else if (int_hgt > org_hgt && int_hgt < tgt_hgt)
		{
			double d2o = (x-org_x)*(x-org_x) + (y-org_y)*(y-org_y);
			dist2org = sqrt(d2o);
			double d2t = (x-tgt_x)*(x-tgt_x) + (y-tgt_y)*(y-tgt_y);
			dist2tgt = sqrt(d2t);
			if (dist2org < dist2tgt)
			{
				blk = LOS_BLOCKED_BY_TERRAIN;
			}
		}
	}
	return (blk);
}

int AirCavMapData::CalculateAverageHeight(int x, int y)
{ 
	// average height is calculated in order to handle the case where you are sighting along hexes with
	// contour lines, where if you draw a line from one hex to the other, it is clear. 
	//
	// For example, 2932 (10m) to 2435 (10m). The intermediate hexes are 20m, so will block LOS, but if you draw 
	// a straight line from one to the other, no contour lines are intersected, so the LOS should be clear.
	// or the illustration on page 5 of the rulebook, between E and D. 2823 is 30m so it blocks LOS according to
	// this algorithm, but it really shouldn't because the LOS runs along the contour line and doesn't intersect it.
	// 
	// A human can easily see this, but because we only have a single value for the contour, we cannot tell where
	// the contour actually is, so we cannot algorithmically handle this case. This workaround handles the latter
	// case, but not the former.
	//
	int contour = getContour(x, y);
	int hgt = getElevation(x, y);
	// a hex without a contour has an absolute elevation, just return this
	if (contour == 0) return hgt;
	// otherwise, calculate the average height from the entire megahex
	if (contour && hgt > 0) hgt -= 5;

	int NOx, NOy;
	CalcAdj(DIRECTION_NO, x, y, &NOx, &NOy);
	int NOhgt = validHex(NOy, NOx) ? getElevation(NOx, NOy) : hgt;
	if (getContour(NOx, NOy) && NOhgt > 0) NOhgt -= 5;
	int NWx, NWy;
	CalcAdj(DIRECTION_NW, x, y, &NWx, &NWy);
	int NWhgt = validHex(NWy, NWx) ? getElevation(NWx, NWy) : hgt;
	if (getContour(NWx, NWy) && NWhgt > 0) NWhgt -= 5;
	int NEx, NEy;
	CalcAdj(DIRECTION_NE, x, y, &NEx, &NEy);
	int NEhgt = validHex(NEy, NEx) ? getElevation(NEx, NEy) : hgt;
	if (getContour(NEx, NEy) && NEhgt > 0) NEhgt -= 5;

	int SOx, SOy;
	CalcAdj(DIRECTION_SO, x, y, &SOx, &SOy);
	int SOhgt = validHex(SOy, SOx) ? getElevation(SOx, SOy) : hgt;
	if (getContour(SOx, SOy) && SOhgt > 0) SOhgt -= 5;
	int SWx, SWy;
	CalcAdj(DIRECTION_SW, x, y, &SWx, &SWy);
	int SWhgt = validHex(SWy, SWx) ? getElevation(SWx, SWy) : hgt;
	if (getContour(SWx, SWy) && SWhgt > 0) SWhgt -= 5;
	int SEx, SEy;
	CalcAdj(DIRECTION_SE, x, y, &SEx, &SEy);
	int SEhgt = validHex(SEy, SEx) ? getElevation(SEx, SEy) : hgt;
	if (getContour(SEx, SEy) && SEhgt > 0) SEhgt -= 5;

	int avgHgt = (hgt + NOhgt + NWhgt + NEhgt + SOhgt + SWhgt + SEhgt) / 7;
	return avgHgt;
}

int AirCavMapData::getTerrain( int x, int y )
{
	if (m_ignoreWaterFeatures && Map[y][x].terrain == RIVER)
		return CLEAR;

	return Map[y][x].terrain;
}

int AirCavMapData::getElevation( int x, int y )
{
	// check if elevation is known, if not ask for it
	if ( validHex(y, x) && Map[y][x].elevation < 0 )
	{
		HexTerrain dlg(x, y);
		if ( dlg.DoModal() == IDOK )
		{
			Map[y][x].terrain = dlg.getTerrain();
			Map[y][x].elevation = dlg.getElevation();
			Map[y][x].contour = dlg.getContour();
			Map[y][x].road = dlg.getRoad();
			Map[y][x].autobahn = dlg.getAutobahn();
			Map[y][x].river = dlg.getRiver();

			Map[y][x].rn = dlg.getRoadHex(DIRECTION_NO);
			Map[y][x].rnw = dlg.getRoadHex(DIRECTION_NW);
			Map[y][x].rsw = dlg.getRoadHex(DIRECTION_SW);
			Map[y][x].rs = dlg.getRoadHex(DIRECTION_SO);
			Map[y][x].rse = dlg.getRoadHex(DIRECTION_SE);
			Map[y][x].rne = dlg.getRoadHex(DIRECTION_NE);

			Map[y][x].an = dlg.getAutobahnHex(DIRECTION_NO);
			Map[y][x].anw = dlg.getAutobahnHex(DIRECTION_NW);
			Map[y][x].asw = dlg.getAutobahnHex(DIRECTION_SW);
			Map[y][x].as = dlg.getAutobahnHex(DIRECTION_SO);
			Map[y][x].ase = dlg.getAutobahnHex(DIRECTION_SE);
			Map[y][x].ane = dlg.getAutobahnHex(DIRECTION_NE);

			Map[y][x].vn = dlg.getRiverHex(DIRECTION_NO);
			Map[y][x].vnw = dlg.getRiverHex(DIRECTION_NW);
			Map[y][x].vsw = dlg.getRiverHex(DIRECTION_SW);
			Map[y][x].vs = dlg.getRiverHex(DIRECTION_SO);
			Map[y][x].vse = dlg.getRiverHex(DIRECTION_SE);
			Map[y][x].vne = dlg.getRiverHex(DIRECTION_NE);
		}
	}
	return Map[y][x].elevation;
}

int AirCavMapData::getContour( int x, int y )
{
	return Map[y][x].contour;
}

int AirCavMapData::getRoad( int x, int y )
{
	return Map[y][x].road;
}

int AirCavMapData::getAutobahn( int x, int y )
{
	if (m_ignoreAutobahn && Map[y][x].autobahn)
		return 0;
	return Map[y][x].autobahn;
}

int AirCavMapData::getRiver( int x, int y )
{
	if (m_ignoreWaterFeatures && Map[y][x].river)
		return 0;
	return Map[y][x].river;
}

int AirCavMapData::getSmoke( int x, int y )
{
	return Map[y][x].smoke;
}

int AirCavMapData::setSmoke( int x, int y, bool artillery, bool toggle )
{
	if ( toggle )
	{
		if ( Map[y][x].smoke ) 
			Map[y][x].smoke = 0;
		else
			Map[y][x].smoke = artillery ? 2 : 1;
	}
	else
	{
		Map[y][x].smoke = artillery ? 2 : 1;
	}
	return Map[y][x].smoke;
}

void AirCavMapData::clearSmoke( int x, int y )
{
	Map[y][x].smoke = 0;
}

int AirCavMapData::getWreck(int x, int y)
{
	return Map[y][x].wreck;
}

int AirCavMapData::setWreck(int x, int y, bool toggle)
{
	if (toggle)
	{
		if (Map[y][x].wreck)
			Map[y][x].wreck = 0;
		else
			Map[y][x].wreck = 1;
	}
	else
	{
		Map[y][x].wreck++;	// track multiple wrecks!
	}
	return Map[y][x].wreck > 0;
}

void AirCavMapData::clearWreck(int x, int y)
{
	Map[y][x].wreck = 0;
}

int AirCavMapData::getRoadHex( int x, int y, int which )
{
	switch( which )
	{
		case DIRECTION_NO: return Map[y][x].rn;
			break;
		case DIRECTION_NW: return Map[y][x].rnw;
			break;
		case DIRECTION_SW: return Map[y][x].rsw;
			break;
		case DIRECTION_SO: return Map[y][x].rs;
			break;
		case DIRECTION_SE: return Map[y][x].rse;
			break;
		case DIRECTION_NE: return Map[y][x].rne;
			break;
	}
	return 0;
}

int AirCavMapData::getAutobahnHex( int x, int y, int which )
{
	if (m_ignoreAutobahn)
		return 0;

	switch (which)
	{
		case DIRECTION_NO: return Map[y][x].an;
			break;
		case DIRECTION_NW: return Map[y][x].anw;
			break;
		case DIRECTION_SW: return Map[y][x].asw;
			break;
		case DIRECTION_SO: return Map[y][x].as;
			break;
		case DIRECTION_SE: return Map[y][x].ase;
			break;
		case DIRECTION_NE: return Map[y][x].ane;
			break;
	}
	return 0;
}

int AirCavMapData::getStreamHex( int x, int y, int which )
{
	if (m_ignoreWaterFeatures)
		return 0;

	switch( which )
	{
		case DIRECTION_NO: return Map[y][x].vn;
			break;
		case DIRECTION_NW: return Map[y][x].vnw;
			break;
		case DIRECTION_SW: return Map[y][x].vsw;
			break;
		case DIRECTION_SO: return Map[y][x].vs;
			break;
		case DIRECTION_SE: return Map[y][x].vse;
			break;
		case DIRECTION_NE: return Map[y][x].vne;
			break;
	}
	return 0;
}

int AirCavMapData::editTerrainData( int x, int y )
{
	HexTerrain dlg(x, y, Map[y][x]);
	if ( dlg.DoModal() == IDOK )
	{
		Map[y][x].terrain = dlg.getTerrain();
		Map[y][x].elevation = dlg.getElevation();
		Map[y][x].contour = dlg.getContour();
		Map[y][x].road = dlg.getRoad();
		Map[y][x].autobahn = dlg.getAutobahn();
		Map[y][x].river = dlg.getRiver();

		Map[y][x].rn = dlg.getRoadHex(DIRECTION_NO);
		Map[y][x].rnw = dlg.getRoadHex(DIRECTION_NW);
		Map[y][x].rsw = dlg.getRoadHex(DIRECTION_SW);
		Map[y][x].rs = dlg.getRoadHex(DIRECTION_SO);
		Map[y][x].rse = dlg.getRoadHex(DIRECTION_SE);
		Map[y][x].rne = dlg.getRoadHex(DIRECTION_NE);

		Map[y][x].an = dlg.getAutobahnHex(DIRECTION_NO);
		Map[y][x].anw = dlg.getAutobahnHex(DIRECTION_NW);
		Map[y][x].asw = dlg.getAutobahnHex(DIRECTION_SW);
		Map[y][x].as = dlg.getAutobahnHex(DIRECTION_SO);
		Map[y][x].ase = dlg.getAutobahnHex(DIRECTION_SE);
		Map[y][x].ane = dlg.getAutobahnHex(DIRECTION_NE);

		Map[y][x].vn = dlg.getRiverHex(DIRECTION_NO);
		Map[y][x].vnw = dlg.getRiverHex(DIRECTION_NW);
		Map[y][x].vsw = dlg.getRiverHex(DIRECTION_SW);
		Map[y][x].vs = dlg.getRiverHex(DIRECTION_SO);
		Map[y][x].vse = dlg.getRiverHex(DIRECTION_SE);
		Map[y][x].vne = dlg.getRiverHex(DIRECTION_NE);

		m_mapDataEdited = true;
	}
	return 1;
}

bool AirCavMapData::validHex( int col, int row )
{
	int maxRow;
	if ( EVEN(col) )
		maxRow = MAX_ROWS;
	else
		maxRow = MAX_ROWS - 1;

	if ( col > MAX_COLUMNS || col < MIN_COLUMNS )
		return false;
	if ( row > maxRow || row < MIN_ROWS )
		return false;
	return true;
}

bool AirCavMapData::borderHex(int col, int row)
{
	int maxRow;
	if (EVEN(col))
		maxRow = MAX_ROWS;
	else
		maxRow = MAX_ROWS - 1;

	if (col == MAX_COLUMNS || col == MIN_COLUMNS)
		return true;
	if (row == maxRow || row == MIN_ROWS)
		return true;
	return false;
}
