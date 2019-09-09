#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include "hdr/gui/gui_button.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a button element
void gui_drawButton(int whichButton, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	static int positionNotCalledCount = 0;

	float startX, startY, width, height;
	float radius = 16;
	float red, green, blue, alpha;

	float textStartX, textStartY;

	if (guiButtons.empty())
		return;

	if (whichButton > guiButtons.size())
	{
		log_logMessage(LOG_LEVEL_EXIT, sys_getString("Passed in invalid index to guiDrawButton [ %i ] greater than [ %i ]", whichButton, guiButtons.size()));
		return;
	}

	if (!guiButtons[whichButton].ready)
		return;

	if (whichButton > guiButtons.size())
		return;

	if (!guiButtons[whichButton].positionCalled)
	{
		if (positionNotCalledCount < ERROR_REPEAT_NUMBER)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Position has not been set for Button [ %i ]", whichButton));
			positionNotCalledCount++;
		}
		return;
	}

	startX = guiButtons[whichButton].startX;
	startY = guiButtons[whichButton].startY;
	width = guiButtons[whichButton].width;
	height = guiButtons[whichButton].height;

	if (!hasFocus)
	{
		red   = guiButtons[whichButton].noFocusColor.r;
		blue  = guiButtons[whichButton].noFocusColor.b;
		green = guiButtons[whichButton].noFocusColor.g;
		alpha = guiButtons[whichButton].noFocusColor.a;
	}
	else
	{
		red   = guiButtons[whichButton].hasFocusColor.r;
		blue  = guiButtons[whichButton].hasFocusColor.b;
		green = guiButtons[whichButton].hasFocusColor.g;
		alpha = guiButtons[whichButton].hasFocusColor.a;
	}

	al_draw_filled_rounded_rectangle(startX, startY, startX + width, startY + height, radius, radius, al_map_rgba(red, green, blue, alpha));

//	al_draw_filled_rounded_rectangle(startX, startY, startX + width, startY + height, radius, radius, al_map_rgba(255, green, blue, 255));

	switch (guiButtons[whichButton].labelPos)
	{
		case GUI_LABEL_CENTER:
			textStartX = (width - fnt_getWidth(guiButtons[whichButton].text)) / 2;
			textStartY = (height - fnt_getHeight()) / 2;
			break;

		case GUI_LABEL_RIGHT:
			textStartX = (width - fnt_getWidth(guiButtons[whichButton].text)) - guiButtons[whichButton].gapSize;
			textStartY = (height - fnt_getHeight()) / 2;
			break;

		case GUI_LABEL_LEFT:
			textStartX = guiButtons[whichButton].gapSize;
			textStartY = (height - fnt_getHeight()) / 2;
			break;

		default:
			log_logMessage (LOG_LEVEL_ERROR, sys_getString("Unknown button label location."));
			return;
			break;
	}

	fnt_setColor_f (red, green, blue, alpha);
	fnt_render(b2Vec2{textStartX + startX, textStartY + startY}, guiButtons[whichButton].text);
}
