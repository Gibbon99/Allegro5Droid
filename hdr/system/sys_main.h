#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_primitives.h"
#include "sys_util.h"

#define TICKS_PER_SECOND 30.0f
#define MAX_FRAMESKIP  5

extern bool   quitProgram;
extern double fps;
extern double thinkFPS;
extern double printFPS;
extern double printThinkFPS;
extern double frameTimePrint;
extern int    windowWidth;
extern int    windowHeight;
extern int    displayRefreshRate;