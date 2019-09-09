#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include <hdr/gui/gui_main.h>
#include "hdr/gui/gui_scrollBox.h"

__GUI_object introScrollBox;

float introScrollBoxStartX;
float introScrollBoxStartY;
float introScrollBoxWidth;
float introScrollBoxHeight;
float introScrollBoxSpeed;
float introScrollBoxGapSize;
float introScrollBoxBackRed;
float introScrollBoxBackGreen;
float introScrollBoxBackBlue;
float introScrollBoxBackAlpha;
float introScrollBoxFontRed;
float introScrollBoxFontGreen;
float introScrollBoxFontBlue;
float introScrollBoxFontAlpha;
float introScrollBoxRadius;


//----------------------------------------------------------------------------------------------------------------------
//
// Render a scrollbox
void gui_renderScrollBox (__GUI_object *thisScrollBox, double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	int    lineCount = 0;
	float drawPositionY;
	b2Vec2 renderPosition;

	std::string currentLineText;

	al_draw_filled_rounded_rectangle (thisScrollBox->startX, thisScrollBox->startY, thisScrollBox->startX + thisScrollBox->width, thisScrollBox->startY + thisScrollBox->height,
			thisScrollBox->radius, thisScrollBox->radius, thisScrollBox->__GUI_element.scrollBox.backgroundColor);

	fnt_setTTF (thisScrollBox->__GUI_element.scrollBox.fontName);

	float alphaStep = 1.0f / fnt_getHeight ();
	float lineAlpha = 0;

	lineCount = 0;
	if (thisScrollBox->__GUI_element.scrollBox.scrollDelay > 0)
	{
		drawPositionY = thisScrollBox->__GUI_element.scrollBox.scrollY;
	}
	else
	{
		drawPositionY = thisScrollBox->__GUI_element.scrollBox.scrollY - thisScrollBox->__GUI_element.scrollBox.previousScrollY;
		drawPositionY *= interpolation;
		drawPositionY += thisScrollBox->__GUI_element.scrollBox.previousScrollY;
	}
	for (auto lineItr = thisScrollBox->scrollBoxLineText.rbegin (); lineItr != thisScrollBox->scrollBoxLineText.rend (); ++lineItr)
	{
		lineAlpha = thisScrollBox->labelHasFocusColor.a;

		if (lineCount == 0)
		{
			thisScrollBox->__GUI_element.scrollBox.lineFade += alphaStep;
			if (thisScrollBox->__GUI_element.scrollBox.lineFade > thisScrollBox->labelHasFocusColor.a)
			{
				thisScrollBox->__GUI_element.scrollBox.lineFade = thisScrollBox->labelHasFocusColor.a;
			}
			lineAlpha = thisScrollBox->__GUI_element.scrollBox.lineFade;
		}

		else if (lineCount == thisScrollBox->__GUI_element.scrollBox.numLinesToPrint - 2)
		{
			lineAlpha = 1.0f - (alphaStep * thisScrollBox->__GUI_element.scrollBox.scrollY);
		}

		renderPosition.x = thisScrollBox->startX + (thisScrollBox->gapSize / 2.0f);

		renderPosition.y = (((thisScrollBox->startY + (thisScrollBox->height - fnt_getHeight ())) - (lineCount * fnt_getHeight ())) - drawPositionY); // - thisScrollBox->__GUI_element.scrollBox.previousScrollY;
//		renderPosition.y *= percentWithinTick;
//		renderPosition.y += thisScrollBox->__GUI_element.scrollBox.previousScrollY;

//renderPosition.y = drawPositionY;

		fnt_setColor_f (thisScrollBox->labelHasFocusColor.r, thisScrollBox->labelHasFocusColor.g, thisScrollBox->labelHasFocusColor.b, lineAlpha);
		fnt_render (renderPosition, sys_getString ("%s", lineItr->c_str ()));

		lineCount++;
		if (lineCount == thisScrollBox->__GUI_element.scrollBox.numLinesToPrint - 1)
		{
			break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup a scrollbox with initial values
void gui_setupScrollBox (int whichScrollBox, const std::string &sourceText)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (whichScrollBox)
	{
		case SCROLLBOX_INTRO:
			introScrollBox.startX                                  = introScrollBoxStartX;
			introScrollBox.startY                                  = introScrollBoxStartY;
			introScrollBox.width                                   = introScrollBoxWidth;
			introScrollBox.height                                  = introScrollBoxHeight;
			introScrollBox.text                                    = sourceText;
			introScrollBox.gapSize                                 = introScrollBoxGapSize;
			introScrollBox.__GUI_element.scrollBox.scrollSpeed     = introScrollBoxSpeed;
			introScrollBox.__GUI_element.scrollBox.backgroundColor = al_map_rgba (introScrollBoxBackRed, introScrollBoxBackGreen, introScrollBoxBackBlue, introScrollBoxBackAlpha);
			introScrollBox.labelHasFocusColor                      = al_map_rgba_f (introScrollBoxFontRed, introScrollBoxFontGreen, introScrollBoxFontBlue, introScrollBoxFontAlpha);
			introScrollBox.radius                                  = introScrollBoxRadius;
			introScrollBox.__GUI_element.scrollBox.currentChar     = 0;
			introScrollBox.__GUI_element.scrollBox.scrollDelay     = 0;
			introScrollBox.__GUI_element.scrollBox.scrollY         = 0.0f;
			introScrollBox.__GUI_element.scrollBox.previousScrollY = 0.0f;
			introScrollBox.__GUI_element.scrollBox.numLinesToPrint = 0;
			introScrollBox.__GUI_element.scrollBox.lineFade        = 0;
			introScrollBox.scrollBoxLineText.clear ();
			strcpy (introScrollBox.__GUI_element.scrollBox.fontName, "intro");

			break;

		case SCROLLBOX_DB:
			break;

		default:
			log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unknown value passed to gui_setupScrollBox [ %i ]", whichScrollBox));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the next line of text to show in the scrollbox
void gui_getNextLineOfText (__GUI_object *thisScrollBox)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string nextWord;
	std::string nextLine;
	std::string nextChar;

	int  previousSpace = 0;
	bool foundLine;

	int lineTextWidth;

	foundLine     = false;
	nextWord      = "";
	nextLine      = "";
	previousSpace = 0;

	fnt_setTTF (thisScrollBox->__GUI_element.scrollBox.fontName);    // Need current font size and width

	while (!foundLine)
	{
		nextChar = thisScrollBox->text[thisScrollBox->__GUI_element.scrollBox.currentChar];
		switch (nextChar.back ())
		{
			case ' ':
				lineTextWidth = fnt_getWidth (nextLine + " " + nextChar + nextWord);
				if (lineTextWidth < thisScrollBox->width - thisScrollBox->gapSize)
				{
					previousSpace = thisScrollBox->__GUI_element.scrollBox.currentChar;
					nextLine.append (" ");
					nextLine.append (nextWord);
					nextWord.clear ();
				}
				else
				{
					thisScrollBox->__GUI_element.scrollBox.currentChar = previousSpace;
					foundLine = true;
				}
				break;

			case '!':
				nextLine += nextWord;
				nextWord.clear ();
				foundLine = true;
				thisScrollBox->__GUI_element.scrollBox.currentChar++;
				break;

			case '#':
				nextLine += nextWord;
				nextWord.clear ();
				foundLine = true;
				thisScrollBox->__GUI_element.scrollBox.currentChar = 0;
				break;

			case '^':
				nextLine += nextWord;
				nextWord.clear ();
				foundLine = true;
				thisScrollBox->__GUI_element.scrollBox.currentChar++;
				thisScrollBox->__GUI_element.scrollBox.scrollDelay = 40;
				break;

			default:
				nextWord += nextChar;
				break;
		}

		thisScrollBox->__GUI_element.scrollBox.currentChar++;
		if (thisScrollBox->__GUI_element.scrollBox.currentChar > (int) thisScrollBox->text.size ())
		{
			nextLine += nextWord;
			foundLine = true;
		}
	}
	thisScrollBox->scrollBoxLineText.push_back (nextLine);
	thisScrollBox->__GUI_element.scrollBox.lineFade = 0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Move the scrollbox text up by one pixel
void gui_scrollOnePixel (__GUI_object *thisScrollBox, double tickTime)
//----------------------------------------------------------------------------------------------------------------------
{
	fnt_setTTF (thisScrollBox->__GUI_element.scrollBox.fontName);

	thisScrollBox->__GUI_element.scrollBox.previousScrollY = thisScrollBox->__GUI_element.scrollBox.scrollY;

	thisScrollBox->__GUI_element.scrollBox.scrollY += thisScrollBox->__GUI_element.scrollBox.scrollSpeed * (float)tickTime;

	if (thisScrollBox->__GUI_element.scrollBox.scrollY > (float) fnt_getHeight () - 1)
	{
		thisScrollBox->__GUI_element.scrollBox.scrollY = 0.0f;
		thisScrollBox->__GUI_element.scrollBox.previousScrollY = -1.0f;
		gui_getNextLineOfText (thisScrollBox);
		thisScrollBox->__GUI_element.scrollBox.numLinesToPrint++;
		if (thisScrollBox->__GUI_element.scrollBox.numLinesToPrint > (thisScrollBox->height) / fnt_getHeight ())
		{
			thisScrollBox->__GUI_element.scrollBox.numLinesToPrint = (thisScrollBox->height) / fnt_getHeight ();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Move the scrollbox, or pause if required
void gui_scrollScrollBox (__GUI_object *thisScrollBox, double tickTime)
//----------------------------------------------------------------------------------------------------------------------
{
	if (thisScrollBox->__GUI_element.scrollBox.scrollDelay > 0)
	{
		thisScrollBox->__GUI_element.scrollBox.scrollDelay--;
		if (thisScrollBox->__GUI_element.scrollBox.scrollDelay < 0)
		{
			thisScrollBox->__GUI_element.scrollBox.scrollDelay = 0;
//			gui_scrollOnePixel (thisScrollBox, tickTime);
		}
	}
	else
	{
		gui_scrollOnePixel (thisScrollBox, tickTime);
	}
}