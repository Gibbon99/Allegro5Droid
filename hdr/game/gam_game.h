#pragma once

#include "system/sys_main.h"

enum BULLET_TYPES
{
	BULLET_TYPE_NORMAL = 0,
	BULLET_TYPE_SINGLE,
	BULLET_TYPE_DOUBLE,
	BULLET_TYPE_DISRUPTER
};

enum DAMAGE_TYPES
{
	DAMAGE_BULLET = 0,
	DAMAGE_EXPLOSION,
	DAMAGE_COLLISION
};

extern int currentAlertLevel;

// Start a new game
void gam_startNewGame();

