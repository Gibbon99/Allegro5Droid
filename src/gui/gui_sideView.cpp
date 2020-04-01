#include <hdr/io/io_fileSystem.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_game.h>
#include <hdr/system/sys_font.h>
#include "hdr/gui/gui_sideView.h"

_basicTunnel        tunnel[NUM_OF_TUNNELS];
_sideviewBasicLevel sideviewLevels[MAX_LEVELS];
_sideviewColors     sideviewColors[SIDEVIEW_NUM_COLORS];
float               drawOffsetX       = 0;
float               sideviewDrawScale;      // From Script
int                 currentDeckNumber = -1;
int                 currentTunnel     = 0;
float               sideviewDrawScaleScript;
float               sideViewTextPosX, sideViewTextPosY;

// ----------------------------------------------------------------------------
//
// Wrap drawing a rectangle into our own function
void gui_sideviewDrawRect (float x1, float y1, float x2, float y2, PARA_COLOR whichColor)
// ----------------------------------------------------------------------------
{
	al_draw_filled_rounded_rectangle (x1 - drawOffsetX, y1, x2 - drawOffsetX, y2, 1, 1, al_map_rgba (whichColor.r, whichColor.g, whichColor.b, whichColor.a));

	al_draw_rounded_rectangle (x1 - drawOffsetX, y1, x2 - drawOffsetX, y2, 1, 1, al_map_rgba (0, 0, 0, 255), 1);
}

//-----------------------------------------------------------------------------
//
// Create a color into the sideview array
void gui_createSideViewColor (int index, int red, int green, int blue, int alpha)
//-----------------------------------------------------------------------------
{
	if ((index < 0) || (index > SIDEVIEW_NUM_COLORS))
	{
		return;
	}

	sideviewColors[index].color.r = red;
	sideviewColors[index].color.g = green;
	sideviewColors[index].color.b = blue;
	sideviewColors[index].color.a = alpha;
}

//-----------------------------------------------------------------------------
//
// setup the way the tunnels are linked to the levels
void gui_setupTunnels ()
//-----------------------------------------------------------------------------
{
	tunnel[0].top    = 19;
	tunnel[0].bottom = 3;
	tunnel[0].decks[0] = 3;
	tunnel[0].decks[1] = 4;
	tunnel[0].decks[2] = 15;
	tunnel[0].decks[3] = 16;
	tunnel[0].decks[4] = 19;
	tunnel[0].current_deck = 0;
	tunnel[0].current      = tunnel[0].decks[tunnel[0].current_deck];

	tunnel[1].top    = 6;
	tunnel[1].bottom = 0;
	tunnel[1].decks[0] = 0;
	tunnel[1].decks[1] = 5;
	tunnel[1].decks[2] = 6;
	tunnel[1].current_deck = 0;
	tunnel[1].current      = tunnel[1].decks[tunnel[1].current_deck];

	tunnel[2].top    = 17;
	tunnel[2].bottom = 15;
	tunnel[2].decks[0] = 15;
	tunnel[2].decks[1] = 17;
	tunnel[2].current_deck = 0;
	tunnel[2].current      = tunnel[2].decks[tunnel[2].current_deck];

	tunnel[3].top    = 20;
	tunnel[3].bottom = 12;
	tunnel[3].decks[0] = 12;
	tunnel[3].decks[1] = 13;
	tunnel[3].decks[2] = 14;
	tunnel[3].decks[3] = 17;
	tunnel[3].decks[4] = 20;
	tunnel[3].current_deck = 0;
	tunnel[3].current      = tunnel[3].decks[tunnel[3].current_deck];

	tunnel[4].top    = 12;
	tunnel[4].bottom = 1;
	tunnel[4].decks[0] = 1;
	tunnel[4].decks[1] = 5;
	tunnel[4].decks[2] = 6;
	tunnel[4].decks[3] = 12;
	tunnel[4].current_deck = 0;
	tunnel[4].current      = tunnel[4].decks[tunnel[4].current_deck];

	tunnel[5].top    = 9;
	tunnel[5].bottom = 2;
	tunnel[5].decks[0] = 2;
	tunnel[5].decks[1] = 8;
	tunnel[5].decks[2] = 9;
	tunnel[5].current_deck = 0;
	tunnel[5].current      = tunnel[5].decks[tunnel[5].current_deck];

	tunnel[6].top    = 14;
	tunnel[6].bottom = 9;
	tunnel[6].decks[0] = 9;
	tunnel[6].decks[1] = 10;
	tunnel[6].decks[2] = 11;
	tunnel[6].decks[3] = 12;
	tunnel[6].decks[4] = 13;
	tunnel[6].decks[5] = 14;
	tunnel[6].current_deck = 0;
	tunnel[6].current      = tunnel[6].decks[tunnel[6].current_deck];

	tunnel[7].top    = 18;
	tunnel[7].bottom = 14;
	tunnel[7].decks[0] = 14;
	tunnel[7].decks[1] = 18;
	tunnel[7].current_deck = 0;
	tunnel[7].current      = tunnel[7].decks[tunnel[7].current_deck];
}

