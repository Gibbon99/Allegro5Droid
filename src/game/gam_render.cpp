#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_player.h>
#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/io/io_resourceImage.h>
#include "hdr/game/gam_render.h"

PARA_BITMAP *completeLevelBMP = nullptr;

struct _tileTexCoords {
	b2Vec2 texCoord;
};

_tileTexCoords *tileTexCoords = nullptr;
int            numTileAcrossInTexture, numTilesDownInTexture;

//----------------------------------------------------------------------------------------------------------------------
//
// Return the backing bitmap to draw map onto
PARA_BITMAP *gam_getCompleteLevel ()
//----------------------------------------------------------------------------------------------------------------------
{
	return completeLevelBMP;
}

//-----------------------------------------------------------------------------
//
// Return texture coords for passed in tile
b2Vec2 gam_getTileTexCoords (int whichTile)
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
void gam_drawSingleTile (float destX, float destY, int whichTile)
//-----------------------------------------------------------------------------
{
	static int    previousTile = -1;
	static b2Vec2 textureCoordsSrc;

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

	if (shipLevel.at (lvl_getCurrentLevelName ()).numEnemiesAlive == 0)
		{
			al_draw_bitmap_region (bitmap.at ("alltiles_dark").bitmap,
			                       textureCoordsSrc.x, textureCoordsSrc.y,
			                       TILE_SIZE, TILE_SIZE,
			                       destX, destY, 0);
		}
	else
		{
			al_draw_bitmap_region (bitmap.at ("alltiles").bitmap,
			                       textureCoordsSrc.x, textureCoordsSrc.y,
			                       TILE_SIZE, TILE_SIZE,
			                       destX, destY, 0);
		}
//	profileTotalTileDrawn++;
}

//-----------------------------------------------------------------------------
//
// Draw all the tiles in memory to the completeLevel bitmap
void gam_drawAllTiles ()
//-----------------------------------------------------------------------------
{
	int whichTile = 0;

	if (completeLevelBMP != nullptr)
		{
			//
			// Already exists - need to destroy and recreate it
			al_destroy_bitmap (completeLevelBMP);
			completeLevelBMP = nullptr;
		}
	//
	// Now recreate it with the correct size for the current level
	if (completeLevelBMP == nullptr)
		{
			completeLevelBMP = al_create_bitmap (shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x * TILE_SIZE,
			                                     shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.y * TILE_SIZE);
			if (nullptr == completeLevelBMP)
				{
					al_show_native_message_box (nullptr, "Allegro Error", "Unable to start Allegro. Exiting", "Could create backing bimap.", nullptr, ALLEGRO_MESSAGEBOX_ERROR);
					return;
				}

			al_set_target_bitmap (completeLevelBMP);
			al_clear_to_color (al_map_rgb (1, 0, 0));
		}
	//
	// Render all tiles onto it
	al_set_target_bitmap (completeLevelBMP);

	for (int countY = 0; countY != shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.y; countY++)
		{
			for (int countX = 0; countX != shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x; countX++)
				{
					whichTile = shipLevel.at (lvl_getCurrentLevelName ()).tiles[(countY * shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x) + countX];
					if (whichTile > 0)
						gam_drawSingleTile (countX * TILE_SIZE, countY * TILE_SIZE, whichTile);
				}
		}
	//
	// Allow backdrop image to show through
	if (1 == renderBackdrop)
		al_convert_mask_to_alpha (completeLevelBMP, al_map_rgb (1, 0, 0));
}

//-----------------------------------------------------------------------------
//
// Draw the visible screen
void gam_drawVisibleScreen (float interpolation)
//-----------------------------------------------------------------------------
{
	b2Vec2 renderPlayerWorldPos;

	renderPlayerWorldPos = playerDroid.worldPos - previousPlayerWorldPos;
	renderPlayerWorldPos *= interpolation;
	renderPlayerWorldPos += previousPlayerWorldPos;

	al_set_target_bitmap (backingBitmap);
	al_draw_bitmap_region (completeLevelBMP,
	                       (float) renderPlayerWorldPos.x - (screenWidth / 2),
	                       (float) renderPlayerWorldPos.y - (screenHeight / 2),
	                       (float) screenWidth, (float) screenHeight, 0, 0, 0);
}