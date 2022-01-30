#pragma once

#include "AirCavCommon.h"


class AirCavMapData
{
public:
	AirCavMapData(void);
	~AirCavMapData(void);

	int OpenMapDataFile(std::string& file_dir);
	int SaveAndCloseMapDataFile( char *msgbox );
	int CloseMapDataFile(void);
	int CalculateLOS( int fx, int fy, int fe, bool ti, int tx, int ty, int te, bool &skylined, char *log );
	int Check_Block (int org_x, int org_y, int fe, int x, int y, int tgt_x, int tgt_y, int te);

public:
	int getContour( int col, int row );
	int getTerrain( int col, int row );
	int getElevation( int col, int row );
	int getRoad( int x, int y );
	int getAutobahn( int x, int y );
	int getRiver( int x, int y );
	int getRoadHex( int x, int y, int a );
	int getAutobahnHex( int x, int y, int r );
	int getStreamHex( int x, int y, int r );
	int editTerrainData( int x, int y );
	int getSmoke( int x, int y );
	int setSmoke( int x, int y, bool toggle = FALSE );
	void clearSmoke( int x, int y );
	bool validHex( int col, int row );
	bool borderHex( int col, int row );
	int getWreck( int x, int y );
	int setWreck( int x, int y, bool toggle = FALSE );
	void clearWreck( int x, int y );
	
	void setIgnoreWaterFeatures()
	{
		if (m_ignoreWaterFeatures)
			m_ignoreWaterFeatures = false;
		else
			m_ignoreWaterFeatures = true;
	}
	void setIgnoreAutobahn()
	{
		if (m_ignoreAutobahn)
			m_ignoreAutobahn = false;
		else
			m_ignoreAutobahn = true;
	}

	bool getMapDataEdited() { return m_mapDataEdited; }
	void setMapDataEdited() { m_mapDataEdited = true; }

private:
	FILE	*map_file_pointer;
	MAP		Map[NCOL][NROW];
	double	blk_x, blk_y;

	std::string map;
	std::string dem;

	bool m_ignoreWaterFeatures;
	bool m_ignoreAutobahn;
	bool m_mapDataEdited;
};


