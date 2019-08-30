#pragma once

#include "system/sys_main.h"


//---------------------------------------------------------
//
// AI Routines for a droid to return to a location
//
//---------------------------------------------------------

// Checks to see if the droid needs to return to patrolling
int ai_isNotPatrolling ( int whichDroid, std::string levelName );

// Need to find the nearest WP to move to
int ai_onResumeDest ( int whichDroid, std::string levelName );

// Work out the AStar path to the destination
int ai_reachedResumeDest ( int whichDroid, std::string levelName );

// Arrived at the destination
int ai_foundResumeDest ( int whichDroid, std::string levelName );
