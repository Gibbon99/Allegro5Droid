#pragma once

#include "system/sys_main.h"


//---------------------------------------------------------
//
// AI Routines for a droid to shoot and react
//
//---------------------------------------------------------

// Flag if a droid witnesses the player shooting another droid
void gam_processWitnessShooting(std::string levelName);

// Work out the chance to shoot for this round
void gam_findChanceToShoot(int whichDroid, std::string levelName);

// Should the Droid shoot or not
int ai_notShoot ( int whichDroid, std::string levelName );

// Locate the player position - see if the droid can take the shot or not
int ai_findBulletDest ( int whichDroid, std::string levelName );

// Take the shot
int ai_shootBullet ( int whichDroid, std::string levelName );

// Flag if a droid witnesses the player transferring into another droid
void gam_processWitnessTransfer(std::string levelName);
