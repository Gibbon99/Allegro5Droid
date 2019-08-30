#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_droidAI.h>
#include "hdr/game/gam_droidAIPatrol.h"
#include <hdr/game/gam_pathFind.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_game.h>

float wayPointDestinationSize;      // From script

//-----------------------------------------------------------------------------------------------------
//
/// \param argc
/// \param argv
/// \return
void ai_processDroidMovement ( const std::string levelName )
//-----------------------------------------------------------------------------------------------------
{
	cpVect		tempPosition;
	cpVect		maxWorldSize;

	if ( shipLevel.at ( levelName ).droid.empty ())
		return;     // Nothing to process

	for ( int index = 0; index != shipLevel.at ( levelName ).numDroids; index++ )
		{
			if ( shipLevel.at ( levelName ).droid[index].currentMode != DROID_MODE_DEAD)
				{
					maxWorldSize.x = shipLevel.at (levelName).levelDimensions.x * TILE_SIZE;
					maxWorldSize.y = shipLevel.at (levelName).levelDimensions.y * TILE_SIZE;

					//
					// Check body is valid
					//
					if (cpTrue == cpSpaceContainsBody (space, shipLevel.at (levelName).droid[index].body))
						{
							tempPosition = cpBodyGetPosition (shipLevel.at (levelName).droid[index].body);

							if ((tempPosition.x < 0) || (tempPosition.y < 0) || (tempPosition.x > maxWorldSize.x)
							    || (tempPosition.y > maxWorldSize.y))
								{
									printf ("ERROR: Setting invalid worldPos [ %3.3f %3.3f ] from body Droid [ %i ] Level [ %s ]\n",
									        tempPosition.x, tempPosition.y, index, levelName.c_str ());
									return;
								}

							shipLevel.at (levelName).droid[index].worldPos = tempPosition;

						}
					else
						{
							printf ("ERROR: Attempting to get position from invalid body - droid [ %i ]\n", index);
							return;
						}
				}
		}
}


//-----------------------------------------------------------------------------
//
// Process all Droid movements
int ai_getNextWaypoint ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	//
	// Get the next waypoint index pointer
	switch ( shipLevel.at (levelName).droid[whichDroid].ai_moveMode )
		{
			case AI_MODE_WAYPOINT:

				switch ( shipLevel.at (levelName).droid[whichDroid].wayPointDirection )
					{
						case WAYPOINT_UP:
							if ( shipLevel.at (levelName).droid[whichDroid].wayPointIndex < shipLevel.at (levelName).numWaypoints - 1 )
								shipLevel.at (levelName).droid[whichDroid].wayPointIndex++;
							else
								shipLevel.at (levelName).droid[whichDroid].wayPointIndex = 0;

						break;

						case WAYPOINT_DOWN:
							shipLevel.at (levelName).droid[whichDroid].wayPointIndex--;

						if ( shipLevel.at (levelName).droid[whichDroid].wayPointIndex < 0 )
							shipLevel.at (levelName).droid[whichDroid].wayPointIndex = shipLevel.at (levelName).numWaypoints - 1;

						break;
					}

			//
			// Save current destination in case we need to reverse
			//
			shipLevel.at (levelName).droid[whichDroid].previousWaypoints = shipLevel.at (levelName).droid[whichDroid].destinationCoords;
			//
			// Get new destination to work out direction vector
			//
			shipLevel.at (levelName).droid[whichDroid].destinationCoords.x = shipLevel.at (levelName).wayPoints[shipLevel.at (levelName).droid[whichDroid].wayPointIndex].x;
			shipLevel.at (levelName).droid[whichDroid].destinationCoords.y = shipLevel.at (levelName).wayPoints[shipLevel.at (levelName).droid[whichDroid].wayPointIndex].y;

			break;

			case AI_MODE_ASTAR:
				shipLevel.at (levelName).droid[whichDroid].currentAStarIndex--; // Move index to next waypoint

			if ( shipLevel.at (levelName).droid[whichDroid].currentAStarIndex < 0 )
				{
//					con_print ( true, true, "Droid has reached destination" );
					shipLevel.at (levelName).droid[whichDroid].ai_moveMode = AI_PATHFIND_END;
					return AI_RESULT_FAILED; // No more waypoints
				}

			//
			// Get new destination to work out direction vector
			//
			shipLevel.at (levelName).droid[whichDroid].destinationCoords.x = path[shipLevel.at (levelName).droid[whichDroid].aStarPathIndex].wayPoints[shipLevel.at (levelName).droid[whichDroid].currentAStarIndex].x;
			shipLevel.at (levelName).droid[whichDroid].destinationCoords.y = path[shipLevel.at (levelName).droid[whichDroid].aStarPathIndex].wayPoints[shipLevel.at (levelName).droid[whichDroid].currentAStarIndex].y;
			break;
		}

	return AI_RESULT_SUCCESS;
}

