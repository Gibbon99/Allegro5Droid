#pragma once

#include "system/sys_main.h"

// Init the builtin system font
bool fnt_initSystemFont();

// Clean up the system font
void fnt_destroySystemFont();

// Draw a line of text in systemfont
void fnt_printSystemFont(float posX, float posY, std::string textLine);