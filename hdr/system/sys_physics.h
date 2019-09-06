#pragma once

#include "system/sys_main.h"

#define DEBUG_PHYSICS    1

#define PHYSIC_TYPE_WALL          0x01
#define PHYSIC_TYPE_ENEMY         0x02
#define PHYSIC_TYPE_PLAYER        0x04
#define PHYSIC_TYPE_BULLET        0x08
#define PHYSIC_TYPE_DOOR          0x16
#define PHYSIC_TYPE_PARTICLE      0x32
#define PHYSIC_TYPE_LIFT          0x64

#define BYTE_ZERO           0
#define BYTE_ONE            1
#define BYTE_TWO            2
#define BYTE_THREE          3

typedef struct
{
	int userType;
	int dataValue;
} _userData;

typedef struct _physicObject
{
	b2BodyDef     bodyDef;                      // Used for physics and collisions
	b2CircleShape shape;
	b2FixtureDef  fixtureDef;
	b2Body        *body;
	_userData *userData;
} _physicObject;

extern float pixelsPerMeter;           // Set from startup script
extern float playerMass;              // Set from startup script
extern float playerRadius;            // Set from startup script
extern float playerFriction;          // Set from startup script
extern float playerElastic;           // Set from startup script

extern b2Vec2 shipGravity;
extern float  shipDamping;              // Set from startup script
extern float  collisionSlop;            // Set from startup script
extern float  wallFriction;             // Set from startup script
extern float  wallRadius;               // Set from startup script

extern std::vector<_physicObject> droidPhysics;

class paraDebugDraw : public b2Draw
{
public:
	paraDebugDraw ();

	~paraDebugDraw () override;

	void DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

	void DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

	void DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color) override;

	void DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) override;

	void DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;

	void DrawTransform (const b2Transform &xf) override;

	void DrawPoint (const b2Vec2 &p, float32 size, const b2Color &color) override;
};

extern paraDebugDraw g_paraDebugDraw;

extern bool physicsStarted;

// Setup Physics engine - run once
bool sys_setupPhysicsEngine ();

// Cleanup Physics engine - run once
void sys_freePhysicsEngine ();

// Create the solid walls for this level
void sys_setupSolidWalls (const std::string levelName);

void sys_changePlayerPhysicsFilter ();

// Setup client player droid physics information
void sys_setupPlayerPhysics ();

// Set the player physics position in the world
void sys_setPlayerPhysicsPosition (b2Vec2 newPosition);

// Create the physics bodies and shapes for the enemy droids
void sys_setupEnemyPhysics (const std::string levelName);

// Update the droids information from physics properties
void sys_updateDroidPosition (const std::string levelName, int whichDroid);

// Debug physics objects
void sys_debugPhysics (std::string levelName);

// Draw physics walls
void sys_debugPhysicsWalls (std::string whichLevel);

// Step the world
void sys_stepPhysicsWorld (float stepAmount);

// Return pointer to current physics world
b2World *sys_getPhysicsWorld ();

// Destroy physics as we leave the level
void sys_destroyEnemyPhysics(const std::string levelName);