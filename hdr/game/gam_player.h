#pragma once

#include "system/sys_main.h"
#include "system/sys_physics.h"
#include "io/io_resourceLevel.h"

extern cpVect        previousPlayerWorldPos;
extern _droid        playerDroid;

// Position the player on the requested lift on the new level
cpVect gam_getLiftWorldPosition (int whichLift, std::string whichLevel);

// Process player movement
void gam_processPlayerMovement ();