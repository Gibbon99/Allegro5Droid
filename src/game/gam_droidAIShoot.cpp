#include <hdr/game/gam_droidAI.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_player.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_game.h>
#include "hdr/game/gam_droidAIShoot.h"

//#define DEBUG_SHOOT 1
//#define USE_LEADING_SHOT 1

// ----------- Values set from script ------------
float witnessShootValue;
float witnessTransferValue;
float ai_beenShot;
float ai_healthAmount;
float ai_playerVisible;
float ai_witnessShoot;
float ai_witnessTransfer;
float ai_greenFactor;
float ai_yellowFactor;
float ai_redFactor;

float thinkInterval = 1.0f / 30.f;

//-----------------------------------------------------------------------------
//
// Should the Droid shoot or not
int ai_notShoot (int whichDroid, const std::string levelName)
//-----------------------------------------------------------------------------
{
	if (!dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].canShoot)
		return AI_RESULT_SUCCESS;

	if (shipLevel.at (levelName).droid[whichDroid].chanceToShoot < 10.0f)
		return AI_RESULT_SUCCESS;

#ifdef DEBUG_SHOOT
	printf ("Droid [ %i ] can shoot\n", whichDroid);
#endif

	if (!shipLevel.at (levelName).droid[whichDroid].weaponCanFire)
		return AI_RESULT_SUCCESS;

#ifdef DEBUG_SHOOT
	printf ("Droid [ %i ] weapon is ready\n", whichDroid);
#endif

	return AI_RESULT_FAILED;
}

