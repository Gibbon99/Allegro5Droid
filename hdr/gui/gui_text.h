#pragma once

#include "system/sys_main.h"

extern int		currentLanguage;

// Return a string based on the key
std::string gui_getString(const std::string key);

// Add a key and string to the map
void gui_addKeyAndText(const std::string key, const std::string textString);