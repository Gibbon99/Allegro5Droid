#include <hdr/game/gam_droids.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_game.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_player.h>
#include "hdr/game/gam_physicsCollisions.h"

cpCollisionHandler *handlerEnemyEnemy;
cpCollisionHandler *handlerEnemyPlayer;

//-------------------------------------------------------------------
//
// Post-Step callback for hitting droid - needed in post-step so when its
// destroyed the shape and body are removed from the world
static void handleDamageDroidCollision ( cpSpace *space, cpShape *shape, int *passedInValues )
//-------------------------------------------------------------------
{
	unsigned char values[4];

	int valuesPassed = 0; // = new int ();

	// deference pointer to data
	valuesPassed = *(static_cast<int*>(passedInValues));

	values[0] = static_cast<unsigned char>((valuesPassed >> 24) & 0xff);        // whichlevel
	values[1] = static_cast<unsigned char>((valuesPassed >> 16) & 0xff);        // which enemy droid index
	values[2] = static_cast<unsigned char>((valuesPassed >> 8) & 0xff);         // Damage type
	values[3] = static_cast<unsigned char>(valuesPassed & 0xff);                // sourceDroid

	printf("Inside damage [ %i %i %i %i ]\n", values[0], values[1], values[2], values[3]);


	if ( 127 == values[3] )   // Player is the source
		gam_damageToDroid ( values[0], values[1], values[2], -1 );
	else
		gam_damageToDroid ( values[0], values[1], values[2], values[3] );

//	delete valuesPassed;
}


//-------------------------------------------------------------------
//
// Check before collision needs to be handled if it should continue
bool handleCollisionDroidCheck ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = PLAYER if userData == -1
	//
	//  Or A = ENEMY and B = ENEMY
	cpShape             *a, *b;
	cpDataPointer       dataPointer_A,          dataPointer_B;
	unsigned char       valuesPassedDroid_A[4], valuesPassedDroid_B[4];

	cpArbiterGetShapes ( arb, &a, &b );

	dataPointer_A = cpShapeGetUserData ( a );
	dataPointer_B = cpShapeGetUserData ( b );

	sys_getPackedBytes ( static_cast<int>((intptr_t) dataPointer_A), valuesPassedDroid_A );
	sys_getPackedBytes ( static_cast<int>((intptr_t) dataPointer_B), valuesPassedDroid_B );

	if ( shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_A[BYTE_ZERO] )).droid[valuesPassedDroid_A[BYTE_ONE]].ignoreCollisions ||
	     shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_B[BYTE_ZERO] )).droid[valuesPassedDroid_B[BYTE_ONE]].ignoreCollisions )
		return cpFalse;

	return cpTrue;
}

//-------------------------------------------------------------------
//
// Handle ENEMY hitting ENEMY droid or PLAYER to ENEMY
void handleCollisionDroidToDroid ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = PLAYER if userData == -1
	//
	//  Or A = ENEMY and B = ENEMY
	cpShape             *a, *b;
	cpDataPointer       dataPointer_A,          dataPointer_B;
	unsigned char       valuesPassedDroid_A[4], valuesPassedDroid_B[4];

	cpArbiterGetShapes ( arb, &a, &b );

	dataPointer_A = cpShapeGetUserData ( a );
	dataPointer_B = cpShapeGetUserData ( b );

	sys_getPackedBytes ( static_cast<int>((intptr_t) dataPointer_A), valuesPassedDroid_A );
	sys_getPackedBytes ( static_cast<int>((intptr_t) dataPointer_B), valuesPassedDroid_B );

	int *passValue = new int ();        // Memory leak?? using delete passValue causes bad values to be passed
	int packedValue = 0;

	if ( 1 == valuesPassedDroid_B[BYTE_TWO] )    // Is B the player
	{
		//
		// Process player vs enemy collision
		if ( !shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_A[BYTE_ZERO] )).droid[valuesPassedDroid_A[BYTE_ONE]].currentMode == DROID_MODE_EXPLODING )
		{
			packedValue = sys_pack4Bytes (valuesPassedDroid_A[BYTE_ZERO], valuesPassedDroid_A[BYTE_ONE], DAMAGE_COLLISION, 127 );
			*passValue = packedValue;

			cpSpaceAddPostStepCallback ( space, (cpPostStepFunc) handleDamageDroidCollision, a, passValue );

// TODO fix up			evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_COLLIDE_1, 0, 0, glm::vec2 (), glm::vec2 (), "" );
		}
		else    // Player collided with exploding sprite - take damage
		{
// TODO			if ( valuesPassedDroid_A[BYTE_ONE] != playerDroid.droidTransferedIntoIndex )
			// Ignore explosion if colliding with recently transferred droid
			{
				packedValue = sys_pack4Bytes (valuesPassedDroid_A[BYTE_ZERO], DAMAGE_EXPLOSION, 0, 0 );
				*passValue = packedValue;

				cpSpaceAddPostStepCallback ( space, (cpPostStepFunc) handleDamageDroidCollision, a, passValue );
//				evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );
			}
		}
	}
	else
	{
		//
		// Droid collided with another droid
		shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_A[BYTE_ZERO] )).droid[valuesPassedDroid_A[BYTE_ONE]].collisionCount++;
		shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_B[BYTE_ZERO] )).droid[valuesPassedDroid_B[BYTE_ONE]].collisionCount++;

		shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_A[BYTE_ZERO] )).droid[valuesPassedDroid_A[BYTE_ONE]].hasCollided = true;
		shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_B[BYTE_ZERO] )).droid[valuesPassedDroid_B[BYTE_ONE]].hasCollided = true;

		shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_A[BYTE_ZERO] )).droid[valuesPassedDroid_A[BYTE_ONE]].collidedWith = valuesPassedDroid_B[BYTE_ONE];
		shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_B[BYTE_ZERO] )).droid[valuesPassedDroid_B[BYTE_ONE]].collidedWith = valuesPassedDroid_A[BYTE_ONE];

		if ( shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_A[BYTE_ZERO] )).droid[valuesPassedDroid_A[BYTE_ONE]].currentMode == DROID_MODE_EXPLODING )
		{
			gam_damageToDroid ( valuesPassedDroid_B[BYTE_ZERO], valuesPassedDroid_B[BYTE_ONE], DAMAGE_EXPLOSION, valuesPassedDroid_A[BYTE_ONE] );
// TODO fix up						evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );
		}
		else
		{
			gam_damageToDroid ( valuesPassedDroid_B[BYTE_ZERO], valuesPassedDroid_B[BYTE_ONE], DAMAGE_COLLISION, valuesPassedDroid_A[BYTE_ONE] );
// TODO fix up						evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_COLLIDE_1, 0, 0, glm::vec2 (), glm::vec2 (), "" );
		}

		if ( shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_B[BYTE_ZERO] )).droid[valuesPassedDroid_B[BYTE_ONE]].currentMode == DROID_MODE_EXPLODING )
		{
			gam_damageToDroid ( valuesPassedDroid_A[BYTE_ZERO], valuesPassedDroid_A[BYTE_ONE], DAMAGE_EXPLOSION, valuesPassedDroid_B[BYTE_ONE] );
// TODO fix up						evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_DAMAGE, 0, 0, glm::vec2 (), glm::vec2 (), "" );
		}
		else
		{
			gam_damageToDroid ( valuesPassedDroid_A[BYTE_ZERO], valuesPassedDroid_A[BYTE_ONE], DAMAGE_COLLISION, valuesPassedDroid_B[BYTE_ONE] );
// TODO fix up						evt_sendEvent ( USER_EVENT_AUDIO, AUDIO_PLAY_SAMPLE, SND_COLLIDE_1, 0, 0, glm::vec2 (), glm::vec2 (), "" );
		}
	}

