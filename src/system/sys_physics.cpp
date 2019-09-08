#include "hdr/system/sys_physics.h"
#include "hdr/io/io_resourceLevel.h"
#include <bitset>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_physicsCollisions.h>
#include <hdr/game/gam_physicActions.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_bullet.h>
#include <hdr/game/gam_lifts.h>

typedef struct
{
	b2BodyDef    bodyDef;
	b2Body       *body = nullptr;
	b2EdgeShape  shape;
	b2FixtureDef fixture;
	_userData    *userData;
} __physicWall;

std::vector<__physicWall>  solidWalls;
std::vector<_physicObject> droidPhysics;

float pixelsPerMeter;           // Set from startup script
float playerMass;                // Set from startup script

float playerRadius;            // Set from startup script
float playerFriction;          // Set from startup script
float playerElastic;           // Set from startup script

b2Vec2 shipGravity;
float  shipDamping;              // Set from startup script
float  collisionSlop;            // Set from startup script
float  wallFriction;             // Set from startup script
float  wallRadius;               // Set from startup script

bool physicsStarted;

b2World *physicsWorld;
int32   velocityIterations = 8;   //how strongly to correct velocity
int32   positionIterations = 3;   //how strongly to correct position

paraDebugDraw g_paraDebugDraw;


paraDebugDraw::paraDebugDraw ()
{
}

paraDebugDraw::~paraDebugDraw ()
{
}

void paraDebugDraw::DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{

}

void paraDebugDraw::DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
	std::vector<ALLEGRO_VERTEX> vertexes;
	ALLEGRO_VERTEX              tempVertex;
	b2Vec2                      tempVert;

	for (int i = 0; i != vertexCount; i++)
	{
		tempVertex.x = vertices[i].x;
		tempVertex.y = vertices[i].y;
		tempVertex.z = 0;

		tempVertex.x *= pixelsPerMeter;
		tempVertex.y *= pixelsPerMeter;

		tempVert.x = tempVertex.x;
		tempVert.y = tempVertex.y;

		tempVert = sys_worldToScreen (tempVert, 100);

		tempVertex.x = tempVert.x;
		tempVertex.y = tempVert.y;

		tempVertex.color = al_map_rgba (155, 155, 0, 5);
		vertexes.push_back (tempVertex);
	}

	al_draw_prim (&vertexes[0], nullptr, nullptr, 0, vertexCount, ALLEGRO_PRIM_TRIANGLE_FAN);

}


void paraDebugDraw::DrawTransform (const b2Transform &xf)
{
}

