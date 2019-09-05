#include <hdr/io/io_resourceLevel.h>
#include <hdr/io/io_logFile.h>
#include "hdr/game/gam_doors.h"

std::vector<_doorTrigger> doorTriggers;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle door sensor trigger
void gam_handleDoorTrigger(int whichDoor, int state)
//----------------------------------------------------------------------------------------------------------------------
{
	if (state == 0)
		printf("Enter Trigger area for door [ %i ]\n", whichDoor);

	if (state == 1)
		printf("Exit Trigger area for door [ %i ]\n", whichDoor);
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

	doorTriggers[whichDoor].userData = new _userData;
	doorTriggers[whichDoor].userData->userType = PHYSIC_TYPE_DOOR;
	doorTriggers[whichDoor].userData->dataValue = (int)whichDoor;
	doorTriggers[whichDoor].body->SetUserData(doorTriggers[whichDoor].userData);

	doorTriggers[whichDoor].shape.SetAsBox((doorTriggers[whichDoor].height  * 0.5f) / pixelsPerMeter, (doorTriggers[whichDoor].width * 0.5f) / pixelsPerMeter);
	doorTriggers[whichDoor].fixtureDef.shape = &doorTriggers[whichDoor].shape;
	doorTriggers[whichDoor].fixtureDef.isSensor = true;
	doorTriggers[whichDoor].body->CreateFixture(&doorTriggers[whichDoor].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the doors for this level and prepare a sensor objecty
void gam_doorTriggerSetup(const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	int   i           = 0;
	int   currentTile = 0;
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
			case DOOR_ACROSS_OPEN_1:
			case DOOR_ACROSS_OPEN_2:
			case DOOR_ACROSS_OPENED:
			case DOOR_ACROSS_CLOSING_1:
			case DOOR_ACROSS_CLOSING_2:
			case DOOR_ACROSS_CLOSED:
			{
				tempDoorTrigger.height          = TILE_SIZE;
				tempDoorTrigger.width           = TILE_SIZE * 2;
				tempDoorTrigger.worldPosition.x = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.worldPosition.y = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.currentFrame    = DOOR_ACROSS_CLOSED;
				tempDoorTrigger.tileIndex       = i;

				doorTriggers.push_back(tempDoorTrigger);

				gam_createDoorSensor(doorTriggers.size() - 1, i);
				break;
			}

			case DOOR_UP_OPEN_1:
			case DOOR_UP_OPEN_2:
			case DOOR_UP_OPENED:
			case DOOR_UP_CLOSING_1:
			case DOOR_UP_CLOSING_2:
			case DOOR_UP_CLOSED:
			{
				tempDoorTrigger.height          = TILE_SIZE * 2;
				tempDoorTrigger.width           = TILE_SIZE;
				tempDoorTrigger.worldPosition.x = ((sourceX * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.worldPosition.y = ((sourceY * TILE_SIZE) + (TILE_SIZE / 2));
				tempDoorTrigger.currentFrame    = DOOR_UP_CLOSED;
				tempDoorTrigger.tileIndex       = i;

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
