#pragma once

#include "system/sys_main.h"

typedef struct
{
	double        angle;
	b2BodyDef     bodyDef;                      // Used for physics and collisions
	b2CircleShape shape;
	b2FixtureDef  fixtureDef;
	b2Body        *body     = nullptr;
	_userData     *userData = nullptr;
	b2Vec2        worldPos;
	b2Vec2        destPos;
	b2Vec2        previousWorldPos;
	b2Vec2        middlePosition;             // TODO set middle position for droids
	b2Vec2        velocity;                   // Current speed
} __bulletObject;

extern std::vector<__bulletObject> bullets;

// Create a new bullet - pass in the source of the bullet
void bul_createNewBullet (int bulletSourceIndex);

// Apply physics to bullets
void bul_applyPhysics();

// Render the bullets
void bul_renderBullets();

// Remove a bullet
void bul_removeBullet(int whichBullet);

// Init bullets for level change
void bul_initBullets();