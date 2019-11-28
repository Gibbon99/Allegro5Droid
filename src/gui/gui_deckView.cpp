#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/game/gam_render.h>
#include "hdr/gui/gui_deckView.h"

static PARA_BITMAP *deckViewBitmap = nullptr;

//-----------------------------------------------------------------------------
//
// Get the deckview bitmap
PARA_BITMAP *gam_getDeckViewBitmap()
//-----------------------------------------------------------------------------
{
	return deckViewBitmap;
}

//-----------------------------------------------------------------------------
//
// Draw the current level onto the entire screen
void gam_showCurrentLevel (float ratio)
//-----------------------------------------------------------------------------
{
	int            countX, countY, whichTile;
	cpVect         worldPosDraw;
	cpVect         drawScreenOffset;
	cpVect         srcTexture;
	cpVect         textureSize;

	static bool wroteBitmap   = false;
	float       smallTileSize = 0.0f;

	if (nullptr == deckViewBitmap)
		{
			deckViewBitmap = al_create_bitmap(screenWidth, screenHeight);
			al_set_target_bitmap (deckViewBitmap);
		}

	al_set_target_bitmap (deckViewBitmap);

	smallTileSize = al_get_bitmap_width(bitmap.at ("alltiles_128").bitmap) / numTileAcrossInTexture;

	textureSize.x = al_get_bitmap_width(bitmap.at ("alltiles_128").bitmap);
	textureSize.y = al_get_bitmap_height(bitmap.at ("alltiles_128").bitmap);

	for (countY = 0; countY != shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.y; countY++)
		{
			for (countX = 0; countX != shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x; countX++)
				{
					whichTile = shipLevel.at (lvl_getCurrentLevelName ()).tiles[(countY * shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x) + countX];
					if (whichTile > 0)
						{
							worldPosDraw.x = countX * smallTileSize;
							worldPosDraw.y = countY * smallTileSize;

							srcTexture.x = (whichTile % numTileAcrossInTexture) * smallTileSize;
							srcTexture.y = (whichTile / numTilesDownInTexture) * smallTileSize;

							al_draw_bitmap_region (bitmap.at ("alltiles_128").bitmap, srcTexture.x, srcTexture.y, smallTileSize, smallTileSize,
							                       worldPosDraw.x, worldPosDraw.y, 0);
						}
				}
		}

		al_set_target_backbuffer(display);
		al_draw_bitmap (deckViewBitmap, 0, 0, 0);

//	term_drawIndicator(TILE_SIZE / 2);
}
