#pragma once

#include "system/sys_main.h"

// Setup all the timers and add to event source
bool tim_initAllTimers ();

// The fade is run from a timer thread
void tim_runFadeProcess(ALLEGRO_TIMER_EVENT *timer_event);

// Called once a second to reset timing counters
void tim_tickTimers (ALLEGRO_TIMER_EVENT *timer_event);