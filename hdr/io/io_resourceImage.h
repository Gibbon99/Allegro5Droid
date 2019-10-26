#pragma once

#include "system/sys_main.h"

extern bool useHoldBitmap;

// Draw a bitmap
void sys_drawBitmap (const std::string &keyName, float posX, float posY, int drawMode, float scaleX, float scaleY);

// Load a bitmap file
void sys_loadResourceBitmap (const std::string &key, const std::string &fileName);

// Get height of image
int sys_getImageHeight(const std::string& keyName);

// Get width of image
int sys_getImageWidth(const std::string& keyName);

