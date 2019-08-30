#pragma once

#include "system/sys_main.h"

enum droidModes
{
	DROID_MODE_NORMAL = 0,
	DROID_MODE_EXPLODING,
	DROID_MODE_DEAD,
	DROID_MODE_TRANSFER
};

//Setup initial values for enemy droids
void gam_initDroidValues ( std::string levelName );

// Render the droids for this level
void gam_renderDroids (std::string levelName, float interpolate);

// Animate the droid
void gam_animateThisLevel ( std::string levelName );

// damageSource can be either a bullet, explosion or collision
void gam_damageToDroid ( int whichLevel, int whichDroid, int damageSource, int sourceDroid );

// process ignore collosions
void gam_processIgnoreCollisions ( std::string whichLevel, int whichDroid );

// Process weapon timings
void gam_droidWeaponCharge ( int whichDroid, std::string levelName );

// Create the lookup table between droid type and sprite name
void gl_setupDroidToSpriteLookup();

// Return the sprite name for a droidType
std::string gl_getSpriteName(int droidType);