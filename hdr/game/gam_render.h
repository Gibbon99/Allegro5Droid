#pragma once

#include "system/sys_main.h"

// Setup up precalculated coords for the tiles
void gam_calcTileTexCoords ();

// Draw all the tiles in memory to the completeLevel bitmap
void gam_drawAllTiles();

// Draw the visible screen
void gam_drawVisibleScreen (float interpolation);