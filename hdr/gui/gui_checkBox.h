#pragma once

#include "system/sys_main.h"

// Draw a checkbox element
void gui_drawCheckbox(int whichObject, bool hasFocus);

// Go through other members of the group and untick them
void gui_hostSetCheckboxTick(const std::string objectID, int whichGroup, bool ticked);

// Set the group that this checkbox belongs to
void gui_hostSetCheckboxGroup(const std::string objectID, int whichGroup);
