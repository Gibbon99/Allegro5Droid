#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_droidAI.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_pathFind.h>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_droidAIPatrol.h>
#include "hdr/game/gam_droidAIFlee.h"

//#define AI_FLEE_DEBUG 1

//------------------------------------------
//
// Droid will flee when it has low health, and there is no healing tile on the level.
//
// Flee direction is based on it's position to the left/right of the player worldpos.
//
// Get the left/right extreme tile from level, and look along the Y for a passable tile.
//
// Set aStar destination to that location
//
//---------------------------------------------

//-----------------------------------------------------------------------------
//
// Droid checks his health status and number of healing tiles
int ai_isDroidHealthyFlee ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	//
	// Put in some tests like - percentage of max and current
	//
	// Maybe Battle droids fight until lower health level than non combatands
	//
	int badHealthLevel = 0;

	badHealthLevel = dataBaseEntry[shipLevel.at ( levelName ).droid[whichDroid].droidType].maxHealth * badHealthFactor;

	if ((shipLevel.at ( levelName ).droid[whichDroid].currentHealth < badHealthLevel) &&
	    (shipLevel.at ( levelName ).healing.empty ()) && (shipLevel.at ( levelName ).numWaypoints > 6))
		{
			return AI_RESULT_FAILED;
		}
	else
		{
			return AI_RESULT_SUCCESS;
		}
}

//-----------------------------------------------------------------------------
// Reset flee flags for when found destination tile or changing level
void ai_resetFleeFlags ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	shipLevel.at ( levelName ).droid[whichDroid].isNotPatrolling = true;
	shipLevel.at ( levelName ).droid[whichDroid].onResumeDestTile = false;
	shipLevel.at ( levelName ).droid[whichDroid].destSet = false;
	shipLevel.at ( levelName ).droid[whichDroid].aStarInitDone = false;
	shipLevel.at ( levelName ).droid[whichDroid].onFleeTile = false;
	shipLevel.at ( levelName ).droid[whichDroid].foundFleeTile = false;
	shipLevel.at ( levelName ).droid[whichDroid].aStarDestinationFound = false;
}

//-----------------------------------------------------------------------------
//
// Has the droid found the flee tile
int ai_onFleeTile ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	//
	// If droid over tile ??
	//
	if ( shipLevel.at ( levelName ).droid[whichDroid].onFleeTile )
		{
#ifdef AI_FLEE_DEBUG
			con_print ( true, false, "Droid [ %i ] found Flee tile destination", whichDroid );
#endif
			shipLevel.at ( levelName ).droid[whichDroid].currentHealth += 5;
			if ( shipLevel.at ( levelName ).droid[whichDroid].currentHealth >
			     dataBaseEntry[shipLevel.at ( levelName ).droid[whichDroid].droidType].maxHealth )
				shipLevel.at ( levelName ).droid[whichDroid].currentHealth = dataBaseEntry[shipLevel.at ( levelName ).droid[whichDroid].droidType].maxHealth;

			//
			// Find way back to origin
			//
			ai_resetFleeFlags ( whichDroid, levelName );
			return AI_RESULT_RUNNING;
		}
	else
		{
			return AI_RESULT_FAILED;
		}
}


