#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_render.h>
#include "hdr/game/gam_doors.h"

std::vector<_doorTrigger> doorTriggers;
float                     doorAnimSpeed = 1.0f;         // From script

//----------------------------------------------------------------------------------------------------------------------
//
// Process the animation of a door
void gam_animateDoor(int whichDoor, int state)
//----------------------------------------------------------------------------------------------------------------------
{
	int doorDelayTime = 0;

	doorTriggers[whichDoor].frameDelay -= 5 * (1.0f / TICKS_PER_SECOND);
	if (doorTriggers[whichDoor].frameDelay > 0.0f)
	{
		evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, state, "");
		return;
	}
	doorTriggers[whichDoor].frameDelay = doorAnimSpeed;

	if (state == GAME_DOOR_STATE_ENTER)
	{
		switch (doorTriggers[whichDoor].currentFrame)
		{
			case DOOR_ACROSS_CLOSED:
				doorTriggers[whichDoor].currentFrame = DOOR_ACROSS_OPEN_1;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
				break;

			case DOOR_ACROSS_OPEN_1:
				doorTriggers[whichDoor].currentFrame = DOOR_ACROSS_OPEN_2;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
				break;

			case DOOR_ACROSS_OPEN_2:
				doorTriggers[whichDoor].currentFrame = DOOR_ACROSS_OPENED;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
				break;

			case DOOR_UP_CLOSED:
				doorTriggers[whichDoor].currentFrame = DOOR_UP_OPEN_1;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
				break;

			case DOOR_UP_OPEN_1:
				doorTriggers[whichDoor].currentFrame = DOOR_UP_OPEN_2;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
				break;

			case DOOR_UP_OPEN_2:
				doorTriggers[whichDoor].currentFrame = DOOR_UP_OPENED;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
				break;
		}
	}

	if (state == GAME_DOOR_STATE_EXIT)
	{
		if (doorTriggers[whichDoor].numberUsing > 0)
		{
			return;
		}

		switch (doorTriggers[whichDoor].currentFrame)
		{
			case DOOR_ACROSS_OPENED:
				doorTriggers[whichDoor].currentFrame = DOOR_ACROSS_CLOSING_1;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
				break;

			case DOOR_ACROSS_CLOSING_1:
				doorTriggers[whichDoor].currentFrame = DOOR_ACROSS_CLOSING_2;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
				break;

			case DOOR_ACROSS_CLOSING_2:
				doorTriggers[whichDoor].currentFrame = DOOR_ACROSS_CLOSED;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
				break;

			case DOOR_UP_OPENED:
				doorTriggers[whichDoor].currentFrame = DOOR_UP_CLOSING_1;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
				break;

			case DOOR_UP_CLOSING_1:
				doorTriggers[whichDoor].currentFrame = DOOR_UP_CLOSING_2;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
				break;

			case DOOR_UP_CLOSING_2:
				doorTriggers[whichDoor].currentFrame = DOOR_UP_CLOSED;
				shipLevel.at(lvl_getCurrentLevelName()).tiles[doorTriggers[whichDoor].tileIndex] = doorTriggers[whichDoor].currentFrame;
				evt_pushEvent(doorDelayTime, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
				break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render current door frames onto map
void gam_renderDoorFrames()
//----------------------------------------------------------------------------------------------------------------------
{
	al_set_target_bitmap(gam_getCompleteLevel());

	for (const auto& doorIndex : doorTriggers)
	{
		if (sys_visibleOnScreen(doorIndex.renderPosition, TILE_SIZE))
		{
			gam_drawSingleTile(doorIndex.renderPosition.x, doorIndex.renderPosition.y, doorIndex.currentFrame);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle door sensor trigger
void gam_handleDoorTrigger(int whichDoor, int state)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (state)
	{
		case GAME_DOOR_STATE_ENTER:
			doorTriggers[whichDoor].numberUsing++;
			evt_pushEvent(0, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_ENTER, "");
			// play door sound
			break;

		case GAME_DOOR_STATE_EXIT:
			doorTriggers[whichDoor].numberUsing--;
			evt_pushEvent(0, PARA_EVENT_GAME, GAME_EVENT_DOOR_ANIMATE, whichDoor, GAME_DOOR_STATE_EXIT, "");
			// play door sound
			break;
	}

}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a door sensor
void gam_createDoorSensor(unsigned long whichDoor, int index)
//----------------------------------------------------------------------------------------------------------------------
{
	doorTriggers[whichDoor].bodyDef.type = b2_staticBody;
	doorTriggers[whichDoor].bodyDef.position.Set(
			doorTriggers[whichDoor].worldPosition.x / pixelsPerMeter,
			doorTriggers[whichDoor].worldPosition.y / pixelsPerMeter);
	doorTriggers[whichDoor].body = sys_getPhysicsWorld()->CreateBody(&doorTriggers[whichDoor].bodyDef);

	doorTriggers[whichDoor].userData            = new _userData;
	doorTriggers[whichDoor].userData->userType  = PHYSIC_TYPE_DOOR;
	doorTriggers[whichDoor].userData->dataValue = (int) whichDoor;
	doorTriggers[whichDoor].body->SetUserData(doorTriggers[whichDoor].userData);

	doorTriggers[whichDoor].shape.SetAsBox(
			(doorTriggers[whichDoor].height * 0.5f) / pixelsPerMeter,
			(doorTriggers[whichDoor].width * 0.5f) / pixelsPerMeter);
	doorTriggers[whichDoor].fixtureDef.shape    = &doorTriggers[whichDoor].shape;
	doorTriggers[whichDoor].fixtureDef.isSensor = true;
	doorTriggers[whichDoor].body->CreateFixture(&doorTriggers[whichDoor].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the doors for this level and prepare a sensor object
void gam_doorTriggerSetup(const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	int i           = 0;
	int currentTile = 0;
	int sourceX     = 0;
	int sourceY     = 0;

	_doorTrigger tempDoorTrigger;

	if (!doorTriggers.empty())  // Empty out array
	{
		doorTriggers.clear();
	}

	sourceX = 0.0f;
	sourceY = 0.0f;
	for (i  = 0; i != shipLevel.at(levelName).levelDimensions.x * shipLevel.at(levelName).levelDimensions.y; i++)
	{
		currentTile = shipLevel.at(levelName).tiles[i];
		if (currentTile < 0)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Tile in doorTriggerSetup is invalid - Tile [ %i ]", i));
		}

		switch (currentTile)
		{
			case DOOR_ACROSS_CLOSED:
			case DOOR_ACROSS_OPEN_1:
			case DOOR_ACROSS_OPEN_2:
			case DOOR_ACROSS_OPENED:
			case DOOR_ACROSS_CLOSING_1:
			case DOOR_ACROSS_CLOSING_2:
			{
				tempDoorTrigger.height           = TILE_SIZE;
				tempDoorTrigger.width            = TILE_SIZE * 2;
				tempDoorTrigger.worldPosition.x  = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.worldPosition.y  = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.renderPosition.x = sourceX * TILE_SIZE;
				tempDoorTrigger.renderPosition.y = sourceY * TILE_SIZE;
				tempDoorTrigger.tileIndex        = i;
				tempDoorTrigger.numberUsing      = 0;
				tempDoorTrigger.frameDelay       = doorAnimSpeed;

				tempDoorTrigger.currentFrame = DOOR_ACROSS_CLOSED;

				doorTriggers.push_back(tempDoorTrigger);

				gam_createDoorSensor(doorTriggers.size() - 1, i);
				break;
			}

			case DOOR_UP_CLOSED:
			case DOOR_UP_OPEN_1:
			case DOOR_UP_OPEN_2:
			case DOOR_UP_OPENED:
			case DOOR_UP_CLOSING_1:
			case DOOR_UP_CLOSING_2:
			{
				tempDoorTrigger.height           = TILE_SIZE * 2;
				tempDoorTrigger.width            = TILE_SIZE;
				tempDoorTrigger.worldPosition.x  = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.worldPosition.y  = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.renderPosition.x = sourceX * TILE_SIZE;
				tempDoorTrigger.renderPosition.y = sourceY * TILE_SIZE;
				tempDoorTrigger.tileIndex        = i;
				tempDoorTrigger.numberUsing      = 0;
				tempDoorTrigger.frameDelay       = doorAnimSpeed;

				tempDoorTrigger.currentFrame = DOOR_UP_CLOSED;

				doorTriggers.push_back(tempDoorTrigger);

				gam_createDoorSensor(doorTriggers.size() - 1, i);
				break;
			}

			default:
				break;
		}
		sourceX++;
		if (sourceX == (int) shipLevel.at(levelName).levelDimensions.x)
		{
			sourceX = 0;
			sourceY++;
		}
	}
}
