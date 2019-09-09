#include <map>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_doors.h>
#include <hdr/gui/gui_main.h>
#include "hdr/system/sys_util.h"

typedef struct
{
	char *pointer;
	int  size;
} _memoryMap;

std::map<std::string, _memoryMap> memoryMap;
b2AABB                            visibleScreenArea;

//----------------------------------------------------------------------------------------------------------------------
//
// Test if a point is inside a rectangle
bool sys_isPointInRect(b2Vec2 testPoint, __BOUNDING_BOX testRectangle)
//----------------------------------------------------------------------------------------------------------------------
{
	if (testPoint.x < testRectangle.x)
		return false;
	if (testPoint.y < testRectangle.y)
		return false;
	if (testPoint.x > testRectangle.x + testRectangle.w)
		return false;
	if (testPoint.y > testRectangle.y + testRectangle.h)
		return false;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string sys_getString(std::string format, ...)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *const zcFormat = format.c_str();

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, format);

	// reliably acquire the size from a copy of the variable argument array
	// and a functionally reliable call to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf(nullptr, 0, zcFormat, vaCopy);
	va_end(vaCopy);

	// return a formatted string without risking memory mismanagement  and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc(iLen + 1);
	std::vsnprintf(zc.data(), zc.size(), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string(zc.data());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Allocate memory and return pointer - record the size as well
char *sys_malloc(int memorySize, const std::string keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	_memoryMap newMemoryMapEntry;

	newMemoryMapEntry.pointer = (char *) malloc(sizeof(char) * memorySize);
	if (newMemoryMapEntry.pointer == nullptr)
	{
		log_logMessage(LOG_LEVEL_EXIT, sys_getString("Memory allocation error for [ %s ]", keyName.c_str()));
	}

	newMemoryMapEntry.size = memorySize;

	memoryMap.insert(std::pair<std::string, _memoryMap>(keyName, newMemoryMapEntry));

	return newMemoryMapEntry.pointer;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Go through memory map and free allocations
void sys_freeMemory()
//----------------------------------------------------------------------------------------------------------------------
{
	if (memoryMap.empty())
	{
		return;
	}

	for (auto &memoryItr : memoryMap)
	{
		printf("%s\n", sys_getString("Free memory [ %i bytes ] - [ %s ]", memoryItr.second.size, memoryItr.first.c_str()).c_str());

		if (memoryItr.second.pointer != nullptr)
		{
			free(memoryItr.second.pointer);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Heads towards destination
b2Vec2 sys_getDirection(b2Vec2 sourcePoint, b2Vec2 destPoint)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempDirection;

	tempDirection = sourcePoint - destPoint;
	tempDirection.Normalize();

	return tempDirection;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Update the visible screen area bounding box
void sys_updateVisibleScreenArea()
//----------------------------------------------------------------------------------------------------------------------
{
	visibleScreenArea.lowerBound.x = playerDroid.worldPos.x - (screenWidth * 0.5);
	visibleScreenArea.lowerBound.y = playerDroid.worldPos.y - (screenHeight * 0.5);
	visibleScreenArea.upperBound.x = playerDroid.worldPos.x + (screenWidth * 0.5);
	visibleScreenArea.upperBound.y = playerDroid.worldPos.y + (screenHeight * 0.5);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Is an object visible on the screen
bool sys_visibleOnScreen(b2Vec2 worldCoord, int shapeSize)
//----------------------------------------------------------------------------------------------------------------------
{
	static int previousShapeSize = -1;
	b2AABB     tempVisibleScreenArea;

	if (previousShapeSize != shapeSize)
	{
		tempVisibleScreenArea = visibleScreenArea;
		tempVisibleScreenArea.lowerBound.x -= shapeSize;
		tempVisibleScreenArea.lowerBound.y -= shapeSize;
		tempVisibleScreenArea.upperBound.x += shapeSize;
		tempVisibleScreenArea.upperBound.x += shapeSize;
		previousShapeSize = shapeSize;

		if (worldCoord.x < tempVisibleScreenArea.lowerBound.x)
		{
			return false;
		}
		if (worldCoord.x > tempVisibleScreenArea.upperBound.x)
		{
			return false;
		}
		if (worldCoord.y < tempVisibleScreenArea.lowerBound.y)
		{
			return false;
		}
		if (worldCoord.y > tempVisibleScreenArea.upperBound.y)
		{
			return false;
		}

		return true;
	}
	if (worldCoord.x < visibleScreenArea.lowerBound.x - shapeSize)
	{
		return false;
	}
	if (worldCoord.x > visibleScreenArea.upperBound.x + shapeSize)
	{
		return false;
	}
	if (worldCoord.y < visibleScreenArea.lowerBound.y - shapeSize)
	{
		return false;
	}
	if (worldCoord.y > visibleScreenArea.upperBound.y + shapeSize)
	{
		return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Convert worldPosition coords to screen coords
b2Vec2 sys_worldToScreen(b2Vec2 worldPos, int shapeSize)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 screenCoords;

	if (sys_visibleOnScreen(worldPos, shapeSize) != 0)      // TODO - check why it's returning -1 -1
	{
		screenCoords.x = worldPos.x - (playerDroid.worldPos.x - (screenWidth / 2));
		screenCoords.y = worldPos.y - (playerDroid.worldPos.y - (screenHeight / 2));
	}
	else
	{
		screenCoords.x = -1;
		screenCoords.y = -1;

		screenCoords.x = worldPos.x - (playerDroid.worldPos.x - (screenWidth / 2));
		screenCoords.y = worldPos.y - (playerDroid.worldPos.y - (screenHeight / 2));
	}
	return screenCoords;
}