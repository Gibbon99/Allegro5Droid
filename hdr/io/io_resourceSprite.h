#pragma once

#include "system/sys_main.h"

// Load a sprite
void io_loadResourceSprite(const std::string& key, const std::string& fileName, int numFrames);

// Render a frame of a sprite
void io_renderSpriteFrame(const std::string& key, int whichFrame, float posX, float posY);

// Render a sprite tinting to a different color
// Position is the center of the sprite - offset by frame width and height
void io_renderTintedSpriteFrame (const std::string &key, int whichFrame, float posX, float posY, float red, float green, float blue, float alpha);

// Render a rotated sprite
void io_renderRotatedSpriteFrame(const std::string &key, int whichFrame, float posX, float posY, float angle);

// Set the transparent pixel color for the sprite
void io_spriteSetAlphaColor(const std::string& key, int red, int green, int blue);

// Test animate sprite
void io_animateEnemySprites (const std::string& levelName, double interpolate);

int io_getFrame();