// ----------------------------------------------------------------------------
//
// load the sideview data from the external file
bool gui_loadSideViewData (const std::string &sideviewFileName)
// ----------------------------------------------------------------------------
{
	int           fileLength;
	char          *fileBuffer = nullptr;
	int           numberLevels;
	int           count;
	int           buf[1];
	unsigned char levelCount[1];
	PARA_MEM_FILE *fp;

	float smallX, largeX;

	smallX = 1000;
	largeX = 0;

	sideviewDrawScale = sideviewDrawScaleScript;

	fileLength = (int) io_getFileSize (sideviewFileName.c_str ());
	if (fileLength < 0)
	{
		log_logMessage (LOG_LEVEL_INFO, sys_getString ("Fatal error getting level file size [ %s ].", sideviewFileName.c_str ()));
		return false;
	}

	fileBuffer = (char *) malloc (sizeof (char) * fileLength);
	if (nullptr == fileBuffer)
	{
		log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Fatal memory allocation error when loading level file [ %s ].", sideviewFileName.c_str ()));
	}

	if (-1 == io_getFileIntoMemory (sideviewFileName.c_str (), fileBuffer))
	{
		free (fileBuffer);
		log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Fatal memory allocation when loading file [ %s ].", sideviewFileName.c_str ()));
	}

	fp = PARA_openMemFile (fileBuffer, fileLength);
	if (nullptr == fp)
	{
		log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Mapping memory to file failed for file [ %s ]", sideviewFileName.c_str ()));
	}

	PARA_readFile (fp, &levelCount, sizeof (levelCount));
	numberLevels = levelCount[0];

	if (MAX_LEVELS != numberLevels)
	{
		log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Bad format or date when reading file [ %s ]", sideviewFileName.c_str ()));
	}

	for (count = 0; count != numberLevels; count++)
	{
		float temp;

		PARA_readFile (fp, (void *) &buf, sizeof (sideviewLevels[count].x1));
		temp = (float) buf[0] * sideviewDrawScale;
		sideviewLevels[count].x1 = temp;

		if (sideviewLevels[count].x1 < smallX)
		{
			smallX = sideviewLevels[count].x1;
		}

		PARA_readFile (fp, (void *) &buf, sizeof (sideviewLevels[count].y1));
		temp = (float) (buf[0] - 100.0f) * sideviewDrawScale;
		sideviewLevels[count].y1 = temp;

		PARA_readFile (fp, (void *) &buf, sizeof (sideviewLevels[count].x2));
		temp = (float) buf[0] * sideviewDrawScale;
		sideviewLevels[count].x2 = temp;

		if (sideviewLevels[count].x2 > largeX)
		{
			largeX = sideviewLevels[count].x2;
		}

		PARA_readFile (fp, (void *) &buf, sizeof (sideviewLevels[count].y2));
		temp = (float) (buf[0] - 100.0f) * sideviewDrawScale;
		sideviewLevels[count].y2 = temp;
	}
	PARA_closeFile (fp);

	free (fileBuffer);

	gui_setupTunnels ();

	drawOffsetX = (screenWidth - (largeX - smallX)) / 2;

	drawOffsetX = 0;

	return true;
}

