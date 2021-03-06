#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_keyboard.h>
#include <hdr/game/gam_lifts.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_droids.h>
#include <hdr/io/io_resources.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_physicActions.h>
#include <hdr/game/gam_bullet.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_hud.h>
#include <hdr/system/sys_audio.h>
#include "hdr/game/gam_player.h"
#include "hdr/game/gam_terminal.h"

b2Vec2 previousPlayerWorldPos;
float  gravity;                 // From script
_droid playerDroid;

//-----------------------------------------------------------------------------
//
// Render the player sprite
void gam_renderPlayer ()
//-----------------------------------------------------------------------------
{
	if (playerDroid.inTransferMode)
		io_renderTintedSpriteFrame ("001", playerDroid.currentFrame, screenWidth / 2, screenHeight / 2, 0.0f, 0.0f, 1.0f, 1.0f);
	else
		io_renderSpriteFrame ("001", playerDroid.currentFrame, screenWidth / 2, screenHeight / 2);
}

//-----------------------------------------------------------------------------
//
// Recharge player weapon
void gam_weaponRechargePlayer (float tickTime)
//-----------------------------------------------------------------------------
{
	if (!playerDroid.weaponCanFire)
	{
		if (dataBaseEntry[playerDroid.droidType].canShoot)
			playerDroid.weaponDelay += dataBaseEntry[playerDroid.droidType].rechargeTime * tickTime;
		else
			playerDroid.weaponDelay += dataBaseEntry[0].rechargeTime * tickTime;

		if (playerDroid.weaponDelay > 1.0f)
		{
			playerDroid.weaponDelay   = 0.0f;
			playerDroid.weaponCanFire = true;
			hud_setText (false, "hudMoving");
		}
	}
}

// ----------------------------------------------------------------------------
//
// Setup player startup values
void gam_initPlayerValues ()
// ----------------------------------------------------------------------------
{
	playerDroid.droidType               = 0;
	playerDroid.currentHealth           = dataBaseEntry[playerDroid.droidType].maxHealth;
	playerDroid.spriteName              = gam_getSpriteName (playerDroid.droidType);
	playerDroid.currentFrame            = 0;
//	playerDroid.numberOfFrames   = sprites.at ("001").numFrames;
	playerDroid.frameDelay              = 1.0f;
	playerDroid.frameAnimCounter        = 1.0f;
	playerDroid.currentSpeed            = 0.0f;
	playerDroid.acceleration            = dataBaseEntry[playerDroid.droidType].accelerate;
	playerDroid.maxSpeed                = dataBaseEntry[playerDroid.droidType].maxSpeed;
	playerDroid.bulletName              = bul_getBulletName (playerDroid.droidType);
	playerDroid.playerDroidTypeDBIndex  = "db_" + gam_getSpriteName (playerDroid.droidType);
	playerDroid.weaponCanFire           = true;
	playerDroid.weaponDelay             = 0.0f;
	playerDroid.inTransferMode          = false;
	playerDroid.transferTargetDroidType = -1;     // No target transfer droid yet
}

// ----------------------------------------------------------------------------
//
// Position the player on the requested lift on the new level
b2Vec2 gam_getLiftWorldPosition (int whichLift, std::string whichLevel)
// ----------------------------------------------------------------------------
{
	int   whichTile, countY, countX, liftCounter;
	int   tilePosX, tilePosY;
	float pixelX, pixelY;

	b2Vec2 returnPosition;

	liftCounter = 0;

	for (countY = 0; countY != shipLevel.at (whichLevel).levelDimensions.y; countY++)
	{
		for (countX = 0; countX != shipLevel.at (whichLevel).levelDimensions.x; countX++)
		{
			whichTile = shipLevel.at (whichLevel).tiles[(countY * shipLevel.at (whichLevel).levelDimensions.x) + countX];

			if (LIFT_TILE == whichTile)
			{
				if (liftCounter == whichLift)
				{
					tilePosX = countX * TILE_SIZE; //countX - ((screenWidth / TILE_SIZE) / 2);
					tilePosY = countY * TILE_SIZE; //countY - ((screenHeight / TILE_SIZE) / 2);
					tilePosY += TILE_SIZE;

					pixelX = TILE_SIZE / 2;
					pixelY = -TILE_SIZE / 2;

					returnPosition.x = tilePosX + pixelX;
					returnPosition.y = tilePosY + pixelY;

					return returnPosition;
				}
				else
				{
					liftCounter++;
				}
			}
		}
	}

	log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to find lift tile on level [ %s ]", whichLevel.c_str ()));
	return returnPosition;
}

