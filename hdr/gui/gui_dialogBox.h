#pragma once

#include "system/sys_main.h"

struct __GUI_dialogBox
{
	std::string      title;
	std::string      text;
	int              positionX;
	int              positionY;
	int              width;
	int              height;
	bool             modal;
	int              selectedObject;
	std::vector<int> objectIDIndex;          // Index into object array
	std::vector<int> objectType;             // Which object array
};

extern std::map<std::string, __GUI_dialogBox> dialogBox;
extern bool                                   dialogBoxActive;
extern std::string                            currentDialogBoxName;

// Render a dialog box
void gui_renderDialogBox ();

// Setup a dialog box from the script
void gui_addDialogBox (const std::string &dialogName, std::string title, std::string text, int posX, int posY, bool modal);

// Activate a dialog box
void gui_activateDialogBox (const std::string &dialogName);

// Remove the active dialog box
void gui_removeDialogBox();