// ----------------------------------------------------------------------------
//
// Show the ship in its sideview on the screen
void gui_drawSideView ()
// ----------------------------------------------------------------------------
{
	int        count;
	int        lifts   = 8;
	int        toLifts = 0;
	float      x1;
	float      y1;
	PARA_COLOR tempAlert;
	b2Vec2     textPosition;
	int        fontLineWidth, fontLineHeight;

	//
	// Draw hold level
	count = 0;
	x1    = sideviewLevels[count].x2;
	y1    = sideviewLevels[count].y2;
	gui_sideviewDrawRect (x1, y1, sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
	toLifts++;

	//
	// Draw all the decks in normal color
	for (count = 1; count != MAX_LEVELS - lifts; count++)
	{
		gui_sideviewDrawRect (sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
		toLifts++;
	}

	//
	// Highlite the current deck in use for lift mode
	if (currentMode == MODE_LIFT_VIEW)
	{
		//
		// highlite current level
		if (0 == currentDeckNumber)
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, sideviewColors[SIDEVIEW_ACTIVE_DECK_COLOR].color);
		}
		else
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, sideviewColors[SIDEVIEW_ACTIVE_DECK_COLOR].color);
		}
	}
	else    // Static view of ship from terminal
	{
		currentDeckNumber = lvl_getDeckNumber (lvl_getCurrentLevelName ());

		switch (currentAlertLevel)
		{
			case ALERT_GREEN_TILE:
				tempAlert.r = 0;
				tempAlert.g = 255;
				tempAlert.b = 0;
				tempAlert.a = 255;
				break;

			case ALERT_YELLOW_TILE:
				tempAlert.r = 255;
				tempAlert.g = 255;
				tempAlert.b = 0;
				tempAlert.a = 255;
				break;

			case ALERT_RED_TILE:
				tempAlert.r = 255;
				tempAlert.g = 0;
				tempAlert.b = 0;
				tempAlert.a = 255;
				break;

			default:
				tempAlert.r = 255;
				tempAlert.g = 255;
				tempAlert.b = 255;
				tempAlert.a = 255;
				break;
		}

		if (0 == currentDeckNumber)
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, tempAlert);
		}
		else
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, tempAlert);
		}
	}

	//
	// Redraw the level and tunnel that overlap
	if ((currentTunnel != 3) && (currentTunnel != 6))
	{
		count = 13;
		gui_sideviewDrawRect (sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
	}
	else
	{
		// using tunnel connecting to level 13
		if (currentDeckNumber != 13)
		{
			count = 13;
			gui_sideviewDrawRect (sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
		}
	}

	//
	// fill in engine part
	gui_sideviewDrawRect (sideviewLevels[7].x1, sideviewLevels[7].y1, sideviewLevels[7].x2, sideviewLevels[7].y2, sideviewColors[SIDEVIEW_ENGINE_COLOR].color);
	//
	// draw the lifts
	for (count = 0; count != lifts; count++)
	{
		if (currentMode == MODE_LIFT_VIEW)      // Only draw highlighted tunnel in lift view
		{
			if (currentTunnel == count) // Draw currentTunnel in use
				gui_sideviewDrawRect (sideviewLevels[count + toLifts].x1, sideviewLevels[count + toLifts].y1, sideviewLevels[count + toLifts].x2, sideviewLevels[count + toLifts].y2, sideviewColors[SIDEVIEW_ACTIVE_LIFT_COLOR].color);
			else
				gui_sideviewDrawRect (sideviewLevels[count + toLifts].x1, sideviewLevels[count + toLifts].y1, sideviewLevels[count + toLifts].x2, sideviewLevels[count + toLifts].y2, sideviewColors[SIDEVIEW_LIFT_COLOR].color);
		}
		else
			gui_sideviewDrawRect (sideviewLevels[count + toLifts].x1, sideviewLevels[count + toLifts].y1, sideviewLevels[count + toLifts].x2, sideviewLevels[count + toLifts].y2, sideviewColors[SIDEVIEW_LIFT_COLOR].color);
	}

	fnt_setColor_f (1.0f, 1.0f, 1.0f, 1.0f);
	fnt_setTTF ("gui");

	fnt_render (b2Vec2{sideViewTextPosX, sideViewTextPosY}, sys_getString ("Deck [ %s ]", lvl_returnLevelNameFromDeck (currentDeckNumber).c_str()));
}

