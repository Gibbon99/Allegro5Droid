#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include "hdr/gui/gui_textBox.h"

//-----------------------------------------------------------------------------
//
// Print the display inside the confines of a text box
void gui_drawTextbox (int whichObject)
//-----------------------------------------------------------------------------
{
	static int  positionNotCalledCount = 0;
	float       printStartY            = 0.0f;
	float       textLength             = 0.0f;
	char        *p                     = nullptr;
	std::string textCopy;
	std::string nextLine;

	if (guiTextBoxes.empty ())
		return;

	if (whichObject > guiTextBoxes.size ())
	{
		log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Passed in invalid index to gui_drawTextbox [ %i ] greater than [ %i ]", whichObject, guiTextBoxes.size ()));
		return;
	}

	if (!guiTextBoxes[whichObject].ready)
		return;

	if (!guiTextBoxes[whichObject].positionCalled)
	{
		if (positionNotCalledCount < ERROR_REPEAT_NUMBER)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Position has not been set for Textbox [ %i ]", whichObject));
			positionNotCalledCount++;
		}
		return;
	}

	nextLine.clear ();
	textLength = 0;

	printStartY = guiTextBoxes[whichObject].boundingBox.y;
	//
	// strtok modifies the string - make a copy
	textCopy    = guiTextBoxes[whichObject].text;
	p           = strtok ((char *) textCopy.c_str (), " ");

	while (p != nullptr)
	{
		textLength = fnt_getWidth (nextLine + " " + p);

		if (*p == '!')
		{
			textLength = guiTextBoxes[whichObject].boundingBox.w + 1;
		}

		if (textLength < guiTextBoxes[whichObject].boundingBox.w)
		{
			nextLine.append (p);
			nextLine.append (" ");
		}
		else
		{
			// Check colors
			fnt_setColor_f (guiTextBoxes[whichObject].hasFocusColor.r, guiTextBoxes[whichObject].hasFocusColor.g, guiTextBoxes[whichObject].hasFocusColor.b, guiTextBoxes[whichObject].hasFocusColor.a);
			fnt_render (b2Vec2{guiTextBoxes[whichObject].boundingBox.x, printStartY}, nextLine);

			printStartY += fnt_getHeight ();
			nextLine.clear ();
			if (*p != '!')
			{
				nextLine.append (p);
			}
			nextLine.append (" ");
		}
		p          = strtok (nullptr, " ");
	}
	fnt_setColor_f (guiTextBoxes[whichObject].hasFocusColor.r, guiTextBoxes[whichObject].hasFocusColor.g, guiTextBoxes[whichObject].hasFocusColor.b, guiTextBoxes[whichObject].hasFocusColor.a);
	fnt_render (b2Vec2{guiTextBoxes[whichObject].boundingBox.x, printStartY}, nextLine);
}
