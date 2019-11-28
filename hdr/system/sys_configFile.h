#pragma once

#include "system/sys_main.h"

// Get startup values from config file
bool cfg_getStartupValues();

// Update value in currently open config file
void cfg_setConfigValue(std::string whichKey, std::string newValue);

// Close the open config file
void cfg_closeConfigFile();