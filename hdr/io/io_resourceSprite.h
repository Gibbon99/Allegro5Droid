#pragma once

#include "system/sys_main.h"

// Load a sprite
void io_loadResourceSprite(std::string key, std::string fileName, int numFrames);

// Render a frame of a sprite
void io_renderSpriteFrame(std::string key, int whichFrame, float posX, float posY);


// Test animate sprite
void io_testAnimateSprite();
int io_getFrame();