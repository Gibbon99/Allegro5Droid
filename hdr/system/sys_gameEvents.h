#pragma once

#include "system/sys_main.h"

extern ALLEGRO_EVENT_QUEUE *eventQueue;

// Process input and game events from queue
void evt_handleEvents();

// Init the event queues
void evt_initEvents();

// Init the Game Loop thread event queue
void evt_initGameLoopQueue();

// Process the events put onto the GameLoop queue - run by detached thread
void *evt_processGameEventQueue ();