//-----------------------------------------------------------------------------
//
// Locate the player position - see if the droid can take the shot or not
int ai_findBulletDest (int whichDroid, const std::string levelName)
//-----------------------------------------------------------------------------
{
	if (shipLevel.at (levelName).droid[whichDroid].visibleToPlayer)
		{
#ifdef DEBUG_SHOOT
			printf ("Droid has a destination to shoot at [ %i ]\n", whichDroid);
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
// Take the shot
int ai_shootBullet (int whichDroid, const std::string levelName)
//-----------------------------------------------------------------------------
{
	cpVect bulletStartPos;
	cpVect bulletDestPos;

#ifdef DEBUG_SHOOT
	printf ("Fire bullet [ %i ]\n", whichDroid);
#endif

	bulletStartPos = shipLevel.at (levelName).droid[whichDroid].worldPos;
	bulletStartPos.y += TILE_SIZE / 2;

	if (shipLevel.at (levelName).droid[whichDroid].targetIndex == -1) // Player is the target
		{
#ifdef USE_LEADING_SHOT
			if (false == gam_calculateInterceptShotPosition (shipLevel.at(levelName).droid[whichDroid].worldPos,
                                                       playerWorldPos,
                                                       playerVelocity,
                                                       10.0f + baseGameSpeed,
                                                       bulletStartPos))
#endif
			{
				// Default to player position
				bulletDestPos = playerDroid.worldPos;

#ifdef USE_LEADING_SHOT
				printf ("DID NOT find solution to leading shot.\n");
#endif
			}
		}
	else
		{
			bulletDestPos = shipLevel.at (levelName).droid[shipLevel.at (levelName).droid[whichDroid].targetIndex].worldPos;
		}

//	bulletDestPos.y += TILE_SIZE / 2;

	evt_pushEvent (-1, MAIN_LOOP_EVENT, MAIN_LOOP_EVENT_ADD_BULLET, dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].bulletType, whichDroid, levelName);

	shipLevel.at (levelName).droid[whichDroid].chanceToShoot = 0.0f;
	shipLevel.at (levelName).droid[whichDroid].weaponCanFire = false;

	return AI_RESULT_SUCCESS;
}

//------------------------------------------------------------
//
// Work out the chance to shoot for this round
void gam_findChanceToShoot (int whichDroid, const std::string levelName)
//------------------------------------------------------------
{
	//
	// Reset to 0 at the start for each?
	//

	if (shipLevel.at (levelName).droid[whichDroid].currentMode == DROID_MODE_DEAD)
		return;

	if (!dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].canShoot)
		return;

	//
	// Process how long droid remembers being shot for
	//
	if (shipLevel.at (levelName).droid[whichDroid].beenShotByPlayer)
		{
			shipLevel.at (levelName).droid[whichDroid].beenShotCountdown -= 1.0f * thinkInterval;
			if (shipLevel.at (levelName).droid[whichDroid].beenShotCountdown < 0.0f)
				shipLevel.at (levelName).droid[whichDroid].beenShotByPlayer = false;
		}

	if (shipLevel.at (levelName).droid[whichDroid].beenShotByPlayer)
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot += ai_beenShot;
	else
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot -= ai_beenShot;

	//
	// Process how long droid remembers witnessing a shooting by the player
	//
	if (shipLevel.at (levelName).droid[whichDroid].witnessShooting)
		{
			shipLevel.at (levelName).droid[whichDroid].witnessShootingCountDown -= 1.0f * thinkInterval;
			if (shipLevel.at (levelName).droid[whichDroid].witnessShootingCountDown < 0.0f)
				shipLevel.at (levelName).droid[whichDroid].witnessShooting = false;

			shipLevel.at (levelName).droid[whichDroid].chanceToShoot += ai_witnessShoot;
			shipLevel.at (levelName).droid[whichDroid].targetIndex = -1;
		}
	else
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot -= ai_witnessShoot;

	//
	// Process how long droid remembers witnessing a transfer by the player
	//
	if (shipLevel.at (levelName).droid[whichDroid].witnessTransfer)
		{
			shipLevel.at (levelName).droid[whichDroid].witnessTransferCountDown -= 1.0f * thinkInterval;
			if (shipLevel.at (levelName).droid[whichDroid].witnessTransferCountDown < 0.0f)
				shipLevel.at (levelName).droid[whichDroid].witnessTransfer = false;

			shipLevel.at (levelName).droid[whichDroid].chanceToShoot += ai_witnessTransfer;
		}
	else
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot -= ai_witnessTransfer;

	//
	// Is Droid healthy enough to engage in combat
	//
	if (shipLevel.at (levelName).droid[whichDroid].currentHealth
	    > dataBaseEntry[shipLevel.at (levelName).droid[whichDroid].droidType].maxHealth / 2)
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot += ai_healthAmount;
	else
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot -= ai_healthAmount;

	//
	// Add the effect of the current alert level
	//
	switch (currentAlertLevel)
		{
			case ALERT_GREEN_TILE: shipLevel.at (levelName).droid[whichDroid].chanceToShoot -= ai_greenFactor;
			break;

			case ALERT_YELLOW_TILE: shipLevel.at (levelName).droid[whichDroid].chanceToShoot += ai_yellowFactor;
			break;

			case ALERT_RED_TILE: shipLevel.at (levelName).droid[whichDroid].chanceToShoot += ai_redFactor;
			break;

			default: break;
		}

	//
	// See if the player is visible or not - enemy will not shoot unless can see player
	//
	if (!shipLevel.at (levelName).droid[whichDroid].visibleToPlayer)
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot = 0.0f;
	//
	// Cap the chance to always above zero
	//
	if (shipLevel.at (levelName).droid[whichDroid].chanceToShoot < 0.0f)
		shipLevel.at (levelName).droid[whichDroid].chanceToShoot = 0.0f;

	if (shipLevel.at (levelName).droid[whichDroid].chanceToShoot > 0.0f)
		printf ("Shoot [ %3.3f ]\n", shipLevel.at (levelName).droid[whichDroid].chanceToShoot);
}

//------------------------------------------------------------
//
// Flag if a droid witnesses the player transferring into another droid
//
// Called when a transfer succeeds
void gam_processWitnessTransfer (const std::string levelName)
//------------------------------------------------------------
{
	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
		{
			if (DROID_MODE_NORMAL == shipLevel.at (levelName).droid[i].currentMode)
				{
					if (shipLevel.at (levelName).droid[i].visibleToPlayer)
						{
							shipLevel.at (levelName).droid[i].witnessTransfer = true;
							shipLevel.at (levelName).droid[i].witnessTransferCountDown = witnessTransferValue;
						}
				}
		}
}
//------------------------------------------------------------
//
// Flag if a droid witnesses the player shooting another droid
//
// Called when player shoots a bullet
void gam_processWitnessShooting (const std::string levelName)
//------------------------------------------------------------
{
	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
		{
			if (DROID_MODE_NORMAL == shipLevel.at (levelName).droid[i].currentMode)
				{
					if (shipLevel.at (levelName).droid[i].visibleToPlayer)
						{
							shipLevel.at (levelName).droid[i].witnessShooting = true;
							shipLevel.at (levelName).droid[i].witnessShootingCountDown = witnessShootValue;
							shipLevel.at (levelName).droid[i].targetIndex = -1;
						}
				}
		}
}



