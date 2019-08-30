#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_player.h>
#include <hdr/system/sys_gameFrameRender.h>
#include "hdr/game/gam_render.h"

PARA_BITMAP *completeLevelBMP = nullptr;

struct _tileTexCoords {
	cpVect texCoord;
};

_tileTexCoords *tileTexCoords   = nullptr;
int            numTileAcrossInTexture, numTilesDownInTexture;
std::string    currentLevelName = "Research";

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
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("File [ %s ] not found – [ %s ]", "alltiles.bmp", oor.what ()));
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

//-----------------------------------------------------------------------------
//
// Draw a single tile from the tile sheet
void inline gam_drawSingleTile (float destX, float destY, int whichTile)
//-----------------------------------------------------------------------------
{
	static int    previousTile = -1;
	static cpVect textureCoordsSrc;

#ifdef TILE_LOOKUP
	if (previousTile != whichTile)
		{
			textureCoords = gam_getTileTexCoords(whichTile);
			previousTile = whichTile;
			profileTileCalcSkipped++;
		}
#else
	if (previousTile != whichTile)
		{
			textureCoordsSrc.x = (float) (whichTile % numTileAcrossInTexture) * TILE_SIZE;
			textureCoordsSrc.y = (float) (whichTile / numTilesDownInTexture) * TILE_SIZE;

			previousTile = whichTile;
//		profileTileCalcSkipped++;
		}
#endif

	al_draw_bitmap_region (bitmap.at ("alltiles").bitmap,
	                       textureCoordsSrc.x, textureCoordsSrc.y,
	                       TILE_SIZE, TILE_SIZE,
	                       destX, destY, 0);

//	profileTotalTileDrawn++;
}

//-----------------------------------------------------------------------------
//
// Draw all the tiles in memory to the completeLevel bitmap
void gam_drawAllTiles ()
//-----------------------------------------------------------------------------
{
	int whichTile = 0;

	if (completeLevelBMP == nullptr)
		{
			completeLevelBMP = al_create_bitmap (shipLevel.at (currentLevelName).levelDimensions.x * TILE_SIZE,
			                                     shipLevel.at (currentLevelName).levelDimensions.y * TILE_SIZE);
			if (nullptr == completeLevelBMP)
				{
					al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could create backing bimap.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
					return;
				}

			al_set_target_bitmap (completeLevelBMP);
			al_clear_to_color (al_map_rgb (1, 0, 0));
		}

	al_set_target_bitmap (completeLevelBMP);

	int countX = 0;
	int countY = 0;

	for (countY = 0; countY != shipLevel.at (currentLevelName).levelDimensions.y; countY++)
		{
			for (countX = 0; countX != shipLevel.at (currentLevelName).levelDimensions.x; countX++)
				{
					whichTile = shipLevel.at (currentLevelName).tiles[(countY * shipLevel.at (currentLevelName).levelDimensions.x) + countX];
					gam_drawSingleTile (countX * TILE_SIZE, countY * TILE_SIZE, whichTile);
				}
		}
}

//-----------------------------------------------------------------------------
//
// Draw the visible screen
void gam_drawVisibleScreen (double interpolation)
//-----------------------------------------------------------------------------
{
	cpVect renderPlayerWorldPos;

	renderPlayerWorldPos = previousPlayerWorldPos + ((playerDroid.worldPos - previousPlayerWorldPos) * interpolation);
	al_set_target_bitmap (backingBitmap);
	al_draw_bitmap_region (completeLevelBMP,
	                       (float)renderPlayerWorldPos.x - (screenWidth / 2),
	                       (float)renderPlayerWorldPos.y - (screenHeight / 2),
	                       (float)screenWidth, (float)screenHeight, 0, 0, 0);
}