#pragma once

#include "system/sys_main.h"

//-----------------------------------------------------------------------------
//
// sideview of ship stuff - used for lifts and database view
//
//-----------------------------------------------------------------------------

#define MAX_LEVELS            29
#define NUM_OF_TUNNELS        8

//-----------------------------------------------------------------------------
//
// Structure for sideview of ship
//
//-----------------------------------------------------------------------------

struct _basicTunnel
{
	int top;
	int bottom;
	int current;
	int current_deck;
	int decks[6];
};

extern _basicTunnel tunnel[NUM_OF_TUNNELS];

struct _sideviewBasicLevel
{
	int x1;
	int y1;
	int x2;
	int y2;
	int up;
	int down;
};

struct _sideviewColors
{
	PARA_COLOR color;
};

extern float               sideviewDrawScale;
extern int                 currentTunnel;
extern int                 currentDeckNumber;
extern _sideviewBasicLevel sideviewLevels[MAX_LEVELS];

// load the sideview data from the external file
bool gui_loadSideViewData(const std::string sideviewFileName);

// Create a color into the sideview array
void gui_createSideViewColor(int index, int red, int green, int blue, int alpha);

// Show the ship in its sideview on the screen
void gui_drawSideView();