//-----------------------------------------------------------------------------
//
// Find the nearest flee tile
int ai_reachedFleeTile ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	b2Vec2 fleeTileLocation;
	b2Vec2 droidWorldPosTiles;

	if ( !shipLevel.at ( levelName ).droid[whichDroid].foundFleeTile )
		{
			if ( !shipLevel.at ( levelName ).droid[whichDroid].aStarInitDone )
				{
					debugAStarIndex = whichDroid;

					shipLevel.at ( levelName ).droid[whichDroid].aStarInitDone = true;    // Reset as well
					fleeTileLocation = ai_findNearestTile ( whichDroid, TILE_TYPE_FLEE, levelName );

#ifdef AI_FLEE_DEBUG
					con_print ( true, true, "Found flee tile World [ %3.2f %3.2f ]", fleeTileLocation.x, fleeTileLocation.y );
#endif

					fleeTileLocation.x = (int) fleeTileLocation.x / TILE_SIZE;
					fleeTileLocation.y = (int) fleeTileLocation.y / TILE_SIZE;

#ifdef AI_FLEE_DEBUG
					con_print ( true, true, "Found flee tile Tile [ %3.2f %3.2f ]", fleeTileLocation.x, fleeTileLocation.y );
					con_print ( true, false, "[ %i ] - Starting AStar to find nearest flee tile", whichDroid );
#endif
					droidWorldPosTiles.x = (int) shipLevel.at ( levelName ).droid[whichDroid].worldPos.x / TILE_SIZE;
					droidWorldPosTiles.y =
							((int) shipLevel.at ( levelName ).droid[whichDroid].worldPos.y + (SPRITE_SIZE / 2)) / TILE_SIZE;

					shipLevel.at ( levelName ).droid[whichDroid].aStarPathIndex = gam_AStarRequestNewPath ( droidWorldPosTiles, fleeTileLocation, whichDroid, levelName );
					if ( shipLevel.at ( levelName ).droid[whichDroid].aStarPathIndex < 0 )
						{
							log_logMessage(LOG_LEVEL_ERROR, sys_getString("Error: Couldn't get a valid path index," ));
							return AI_RESULT_FAILED;    //TODO: Check this is the right code to return
						}
				}

			//
			// Create the aStar until we reach the flee tile
			//
			// Ask if the thread has finished creating the path and waypoints
			if ( gam_AStarIsPathReady ( shipLevel.at ( levelName ).droid[whichDroid].aStarPathIndex ))
				{
#ifdef AI_FLEE_DEBUG
					con_print ( true, false, "[ %i ] - Have not found the AStar destination", whichDroid );
#endif
					shipLevel.at ( levelName ).droid[whichDroid].foundFleeTile = true;
					shipLevel.at ( levelName ).droid[whichDroid].currentAStarIndex = gam_AStarGetNumWaypoints ( shipLevel.at ( levelName ).droid[whichDroid].aStarPathIndex );

					//
					// Put pointer at the start of the list
					shipLevel.at ( levelName ).droid[whichDroid].currentAStarIndex++;

					shipLevel.at ( levelName ).droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					ai_getNextWaypoint ( whichDroid, levelName );

					return AI_RESULT_RUNNING;
				}
			return AI_RESULT_FAILED; // Now start moving
		}

//
// This is run each time until the droid reaches the flee tile
//
	if ( shipLevel.at ( levelName ).droid[whichDroid].ai_moveMode == AI_PATHFIND_END )
		{
#ifdef AI_FLEE_DEBUG
			con_print ( true, true, "[ %i ] - Droid has reached flee tile.", whichDroid );
			if (debugAStarIndex == whichDroid)
				debugAStarIndex = -1;
#endif
			return AI_RESULT_SUCCESS;
		}
	else
		{
			if ( AI_RESULT_SUCCESS == ai_moveDroidToWaypoint ( whichDroid, levelName ))
				{
					shipLevel.at ( levelName ).droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					if ( AI_RESULT_FAILED == ai_getNextWaypoint ( whichDroid, levelName ))
						{
							shipLevel.at ( levelName ).droid[whichDroid].ai_moveMode = AI_PATHFIND_END;
							// Reset ready for next time
							shipLevel.at ( levelName ).droid[whichDroid].onFleeTile = true;
							shipLevel.at ( levelName ).droid[whichDroid].foundFleeTile = false;
							shipLevel.at ( levelName ).droid[whichDroid].aStarInitDone = false;
							gam_AStarRemovePath ( shipLevel.at ( levelName ).droid[whichDroid].aStarPathIndex );
						}
				}
			return AI_RESULT_RUNNING;
		}
	return AI_RESULT_RUNNING;
}

