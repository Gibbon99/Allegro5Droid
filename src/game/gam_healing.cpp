#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_render.h>
#include "hdr/game/gam_healing.h"

std::vector<__healingSensor> healing;
float                        healingAnimSpeed;

//----------------------------------------------------------------------------------------------------------------------
//
// Create a healing tile sensor
void gam_createHealingSensor (unsigned long whichHealingTile, int index)
//----------------------------------------------------------------------------------------------------------------------
{
	healing[whichHealingTile].bodyDef.type = b2_staticBody;
	healing[whichHealingTile].bodyDef.position.Set (healing[whichHealingTile].worldPosition.x / pixelsPerMeter, healing[whichHealingTile].worldPosition.y / pixelsPerMeter);
	healing[whichHealingTile].body = sys_getPhysicsWorld ()->CreateBody (&healing[whichHealingTile].bodyDef);

	healing[whichHealingTile].userData            = new _userData;
	healing[whichHealingTile].userData->userType  = PHYSIC_TYPE_HEALING;
	healing[whichHealingTile].userData->dataValue = (int) index;
	healing[whichHealingTile].body->SetUserData (healing[whichHealingTile].userData);

	healing[whichHealingTile].shape.SetAsBox ((healing[whichHealingTile].height) / pixelsPerMeter, (healing[whichHealingTile].width) / pixelsPerMeter);
	healing[whichHealingTile].fixtureDef.shape    = &healing[whichHealingTile].shape;
	healing[whichHealingTile].fixtureDef.isSensor = true;
	healing[whichHealingTile].body->CreateFixture (&healing[whichHealingTile].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create healing sensors for this level
void gam_createHealingSensors (const std::string &levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	__healingSensor tempHealingSensor{};
	int             healingCounter;

	if (!healing.empty ())
		gam_clearHealing ();

	healingCounter = 0;
	for (auto healingItr : shipLevel.at (levelName).healing)
	{
		tempHealingSensor.worldPosition.x = healingItr.renderPosition.x + (TILE_SIZE / 2);
		tempHealingSensor.worldPosition.y = healingItr.renderPosition.y + (TILE_SIZE / 2);

		tempHealingSensor.width  = TILE_SIZE / 8;
		tempHealingSensor.height = TILE_SIZE / 8;

		healing.push_back (tempHealingSensor);
		gam_createHealingSensor (healing.size () - 1, healingCounter++);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check status of healing tiles
void gam_processHealingTiles ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &healingItr : healing)
	{
		if (healingItr.inUse)
		{
			printf ("Healing tile [ %i ] is in use by [ %i ]\n", healingItr.userData->dataValue, healingItr.inUseBy);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Stop this healing tile
void gam_stopHealing (int whichTile)
//----------------------------------------------------------------------------------------------------------------------
{
	healing[whichTile].inUse   = false;
	healing[whichTile].inUseBy = -2;
	//
	// Stop healing sound for this tile
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start this healing tile - pass in droid on tile and healing index. whichDroid == -1 is player
void gam_startHealing (int whichDroid, int whichTile)
//----------------------------------------------------------------------------------------------------------------------
{
	healing[whichTile].inUse   = true;
	healing[whichTile].inUseBy = whichDroid;
	//
	// Start healing sound - get healing worldPosition if not player to attenuate sound
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render current door frames onto map
void gam_renderHealingFrames (const std::string &levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	al_set_target_bitmap (gam_getCompleteLevel ());

	for (const auto &healingItr : shipLevel.at (levelName).healing)
	{
		if (sys_visibleOnScreen (healingItr.renderPosition, TILE_SIZE))
		{
			gam_drawSingleTile (healingItr.renderPosition.x, healingItr.renderPosition.y, healingItr.currentFrame);
		}
	}
}

// ----------------------------------------------------------------------------
//
// Animate healing tiles on current level
void gam_animateHealing (const std::string levelName, float tickTime)
// ----------------------------------------------------------------------------
{
	if (!shipLevel.at (levelName).healing.empty ())
	{
		for (auto &healingItr: shipLevel.at (levelName).healing)
		{
			healingItr.frameDelay -= (healingAnimSpeed * tickTime);
			if (healingItr.frameDelay < 0.0f)
			{
				healingItr.frameDelay = 1.0f;
				healingItr.currentFrame++;
				if (healingItr.currentFrame > HEALING_TILE + 3)
					healingItr.currentFrame = HEALING_TILE;

				shipLevel.at (levelName).tiles[healingItr.pos] = healingItr.currentFrame;
			}
		}
	}
//		gam_AStarProcessPaths (levelItr.first );
}

//----------------------------------------------------------------------------
//
// Free memory and clear healing array
void gam_clearHealing ()
//----------------------------------------------------------------------------
{
	for (auto &healingItr : healing)
	{
		if (healingItr.userData != nullptr)
			delete (healingItr.userData);

		if (healingItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (healingItr.body);
	}
	healing.clear ();
}

//----------------------------------------------------------------------------
//
// Find out where on the level the healing tiles are
// Remember for this level for animation
void gam_findHealingTiles (std::string levelName)
// ----------------------------------------------------------------------------
{
	int           index;
	int           currentTile;
	int           countX, countY;
	int           countHealing;
	_basicHealing tempHealing{};

	for (index = 0; index < shipLevel.at (levelName).levelDimensions.x * shipLevel.at (levelName).levelDimensions.y; index++)
	{
		switch (shipLevel.at (levelName).tiles[index])
		{
			case HEALING_TILE:
			case HEALING_TILE + 1:
			case HEALING_TILE + 2:
			case HEALING_TILE + 3:
				tempHealing.pos          = index;
				tempHealing.currentFrame = HEALING_TILE;
				tempHealing.frameDelay   = 0.0f;
				shipLevel.at (levelName).healing.push_back (tempHealing);
				break;

			default:
				break;
		}
	}

	countHealing = 0;
	countX       = 0;
	countY       = 0;
	for (index   = 0; index < (int) shipLevel.at (levelName).levelDimensions.x * (int) shipLevel.at (levelName).levelDimensions.y; index++)
	{
		currentTile = shipLevel.at (levelName).tiles[((countY * ((int) shipLevel.at (levelName).levelDimensions.x)) + countX)];

		switch (currentTile)
		{
			case HEALING_TILE:
			case HEALING_TILE + 1:
			case HEALING_TILE + 2:
			case HEALING_TILE + 3:
				shipLevel.at (levelName).healing[countHealing].renderPosition.x = countX * TILE_SIZE;
				shipLevel.at (levelName).healing[countHealing].renderPosition.y = countY * TILE_SIZE;

				shipLevel.at (levelName).healing[countHealing].worldPos.x = countX * TILE_SIZE;
				shipLevel.at (levelName).healing[countHealing].worldPos.y = countY * TILE_SIZE;

				countHealing++;
				break;

			default:
				break;
		}

		countX++;

		if (countX == (int) shipLevel.at (levelName).levelDimensions.x)
		{
			countX = 0;
			countY++;
		}
	}
}


