#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <io.h>
#include "AirCavMapData.h"
#include "HexTerrain.h"

#pragma warning(disable:4996)

#define EPSILON 0.00001
#define DBLEQUAL(a1, a2) (((a1 - a2) < EPSILON) && ( (a1 - a2) > -EPSILON))
#define EPSILONTAN 15.0
#define DBLEQUALTAN(a1, a2) (((a1 - a2) < EPSILONTAN) && ( (a1 - a2) > -EPSILONTAN))

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
		case NO:
			*x = cur_x;
			*y = cur_y - 1;
			break;
		case NE:
			if (EVEN(cur_x))
				*y = cur_y - 1;
			else
				*y = cur_y;
			*x = cur_x + 1;
			break;
		case SE:
			if (EVEN(cur_x))
				*y = cur_y;
			else
				*y = cur_y + 1;
			*x = cur_x + 1;
			break;
		case SO:
			*x = cur_x;
			*y = cur_y + 1;
			break;
		case SW:
			if (EVEN(cur_x))
				*y = cur_y;
			else
				*y = cur_y + 1;
			*x = cur_x - 1;
			break;
		case NW:
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
   map_file_pointer = fopen (map.c_str(), "rb");
	if (map_file_pointer == NULL) 
	{
//		printf("file open failed\n");
		return 0;
	}
//	else 
//		printf("file open successful\n");

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
	int i,j, h = 0;

	/* reopen the old file, write new data and close data file */
	if (map_file_pointer)
		map_file_pointer = freopen (map.c_str(), "wb", map_file_pointer);
	else
		map_file_pointer = fopen (map.c_str(), "wb");

	if (map_file_pointer == NULL) 
		return 0;

	for (i=0; i < NROW; i++)
	{
		for (j=0; j < NCOL; j++)
		{
			fwrite (&Map[j][i].terrain,   sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].elevation, sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].contour,   sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].road,      sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].autobahn,  sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].river,     sizeof(int), 1, map_file_pointer);

			fwrite (&Map[j][i].rnw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].rn,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].rne,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].rsw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].rs,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].rse,        sizeof(int), 1, map_file_pointer);

			fwrite (&Map[j][i].anw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].an,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].ane,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].asw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].as,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].ase,        sizeof(int), 1, map_file_pointer);

			fwrite (&Map[j][i].vnw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].vn,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].vne,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].vsw,        sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].vs,         sizeof(int), 1, map_file_pointer);
			fwrite (&Map[j][i].vse,        sizeof(int), 1, map_file_pointer);

			fwrite (&Map[j][i].unit,       sizeof(int), 1, map_file_pointer);

			if (Map[j][i].elevation >= 0) h++;
		}
	}

	i = fclose (map_file_pointer);
	if (i != 0)
		return 0;

	// write out the DEM
	FILE *dem_file_pointer;
	dem_file_pointer = fopen ("dem.txt", "w");

	if (dem_file_pointer == NULL) 
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
				e = 0.0;
			else if (Map[c][r].river)
				e = -10.0;
			else if (Map[c][r].road)
				e = -20.0;
			else
				e = (double)Map[c][r].elevation;

			fprintf (dem_file_pointer, "%f, %f, %f\n", x, y, e);
		}
	}
	fclose(dem_file_pointer);

	sprintf( msgbox, "Number of hexes known: %d out of %d (%3.1f percent)\n", h, NROW*NCOL, (double)h/(double)(NROW*NCOL)*(double)100.0 );

    return 1;
}


