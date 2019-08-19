#pragma once

#include "system/sys_main.h"

// Called from script to load a resource
void sys_loadResource(std::string key, std::string fileName, int type, int numFrames, int size);