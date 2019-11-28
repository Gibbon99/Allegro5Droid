#pragma once

#include "system/sys_main.h"

#define BLOCK_HEIGHT             8
#define BLOCK_WIDTH              4
#define BLOCK_STEP               2

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

extern ALLEGRO_BITMAP *transferGameBitmap;
extern ALLEGRO_COLOR transferColorStatusCell;
extern ALLEGRO_COLOR transferColorBorder;
extern ALLEGRO_COLOR transferColorLeftActive;
extern ALLEGRO_COLOR transferColorRightActive;
extern float activeTokenPosX;

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
	TRANSFER_ROW_LAUNCHER_RIGHT,
	TRANSFER_ROW_LAUNCHER_RIGHT_COLOR,
	TRANSFER_ROW_LAUNCHER_LEFT_COLOR
};

typedef struct
{
	int leftSideType;
	int rightSideType;
	int currentColor;
	float leftSideActiveCounter;
	float rightSideActiveCounter;
	float startX;
	float startY;
	float rightSideActiveAlphaCount;
	float rightSideActiveAlphaColor;
	float leftSideActiveAlphaCount;
	float leftSideActiveAlphaColor;
	bool rightSideActive;
	bool leftSideActive;
	bool rightSideActiveIsOn;
	bool leftSideActiveIsOn;
} __transferRow;

extern std::vector<__transferRow> transferRows;

// Render the current transfer game elements
void trn_renderTransferGame();

// Set a color for the transfer game
void trn_setTransferColor(int whichSide, float red, float green, float blue, float alpha);

// Return the bitmap for the transfer game screen
ALLEGRO_BITMAP *trn_getTransferScreen();

// Return the transfer bitmap width
float trn_getTransferBitmapWidth();

// Return the transfer bitmap height
float trn_getTransferBitmapHeight();
