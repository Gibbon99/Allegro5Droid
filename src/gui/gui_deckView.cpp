#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/game/gam_render.h>
#include <hdr/game/gam_player.h>
#include <hdr/system/sys_timerFunctions.h>
#include "hdr/gui/gui_deckView.h"

static PARA_BITMAP *deckViewBitmap = nullptr;

//-----------------------------------------------------------------------------
//
// Get the deckview bitmap
PARA_BITMAP *gam_getDeckViewBitmap ()
//-----------------------------------------------------------------------------
{
	return deckViewBitmap;
}

//-----------------------------------------------------------------------------
//
// Draw the player indicator on the deckview map
void gui_renderPlayerLocation(float tileSize)
//-----------------------------------------------------------------------------
{
	int tileLocationX, tileLocationY;

	tileLocationX = previousPlayerWorldPos.x / TILE_SIZE;
	tileLocationY = previousPlayerWorldPos.y / TILE_SIZE;

	tileLocationX *= tileSize;
	tileLocationY *= tileSize;

	tileLocationY += bitmap.at("hud").height;
	tileLocationY += tileSize / 2;
	tileLocationX += tileSize / 2;

	if (tim_getIconState())
		al_draw_filled_circle (tileLocationX, tileLocationY, tileSize / 2, al_map_rgb(255,255,255));
}

//-----------------------------------------------------------------------------
//
// Draw the current level onto the entire screen
void gui_showCurrentLevel ()
//-----------------------------------------------------------------------------
{
	int         countX, countY, whichTile, levelDimensionX, levelDimensionY;
	float       hudHeight;
	float       smallTileSize;
	float       tileScale;
	float       tileSize;
	cpVect      worldPosDraw;
	std::string levelNameCache;

	al_clear_to_color (al_map_rgba (0, 0, 0, 255));

	smallTileSize = 8.0f;

	levelNameCache = lvl_getCurrentLevelName ();
	levelDimensionX = shipLevel.at (levelNameCache).levelDimensions.x;
	levelDimensionY = shipLevel.at (levelNameCache).levelDimensions.y;
	hudHeight       = (float) bitmap.at ("hud").height;

	if (levelDimensionX * smallTileSize > screenWidth)
	{
		tileScale = screenWidth / (levelDimensionX * smallTileSize);
		tileSize  = smallTileSize * tileScale;
	}
	else
	{
		tileSize = smallTileSize;
	}

	for (countY = 0; countY != levelDimensionY; countY++)
	{
		for (countX = 0; countX != levelDimensionX; countX++)
		{
			whichTile = shipLevel.at (levelNameCache).tiles[(countY * levelDimensionX) + countX];
			if (whichTile > 0)
			{
				worldPosDraw.x = (float) countX * tileSize;
				worldPosDraw.y = (float) countY * tileSize + hudHeight;

				if (whichTile > 40)
				{
					al_draw_filled_rectangle (worldPosDraw.x, worldPosDraw.y, worldPosDraw.x + tileSize, worldPosDraw.y + tileSize, al_map_rgba (100, 30, 200, 255));
					al_draw_rectangle (worldPosDraw.x, worldPosDraw.y, worldPosDraw.x + tileSize, worldPosDraw.y + tileSize, al_map_rgba (0, 30, 100, 255), 1);
				}
				else
				{
					al_draw_filled_rectangle (worldPosDraw.x, worldPosDraw.y, worldPosDraw.x + tileSize, worldPosDraw.y + tileSize, al_map_rgba (100, 30, 20, 255));
				}
			}
		}
	}
	gui_renderPlayerLocation(tileSize);

	fnt_setColor_f (0.7f, 0.7f, 0.7f, 1.0f);
	fnt_setTTF ("gui");
	fnt_render (b2Vec2{20, static_cast<float32>(bitmap.at("hud").height + 3)}, sys_getString ("%s", shipLevel.at(levelNameCache).levelName));
}
