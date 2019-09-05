#pragma once

#include "system/sys_main.h"

//-----------------------------------------------------------------------------
//
// structure used for doors
//
//-----------------------------------------------------------------------------

typedef struct
{
	int            tileIndex;
	int            height;
	int            width;
	cpVect         topLeft;
	cpVect         topRight;
	cpVect         botLeft;
	cpVect         botRight;
	int            currentFrame;            // which frame are we on
	float          frameDelay;                // speed to animate them at
	float          nextFrame;                // counter for incrementing to next frame
	bool           inUse;
	b2Vec2         worldPosition = {0, 0};
	b2BodyDef      bodyDef;                      // Used for physics and collisions
	b2PolygonShape shape;
	b2FixtureDef   fixtureDef;
	b2Body         *body;
	_userData      *userData;
} _doorTrigger;

//-----------------------------------------------------------------------------
//
// defines used for controlling doors - tile number
//-----------------------------------------------------------------------------
#define DOOR_ACROSS_OPEN_1        15
#define DOOR_ACROSS_OPEN_2        16
#define DOOR_ACROSS_OPENED        NO_PASS_TILE + 10
#define DOOR_ACROSS_CLOSING_1     22
#define DOOR_ACROSS_CLOSING_2     21
#define DOOR_ACROSS_CLOSED        14

#define DOOR_UP_OPEN_1            12
#define DOOR_UP_OPEN_2            13
#define DOOR_UP_OPENED            NO_PASS_TILE + 9
#define DOOR_UP_CLOSING_1         19
#define DOOR_UP_CLOSING_2         18
#define DOOR_UP_CLOSED            11

// Find the doors for this level and prepare a sensor objecty
void gam_doorTriggerSetup(const std::string levelName);

// Handle door sensor trigger
void gam_handleDoorTrigger(int whichDoor, int state);

// Debug door trigger sensors
void gam_debugDoorTriggers();