#include <hdr/system/sys_font.h>
#include <hdr/gui/gui_textBox.h>
#include <hdr/gui/gui_label.h>
#include <hdr/gui/gui_image.h>
#include <hdr/gui/gui_button.h>
#include <hdr/gui/gui_checkBox.h>
#include "hdr/gui/gui_render.h"

//-----------------------------------------------------------------------------
//
// Draw a key based on passed in color and mode
void gui_drawObject (int objectType, int whichObject, bool hasFocus)
//-----------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			gui_drawButton (whichObject, hasFocus);
			break;

		case GUI_OBJECT_CHECKBOX:
			gui_drawCheckbox (whichObject, hasFocus);
			break;

		case GUI_OBJECT_TEXTBOX:
			gui_drawTextbox (whichObject);
			break;

		case GUI_OBJECT_LABEL:
			gui_drawLabel (whichObject);
			break;

		case GUI_OBJECT_IMAGE:
			gui_drawImage (whichObject);
			break;

		default:
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Main routine for drawing all the GUI screens
void gui_drawGUI ()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Draw GUI object elements
	int indexCount = 0;

	if (!isGUIStarted)
		return;

	fnt_setTTF ("gui");

	for (indexCount = 0; indexCount != guiScreens[currentGUIScreen].objectIDIndex.size (); indexCount++)
	{
		if (guiScreens[currentGUIScreen].selectedObject == indexCount)
			gui_drawObject (guiScreens[currentGUIScreen].objectType[indexCount], guiScreens[currentGUIScreen].objectIDIndex[indexCount], true);
		else
			gui_drawObject (guiScreens[currentGUIScreen].objectType[indexCount], guiScreens[currentGUIScreen].objectIDIndex[indexCount], false);
	}
}