//	delete passValue;
}


//-------------------------------------------------------------------
//
// Collision between PLAYER and DROID - check transfer status
// Continue to post solve if TRANSFER == FALSE
bool handleCollisionTransferCheck ( cpArbiter *arb, cpSpace *space, int *unused )
//-------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = PLAYER if userData == -1
	//
	//  Or A = ENEMY and B = ENEMY
	cpShape             *a, *b;
	cpDataPointer       dataPointer_A, dataPointer_B;
	unsigned char       valuesPassedDroid_A[4], valuesPassedDroid_B[4];

	cpArbiterGetShapes ( arb, &a, &b );

	dataPointer_A = cpShapeGetUserData ( a );
	dataPointer_B = cpShapeGetUserData ( b );

	sys_getPackedBytes ( static_cast<int>((intptr_t) dataPointer_A), valuesPassedDroid_A );
	sys_getPackedBytes ( static_cast<int>((intptr_t) dataPointer_B), valuesPassedDroid_B );

	if ( 1 == valuesPassedDroid_B[BYTE_TWO] )    // Is B the player
	{
		if ( playerDroid.currentMode == DROID_MODE_TRANSFER )
		{
			printf ("collision - start transfer\n");
// TODO			gam_initTransfer(valuesPassedDroid_A[BYTE_ONE]);    // Pass in the droid to transfer with
			cpBodySetVelocity ( shipLevel.at ( lvl_returnLevelNameFromDeck ( valuesPassedDroid_A[BYTE_ZERO] )).droid[valuesPassedDroid_A[BYTE_ONE]].body, cpVect{ 0, 0} );
			return cpFalse;    // Don't continue processing collision
		}

	}
	return cpTrue;    // Continue processing collision
}

//-------------------------------------------------------------------
//
// Setup all the collision handlers
void sys_setupCollisionHandlers ()
//-------------------------------------------------------------------
{
	//
	// Handle collision between ENEMY and ENEMY
	//
	handlerEnemyEnemy = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_ENEMY, PHYSIC_TYPE_ENEMY );
	handlerEnemyEnemy->beginFunc = (cpCollisionBeginFunc) handleCollisionDroidCheck;
	handlerEnemyEnemy->postSolveFunc = (cpCollisionPostSolveFunc) handleCollisionDroidToDroid;
	//
	// Handle collision between ENEMY and PLAYER
	//
	handlerEnemyPlayer = cpSpaceAddCollisionHandler ( space, PHYSIC_TYPE_ENEMY, PHYSIC_TYPE_PLAYER );
	handlerEnemyPlayer->beginFunc = (cpCollisionBeginFunc) handleCollisionTransferCheck;
	handlerEnemyPlayer->postSolveFunc = (cpCollisionPostSolveFunc) handleCollisionDroidToDroid;
}
