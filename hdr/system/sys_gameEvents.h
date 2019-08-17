#pragma once

#include "system/sys_main.h"

extern ALLEGRO_EVENT_QUEUE *eventQueue;

// Process input and game events from queue
void evt_handleEvents();

// Init the event queues
void evt_initEvents();