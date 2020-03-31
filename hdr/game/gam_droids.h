#pragma once

#include "system/sys_main.h"

enum droidModes
{
	DROID_MODE_NORMAL = 0,
	DROID_MODE_EXPLODING,
	DROID_MODE_DEAD,
	DROID_MODE_TRANSFER
};

extern float droidAnimateSpeed;
extern float explodeAnimateSpeed;

//Setup initial values for enemy droids
void gam_initDroidValues ( std::string levelName );

// Render the droids for this level
void gam_renderDroids (std::string levelName, float interpolate);

// Animate the droid
void gam_animateThisLevel ( std::string levelName );

// damageSource can be either a bullet, explosion or collision
void gam_damageToDroid (int targetDroid, int damageSource, int sourceDroid, int eventSource);

// process ignore collosions
void gam_processIgnoreCollisions ( const std::string& whichLevel, int whichDroid );

// Process weapon timings
void gam_droidWeaponCharge ( const std::string& levelName, int whichDroid, float tickTime );

// Create the lookup table between droid type and sprite name
void gam_setupDroidToSpriteLookup();

// Return the sprite name for a droidType
std::string gam_getSpriteName(int droidType);

// This removes the droid from the current game - sets status to dead, and removes physics body
void gam_removeDroid(int whichDroid);

// Droid health has changed; either damage or healing - adjust the animation
void gam_adjustHealthAnimation(const std::string& tempCurrentLevel, int targetDroid);