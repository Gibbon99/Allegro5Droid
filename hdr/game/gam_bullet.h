#pragma once

#include "system/sys_main.h"

typedef struct
{
	float          angle;
	b2BodyDef      bodyDef;                      // Used for physics and collisions
	b2CircleShape  shape;
	b2PolygonShape boxShape;
	b2FixtureDef   fixtureDef;
	b2Body         *body     = nullptr;
	_userData      *userData = nullptr;
	b2Vec2         worldPos;
	b2Vec2         destPos;
	b2Vec2         previousWorldPos;
	b2Vec2         middlePosition;             // TODO set middle position for droids
	b2Vec2         velocity;                   // Current speed
	int            currentFrame;
	float          frameAnimCounter;
	int            sourceIndex;
	int            type;
} __bulletObject;

extern std::vector<__bulletObject> bullets;
extern float                       bulletAnimSpeed;          // From script
extern float                       bulletMoveSpeed;          // From script
extern float                       bulletDensity;            // From script

// Create a new bullet - pass in the source of the bullet
void bul_createNewBullet (int bulletSourceIndex);

// Apply physics to bullets
void bul_applyPhysics (double tickTime);

// Render the bullets
void bul_renderBullets ();

// Remove a bullet
void bul_removeBullet (int whichBullet);

// Init bullets for level change
void bul_initBullets ();

// Return a string with the current bullet sprite key according to the sprite type
std::string bul_getBulletName(int droidType);

// Clear memory for bullets
void bul_clearAllBullets();