void paraDebugDraw::DrawPoint (const b2Vec2 &p, float32 size, const b2Color &color)
{
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a debug line segment
void paraDebugDraw::DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 startLine;
	b2Vec2 endLine;

	startLine = p1;
	endLine   = p2;

	startLine.x *= pixelsPerMeter;
	startLine.y *= pixelsPerMeter;

	endLine.x *= pixelsPerMeter;
	endLine.y *= pixelsPerMeter;

	startLine = sys_worldToScreen (startLine, 100);
	endLine   = sys_worldToScreen (endLine, 100);

	al_draw_line (startLine.x, startLine.y, endLine.x, endLine.y, al_map_rgba (50, 0, 155, 30), 2);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a solid debug circle
void paraDebugDraw::DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempPosition;

	tempPosition = center;
	tempPosition *= pixelsPerMeter;
	radius *= pixelsPerMeter;

	tempPosition = sys_worldToScreen (tempPosition, radius);

	al_draw_filled_circle (tempPosition.x, tempPosition.y, radius, al_map_rgba (0, 150, 0, 20));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Debug function to show physics circle
void paraDebugDraw::DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return pointer to current physics world
b2World *sys_getPhysicsWorld ()
//----------------------------------------------------------------------------------------------------------------------
{
	return physicsWorld;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Step the world
void sys_stepPhysicsWorld (float stepAmount)
//----------------------------------------------------------------------------------------------------------------------
{
	physicsWorld->Step (stepAmount, velocityIterations, positionIterations);
}

//-------------------------------------------------------------------
//
// Set the player physics position in the world
void sys_setPlayerPhysicsPosition (b2Vec2 newPosition)
//-------------------------------------------------------------------
{
	if (playerDroid.body == nullptr)
	{
		return;
	}
	newPosition.x /= pixelsPerMeter;
	newPosition.y /= pixelsPerMeter;
	playerDroid.body->SetTransform (newPosition, playerDroid.body->GetAngle ());
}

//-------------------------------------------------------------------
//
// Change the physics shape filter for the player on level change
void sys_changePlayerPhysicsFilter ()
//-------------------------------------------------------------------
{

}

//-------------------------------------------------------------------
//
// Setup client player droid physics information
void sys_setupPlayerPhysics ()
//-------------------------------------------------------------------
{
	if (playerDroid.body != nullptr)
	{
		return;
	}

//	playerDroid.worldPos = gam_getLiftWorldPosition(0, lvl_getCurrentLevelName());

	playerDroid.bodyDef.type = b2_dynamicBody;
	playerDroid.bodyDef.position.Set (playerDroid.worldPos.x / pixelsPerMeter, playerDroid.worldPos.y / pixelsPerMeter);
	playerDroid.bodyDef.angle = 0;
	playerDroid.body          = physicsWorld->CreateBody (&playerDroid.bodyDef);

	playerDroid.userData            = new _userData;
	playerDroid.userData->userType  = PHYSIC_TYPE_PLAYER;
	playerDroid.userData->dataValue = -1;
	playerDroid.body->SetUserData (playerDroid.userData);

	playerDroid.shape.m_radius = (float) (SPRITE_SIZE * 0.5f) / pixelsPerMeter;
	playerDroid.shape.m_p.Set (0, 0);

	playerDroid.fixtureDef.shape       = &playerDroid.shape;
	playerDroid.fixtureDef.density     = 1;
	playerDroid.fixtureDef.friction    = 0.3f;
	playerDroid.fixtureDef.restitution = 1.0f;
	playerDroid.body->CreateFixture (&playerDroid.fixtureDef);

	playerDroid.velocity.x = 0.0f;
	playerDroid.velocity.y = 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup Physics engine - run once
bool sys_setupPhysicsEngine ()
//----------------------------------------------------------------------------------------------------------------------
{
	// Define the gravity vector.
	b2Vec2 gravity (0.0f, 0.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	physicsWorld = new b2World (gravity);

	physicsWorld->SetContactListener (&myContactListenerInstance);
	g_paraDebugDraw.SetFlags (b2Draw::e_shapeBit);  // Draw shapes
	physicsWorld->SetDebugDraw (&g_paraDebugDraw);

	physicsStarted = true;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cleanup Physics engine - run once
void sys_freePhysicsEngine ()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_clearLifts();
	sys_clearSolidWalls();
	sys_clearDroids();
	bul_clearAllBullets();
	gam_clearAllDoors();
	delete physicsWorld;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Free memory for solid walls and free bodies
void sys_clearSolidWalls()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &wallItr : solidWalls)
	{
		if (wallItr.userData != nullptr)
			delete (wallItr.userData);
		if (wallItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (wallItr.body);
	}
	solidWalls.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the solid walls for this level
void sys_setupSolidWalls (const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2              wallStart, wallFinish;
	std::vector<b2Vec2> wallPositions;
	__physicWall        tempWall;

	if (0 == shipLevel.at (levelName).numLineSegments)
	{
		return;
	}


	if (!wallPositions.empty ())
		wallPositions.clear ();

	if (!solidWalls.empty ())
	{
		sys_clearSolidWalls();
	}

	for (int i = 0; i != shipLevel.at (levelName).numLineSegments; i++)
	{
		wallStart.x = shipLevel.at (levelName).lineSegments[i].start.x / pixelsPerMeter;
		wallStart.y = shipLevel.at (levelName).lineSegments[i].start.y / pixelsPerMeter;

		wallFinish.x = shipLevel.at (levelName).lineSegments[i].finish.x / pixelsPerMeter;
		wallFinish.y = shipLevel.at (levelName).lineSegments[i].finish.y / pixelsPerMeter;

		tempWall.bodyDef.type = b2_staticBody;
		tempWall.bodyDef.position.Set (0, 0);
		tempWall.body = physicsWorld->CreateBody (&tempWall.bodyDef);

		tempWall.userData            = new _userData;
		tempWall.userData->userType  = PHYSIC_TYPE_WALL;
		tempWall.userData->dataValue = i;
		tempWall.body->SetUserData (tempWall.userData);

		tempWall.shape.Set (wallStart, wallFinish);
		tempWall.fixture.shape = &tempWall.shape;
		tempWall.body->CreateFixture (&tempWall.fixture);

		solidWalls.push_back (tempWall);
		wallPositions.push_back (wallStart);
	}

	// TODO: Remove this after game finished and all physics objects destroyed
	if (shipLevel.at (levelName).wallPhysicsCreated)
	{     // Create the physics as we visit the level
		return;
	}

//	solidWallChain.CreateLoop(&wallPositions[0], wallPositions.size());

	shipLevel.at (levelName).wallPhysicsCreated = true;
}

//-------------------------------------------------------------------
//
// Free memory and destroy bodies for droids
void sys_clearDroids()
//-------------------------------------------------------------------
{
	for (auto &droidItr : droidPhysics)
	{
		if (droidItr.userData != nullptr)
			delete (droidItr.userData);

		if (droidItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (droidItr.body);
	}
	droidPhysics.clear();
}
//-------------------------------------------------------------------
//
// Create the physics bodies and shapes for the enemy droids
void sys_setupEnemyPhysics (const std::string levelName)
//-------------------------------------------------------------------
{
	_physicObject tempDroid;

	if (!physicsStarted)
	{
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Attempting to setup droid physics with no engine."));
		return;
	}

	if (!droidPhysics.empty())
	{
		sys_clearDroids();
	}

	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
	{
		if (shipLevel.at(levelName).droid[i].currentMode == DROID_MODE_NORMAL)
		{
			tempDroid.index = i;
			tempDroid.bodyDef.type = b2_dynamicBody;
			tempDroid.bodyDef.position.Set (shipLevel.at (levelName).droid[i].worldPos.x / pixelsPerMeter, shipLevel.at (levelName).droid[i].worldPos.y / pixelsPerMeter);
			tempDroid.bodyDef.angle = 0;
			tempDroid.body          = physicsWorld->CreateBody (&tempDroid.bodyDef);

			tempDroid.userData            = new _userData;
			tempDroid.userData->userType  = PHYSIC_TYPE_ENEMY;
			tempDroid.userData->dataValue = i;
			tempDroid.body->SetUserData (tempDroid.userData);

			tempDroid.shape.m_radius = (float) (SPRITE_SIZE * 0.5f) / pixelsPerMeter;
			tempDroid.shape.m_p.Set (0, 0);

			tempDroid.fixtureDef.shape       = &tempDroid.shape;
			tempDroid.fixtureDef.density     = 1;
			tempDroid.fixtureDef.friction    = 0.3f;
			tempDroid.fixtureDef.restitution = 1.0f;
			tempDroid.body->CreateFixture (&tempDroid.fixtureDef);

			droidPhysics.push_back (tempDroid);
		}
	}
}

//---------------------------------------------------------------
//
// Update the droids information from physics properties
void sys_updateDroidPosition (const std::string levelName, int whichDroid)
//---------------------------------------------------------------
{
	b2Vec2     tempPosition;
	b2Vec2     maxWorldSize;
	static int frameCount = 0;

	try
	{
		maxWorldSize.x = shipLevel.at (levelName).levelDimensions.x * TILE_SIZE;
		maxWorldSize.y = shipLevel.at (levelName).levelDimensions.y * TILE_SIZE;
	}

	catch (const std::out_of_range &oor)
	{
		printf ("updateDroidPosition - Unable to find level [ %s ]\n", levelName.c_str ());
		return;
	}
	//
	// Check body is valid
	//
	if (nullptr != shipLevel.at (levelName).droid[whichDroid].body)
	{
		tempPosition = droidPhysics[whichDroid].body->GetPosition ();      // Get position in meters
		tempPosition.x *= pixelsPerMeter;           // Change to pixels
		tempPosition.y *= pixelsPerMeter;

		if ((tempPosition.x < 0) || (tempPosition.y < 0) || (tempPosition.x > maxWorldSize.x) || (tempPosition.y > maxWorldSize.y))
		{
			printf ("ERROR: Setting invalid worldPos [ %3.3f %3.3f ] from body Droid [ %i ] Level [ %s ] Frame [ %i ]\n", tempPosition.x, tempPosition.y, whichDroid, levelName.c_str (), static_cast<int>(frameCount));
			return;
		}

		shipLevel.at (levelName).droid[whichDroid].previousWorldPos = shipLevel.at (levelName).droid[whichDroid].worldPos;
		shipLevel.at (levelName).droid[whichDroid].worldPos         = tempPosition;

		droidPhysics[whichDroid].body->SetLinearVelocity ({0, 0});
	}
	else
	{
		printf ("ERROR: Attempting to get updateDroidPosition from invalid body - droid [ %i ]\n", whichDroid);
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------
//
// Process a frame of physics - called from Fixed Update
void sys_processPhysics (double tickTime)
//----------------------------------------------------------------------------------------------------------------
{
	bul_applyPhysics (tickTime);
	playerDroid.body->ApplyForce (playerDroid.velocity, playerDroid.body->GetWorldCenter (), true); // TODO do this way - bulletItr.velocity.operator*= (bulletMoveSpeed);
	sys_stepPhysicsWorld (1.0f / TICKS_PER_SECOND);
	gam_processPhysicActions ();
	playerDroid.body->SetLinearVelocity ({0, 0});
}