#pragma once

#include "system/sys_main.h"

extern float deckViewRatio;

// Get the positions of terminals on the current level
void gam_findTerminalPositions ( const std::string &levelName );

// Pressed the action key while over a terminal tile
void gam_performTerminalAction();
