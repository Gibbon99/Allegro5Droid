#pragma once

#include "system/sys_main.h"

//---------------------------------------------------------
//
// AI Routines for droid fleeing
//
//---------------------------------------------------------

// Find the nearest flee tile
int ai_reachedFleeTile (int whichDroid, std::string levelName);

// Has the droid found the flee tile
int ai_onFleeTile (int whichDroid, std::string levelName);

// Droid checks his health status and number of healing tiles
int ai_isDroidHealthyFlee (int whichDroid, std::string levelName);

// Reset flee flags for when found destination tile or changing level
void ai_resetFleeFlags ( int whichDroid, std::string levelName );
