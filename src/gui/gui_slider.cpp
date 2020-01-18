#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include <hdr/io/io_resourceLevel.h>
#include "hdr/gui/gui_slider.h"


//-----------------------------------------------------------------------------
//
// Set the slider to the passed in value
void gui_hostSetSliderValue(const std::string& objectID, const std::string& value)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	for (auto &sliderItr : guiSliders)
	{
		if (sliderItr.objectID == objectID)
		{
			for (std::vector<_sliderElement>::iterator stepItr = sliderItr.element.begin(); stepItr != sliderItr.element.end(); ++stepItr)
			{
				switch (stepItr->type)
				{
					case SLIDER_TYPE_INT:
						if (stepItr->value == value)
						{
							sliderItr.currentStep = indexCount;
							return;
						}
						break;
				}
				indexCount += 1;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a slider element
void gui_drawSlider(int whichSlider, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	static int positionNotCalledCount = 0;

	std::string 		printLabel;
	std::string 		printValue;
	b2Vec2		        printLabelPos;
	b2Vec2		        printValuePos;

	float startX, startY, width, height;
	float radius                      = 4;
	float red, green, blue, alpha;
	float selectorX;

	float sliderSelectorRadius = 5.0f;

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

	printLabel = guiSliders[whichSlider].text;
	printValue = guiSliders[whichSlider].element[guiSliders[whichSlider].currentStep].label;

	printValuePos.x = guiSliders[whichSlider].boundingBox.x + ( ( guiSliders[whichSlider].boundingBox.w - fnt_getWidth ( printValue ) ) / 2 );
	printLabelPos.x = guiSliders[whichSlider].boundingBox.x + ( ( guiSliders[whichSlider].boundingBox.w - fnt_getWidth ( printLabel ) ) / 2 );

	printValuePos.y = guiSliders[whichSlider].boundingBox.y + guiSliders[whichSlider].boundingBox.h + 2;
	printLabelPos.y = guiSliders[whichSlider].boundingBox.y - ( fnt_getHeight ( ) + 2 );

	if ( guiSliders[whichSlider].currentStep == 0 )
		selectorX = guiSliders[whichSlider].boundingBox.x;
	else if ( guiSliders[whichSlider].currentStep == (int)guiSliders[whichSlider].element.size() )
		selectorX = guiSliders[whichSlider].boundingBox.x + guiSliders[whichSlider].boundingBox.w;	// position at the end
	else
	{
		selectorX = ( ( guiSliders[whichSlider].boundingBox.w / ( guiSliders[whichSlider].element.size() - 1 ) ) * guiSliders[whichSlider].currentStep ) + guiSliders[whichSlider].boundingBox.x;
	}


	startX = guiSliders[whichSlider].startX;
	startY = guiSliders[whichSlider].startY;
	width  = guiSliders[whichSlider].boundingBox.w;
	height = guiSliders[whichSlider].boundingBox.h;

	if (!hasFocus)
		{
			red   = guiSliders[whichSlider].noFocusColor.r;
			green = guiSliders[whichSlider].noFocusColor.g;
			blue  = guiSliders[whichSlider].noFocusColor.b;
			alpha = guiSliders[whichSlider].noFocusColor.a;
		}
	else
		{
			red   = guiSliders[whichSlider].hasFocusColor.r;
			green = guiSliders[whichSlider].hasFocusColor.g;
			blue  = guiSliders[whichSlider].hasFocusColor.b;
			alpha = guiSliders[whichSlider].hasFocusColor.a;
		}

	fnt_setColor_f (red, green, blue, alpha);
	fnt_render(b2Vec2{printLabelPos.x, printLabelPos.y,}, guiSliders[whichSlider].text);
	fnt_render ( b2Vec2{printValuePos.x, printValuePos.y}, printValue.c_str() );

	al_draw_filled_circle ( selectorX, guiSliders[whichSlider].boundingBox.y + ( guiSliders[whichSlider].boundingBox.h / 2 ), sliderSelectorRadius, al_map_rgba_f (red, green, blue, alpha));
	al_draw_filled_rounded_rectangle (startX, startY, startX + width, startY + height, radius, radius, al_map_rgba_f (red, green, blue, alpha));
}