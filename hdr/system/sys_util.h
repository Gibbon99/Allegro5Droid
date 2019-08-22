#pragma once

#include "system/sys_main.h"

// Pass in string and parameters to format and return a string
std::string sys_getString (std::string format, ...);

// Allocate memory and return pointer - record the size as well
char *sys_malloc(int memorySize, const std::string keyName);

// Go through memory map and free allocations
void sys_freeMemory();