int AirCavMapData::CalculateLOS( int org_x, int org_y, int org_elev, int tgt_x, int tgt_y, int tgt_elev, char *logString )
{
	char logBuffer[256];
	int Fdir, Ldir, Rdir, Rx, Ry, Fx, Fy, Lx, Ly, cur_x, cur_y;
	int clear_LOS, blk1, blk2, range;
	double LOS_vec_x, LOS_vec_y, angle;
	double front_x, front_y, left_x, left_y, right_x, right_y;
	double Ftan, Ltan, Rtan, Fangle, Langle, Rangle, SmallTan;
	double Flen, Llen, Rlen, LOS_len;
	double Fvec_x, Fvec_y, Lvec_x, Lvec_y, Rvec_x, Rvec_y;
	double Fvec, Lvec, Rvec;

	sprintf( logBuffer, "Calculating LOS from %02d%02d to %02d%02d\n", org_y, org_x, tgt_y, tgt_x);
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
		{ Ldir = NE;   Fdir = SE;   Rdir = SO; }
	else if (LOS_vec_y > 0 && LOS_vec_x < 0)
		{ Ldir = SO;   Fdir = SW;   Rdir = NW; }
	else if (LOS_vec_y > 0 && LOS_vec_x == 0)
		{ Ldir = SE;   Fdir = SO;   Rdir = SW; }
	else if (LOS_vec_y <= 0 && LOS_vec_x > 0)
		{ Ldir = NO;   Fdir = NE;   Rdir = SE; }
	else if (LOS_vec_y <= 0 && LOS_vec_x < 0)
		{ Ldir = SW;   Fdir = NW;   Rdir = NO; }
	else if (LOS_vec_y < 0 && LOS_vec_x == 0)
		{ Ldir = NW;   Fdir = NO;   Rdir = NE; }
	else
	   return(0);		// same hex, just return 0

	getElevation( org_x, org_y );
	getElevation( tgt_x, tgt_y );
	sprintf( logBuffer, "Origin Hex %02d%02d   ", org_y, org_x );
	strcat( logString, logBuffer );
	sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[org_y][org_x].terrain],Map[org_y][org_x].elevation+org_elev );
	strcat( logString, logBuffer );
	sprintf( logBuffer, "Target Hex %02d%02d   ", tgt_y, tgt_x );
	strcat( logString, logBuffer );
	sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[tgt_y][tgt_x].terrain],Map[tgt_y][tgt_x].elevation+tgt_elev );
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
	if ( (DBLEQUALTAN(SmallTan, Ftan)) && (DBLEQUALTAN(SmallTan, Rtan)) )
	{
		blk1 = Check_Block (org_x, org_y, org_elev, Fx, Fy, tgt_x, tgt_y, tgt_elev );
		sprintf( logBuffer, "Hex 1: %02d%02d result: %s  ", Fy, Fx, blk1?"Blocked":"Clear" );
		strcat( logString, logBuffer );
		sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[Fy][Fx].terrain],Map[Fy][Fx].elevation );
		strcat( logString, logBuffer );
		blk2 = Check_Block (org_x, org_y, org_elev, Rx, Ry, tgt_x, tgt_y, tgt_elev);
		sprintf( logBuffer, "Hex 2: %02d%02d result: %s  ", Ry, Rx, blk2?"Blocked":"Clear" );
		strcat( logString, logBuffer );
		sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[Ry][Rx].terrain],Map[Ry][Rx].elevation );
		strcat( logString, logBuffer );
		if (blk1 & blk2)
		{
			clear_LOS = 0;
			blk_x = Fx;
			blk_y = Fy;
		}
		cur_x = Fx;
		cur_y = Fy;
	}
	else if ( (DBLEQUALTAN(SmallTan, Ftan)) && (DBLEQUALTAN(SmallTan, Ltan)) )
	{
		blk1 = Check_Block (org_x, org_y, org_elev, Fx, Fy, tgt_x, tgt_y, tgt_elev);
		sprintf( logBuffer, "Hex 1: %02d%02d result: %s  ", Fy, Fx, blk1?"Blocked":"Clear" );
		strcat( logString, logBuffer );
		sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[Fy][Fx].terrain],Map[Fy][Fx].elevation );
		strcat( logString, logBuffer );
		blk2 = Check_Block (org_x, org_y, org_elev, Lx, Ly, tgt_x, tgt_y, tgt_elev);
		sprintf( logBuffer, "Hex 2: %02d%02d result: %s  ", Ly, Lx, blk2?"Blocked":"Clear" );
		strcat( logString, logBuffer );
		sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[Ly][Lx].terrain],Map[Ly][Lx].elevation );
		strcat( logString, logBuffer );
		if (blk1 & blk2)
		{
			clear_LOS = 0;
			blk_x = Fx;
			blk_y = Fy;
		}
		cur_x = Fx;
		cur_y = Fy;
	}
	else if ( DBLEQUAL(SmallTan, Ltan) )
	{
		blk1 = Check_Block (org_x, org_y, org_elev, Lx, Ly, tgt_x, tgt_y, tgt_elev);
		sprintf( logBuffer, "Hex: %02d%02d result: %s  ", Ly, Lx, blk1?"Blocked":"Clear" );
		strcat( logString, logBuffer );
		sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[Ly][Lx].terrain],Map[Ly][Lx].elevation );
		strcat( logString, logBuffer );
		if (blk1)
		{
			clear_LOS = 0;
			blk_x = Lx;
			blk_y = Ly;
		}
		cur_x = Lx;
		cur_y = Ly;
	}
	else if ( DBLEQUAL(SmallTan, Ftan) )
	{
		blk1 = Check_Block (org_x, org_y, org_elev, Fx, Fy, tgt_x, tgt_y, tgt_elev);
		sprintf( logBuffer, "Hex: %02d%02d result: %s  ", Fy, Fx, blk1?"Blocked":"Clear" );
		strcat( logString, logBuffer );
		sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[Fy][Fx].terrain],Map[Fy][Fx].elevation );
		strcat( logString, logBuffer );
		if (blk1)
		{
			clear_LOS = 0;
			blk_x = Fx;
			blk_y = Fy;
		}
		cur_x = Fx;
		cur_y = Fy;
	}
	else if ( DBLEQUAL(SmallTan, Rtan) )
	{
		blk1 = Check_Block (org_x, org_y, org_elev, Rx, Ry, tgt_x, tgt_y, tgt_elev);
		sprintf( logBuffer, "Hex: %02d%02d result: %s  ", Ry, Rx, blk1?"Blocked":"Clear" );
		strcat( logString, logBuffer );
		sprintf( logBuffer, "Terrain: %s  Elevation: %d\n",TerrainString[Map[Ry][Rx].terrain],Map[Ry][Rx].elevation );
		strcat( logString, logBuffer );
		if (blk1)
		{
			clear_LOS = 0;
			blk_x = Rx;
			blk_y = Ry;
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
	if (! clear_LOS)
	{
		if (cur_x == tgt_x && cur_y == tgt_y)
			clear_LOS = 1;
		else
			range = 0;
		return (range);
	}
	else
	{
		if (cur_x == tgt_x && cur_y == tgt_y)
			return (range);
		else
			goto top;
	}
}

int AirCavMapData::Check_Block (int org_x, int org_y, int org_elev, int x, int y, int tgt_x, int tgt_y, int tgt_elev)
{
	double dist2tgt, dist2org;
	int tgt_hgt, org_hgt, int_hgt, blk;

	blk = 0;
	int_hgt = getElevation( x, y );
	org_hgt = getElevation( org_x, org_y ) + org_elev;
	tgt_hgt = getElevation( tgt_x, tgt_y ) + tgt_elev;

	if (Map[y][x].terrain == WOODS || Map[y][x].terrain == TOWN)
	{
		int_hgt += 10;
	}

	if (int_hgt > org_hgt && int_hgt > tgt_hgt)
	{
		blk = 1;
	}
	else if (Map[y][x].smoke == 1)
	{
		blk = 1;
	}
	else if (org_hgt > tgt_hgt)
	{
		if (int_hgt >= org_hgt)
		{
			blk = 1;
		}
		else if (int_hgt < org_hgt && int_hgt > tgt_hgt)
		{
			double d2o = (x-org_x)*(x-org_x) + (y-org_y)*(y-org_y);
			dist2org = sqrt(d2o);
			double d2t = (x-tgt_x)*(x-tgt_x) + (y-tgt_y)*(y-tgt_y);
			dist2tgt = sqrt(d2t);
			if (dist2tgt < dist2org)
			{
				blk = 1;
			}
		}
	}
	else if (org_hgt < tgt_hgt)
	{
		if (int_hgt >= tgt_hgt)
		{
			blk = 1;
		}
		else if (int_hgt > org_hgt && int_hgt < tgt_hgt)
		{
			double d2o = (x-org_x)*(x-org_x) + (y-org_y)*(y-org_y);
			dist2org = sqrt(d2o);
			double d2t = (x-tgt_x)*(x-tgt_x) + (y-tgt_y)*(y-tgt_y);
			dist2tgt = sqrt(d2t);
			if (dist2org < dist2tgt)
			{
				blk = 1;
			}
		}
	}
	return (blk);
}

int AirCavMapData::getTerrain( int x, int y )
{
	return Map[y][x].terrain;
}

int AirCavMapData::getElevation( int x, int y )
{
	// check if elevation is known, if not ask for it
	if ( Map[y][x].elevation < 0 )
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
			Map[y][x].rn = dlg.getRoadHex(0);
			Map[y][x].rnw = dlg.getRoadHex(1);
			Map[y][x].rsw = dlg.getRoadHex(2);
			Map[y][x].rs = dlg.getRoadHex(3);
			Map[y][x].rse = dlg.getRoadHex(4);
			Map[y][x].rne = dlg.getRoadHex(5);
			Map[y][x].an = dlg.getAutobahnHex(0);
			Map[y][x].anw = dlg.getAutobahnHex(1);
			Map[y][x].asw = dlg.getAutobahnHex(2);
			Map[y][x].as = dlg.getAutobahnHex(3);
			Map[y][x].ase = dlg.getAutobahnHex(4);
			Map[y][x].ane = dlg.getAutobahnHex(5);
			Map[y][x].vn = dlg.getRiverHex(0);
			Map[y][x].vnw = dlg.getRiverHex(1);
			Map[y][x].vsw = dlg.getRiverHex(2);
			Map[y][x].vs = dlg.getRiverHex(3);
			Map[y][x].vse = dlg.getRiverHex(4);
			Map[y][x].vne = dlg.getRiverHex(5);
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
	return Map[y][x].autobahn;
}

int AirCavMapData::getRiver( int x, int y )
{
	return Map[y][x].river;
}

int AirCavMapData::getSmoke( int x, int y )
{
	return Map[y][x].smoke;
}

int AirCavMapData::setSmoke( int x, int y, bool toggle )
{
	if ( toggle )
	{
		if ( Map[y][x].smoke ) 
			Map[y][x].smoke = 0;
		else
			Map[y][x].smoke = 1;
	}
	else
	{
		Map[y][x].smoke = 1;
	}
	return Map[y][x].smoke;
}

void AirCavMapData::clearSmoke( int x, int y )
{
	Map[y][x].smoke = 0;
}

int AirCavMapData::getRoadHex( int x, int y, int which )
{
	switch( which )
	{
		case 0: return Map[y][x].rn;
			break;
		case 1: return Map[y][x].rnw;
			break;
		case 2: return Map[y][x].rsw;
			break;
		case 3: return Map[y][x].rs;
			break;
		case 4: return Map[y][x].rse;
			break;
		case 5: return Map[y][x].rne;
			break;
	}
	return 0;
}

int AirCavMapData::getAutobahnHex( int x, int y, int which )
{
	switch( which )
	{
		case 0: return Map[y][x].an;
			break;
		case 1: return Map[y][x].anw;
			break;
		case 2: return Map[y][x].asw;
			break;
		case 3: return Map[y][x].as;
			break;
		case 4: return Map[y][x].ase;
			break;
		case 5: return Map[y][x].ane;
			break;
	}
	return 0;
}

int AirCavMapData::getStreamHex( int x, int y, int which )
{
	switch( which )
	{
		case 0: return Map[y][x].vn;
			break;
		case 1: return Map[y][x].vnw;
			break;
		case 2: return Map[y][x].vsw;
			break;
		case 3: return Map[y][x].vs;
			break;
		case 4: return Map[y][x].vse;
			break;
		case 5: return Map[y][x].vne;
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
		Map[y][x].rn = dlg.getRoadHex(0);
		Map[y][x].rnw = dlg.getRoadHex(1);
		Map[y][x].rsw = dlg.getRoadHex(2);
		Map[y][x].rs = dlg.getRoadHex(3);
		Map[y][x].rse = dlg.getRoadHex(4);
		Map[y][x].rne = dlg.getRoadHex(5);
		Map[y][x].an = dlg.getAutobahnHex(0);
		Map[y][x].anw = dlg.getAutobahnHex(1);
		Map[y][x].asw = dlg.getAutobahnHex(2);
		Map[y][x].as = dlg.getAutobahnHex(3);
		Map[y][x].ase = dlg.getAutobahnHex(4);
		Map[y][x].ane = dlg.getAutobahnHex(5);
		Map[y][x].vn = dlg.getRiverHex(0);
		Map[y][x].vnw = dlg.getRiverHex(1);
		Map[y][x].vsw = dlg.getRiverHex(2);
		Map[y][x].vs = dlg.getRiverHex(3);
		Map[y][x].vse = dlg.getRiverHex(4);
		Map[y][x].vne = dlg.getRiverHex(5);
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