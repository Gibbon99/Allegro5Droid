#pragma once

#include "system/sys_main.h"

struct __GUI_messageBox
{
	int objectID;
	std::string title;
	std::string text;
	int positionX;
	int positionY;
	int width;
	int height;
	bool modal;
};

extern std::map<int, __GUI_messageBox> messageBox;

// Render a message box
void gui_renderMessageBox(int objectID);

// Setup a message box from the script
void gui_addMessageBox(int boxName, std::string title, std::string text, int posX, int posY, bool modal);
