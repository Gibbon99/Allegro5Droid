#pragma once

#include "system/sys_main.h"

// Main routine for drawing all the GUI screens
void gui_drawGUI ();

// Draw a key based on passed in color and mode
void gui_drawObject (int objectType, int whichObject, bool hasFocus);