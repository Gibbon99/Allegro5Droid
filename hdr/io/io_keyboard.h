#pragma once

#include "system/sys_main.h"

enum keyActions
{
	gameLeft = 1,
	gameRight,
	gameDown,
	gameUp,
	gameAction,
	gamePause,
	gameEscape,
	consoleAction,
	gameScreenShot,
	NUMBER_ACTIONS
};

typedef struct {

	std::string text;
	int         keyValue;
	bool        currentlyPressed;
} __KeyBindings;

extern __KeyBindings    keyBinding[NUMBER_ACTIONS];

// Set the default values for keybindings
void io_setDefaultKeybindings();

// Assign the text description for each key
void io_setKeyDescription();