//-----------------------------------------------------------------------------
//
// Can this droid make a move this turn
//
// Will fail if it is currently in a collision with either another droid or
// the player
// TODO: Add in player collision
int ai_canMove ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	//
	// Droid can always move as it's ignoring any collisions with other droids
	//
	if ( shipLevel.at ( levelName).droid[whichDroid].ignoreCollisions )
		{
			return AI_RESULT_SUCCESS;
		}

	if ( shipLevel.at ( levelName).droid[whichDroid].hasCollided )
		{
			return AI_RESULT_FAILED;
		}
	else
		{
			return AI_RESULT_SUCCESS;
		}

	return AI_RESULT_SUCCESS;
}

//-----------------------------------------------------------------------------
//
// Process movements for the passed in droid
// Work out heading vector and velocity
//
// Return AI_RESULT_SUCCESS when we have reached the destination
//
int ai_moveDroidToWaypoint ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	float wayPointDistance;

//	float thinkInterval = 1.0f / 30.f;

	wayPointDistance = cpvdist ( shipLevel.at (levelName).droid[whichDroid].destinationCoords, shipLevel.at (levelName).droid[whichDroid].worldPos );

	if ( wayPointDistance < wayPointDestinationSize + 20 )
		{
			shipLevel.at (levelName).droid[whichDroid].currentSpeed = dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].maxSpeed * 0.5f;
		}
	else
		{
			// Acclerate faster on higher alert levels
			switch (currentAlertLevel)
				{
					case ALERT_GREEN_TILE:
						shipLevel.at (levelName).droid[whichDroid].currentSpeed += dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].accelerate; // * thinkInterval;
					break;

					case ALERT_YELLOW_TILE:
						shipLevel.at (levelName).droid[whichDroid].currentSpeed += (dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].accelerate * 1.2f); //  * thinkInterval;
					break;

					case ALERT_RED_TILE:
						shipLevel.at (levelName).droid[whichDroid].currentSpeed += (dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].accelerate * 1.5f); //  * thinkInterval;
					break;
				}

			if ( shipLevel.at (levelName).droid[whichDroid].currentSpeed > dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].maxSpeed )
				shipLevel.at (levelName).droid[whichDroid].currentSpeed = dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].maxSpeed;
		}


	shipLevel.at (levelName).droid[whichDroid].currentSpeed = 0.003f;       // TODO update database values



	shipLevel.at (levelName).droid[whichDroid].destDirection = sys_getDirection ( shipLevel.at (levelName).droid[whichDroid].destinationCoords, shipLevel.at (levelName).droid[whichDroid].worldPos );

	shipLevel.at (levelName).droid[whichDroid].velocity = cpvnormalize ( shipLevel.at (levelName).droid[whichDroid].destDirection );
	shipLevel.at (levelName).droid[whichDroid].velocity = cpvmult ( shipLevel.at (levelName).droid[whichDroid].velocity, shipLevel.at (levelName).droid[whichDroid].currentSpeed );
	//
	// Move the droid
	//
	if ( cpFalse == cpBodyIsSleeping ( shipLevel.at (levelName).droid[whichDroid].body ) )
		{
			if ( cpTrue == cpSpaceContainsBody ( space, shipLevel.at (levelName).droid[whichDroid].body ) )
				{
					cpBodySetForce ( shipLevel.at (levelName).droid[whichDroid].body, shipLevel.at (levelName).droid[whichDroid].velocity );
				}
		}
		//
	// See if the droid has reached it's destination
	//
	//
	// How far between current and destination position
	//
	// If it's less then we have reached the waypoint destination
	if ( wayPointDistance < wayPointDestinationSize )
		{
			shipLevel.at (levelName).droid[whichDroid].velocity = cpvzero;
			cpBodySetVelocity ( shipLevel.at (levelName).droid[whichDroid].body, shipLevel.at (levelName).droid[whichDroid].velocity );
			return AI_RESULT_SUCCESS;
		}
	else
		{
			return AI_RESULT_RUNNING; // still hasn't made it to the destination
		}
}

//-----------------------------------------------------------------------------
//
// This droid has collided - can it reverse direction and move
//
int ai_canReverseDirection ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	if ( shipLevel.at (levelName).droid[whichDroid].droidType < shipLevel.at (levelName).droid[shipLevel.at (levelName).droid[whichDroid].collidedWith].droidType )
		{
			switch ( shipLevel.at (levelName).droid[whichDroid].wayPointDirection )
				{
					case WAYPOINT_DOWN:
						shipLevel.at (levelName).droid[whichDroid].wayPointDirection = WAYPOINT_UP;
					break;

					case WAYPOINT_UP:
						shipLevel.at (levelName).droid[whichDroid].wayPointDirection = WAYPOINT_DOWN;
					break;
				}

			ai_getNextWaypoint ( whichDroid, levelName );
		}

	return AI_RESULT_SUCCESS;
}

