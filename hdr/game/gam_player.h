#pragma once

#include "system/sys_main.h"
#include "system/sys_physics.h"

extern cpVect playerWorldPos;
extern cpVect previousPlayerWorldPos;
extern cpVect        playerVelocity;
extern _physicObject playerPhysicsObjectClient;

// Position the player on the requested lift on the new level
cpVect gam_getLiftWorldPosition (int whichLift, std::string whichLevel);

// Process player movement
void gam_processPlayerMovement();