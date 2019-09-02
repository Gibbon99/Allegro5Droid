#pragma once

#include <hdr/Box2D/Common/b2Math.h>
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

// Put four bytes ( chars ) into one int value
int sys_pack4Bytes(char one, char two, char three, char four);

// Returns 4 char bytes into passed in array from INT parameter
void sys_getPackedBytes(int sourceNumber, unsigned char *returnArray);
