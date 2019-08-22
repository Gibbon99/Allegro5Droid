#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resources.h>
#include "hdr/game/gam_render.h"

struct _tileTexCoords {
	cpVect texCoord;
};

_tileTexCoords *tileTexCoords = nullptr;
int            numTileAcrossInTexture, numTilesDownInTexture;

//-----------------------------------------------------------------------------
//
// Return texture coords for passed in tile
cpVect gam_getTileTexCoords (int whichTile)
//-----------------------------------------------------------------------------
{
	return tileTexCoords[whichTile].texCoord;
}

//-----------------------------------------------------------------------------
//
// Setup up precalculated coords for the tiles
void gam_calcTileTexCoords ()
//-----------------------------------------------------------------------------
{
	int totalNumTiles;
	//
	// How many tiles fit into the texture
	try
		{
			numTileAcrossInTexture = bitmap.at ("alltiles").width / TILE_SIZE;
			numTilesDownInTexture  = bitmap.at ("alltiles").height / TILE_SIZE;
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("File [ %s ] not found – [ %s ]", "alltiles.bmp", oor.what()));
			return;
		}

	totalNumTiles = numTileAcrossInTexture * numTilesDownInTexture;
	//
	// Get enough memory to hold all the coords
	tileTexCoords = (_tileTexCoords *) sys_malloc ((sizeof (_tileTexCoords) * totalNumTiles), "tileTecCoords");
	if (nullptr == tileTexCoords)
		{
			log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to get memory to hold tile coordinates."));
		}

	for (int i = 0; i != totalNumTiles; i++)
		{
			tileTexCoords[i].texCoord.x = (float) (i % numTileAcrossInTexture) * TILE_SIZE;
			tileTexCoords[i].texCoord.y = (float) (i / numTilesDownInTexture) * TILE_SIZE;
		}
}

