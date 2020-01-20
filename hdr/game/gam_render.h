#pragma once

#include "system/sys_main.h"

extern int            numTileAcrossInTexture, numTilesDownInTexture;
extern std::string      tileType;
extern std::string      tileColor;

// Setup up precalculated coords for the tiles
void gam_calcTileTexCoords ();

// Draw all the tiles in memory to the completeLevel bitmap
void gam_drawAllTiles();

// Draw the visible screen
void gam_drawVisibleScreen (float interpolation);

// Draw a single tile from the tile sheet
void inline gam_drawSingleTile (float destX, float destY, int whichTile);

// Return the backing bitmap to draw map onto
PARA_BITMAP *gam_getCompleteLevel();