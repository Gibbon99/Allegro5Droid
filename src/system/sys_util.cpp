#include <map>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_player.h>
#include "hdr/system/sys_util.h"

typedef struct
{
	char *pointer;
	int  size;
} _memoryMap;

std::map<std::string, _memoryMap> memoryMap;
cpBB                              visibleScreenArea;

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
cpVect sys_getDirection(cpVect sourcePoint, cpVect destPoint)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect tempDirection;

	tempDirection = cpvsub(sourcePoint, destPoint);
	tempDirection = cpvnormalize(tempDirection);

	return tempDirection;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Update the visible screen area bounding box
void sys_updateVisibleScreenArea()
//----------------------------------------------------------------------------------------------------------------------
{
	visibleScreenArea.l = playerDroid.worldPos.x - (screenWidth * 0.5);
	visibleScreenArea.b = playerDroid.worldPos.y + (screenHeight * 0.5);
	visibleScreenArea.r = playerDroid.worldPos.x + (screenWidth * 0.5);
	visibleScreenArea.t = playerDroid.worldPos.y - (screenHeight * 0.5);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Is an object visible on the screen
cpBool sys_visibleOnScreen(cpVect worldCoord, int shapeSize)
//----------------------------------------------------------------------------------------------------------------------
{
	static int previousShapeSize = -1;
	cpBB       tempVisibleScreenArea;

	if (previousShapeSize != shapeSize)
	{
		tempVisibleScreenArea = visibleScreenArea;
		tempVisibleScreenArea.l -= shapeSize;
		tempVisibleScreenArea.b += shapeSize;
		tempVisibleScreenArea.r += shapeSize;
		tempVisibleScreenArea.t -= shapeSize;
		previousShapeSize = shapeSize;
		return !cpBBContainsVect(tempVisibleScreenArea, worldCoord);
	}
	return !cpBBContainsVect(visibleScreenArea, worldCoord);
}


//----------------------------------------------------------------------------------------------------------------------
//
// Convert worldPosition coords to screen coords
cpVect sys_worldToScreen(cpVect worldPos, int shapeSize)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect screenCoords;

	if (sys_visibleOnScreen(worldPos, shapeSize) != 0)
	{
		screenCoords.x = worldPos.x - (playerDroid.worldPos.x - (screenWidth / 2));
		screenCoords.y = worldPos.y - (playerDroid.worldPos.y - (screenHeight / 2));
	}
	else
	{
		screenCoords.x = -1;
		screenCoords.y = -1;
	}
	return screenCoords;
}


//----------------------------------------------------------------------------------------------------------------------
//
// Put four bytes ( chars ) into one int value
int sys_pack4Bytes(char one, char two, char three, char four)
//----------------------------------------------------------------------------------------------------------------------
{
	int returnValue = 0;

	returnValue <<= 8;
	returnValue |= one;
	returnValue <<= 8;
	returnValue |= two;
	returnValue <<= 8;
	returnValue |= three;
	returnValue <<= 8;
	returnValue |= four;

	return returnValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Returns 4 char bytes into passed in array from INT parameter
void sys_getPackedBytes(int sourceNumber, unsigned char *returnArray)
//----------------------------------------------------------------------------------------------------------------------
{
	returnArray[0] = static_cast<unsigned char>((sourceNumber >> 24) & 0xff);
	returnArray[1] = static_cast<unsigned char>((sourceNumber >> 16) & 0xff);
	returnArray[2] = static_cast<unsigned char>((sourceNumber >> 8) & 0xff);
	returnArray[3] = static_cast<unsigned char>(sourceNumber & 0xff);
}