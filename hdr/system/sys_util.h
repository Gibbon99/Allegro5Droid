#pragma once

#include <hdr/chipmunk/cpVect.h>
#include "system/sys_main.h"

// Pass in string and parameters to format and return a string
std::string sys_getString (std::string format, ...);

// Allocate memory and return pointer - record the size as well
char *sys_malloc(int memorySize, const std::string keyName);

// Go through memory map and free allocations
void sys_freeMemory();

// Heads towards destination
cpVect sys_getDirection ( cpVect sourcePoint, cpVect destPoint );

// Convert worldPosition coords to screen coords
cpVect sys_worldToScreen ( cpVect worldPos, int shapeSize );

// Is an object visible on the screen
bool sys_visibleOnScreen (cpVect worldCoord, int shapeSize);

// Put four bytes ( chars ) into one int value
int sys_pack4Bytes(char one, char two, char three, char four);