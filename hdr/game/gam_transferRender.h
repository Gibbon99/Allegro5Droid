#pragma once

#include "system/sys_main.h"

extern int transferBorderThickness;
extern int numberTransferRows;
extern float transferRowHeight;
extern float transferRowCellWidth;
extern float transferRowStartY;
extern float transferBackgroundStartX;
extern float transferBackgroundStartY;
extern float transferBackgroundWidth;
extern float transferBackgroundHeight;
extern float transferStatusTabWidth;
extern float transferStatusTabHeight;
extern float transferSidebarGap;
extern float transferSidebarWidth;

extern int transferPlayerWhichSide;
extern float transferLineThickness;

extern int transferBitmapWidth;
extern int transferBitmapHeight;

enum rowTypes
{
	TRANSFER_ROW_FULL_LINE = 1,
	TRANSFER_ROW_HALF_LINE,
	TRANSFER_ROW_3_4_LINE,
	TRANSFER_ROW_FULL_LINE_1,
	TRANSFER_ROW_QUARTER_LINE,
	TRANSFER_ROW_REPEAT_HALF,
	TRANSFER_ROW_REPEAT_QUARTER,
	TRANSFER_ROW_FULL_LINE_2,
	TRANSFER_ROW_REVERSE_HALF,
	TRANSFER_ROW_REVERSE_QUARTER,
	TRANSFER_ROW_TWO_INTO_ONE_MIDDLE,
	TRANSFER_ROW_ONE_INTO_TWO_MIDDLE,
	TRANSFER_ROW_FULL_LINE_3,
	TRANSFER_ROW_ONE_INTO_TWO_TOP,
	TRANSFER_ROW_ONE_INTO_TWO_BOTTOM,
	TRANSFER_ROW_TWO_INTO_ONE_TOP,
	TRANSFER_ROW_TWO_INTO_ONE_BOTTOM,
	TRANSFER_ROW_LAUNCHER_LEFT,
	TRANSFER_ROW_LAUNCHER_RIGHT
};

typedef struct
{
	int leftSideType;
	int rightSideType;
	int currentColor;
	int leftSideActiveCounter;
	int rightSideActiveCounter;
	float startX;
	float startY;
} __transferRow;

extern std::vector<__transferRow> transferRows;

// Setup the default values for the transfer rows
void trn_initTransferValues();

// Render the current transfer game elements
void trn_renderTransferGame();

// Set a color for the transfer game
void trn_setTransferColor(int whichSide, float red, float green, float blue, float alpha);

// Return the bitmap for the tranfer game screen
ALLEGRO_BITMAP *trn_getTransferScreen();

// Return the transfer bitmap width
float trn_getTransferBitmapWidth();

// Return the transfer bitmap height
float trn_getTransferBitmapHeight();