#include "hdr/system/sys_physics.h"
#include "hdr/io/io_resourceLevel.h"
#include <bitset>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_physicsCollisions.h>

typedef struct
{
	b2BodyDef    bodyDef;
	b2Body       *body;
	b2EdgeShape  shape;
	b2FixtureDef fixture;
} __physicWall;

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

//----------------------------------------------------------------------------------------------------------------------
//
// Step the world
void sys_stepPhysicsWorld(float stepAmount)
//----------------------------------------------------------------------------------------------------------------------
{
	physicsWorld->Step(stepAmount, velocityIterations, positionIterations);
}

//-------------------------------------------------------------------
//
// Set the player physics position in the world
void sys_setPlayerPhysicsPosition(b2Vec2 newPosition)
//-------------------------------------------------------------------
{
	if (playerDroid.body == nullptr)
	{
		return;
	}

	playerDroid.bodyDef.position.Set(newPosition.x / pixelsPerMeter, newPosition.y / pixelsPerMeter);
}

//-------------------------------------------------------------------
//
// Change the physics shape filter for the player on level change
void sys_changePlayerPhysicsFilter()
//-------------------------------------------------------------------
{

}

//-------------------------------------------------------------------
//
// Setup client player droid physics information
void sys_setupPlayerPhysics()
//-------------------------------------------------------------------
{
	if (playerDroid.body != nullptr)
	{
		return;
	}

	playerDroid.worldPos = gam_getLiftWorldPosition(0, lvl_getCurrentLevelName());

	playerDroid.bodyDef.type = b2_dynamicBody;
	playerDroid.bodyDef.position.Set(playerDroid.worldPos.x / pixelsPerMeter, playerDroid.worldPos.y / pixelsPerMeter);
	playerDroid.bodyDef.angle = 0;
	playerDroid.body          = physicsWorld->CreateBody(&playerDroid.bodyDef);

	playerDroid.shape.m_radius = (float) (SPRITE_SIZE * 0.5f) / pixelsPerMeter;
	playerDroid.shape.m_p.Set(0, 0);

	playerDroid.fixtureDef.shape       = &playerDroid.shape;
	playerDroid.fixtureDef.density     = 1;
	playerDroid.fixtureDef.friction    = 0.3f;
	playerDroid.fixtureDef.restitution = 0.1f;
	playerDroid.body->CreateFixture(&playerDroid.fixtureDef);

	playerDroid.velocity.x = 0.0f;
	playerDroid.velocity.y = 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup Physics engine - run once
bool sys_setupPhysicsEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	// Define the gravity vector.
	b2Vec2 gravity(0.0f, 0.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	physicsWorld = new b2World(gravity);

	physicsStarted = true;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cleanup Physics engine - run once
void sys_freePhysicsEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	delete physicsWorld;

}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw physics walls
void sys_debugPhysicsWalls(std::string whichLevel)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 wallStart, wallFinish;

	for (intptr_t i = 0; i != shipLevel.at(whichLevel).numLineSegments; i++)
	{
		wallStart.x = shipLevel.at(whichLevel).lineSegments[i].start.x;
		wallStart.y = shipLevel.at(whichLevel).lineSegments[i].start.y;

		wallFinish.x = shipLevel.at(whichLevel).lineSegments[i].finish.x;
		wallFinish.y = shipLevel.at(whichLevel).lineSegments[i].finish.y;

		wallStart  = sys_worldToScreen(wallStart, 100);
		wallFinish = sys_worldToScreen(wallFinish, 100);

		al_draw_line(wallStart.x, wallStart.y, wallFinish.x, wallFinish.y, al_map_rgb(155, 100, 255), 2);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the solid walls for this level
void sys_setupSolidWalls(const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2              wallStart, wallFinish;
	std::vector<b2Vec2> wallPositions;
	__physicWall    tempWall;

	if (0 == shipLevel.at(levelName).numLineSegments)
	{
		return;
	}

	for (int i = 0; i != shipLevel.at(levelName).numLineSegments; i++)
	{
		wallStart.x = shipLevel.at(levelName).lineSegments[i].start.x / pixelsPerMeter;
		wallStart.y = shipLevel.at(levelName).lineSegments[i].start.y / pixelsPerMeter;

		wallFinish.x = shipLevel.at(levelName).lineSegments[i].finish.x / pixelsPerMeter;
		wallFinish.y = shipLevel.at(levelName).lineSegments[i].finish.y / pixelsPerMeter;

		tempWall.bodyDef.type = b2_staticBody;
		tempWall.bodyDef.position.Set(0, 0);
		tempWall.body = physicsWorld->CreateBody(&tempWall.bodyDef);

		tempWall.shape.Set(wallStart, wallFinish);
		tempWall.fixture.shape = &tempWall.shape;
		tempWall.body->CreateFixture(&tempWall.fixture);

		wallPositions.push_back(wallStart);
	}

	// TODO: Remove this after game finished and all physics objects destroyed
	if (shipLevel.at(levelName).wallPhysicsCreated)
	{     // Create the physics as we visit the level
		return;
	}

//	solidWallChain.CreateLoop(&wallPositions[0], wallPositions.size());

	shipLevel.at(levelName).wallPhysicsCreated = true;
}

//-------------------------------------------------------------------
//
// Create the physics bodies and shapes for the enemy droids
void sys_setupEnemyPhysics(const std::string levelName)
//-------------------------------------------------------------------
{
	std::bitset<32> droidBitset;        // Use standard bitmasks

	if (!physicsStarted)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Attempting to setup droid physics with no engine."));
		return;
	}

	if (shipLevel.at(levelName).droidPhysicsCreated)
	{
		return;
	}

	for (int i = 0; i != shipLevel.at(levelName).numDroids; i++)
	{
		shipLevel.at(levelName).droid[i].bodyDef.type = b2_dynamicBody;
		shipLevel.at(levelName).droid[i].bodyDef.position.Set(
				shipLevel.at(levelName).droid[i].worldPos.x / pixelsPerMeter,
				shipLevel.at(levelName).droid[i].worldPos.y / pixelsPerMeter);
		shipLevel.at(levelName).droid[i].bodyDef.angle = 0;
		shipLevel.at(levelName).droid[i].body          = physicsWorld->CreateBody(&shipLevel.at(levelName).droid[i].bodyDef);

		shipLevel.at(levelName).droid[i].shape.m_radius = (float) (SPRITE_SIZE * 0.5f) / pixelsPerMeter;
		shipLevel.at(levelName).droid[i].shape.m_p.Set(0, 0);

		shipLevel.at(levelName).droid[i].fixtureDef.shape       = &shipLevel.at(levelName).droid[i].shape;
		shipLevel.at(levelName).droid[i].fixtureDef.density     = 1;
		shipLevel.at(levelName).droid[i].fixtureDef.friction    = 0.3f;
		shipLevel.at(levelName).droid[i].fixtureDef.restitution = 0.1f;
		shipLevel.at(levelName).droid[i].body->CreateFixture(&shipLevel.at(levelName).droid[i].fixtureDef);
	}

	shipLevel.at(levelName).droidPhysicsCreated = true;
}

//-----------------------------------------------------------------------------
//
// Debug physics objects
void sys_debugPhysics(std::string levelName)
//-----------------------------------------------------------------------------
{
	b2Vec2 tempPosition;

	for (int i = 0; i != shipLevel.at(levelName).numDroids; i++)
	{
		//
		// Check body is valid
		//
//			if (cpTrue == cpSpaceContainsBody (space, shipLevel.at (levelName).droid[i].body))
		{
			tempPosition = shipLevel.at(levelName).droid[i].body->GetPosition();
			tempPosition *= pixelsPerMeter;

			tempPosition = sys_worldToScreen(tempPosition, SPRITE_SIZE);


			al_draw_circle(tempPosition.x, tempPosition.y, 12, al_map_rgb(255, 255, 255), 2);
		}
	}

	tempPosition = playerDroid.body->GetPosition();
	tempPosition *= pixelsPerMeter;
	tempPosition = sys_worldToScreen(tempPosition, SPRITE_SIZE);
	al_draw_circle(tempPosition.x, tempPosition.y, 12, al_map_rgb(255, 255, 255), 2);
}

//---------------------------------------------------------------
//
// Update the droids information from physics properties
void sys_updateDroidPosition(const std::string levelName, int whichDroid)
//---------------------------------------------------------------
{
	b2Vec2     tempPosition;
	b2Vec2     maxWorldSize;
	static int frameCount = 0;

	try
	{
		maxWorldSize.x = shipLevel.at(levelName).levelDimensions.x * TILE_SIZE;
		maxWorldSize.y = shipLevel.at(levelName).levelDimensions.y * TILE_SIZE;
	}

	catch (const std::out_of_range &oor)
	{
		printf("updateDroidPosition - Unable to find level [ %s ]\n", levelName.c_str());
		return;
	}
	//
	// Check body is valid
	//
//	if (cpTrue == cpSpaceContainsBody (space, shipLevel.at (levelName).droid[whichDroid].body))
	{
		tempPosition = shipLevel.at(levelName).droid[whichDroid].body->GetPosition();      // Get position in meters
		tempPosition.x *= pixelsPerMeter;           // Change to pixels
		tempPosition.y *= pixelsPerMeter;

		if (0 == whichDroid)
		{
			printf("Position [ %f %f ]\n", tempPosition.x, tempPosition.y);
		}

		if ((tempPosition.x < 0) || (tempPosition.y < 0) || (tempPosition.x > maxWorldSize.x) ||
		    (tempPosition.y > maxWorldSize.y))
		{
			printf("ERROR: Setting invalid worldPos [ %3.3f %3.3f ] from body Droid [ %i ] Level [ %s ] Frame [ %i ]\n", tempPosition.x, tempPosition.y, whichDroid, levelName.c_str(), static_cast<int>(frameCount));
			return;
		}

		shipLevel.at(levelName).droid[whichDroid].previousWorldPos = shipLevel.at(levelName).droid[whichDroid].worldPos;
		shipLevel.at(levelName).droid[whichDroid].worldPos         = tempPosition;

		shipLevel.at(levelName).droid[whichDroid].body->SetLinearVelocity({0, 0});
	}
/*
	else
		{
			printf ("ERROR: Attempting to get position from invalid body - droid [ %i ]\n", whichDroid);
			return;
		}
		*/
}


