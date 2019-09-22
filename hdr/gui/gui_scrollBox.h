#pragma once

#include "system/sys_main.h"

extern float introScrollBoxStartX;
extern float introScrollBoxStartY;
extern float introScrollBoxWidth;
extern float introScrollBoxHeight;
extern float introScrollBoxSpeed;
extern float introScrollBoxGapSize;
extern float introScrollBoxBackRed;
extern float introScrollBoxBackGreen;
extern float introScrollBoxBackBlue;
extern float introScrollBoxBackAlpha;
extern float introScrollBoxFontRed;
extern float introScrollBoxFontGreen;
extern float introScrollBoxFontBlue;
extern float introScrollBoxFontAlpha;
extern float introScrollBoxRadius;

extern float databaseScrollBoxStartX;
extern float databaseScrollBoxStartY;
extern float databaseScrollBoxWidth;
extern float databaseScrollBoxHeight;
extern float databaseScrollBoxSpeed;
extern float databaseScrollBoxGapSize;
extern float databaseScrollBoxBackRed;
extern float databaseScrollBoxBackGreen;
extern float databaseScrollBoxBackBlue;
extern float databaseScrollBoxBackAlpha;
extern float databaseScrollBoxFontRed;
extern float databaseScrollBoxFontGreen;
extern float databaseScrollBoxFontBlue;
extern float databaseScrollBoxFontAlpha;
extern float databaseScrollBoxRadius;

// Setup a scrollbox with initial values
void gui_setupScrollBox(int whichScrollBox, const std::string& sourceText);


// Move the scrollbox, or pause if required
void gui_scrollScrollBox (__GUI_object *thisScrollBox, double tickTime);

// Render a scrollbox
void gui_renderScrollBox (__GUI_object *thisScrollBox, double interpolation);