#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_primitives.h"
#include "sys_util.h"

#define TICKS_PER_SECOND 30
#define MAX_FRAMESKIP  5

extern bool         quitProgram;
extern unsigned int fps;
extern unsigned int thinkFPS;
extern unsigned int printFPS;
extern unsigned int printThinkFPS;
extern double       frameTimePrint;
extern double       frameTime;
extern int          windowWidth;
extern int          windowHeight;
