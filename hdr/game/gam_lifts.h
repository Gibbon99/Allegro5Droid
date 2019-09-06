#pragma once

#include "system/sys_main.h"
#include "system/sys_physics.h"

typedef struct
{
	float          width;
	float          height;
	b2Vec2         worldPosition = {0, 0};
	b2BodyDef      bodyDef;                      // Used for physics and collisions
	b2PolygonShape shape;
	b2FixtureDef   fixtureDef;
	b2Body         *body;
	_userData      *userData;
} __liftSensor;

extern std::vector<__liftSensor> lifts;

// Setup lifts
void gam_setupLifts ();

// Move the lift position
void gam_moveLift(int direction);

// Get the tunnel being used by the lift the player sprite is over
void gam_getTunnelToUse ();

// Get the positions of lifts
void gam_findLiftPositions ( const std::string &levelName );