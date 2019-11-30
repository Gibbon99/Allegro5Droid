#pragma once

#include "system/sys_main.h"

enum BULLET_TYPES {
	BULLET_TYPE_NORMAL = 0,
	BULLET_TYPE_SINGLE,
	BULLET_TYPE_DOUBLE,
	BULLET_TYPE_DISRUPTER
};

enum DAMAGE_TYPES {
	DAMAGE_BULLET = 0,
	DAMAGE_EXPLOSION,
	DAMAGE_COLLISION
};

extern int currentAlertLevel;
extern int yellowAlertLevel;
extern int redAlertLevel;
extern std::string currentAlertLevelSoundName;

extern float scoreDelay;
// Start a new game
void gam_startNewGame ();

// Process score and set current Alert level
void gam_processScore (float tickTime);

// Add passed in value to currentScore
void gam_addToScore (int scoreValue);

// Return the current score as a string for displaying on the HUD
std::string gam_getScore();

// Power down a level - all droids are dead
void gam_powerDownLevel(std::string whichLevel);