#pragma once

#include <hdr/Box2D/Common/b2Math.h>
#include <hdr/gui/gui_main.h>
#include "system/sys_main.h"

// Pass in string and parameters to format and return a string
std::string sys_getString (std::string format, ...);

// Allocate memory and return pointer - record the size as well
char *sys_malloc(int memorySize, const std::string keyName);

// Go through memory map and free allocations
void sys_freeMemory();

// Heads towards destination
b2Vec2 sys_getDirection ( b2Vec2 sourcePoint, b2Vec2 destPoint );

// Convert worldPosition coords to screen coords
b2Vec2 sys_worldToScreen ( b2Vec2 worldPos, int shapeSize );

// Update the visible screen area bounding box
void sys_updateVisibleScreenArea();

// Is an object visible on the screen
bool sys_visibleOnScreen (b2Vec2 worldCoord, int shapeSize);

// Test if a point is inside a rectangle
bool sys_isPointInRect(b2Vec2 testPoint, __BOUNDING_BOX testRectangle);

// Get a random number capped at parameter
int sys_getCappedRandomNum (int cap);

// Convert a bool to string for script
std::string sys_boolToString(bool boolValue);

// Convert a int to a string for script
std::string sys_intToString(int intValue);