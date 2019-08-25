#pragma once

#include "system/sys_main.h"

extern std::string currentLevelName;

// Setup up precalculated coords for the tiles
void gam_calcTileTexCoords ();

// Draw all the tiles in memory to the completeLevel bitmap
void gam_drawAllTiles();

// Draw the visible screen
void gam_drawVisibleScreen (double interpolation);