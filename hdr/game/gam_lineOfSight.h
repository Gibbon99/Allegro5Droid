#pragma once

#include "system/sys_main.h"

enum VISIBLE_STATES
{
	VISIBLE_STATE_GO_VISIBLE = 0,
	VISIBLE_STATE_GO_NOT_VISIBLE
};

// Test each droid against the player droid to see if the ray intersects a wall
void gam_doLineOfSight ();

