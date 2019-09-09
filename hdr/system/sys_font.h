#pragma once

#include "system/sys_main.h"

// Init the builtin system font
bool fnt_initSystemFont();

// Clean up the system font
void fnt_destroySystemFont();

// Draw a line of text in systemfont
void fnt_printSystemFont(float posX, float posY, const std::string& textLine);

// Set the current font to the passed in TTF
void fnt_setTTF (std::string fontName);

// Set the current font color - uses floats to get correct alpha colors
void fnt_setColor_f(float red, float green, float blue, float alpha);

// Render a string using the TTF or builtin
void fnt_render (b2Vec2 renderPosition, const std::string &renderText);

// Return the width of a text string using the current font
float fnt_getWidth(const std::string& renderText);

// Get the height of a font
float fnt_getHeight();

// Free the fonts
void fnt_destroyFonts();
