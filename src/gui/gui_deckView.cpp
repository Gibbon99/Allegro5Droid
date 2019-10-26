#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/game/gam_render.h>
#include "hdr/gui/gui_deckView.h"

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
	static ALLEGRO_BITMAP *ratioBitmap = nullptr;

	static bool wroteBitmap = false;
	float newTileSize = 0.0f;

	if (nullptr == ratioBitmap)
		{
			ratioBitmap = al_create_bitmap(screenWidth, screenHeight);

			al_set_target_bitmap (ratioBitmap);

			al_draw_scaled_bitmap (gam_getCompleteLevel(), 0, 0, al_get_bitmap_width(gam_getCompleteLevel()), al_get_bitmap_height(gam_getCompleteLevel()),
					0, 0, al_get_bitmap_width(ratioBitmap), al_get_bitmap_height(ratioBitmap), 0);

			al_set_target_bitmap(al_get_backbuffer(display));

			/*
			if (wroteBitmap == false)
				{
					al_save_bitmap("ratio.bmp", ratioBitmap);
					wroteBitmap = true;
				}
			 */
		}
	al_draw_bitmap (ratioBitmap, 0, 0, 0);

	return;

	if (nullptr == ratioBitmap)
		{
			ratioBitmap = al_create_bitmap (bitmap.at ("alltiles_128").width * ratio, bitmap.at ("alltiles_128").height * ratio);
			if (nullptr == ratioBitmap)
				log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to create ratio sized bitmap for deck view."));

			al_set_target_bitmap (ratioBitmap);   // draw the ratio sized bitmap here
			al_draw_scaled_bitmap (bitmap.at("alltiles_128").bitmap, 0, 0, bitmap.at("alltiles_128").width, bitmap.at("alltiles_128").height,
			                       0, 0, bitmap.at ("alltiles_128").width * ratio, bitmap.at ("alltiles_128").height * ratio, 0);

			al_set_target_bitmap(al_get_backbuffer(display));
		}



	if (wroteBitmap == false)
		{
			al_save_bitmap("ratio.bmp", ratioBitmap);
			wroteBitmap = true;
		}

	newTileSize = al_get_bitmap_width(ratioBitmap) / 8;

	textureSize.x = al_get_bitmap_width(ratioBitmap); // / 8;
	textureSize.y = al_get_bitmap_height(ratioBitmap); // / 8;

	drawScreenOffset.x = (screenWidth - (shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x * (newTileSize / 2))) / 2;
	drawScreenOffset.y = (screenHeight - (shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.y * (newTileSize / 2))) / 2;

	drawScreenOffset.x = 0;
	drawScreenOffset.y = 0;

	for (countY = 0; countY != shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.y; countY++)
		{
			for (countX = 0; countX != shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x; countX++)
				{
					whichTile = shipLevel.at (lvl_getCurrentLevelName ()).tiles[(countY * shipLevel.at (lvl_getCurrentLevelName ()).levelDimensions.x) + countX];
					if (0 != whichTile)
						{
							worldPosDraw.x = (countX * (newTileSize + drawScreenOffset.x));
							worldPosDraw.y = (countY * (newTileSize + drawScreenOffset.y));

							srcTexture.x = (whichTile % (int) textureSize.x) * ((newTileSize));
							srcTexture.y = (whichTile / (int) textureSize.y) * ((newTileSize));

//							al_draw_bitmap_region (ratioBitmap, srcTexture.x, srcTexture.y, TILE_SIZE / 2, TILE_SIZE / 2, 0, 0, 0);
							al_draw_bitmap_region (ratioBitmap, srcTexture.x, srcTexture.y, newTileSize, newTileSize,
							                       worldPosDraw.x, worldPosDraw.y, 0);

						}
				}
		}
//		al_draw_bitmap (ratioBitmap, 0, 0, 0);

//	sys_printStringExt(33.0f, winHeight - 65, "Deck View - %s", shipLevel[currentLevel].levelName);

//	term_drawIndicator(TILE_SIZE / 2);
}
