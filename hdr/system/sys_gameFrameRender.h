#pragma once

#include "system/sys_main.h"
#include <hdr/system/sys_font.h>

extern float scaleW;
extern float scaleH;
extern float scaleX;
extern float scaleY;

// Render a frame once
void sys_displayScreen(double interpolation);

// Push a value onto the frametime Queue
void sys_pushFrameTimeIntoQueue(double thisFrameTime, float factor);

// Init render variables
bool sys_initRenderVariables();