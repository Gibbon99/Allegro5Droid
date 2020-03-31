#pragma once

#include "system/sys_main.h"

extern          float wayPointDestinationSize;      // From script

//--------------------------------------------------------------------
//
// Functions run by the aiTree - processing Droid actions
//
//--------------------------------------------------------------------
//
// Process all Droid movements
int ai_getNextWaypoint ( int whichDroid, std::string levelName );

// Can this droid make a move this turn
//
// Will fail if it is currently in a collision with either another droid or
// the player
int ai_canMove(int whichDroid, std::string levelName);

// This droid has collided - can it reverse direction and move
int ai_canReverseDirection(int whichDroid, const std::string& levelName);

int ai_moveDroidToWaypoint ( int whichDroid, const std::string levelName );

void ai_processDroidMovement ( std::string levelName );

// Two droids have collided - one needs to reverse direction
void ai_handleDroidCollision(int indexDroidA, int indexDroidB);