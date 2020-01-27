#pragma once

#include "system/sys_main.h"

extern int splashTimeout;

// Setup all the timers and add to event source
bool tim_initAllTimers ();

// The fade is run from a timer thread
void tim_runFadeProcess(ALLEGRO_TIMER_EVENT *timer_event);

// Called once a second to reset timing counters
void tim_tickTimers (ALLEGRO_TIMER_EVENT *timer_event);

// Function that is called when the splash screentimeout is reached
void tim_changeToGUI(ALLEGRO_TIMER_EVENT *timer_event);

// Function to animate the deckview location icon
void tim_animateLocationIcon(ALLEGRO_TIMER_EVENT *time_event);

// Return the value of the Icon visible flag
bool tim_getIconState();