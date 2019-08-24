#include "hdr/system/sys_physics.h"
#include "hdr/io/io_resourceLevel.h"
#include <bitset>
#include <hdr/game/gam_player.h>

float   playerMass;              // Set from startup script

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
void sys_setClientPlayerPhysicsPosition ( cpVect newPosition )
//-------------------------------------------------------------------
{
	if ( playerPhysicsObjectClient.body == nullptr )
		return;

	cpBodySetPosition ( playerPhysicsObjectClient.body, newPosition );
}

//-------------------------------------------------------------------
//
// Setup client player droid physics information
void sys_setupClientPlayerPhysics()
//-------------------------------------------------------------------
{
	cpVect	playerOffset;

	playerOffset = {0.0f, 0.0f};

	if ( playerPhysicsObjectClient.body != nullptr )
		return;

	playerMass = 10.5f;
	playerRadius = 24.0f;

	playerPhysicsObjectClient.body = cpSpaceAddBody ( space, cpBodyNew ( playerMass,  cpMomentForCircle ( playerMass, 0.0f, playerRadius, playerOffset ) ) );
	cpBodySetMass ( playerPhysicsObjectClient.body, playerMass );

	playerPhysicsObjectClient.shape = cpSpaceAddShape ( space, cpCircleShapeNew ( playerPhysicsObjectClient.body, playerRadius, playerOffset ) );
	cpShapeSetFriction ( playerPhysicsObjectClient.shape, playerFriction );
	cpShapeSetElasticity ( playerPhysicsObjectClient.shape, playerElastic );
	cpShapeSetCollisionType ( playerPhysicsObjectClient.shape, PHYSIC_TYPE_PLAYER );
	cpShapeSetUserData ( playerPhysicsObjectClient.shape, (cpDataPointer)-1 );	// Passed into collision routine

	playerVelocity.x = 0.0f;
	playerVelocity.y = 0.0f;
}


//----------------------------------------------------------------------------------------------------------------------
//
// Setup Physics engine - run once
bool sys_setupPhysicsEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	shipGravity = cpv(0, 0);
	space       = cpSpaceNew();
	cpSpaceSetGravity(space, shipGravity);
	cpSpaceSetDamping(space, shipDamping);
	// Amount of overlap between shapes that is allowed.
	cpSpaceSetCollisionSlop(space, collisionSlop);

//	sys_setupCollisionHandlers();

	physicsStarted = true;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cleanup Physics engine - run once
void sys_freePhysicsEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	if (nullptr == space)
		{
			return;
		}

	cpSpaceFree(space);
}


//----------------------------------------------------------------------------------------------------------------------
//
// Create the solid walls for this level
void sys_createSolidWalls(const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect          wallStart, wallFinish;
	_physicObject   tempWall;
	cpShapeFilter   wallShapeFilter;
	std::bitset<32> wallBitset;

	if (0 == shipLevel.at(levelName).numLineSegments)
		{
			return;
		}

	// TODO: Remove this after game finished and all physics objects destroyed
//	if ( shipLevel.at ( levelName ).wallPhysicsCreated )     // Create the physics as we visit the level
//		return;

	shipLevel.at(levelName).solidWalls.clear();
	shipLevel.at(levelName).solidWalls.reserve(static_cast<unsigned long>(shipLevel.at(levelName).numLineSegments));

	for (intptr_t i = 0; i != shipLevel.at(levelName).numLineSegments; i++)
		{
			wallStart  = shipLevel.at(levelName).lineSegments[i].start;
			wallFinish = shipLevel.at(levelName).lineSegments[i].finish;

			tempWall.body  = cpBodyNewStatic();
			tempWall.shape = cpSegmentShapeNew(tempWall.body, wallStart, wallFinish, wallRadius);
			shipLevel.at(levelName).solidWalls.push_back(tempWall);
			cpSpaceAddShape(space, shipLevel.at(levelName).solidWalls[i].shape);
			cpShapeSetFriction(shipLevel.at(levelName).solidWalls[i].shape, wallFriction);

			cpShapeSetCollisionType(shipLevel.at(levelName).solidWalls[i].shape, PHYSIC_TYPE_WALL);
			//
			// Setup collision shape filtering bitmasks
			//
			wallBitset.reset();                                         // Clear the bitset
			wallBitset = shipLevel.at(levelName).deckCategory;       // Each wall on this level is in this category
			wallShapeFilter.categories = static_cast<cpBitmask>(wallBitset.to_ulong());     // Set the category

			wallBitset.reset();                                         // Clear the bitset

			wallBitset = shipLevel.at(levelName).deckCategory;       // Collide with everything in this category ( includes other droids on this level )
			wallBitset[PHYSIC_TYPE_PLAYER] = true;                      // and the player
			wallShapeFilter.mask = static_cast<cpBitmask>(wallBitset.to_ulong());

			wallShapeFilter.group = CP_NO_GROUP;

			cpShapeSetFilter(shipLevel.at(levelName).solidWalls[i].shape, wallShapeFilter);

			cpShapeSetUserData(shipLevel.at(levelName).solidWalls[i].shape, (cpDataPointer) i);
		}

	shipLevel.at(levelName).wallPhysicsCreated = true;
	cpSpaceReindexStatic(space);
}
