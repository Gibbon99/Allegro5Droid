#include <hdr/game/gam_droidAI.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_droidAIHealth.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_pathFind.h>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_droidAIPatrol.h>

//#define AI_HEALTH_DEBUG 1

/*stayHealthy (SELECTOR - stop on success)

- ai_isDroidHealthy (SUCCESS - health is OK)
- findHealingTile (SUCCESS - found a tile) - (RUNNING - aStar) (FAILURE - no tile on level)
- moveToLocation (SUCCESS - reached tile) (RUNNING - still moving) (FAILURE - ??)
- healDamage (SUCCESS - health restored ) (RUNNING - still healing)
*/
//-----------------------------------------------------------------------------
//
// Droid checks his health status
int ai_isDroidHealthy ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	//
	// Put in some tests like - percentage of max and current
	//
	// Maybe Battle droids fight until lower health level than non combatands
	//
	int badHealthLevel = 0;

	badHealthLevel = static_cast<int>(dataBaseEntry[shipLevel.at(levelName ).droid[whichDroid].droidType].maxHealth * badHealthFactor);

	if ( ( shipLevel.at( levelName ).droid[whichDroid].currentHealth <= badHealthLevel ) && ( !shipLevel.at( levelName ).healing.empty() ) )
		{
#ifdef AI_HEALTH_DEBUG
			printf ("Droid [ %i ] has health [ %i ] from level [ %i ]\n", whichDroid, shipLevel.at( levelName ).droid[whichDroid].currentHealth, badHealthLevel);
#endif
			return AI_RESULT_FAILED;
		}
	else
		{
			return AI_RESULT_SUCCESS;
		}
}

//-----------------------------------------------------------------------------
//
// Reset health flags for when found healing tile or changing level
void ai_resetHealthFlags ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	shipLevel.at( levelName ).droid[whichDroid].isNotPatrolling = true;
	shipLevel.at( levelName ).droid[whichDroid].onResumeDestTile = false;
	shipLevel.at( levelName ).droid[whichDroid].destSet = false;
	shipLevel.at( levelName ).droid[whichDroid].aStarInitDone = false;
	shipLevel.at( levelName ).droid[whichDroid].onHealingTile = false;
	shipLevel.at( levelName ).droid[whichDroid].foundHealingTile = false;
	shipLevel.at( levelName ).droid[whichDroid].aStarDestinationFound = false;
}

//-----------------------------------------------------------------------------
//
// Is the droid currently located on a healing tile
int ai_onHealingTile ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	//
	// If droid over tile ??
	//
	if ( shipLevel.at( levelName ).droid[whichDroid].onHealingTile )
		{
#ifdef AI_HEALTH_DEBUG
			con_print ( CON_INFO, false, "Droid [ %i ] fully healed", whichDroid );
#endif
			shipLevel.at( levelName ).droid[whichDroid].currentHealth = dataBaseEntry[shipLevel.at( levelName ).droid[whichDroid].droidType].maxHealth;
			//
			// Find way back to origin
			//
			ai_resetHealthFlags(whichDroid, levelName);
			return AI_RESULT_RUNNING;
		}
	else
		{

			//			con_print ( true, false, "[ %i ] - Droid still not on healing tile", whichDroid );
			return AI_RESULT_FAILED;
		}
}

//-----------------------------------------------------------------------------
//
// Find the nearest healing tile
int ai_reachedHealingTile ( int whichDroid, const std::string levelName )
//-----------------------------------------------------------------------------
{
	cpVect healingTileLocation;
	cpVect	droidWorldPosTiles;

	if ( !shipLevel.at( levelName ).droid[whichDroid].foundHealingTile )
		{
			if ( !shipLevel.at( levelName ).droid[whichDroid].aStarInitDone )
				{
					debugAStarIndex = whichDroid;

					shipLevel.at( levelName ).droid[whichDroid].aStarInitDone = true;	// Reset as well
					healingTileLocation = ai_findNearestTile ( whichDroid, TILE_TYPE_HEALING, levelName );

					if (healingTileLocation.x == -1)
						{
							// had a problem finding a healing tile
							printf("Error: Couldn't find healing tile to use.\n");
							return AI_RESULT_FAILED;
						}

					healingTileLocation.x = healingTileLocation.x / (float)TILE_SIZE;
					healingTileLocation.y = healingTileLocation.y / (float)TILE_SIZE;

#ifdef AI_HEALTH_DEBUG
					con_print ( true, false, "[ %i ] - Starting AStar to find nearest healing tile", whichDroid );
#endif
					droidWorldPosTiles.x = ( int ) shipLevel.at( levelName ).droid[whichDroid].worldPos.x / TILE_SIZE;
					droidWorldPosTiles.y = ( ( int ) shipLevel.at( levelName ).droid[whichDroid].worldPos.y + ( SPRITE_SIZE / 2 ) ) / TILE_SIZE;

					shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex = gam_AStarRequestNewPath ( droidWorldPosTiles, healingTileLocation, whichDroid, levelName );
					if (shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex < 0)
						{
							log_logMessage(LOG_LEVEL_ERROR, sys_getString("Error: Couldn't get a valid path index,"));
							return AI_RESULT_FAILED;
						}
				}

			//
			// Create the aStar until we reach the healing tile
			//
			// Ask if the thread has finished creating the path and waypoints
			if ( gam_AStarIsPathReady ( shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex ))
				{
#ifdef AI_HEALTH_DEBUG
					con_print ( true, false, "[ %i ] - Have not found the AStar destination", whichDroid );
#endif
					shipLevel.at( levelName ).droid[whichDroid].foundHealingTile = true;
					shipLevel.at( levelName ).droid[whichDroid].currentAStarIndex = gam_AStarGetNumWaypoints ( shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex );

					//
					// Put pointer at the start of the list
					shipLevel.at( levelName ).droid[whichDroid].currentAStarIndex++;

					shipLevel.at( levelName ).droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					ai_getNextWaypoint ( whichDroid, levelName );

					return AI_RESULT_RUNNING;
				}
			return AI_RESULT_FAILED; // Now start moving
		}

//
// This is run each time until the droid reaches the healing tile
//
	if ( shipLevel.at( levelName ).droid[whichDroid].ai_moveMode == AI_PATHFIND_END )
		{
#ifdef AI_HEALTH_DEBUG
			con_print ( true, true, "[ %i ] - Droid has reached healing tile.", whichDroid );
#endif
			if (debugAStarIndex == whichDroid)
				debugAStarIndex = -1;

			return AI_RESULT_SUCCESS;
		}
	else
		{
			if ( AI_RESULT_SUCCESS == ai_moveDroidToWaypoint ( whichDroid, levelName ) )
				{
					shipLevel.at( levelName ).droid[whichDroid].ai_moveMode = AI_MODE_ASTAR;
					if ( AI_RESULT_FAILED == ai_getNextWaypoint ( whichDroid, levelName ) )
						{
							shipLevel.at( levelName ).droid[whichDroid].ai_moveMode = AI_PATHFIND_END;
							// Reset ready for next time
							shipLevel.at( levelName ).droid[whichDroid].onHealingTile = true;
							shipLevel.at( levelName ).droid[whichDroid].foundHealingTile = false;
							shipLevel.at( levelName ).droid[whichDroid].aStarInitDone = false;
							//if (shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex > -1)
							gam_AStarRemovePath ( shipLevel.at( levelName ).droid[whichDroid].aStarPathIndex );
						}
				}
			return AI_RESULT_RUNNING;
		}

	return AI_RESULT_RUNNING;
}

