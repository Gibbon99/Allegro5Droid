#include "hdr/system/sys_physics.h"
#include "hdr/io/io_resourceLevel.h"
#include <bitset>
#include <hdr/game/gam_player.h>
#include <hdr/io/io_logFile.h>

float playerMass;              // Set from startup script

cpSpace *space;
float   playerRadius;            // Set from startup script
float   playerFriction;          // Set from startup script
float   playerElastic;           // Set from startup script

cpVect shipGravity;
float  shipDamping;              // Set from startup script
float  collisionSlop;            // Set from startup script
float  wallFriction;             // Set from startup script
float  wallRadius;               // Set from startup script

bool physicsStarted;

//-------------------------------------------------------------------
//
// Set the player physics position in the world
void sys_setPlayerPhysicsPosition (cpVect newPosition)
//-------------------------------------------------------------------
{
	if (playerDroid.body == nullptr)
		return;

	cpBodySetPosition (playerDroid.body, newPosition);
}

//-------------------------------------------------------------------
//
// Setup client player droid physics information
void sys_setupPlayerPhysics ()
//-------------------------------------------------------------------
{
	cpVect playerOffset;

	playerOffset = {0.0f, 0.0f};

	if (playerDroid.body != nullptr)
		return;

	playerMass   = 10.5f;
	playerRadius = 24.0f;

	playerDroid.body = cpSpaceAddBody (space, cpBodyNew (playerMass, cpMomentForCircle (playerMass, 0.0f, playerRadius, playerOffset)));
	cpBodySetMass (playerDroid.body, playerMass);

	playerDroid.shape = cpSpaceAddShape (space, cpCircleShapeNew (playerDroid.body, playerRadius, playerOffset));
	cpShapeSetFriction (playerDroid.shape, playerFriction);
	cpShapeSetElasticity (playerDroid.shape, playerElastic);
	cpShapeSetCollisionType (playerDroid.shape, PHYSIC_TYPE_PLAYER);
	cpShapeSetUserData (playerDroid.shape, (cpDataPointer) -1);  // Passed into collision routine

	playerDroid.velocity.x = 0.0f;
	playerDroid.velocity.y = 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup Physics engine - run once
bool sys_setupPhysicsEngine ()
//----------------------------------------------------------------------------------------------------------------------
{
	shipGravity = cpv (0, 0);
	space       = cpSpaceNew ();
	cpSpaceSetGravity (space, shipGravity);
	cpSpaceSetDamping (space, shipDamping);
	// Amount of overlap between shapes that is allowed.
	cpSpaceSetCollisionSlop (space, collisionSlop);

//	sys_setupCollisionHandlers();

	physicsStarted = true;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cleanup Physics engine - run once
void sys_freePhysicsEngine ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (nullptr == space)
		{
			return;
		}

	cpSpaceFree (space);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the solid walls for this level
void sys_createSolidWalls (const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect          wallStart, wallFinish;
	_physicObject   tempWall;
	cpShapeFilter   wallShapeFilter;
	std::bitset<32> wallBitset;

	if (0 == shipLevel.at (levelName).numLineSegments)
		{
			return;
		}

	// TODO: Remove this after game finished and all physics objects destroyed
//	if ( shipLevel.at ( levelName ).wallPhysicsCreated )     // Create the physics as we visit the level
//		return;

	shipLevel.at (levelName).solidWalls.clear ();
	shipLevel.at (levelName).solidWalls.reserve (static_cast<unsigned long>(shipLevel.at (levelName).numLineSegments));

	for (intptr_t i = 0; i != shipLevel.at (levelName).numLineSegments; i++)
		{
			wallStart  = shipLevel.at (levelName).lineSegments[i].start;
			wallFinish = shipLevel.at (levelName).lineSegments[i].finish;

			tempWall.body  = cpBodyNewStatic ();
			tempWall.shape = cpSegmentShapeNew (tempWall.body, wallStart, wallFinish, wallRadius);
			shipLevel.at (levelName).solidWalls.push_back (tempWall);
			cpSpaceAddShape (space, shipLevel.at (levelName).solidWalls[i].shape);
			cpShapeSetFriction (shipLevel.at (levelName).solidWalls[i].shape, wallFriction);

			cpShapeSetCollisionType (shipLevel.at (levelName).solidWalls[i].shape, PHYSIC_TYPE_WALL);
			//
			// Setup collision shape filtering bitmasks
			//
			wallBitset.reset ();                                         // Clear the bitset
			wallBitset = shipLevel.at (levelName).deckCategory;       // Each wall on this level is in this category
			wallShapeFilter.categories = static_cast<cpBitmask>(wallBitset.to_ulong ());     // Set the category

			wallBitset.reset ();                                         // Clear the bitset

			wallBitset = shipLevel.at (levelName).deckCategory;       // Collide with everything in this category ( includes other droids on this level )
			wallBitset[PHYSIC_TYPE_PLAYER] = true;                      // and the player
			wallShapeFilter.mask = static_cast<cpBitmask>(wallBitset.to_ulong ());

			wallShapeFilter.group = CP_NO_GROUP;

			cpShapeSetFilter (shipLevel.at (levelName).solidWalls[i].shape, wallShapeFilter);

			cpShapeSetUserData (shipLevel.at (levelName).solidWalls[i].shape, (cpDataPointer) i);
		}

	shipLevel.at (levelName).wallPhysicsCreated = true;
	cpSpaceReindexStatic (space);
}

//-------------------------------------------------------------------
//
// Create the physics bodies and shapes for the enemy droids
void sys_createEnemyPhysics (const std::string levelName)
//-------------------------------------------------------------------
{
	std::bitset<32> droidBitset;        // Use standard bitmasks
	cpShapeFilter   droidShapeFilter;
	int             packedValue;

	if (!physicsStarted)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Attempting to setup droid physics with no engine."));
			return;
		}

	if (shipLevel.at (levelName).droidPhysicsCreated)
		return;

	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
		{

			shipLevel.at (levelName).droid[i].body = cpSpaceAddBody (space, cpBodyNew (shipLevel.at (levelName).droid[i].mass, cpMomentForCircle (shipLevel.at (levelName).droid[i].mass, 0.0f, playerRadius, cpvzero)));

			cpBodySetMass (shipLevel.at (levelName).droid[i].body, shipLevel.at (levelName).droid[i].mass);

			shipLevel.at (levelName).droid[i].shape = cpSpaceAddShape (space, cpCircleShapeNew (shipLevel.at (levelName).droid[i].body, playerRadius, cpvzero));
			cpShapeSetFriction (shipLevel.at (levelName).droid[i].shape, playerFriction);
			cpShapeSetElasticity (shipLevel.at (levelName).droid[i].shape, playerElastic);

			cpShapeSetCollisionType (shipLevel.at (levelName).droid[i].shape, PHYSIC_TYPE_ENEMY);
			packedValue = sys_pack4Bytes (static_cast<char>(lvl_getDeckNumber (levelName)), (char) i, 0, 0);
			cpShapeSetUserData (shipLevel.at (levelName).droid[i].shape, (cpDataPointer) packedValue);    // Passed into collision routine

			cpBodySetPosition (shipLevel.at (levelName).droid[i].body, shipLevel.at (levelName).droid[i].worldPos);
			//
			// Setup the bitmasks for collision filtering
			//

			droidBitset.reset ();                    // Clear it
			droidBitset = shipLevel.at (levelName).deckCategory;  // Set category to this level
			droidShapeFilter.categories = static_cast<cpBitmask>(droidBitset.to_ulong ());    // Set chipmunk cpBitmask

			droidBitset.reset ();                    // Clear it again

			droidBitset = shipLevel.at (levelName).deckCategory;      // will collide with everything on this level ( this category )
			droidBitset[PHYSIC_TYPE_PLAYER] = true;                     // and the player
			droidShapeFilter.mask = static_cast<cpBitmask>(droidBitset.to_ulong ());          // Set chipmunk cpBitmask

			droidShapeFilter.group = CP_NO_GROUP;                       // Doesn't belong to any group

			cpShapeSetFilter (shipLevel.at (levelName).droid[i].shape, droidShapeFilter);   // Set the category and collision mask
		}

	shipLevel.at (levelName).droidPhysicsCreated = true;
}

//-----------------------------------------------------------------------------
//
// Debug physics objects
void sys_debugPhysics(std::string levelName)
//-----------------------------------------------------------------------------
{
	cpVect tempPosition;

	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
		{
			//
			// Check body is valid
			//
//			if (cpTrue == cpSpaceContainsBody (space, shipLevel.at (levelName).droid[i].body))
				{
					tempPosition = cpBodyGetPosition (shipLevel.at (levelName).droid[i].body);

					tempPosition = sys_worldToScreen ( tempPosition, 24 );

					al_draw_circle (tempPosition.x, tempPosition.y, 12, al_map_rgb (255, 255, 255), 2);
				}
		}
}

//---------------------------------------------------------------
//
// Update the droids information from physics properties
void drd_updateDroidPosition (const std::string levelName, int whichDroid)
//---------------------------------------------------------------
{
	cpVect     tempPosition;
	cpVect     maxWorldSize;
	static int frameCount = 0;

	maxWorldSize.x = shipLevel.at (levelName).levelDimensions.x * TILE_SIZE;
	maxWorldSize.y = shipLevel.at (levelName).levelDimensions.y * TILE_SIZE;

	//
	// Check body is valid
	//
	if (cpTrue == cpSpaceContainsBody (space, shipLevel.at (levelName).droid[whichDroid].body))
		{
			tempPosition = cpBodyGetPosition (shipLevel.at (levelName).droid[whichDroid].body);

			if ((tempPosition.x < 0) || (tempPosition.y < 0) || (tempPosition.x > maxWorldSize.x) ||
			    (tempPosition.y > maxWorldSize.y))
				{
			printf ( "ERROR: Setting invalid worldPos [ %3.3f %3.3f ] from body Droid [ %i ] Level [ %s ] Frame [ %i ]\n", tempPosition.x, tempPosition.y, whichDroid, levelName.c_str (), static_cast<int>(frameCount));
					return;
				}

				shipLevel.at(levelName).droid[whichDroid].previousWorldPos = shipLevel.at (levelName).droid[whichDroid].worldPos;
			shipLevel.at (levelName).droid[whichDroid].worldPos = tempPosition;
		}
	else
		{
			printf ("ERROR: Attempting to get position from invalid body - droid [ %i ]\n", whichDroid);
			return;
		}
}


