#pragma once

#include "system/sys_main.h"

// Draw a slider element
void gui_drawSlider(int whichSlider, bool hasFocus);

// Set the slider to the passed in value
void gui_hostSetSliderValue(const std::string& objectID, const std::string& value);

// Get the value of the current slot in a slider
std::string gui_hostGetSliderValue(std::string objectID);