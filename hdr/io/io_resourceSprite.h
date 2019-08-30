#pragma once

#include "system/sys_main.h"

// Load a sprite
void io_loadResourceSprite(std::string key, std::string fileName, int numFrames);

// Render a frame of a sprite
void io_renderSpriteFrame(std::string key, int whichFrame, float posX, float posY);

// Render a sprite tinting to a different color
// Position is the center of the sprite - offset by frame width and height
void io_renderTintedSpriteFrame(std::string key, int whichFrame, float posX, float posY, int red, int green, int blue);

// Set the transparent pixel color for the sprite
void io_spriteSetAlphaColor(std::string key, int red, int green, int blue);

// Test animate sprite
void io_testAnimateSprite();
int io_getFrame();