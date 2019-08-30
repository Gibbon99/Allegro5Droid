#pragma once

#include "system/sys_main.h"

#define DEBUG_PHYSICS    1

#define PHYSIC_TYPE_WALL          0x01
#define PHYSIC_TYPE_ENEMY         0x02
#define PHYSIC_TYPE_PLAYER        0x04
#define PHYSIC_TYPE_BULLET        0x08
#define PHYSIC_TYPE_DOOR          0x16
#define PHYSIC_TYPE_PARTICLE      0x32

#define BYTE_ZERO           0
#define BYTE_ONE            1
#define BYTE_TWO            2
#define BYTE_THREE          3

typedef struct _physicObject {
	cpBody  *body;
	cpShape *shape;
} _physicObject;

extern cpSpace *space;
extern float   playerMass;              // Set from startup script
extern float   playerRadius;            // Set from startup script
extern float   playerFriction;          // Set from startup script
extern float   playerElastic;           // Set from startup script

extern cpVect shipGravity;
extern float  shipDamping;              // Set from startup script
extern float  collisionSlop;            // Set from startup script
extern float  wallFriction;             // Set from startup script
extern float  wallRadius;               // Set from startup script

extern bool physicsStarted;

// Setup Physics engine - run once
bool sys_setupPhysicsEngine();

// Cleanup Physics engine - run once
void sys_freePhysicsEngine();

// Create the solid walls for this level
void sys_createSolidWalls(const std::string levelName);

// Setup client player droid physics information
void sys_setupPlayerPhysics();

// Set the player physics position in the world
void sys_setPlayerPhysicsPosition (cpVect newPosition );

// Create the physics bodies and shapes for the enemy droids
void sys_createEnemyPhysics (const std::string levelName);

// Update the droids information from physics properties
void drd_updateDroidPosition (const std::string levelName, int whichDroid);

// Debug physics objects
void sys_debugPhysics(std::string levelName);