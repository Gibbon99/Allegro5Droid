#pragma once

#include "system/sys_main.h"
#include <hdr/system/sys_font.h>

enum fadeValues
{
	FADE_ON = 0,
	FADE_OFF,
	FADE_NONE
};

extern bool           showDebugPhysics;
extern float          scaleW;
extern float          scaleH;
extern float          scaleX;
extern float          scaleY;
extern ALLEGRO_BITMAP *backingBitmap;
extern ALLEGRO_BITMAP *previousScreen;
extern float          fadeAlphaValue;
extern int            fadeInProgress;

// Render a frame once
void sys_displayScreen (double interpolation);

// Push a value onto the frametime Queue
void sys_pushFrameTimeIntoQueue (double thisFrameTime, float factor);

// Init render variables
bool sys_initRenderVariables ();