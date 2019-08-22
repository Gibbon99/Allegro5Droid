#pragma once

#include <system/sys_main.h>

enum consoleColors
{
	WHITE = 0,
	RED,
	GREEN,
	BLUE,
	BLACK
};

typedef struct
{
	std::string conLine;
	__paraColor conLineColor;
} _conLine;

extern std::vector<_conLine> conLines;
extern _conLine              conCurrentLine;

extern bool isDoneConsole;

// Push an event onto the console queue
void con_pushNewEvent(union PARA_EVENT newEvent);

// Process the events put onto the console queue - run by detached thread
void *con_processConsoleEventQueue();

// Init the console system
void con_initConsole();

// Stop the console
void con_stopConsole();

// Draw the contents of the console
void con_renderConsole();

// Set the number of visible console lines to render on one screen
void gam_setVisibleConLines (int fontHeight);