//-----------------------------------------------------------------------------
//
// Process player movement
void gam_processPlayerMovement ()
//-----------------------------------------------------------------------------
{
	if (keyBinding[gameLeft].currentlyPressed)
	{

		playerDroid.velocity.x -= playerDroid.acceleration;
		if (playerDroid.velocity.x < -playerDroid.maxSpeed)
		{
			playerDroid.velocity.x = -playerDroid.maxSpeed;
		}
	}

	else if (keyBinding[gameRight].currentlyPressed)
	{
		playerDroid.velocity.x += playerDroid.acceleration;
		if (playerDroid.velocity.x > playerDroid.maxSpeed)
		{
			playerDroid.velocity.x = playerDroid.maxSpeed;
		}
	}

	if (keyBinding[gameUp].currentlyPressed)
	{
		playerDroid.velocity.y -= playerDroid.acceleration;
		if (playerDroid.velocity.y < -playerDroid.maxSpeed)
		{
			playerDroid.velocity.y = -playerDroid.maxSpeed;
		}
	}

	else if (keyBinding[gameDown].currentlyPressed)
	{
		playerDroid.velocity.y += playerDroid.acceleration;
		if (playerDroid.velocity.y > playerDroid.maxSpeed)
		{
			playerDroid.velocity.y = playerDroid.maxSpeed;
		}
	}

//
// Do gravity slowdown when no key is pressed
	if (!keyBinding[gameLeft].currentlyPressed)
	{
		if (playerDroid.velocity.x < 0.0f)
		{
			playerDroid.velocity.x += gravity;
			if (playerDroid.velocity.x > 0.0f)
			{
				playerDroid.velocity.x = 0.0f;
			}
		}
	}

	if (!keyBinding[gameRight].currentlyPressed)
	{
		if (playerDroid.velocity.x > 0.0f)
		{
			playerDroid.velocity.x -= gravity;
			if (playerDroid.velocity.x < 0.0f)
			{
				playerDroid.velocity.x = 0.0f;
			}
		}
	}

	if (!keyBinding[gameUp].currentlyPressed)
	{
		if (playerDroid.velocity.y < 0.0f)
		{
			playerDroid.velocity.y += gravity;
			if (playerDroid.velocity.y > 0.0f)
			{
				playerDroid.velocity.y = 0.0f;
			}
		}
	}

	if (!keyBinding[gameDown].currentlyPressed)
	{
		if (playerDroid.velocity.y > 0.0f)
		{
			playerDroid.velocity.y -= gravity;
			if (playerDroid.velocity.y < 0.0f)
			{
				playerDroid.velocity.y = 0.0f;
			}
		}
	}

	previousPlayerWorldPos = playerDroid.worldPos;
	playerDroid.worldPos = playerDroid.body->GetPosition ();     // GetPosition is in meters
	playerDroid.worldPos.x *= pixelsPerMeter;                   // Change to pixels
	playerDroid.worldPos.y *= pixelsPerMeter;
}

//----------------------------------------------------------------------------
//
// Start all the actions resulting from pressing the action key
void gam_processActionKey ()
//----------------------------------------------------------------------------
{
	hud_setText (false, "hudMoving");

	if (playerDroid.inTransferMode)
		hud_setText (false, "hudTransfer");
	//
	// Actions when no movements are down
	if ((!keyBinding[gameLeft].currentlyPressed) && (!keyBinding[gameRight].currentlyPressed) && (!keyBinding[gameDown].currentlyPressed) && (!keyBinding[gameUp].currentlyPressed))
	{
		if (playerDroid.overLiftTile)
		{
			if (!playerDroid.inTransferMode)
			{
				gam_performLiftAction ();
				keyBinding[gameAction].currentlyPressed = false;
				return;
			}
		}

		if (playerDroid.overTerminalTile)
		{
			if (!playerDroid.inTransferMode)
			{
				gam_performTerminalAction ();
				keyBinding[gameAction].currentlyPressed = false;
				return;
			}
		}

		if (!playerDroid.inTransferMode)
		{
			playerDroid.inTransferMode = true;
			evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_LOOP, "transferMove");
			hud_setText (false, "hudTransfer");
		}
	}

	if (!playerDroid.inTransferMode)
	{
		if ((keyBinding[gameLeft].currentlyPressed) || (keyBinding[gameRight].currentlyPressed) || (keyBinding[gameDown].currentlyPressed) || (keyBinding[gameUp].currentlyPressed))
		{
			if (playerDroid.weaponCanFire)
			{
				if ((playerDroid.velocity.x == 0.0f) && (playerDroid.velocity.y == 0.0f))       // Don't create bullet with no velocity
					return;

				gam_addPhysicAction (PHYSIC_EVENT_TYPE_NEW_BULLET, 0, 0, 0, -1, {0, 0});
				keyBinding[gameAction].currentlyPressed = false;
				playerDroid.weaponCanFire               = false;
				hud_setText (false, "hudRecharging");
				return;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Destroy the player and end game
void gam_destroyPlayer ()
//---------------------------------------------------------------------------------------------------------------
{
	printf ("Player is dead\n");

}
