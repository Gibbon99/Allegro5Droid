#pragma once

#include "system/sys_main.h"
#include "system/sys_physics.h"
#include "io/io_resourceLevel.h"

extern b2Vec2        previousPlayerWorldPos;
extern _droid        playerDroid;
extern float  playerAcceleration;
extern float  playerMaxSpeed;
extern float  gravity;

// Position the player on the requested lift on the new level
b2Vec2 gam_getLiftWorldPosition (int whichLift, std::string whichLevel);

// Process player movement
void gam_processPlayerMovement ();

// Start all the actions resulting from pressing the action key
void gam_processActionKey();

// Setup player startup values
void gam_initPlayerValues ();

// Recharge player weapon
void gam_weaponRechargePlayer(float tickTime);

// Render the player sprite
void gam_renderPlayer();

// Destroy the player and end game
void gam_destroyPlayer();