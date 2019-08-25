#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_keyboard.h>
#include "hdr/game/gam_player.h"

cpVect        playerWorldPos;
cpVect        previousPlayerWorldPos;
cpVect        playerVelocity;
float         playerAcceleration = 0.001f;
float         playerMaxSpeed     = 0.010f;
float         gravity            = 0.020f;
_physicObject playerPhysicsObjectClient;

// ----------------------------------------------------------------------------
//
// Position the player on the requested lift on the new level
cpVect gam_getLiftWorldPosition (int whichLift, std::string whichLevel)
// ----------------------------------------------------------------------------
{
	int   whichTile, countY, countX, liftCounter;
	int   tilePosX, tilePosY;
	float pixelX, pixelY;

	cpVect returnPosition;

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
									tilePosX = countX - ((screenWidth / TILE_SIZE) / 2);
									tilePosY = countY - ((screenHeight / TILE_SIZE) / 2);
									tilePosY += 1;

									pixelX = TILE_SIZE / 2;
									pixelY = -TILE_SIZE / 2;

									returnPosition.x = (tilePosX * TILE_SIZE) + pixelX;
									returnPosition.y = (tilePosY * TILE_SIZE) + pixelY;

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
			playerVelocity.x -= playerAcceleration;
			if (playerVelocity.x < -playerMaxSpeed)
				{
					playerVelocity.x = -playerMaxSpeed;
				}
		}

	else if (keyBinding[gameRight].currentlyPressed)
		{
			playerVelocity.x += playerAcceleration;
			if (playerVelocity.x > playerMaxSpeed)
				{
					playerVelocity.x = playerMaxSpeed;
				}
		}

	if (keyBinding[gameUp].currentlyPressed)
		{
			playerVelocity.y -= playerAcceleration;
			if (playerVelocity.y < -playerMaxSpeed)
				{
					playerVelocity.y = -playerMaxSpeed;
				}
		}

	else if (keyBinding[gameDown].currentlyPressed)
		{
			playerVelocity.y += playerAcceleration;
			if (playerVelocity.y > playerMaxSpeed)
				{
					playerVelocity.y = playerMaxSpeed;
				}
		}

//
// Do gravity slowdown when no key is pressed
	if (!keyBinding[gameLeft].currentlyPressed)
		{
			if (playerVelocity.x < 0.0f)
				{
					playerVelocity.x += gravity;
					if (playerVelocity.x > 0.0f)
						{
							playerVelocity.x = 0.0f;
						}
				}
		}

	if (!keyBinding[gameRight].currentlyPressed)
		{
			if (playerVelocity.x > 0.0f)
				{
					playerVelocity.x -= gravity;
					if (playerVelocity.x < 0.0f)
						{
							playerVelocity.x = 0.0f;
						}
				}
		}

	if (!keyBinding[gameUp].currentlyPressed)
		{
			if (playerVelocity.y < 0.0f)
				{
					playerVelocity.y += gravity;
					if (playerVelocity.y > 0.0f)
						{
							playerVelocity.y = 0.0f;
						}
				}
		}

	if (!keyBinding[gameDown].currentlyPressed)
		{
			if (playerVelocity.y > 0.0f)
				{
					playerVelocity.y -= gravity;
					if (playerVelocity.y < 0.0f)
						{
							playerVelocity.y = 0.0f;
						}
				}
		}

	previousPlayerWorldPos = playerWorldPos;
	playerWorldPos         = cpBodyGetPosition (playerPhysicsObjectClient.body);
}

