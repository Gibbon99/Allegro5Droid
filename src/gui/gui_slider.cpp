//
// Created by dberry on 17/11/19.
//

#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include "hdr/gui/gui_slider.h"


//----------------------------------------------------------------------------------------------------------------------
//
// Draw a slider element
void gui_drawSlider(int whichSlider, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	static int positionNotCalledCount = 0;

	float startX, startY, width, height;
	float radius                      = 4;
	float red, green, blue, alpha;

	float textStartX, textStartY;

	if (guiSliders.empty ())
		return;

	if (whichSlider > guiSliders.size ())
		{
			log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Passed in invalid index to guiSlider [ %i ] greater than [ %i ]", whichSlider, guiSliders.size ()));
			return;
		}

	if (!guiSliders[whichSlider].ready)
		return;

	if (whichSlider > guiSliders.size ())
		return;

	if (!guiSliders[whichSlider].positionCalled)
		{
			if (positionNotCalledCount < ERROR_REPEAT_NUMBER)
				{
					log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Position has not been set for Slider [ %i ]", whichSlider));
					positionNotCalledCount++;
				}
			return;
		}

	fnt_setTTF (guiSliders[whichSlider].fontName);

	startX = guiSliders[whichSlider].startX;
	startY = guiSliders[whichSlider].startY;
	width  = guiSliders[whichSlider].width;
	height = guiSliders[whichSlider].height;

	if (!hasFocus)
		{
			red   = guiSliders[whichSlider].noFocusColor.r;
			blue  = guiSliders[whichSlider].noFocusColor.b;
			green = guiSliders[whichSlider].noFocusColor.g;
			alpha = guiSliders[whichSlider].noFocusColor.a;
		}
	else
		{
			red   = guiSliders[whichSlider].hasFocusColor.r;
			blue  = guiSliders[whichSlider].hasFocusColor.b;
			green = guiSliders[whichSlider].hasFocusColor.g;
			alpha = guiSliders[whichSlider].hasFocusColor.a;
		}

	al_draw_filled_rounded_rectangle (startX, startY, startX + width, startY + height, radius, radius, al_map_rgba_f (red, green, blue, alpha));
}