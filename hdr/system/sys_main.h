#pragma once

#define ALLEGRO_UNSTABLE 1

#include <string>
#include <vector>
#include <cstdio>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_physfs.h"
#include "allegro5/allegro_audio.h"
#include "allegro5/allegro_acodec.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_memfile.h"
#include "system/sys_wrapper.h"
#include "system/sys_util.h"
#include "data/sharedDefines.h"
#include "Box2D/Box2D.h"

#define TICKS_PER_SECOND  30.0f
#define TILE_SIZE         32.0f

#define NO_PASS_TILE                40
#define LIFT_TILE                   NO_PASS_TILE + 1
#define HEALING_TILE                LIFT_TILE + 4

#define ALERT_GREEN_TILE            4
#define ALERT_YELLOW_TILE           5
#define ALERT_RED_TILE              6

#define TERMINAL_TOP                51
#define TERMINAL_BOTTOM             52
#define TERMINAL_RIGHT              53
#define TERMINAL_LEFT               54

#define HEALING_TILE                LIFT_TILE + 4

#define SPRITE_SIZE   24

#define ERROR_REPEAT_NUMBER 3

#define MAX_DROID_TYPES 23

#define DEGTORAD 0.0174532925199432957f
#define MY_PI 3.1415926535f

enum GAME_MODES {

};

typedef int (*functionPtr) (...);

extern bool   quitProgram;
extern double fps;
extern double thinkFPS;
extern double printFPS;
extern double printThinkFPS;
extern double frameTimePrint;
extern int    windowWidth;
extern int    windowHeight;
extern int    displayRefreshRate;
extern int    screenWidth;
extern int    screenHeight;
extern int    screenType;
extern int    currentMode;

typedef struct {
	float red;
	float green;
	float blue;
	float alpha;
}             __paraColor;

// Change to a new mode
void sys_changeMode (int newMode, bool fade);

// Return logical screen width
float sys_getLogicalWidth ();

// Return the logical screen height
float sys_getLogicalHeight ();

// Return current tickTime
double sys_getTickTime ();