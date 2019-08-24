#pragma once

#include "system/sys_main.h"

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_TIMER   *timingTimer;
extern ALLEGRO_TIMER   *fadeTimer;

extern bool initResourcesAllDone;

// Run init for system startup
void sys_initAll ();
