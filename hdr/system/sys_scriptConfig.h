#pragma once

#include "system/sys_main.h"
#include "system/sys_scriptConfig.h"
#include "system/sys_scriptEngine.h"
#include "system/sys_scriptFunctions.h"

// Setup all the variables to be used
void sys_scriptInitVariables();

// Setup all the functions to be used
void sys_scriptInitFunctions();

// This is how we call a script from the Host program
void sys_scriptInitScriptFunctions();
