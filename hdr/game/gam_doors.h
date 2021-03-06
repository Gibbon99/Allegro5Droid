#pragma once

#include "system/sys_main.h"

enum doorBulletDirection
{
	DIRECTION_UP = 0,
	DIRECTION_ACROSS
};

//-----------------------------------------------------------------------------
//
// structure used for doors
//
//-----------------------------------------------------------------------------

typedef struct
{
	int            tileIndex;
	float          height;
	float          width;
	int            currentFrame;            // which frame are we on
	float          frameDelay;                // speed to animate them at
	int            numberUsing;
	int            direction;

	bool inUse;
	b2Vec2         topLeft;
	b2Vec3         topRight;
	b2Vec2         botLeft;
	b2Vec2         botRight;

	b2Vec2         worldPosition  = {0, 0};
	b2Vec2         renderPosition = {0, 0};
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

extern float                     doorAnimSpeed;
extern std::vector<_doorTrigger> doorBulletSensor;
extern std::vector<_doorTrigger> doorTriggers;

// Find the doors for this level and prepare a sensor objecty
void gam_doorTriggerSetup (const std::string levelName);

// Render current door frames onto map
void gam_renderDoorFrames ();

// Clear out memory for door triggers
void gam_clearAllDoors ();

// Change the collision filters for a door
void gam_changeDoorFilters (int doorState, int whichDoor);

// Process all the doors that are currently inUse
void gam_doorProcessActions();

// Check door trigger areas against sprite positions
void gam_doorCheckTriggerAreas();

// Debug door states and current frame
void gam_debugDoors();