#pragma once

#include "system/sys_main.h"

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_TIMER   *timingTimer;
extern ALLEGRO_TIMER   *fadeTimer;
extern ALLEGRO_TIMER   *splashTimer;

extern bool initResourcesAllDone;
extern int  fullScreen; // Set for checkbox in GUI script

// Run init for system startup
void sys_initAll ();
