#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include <hdr/gui/gui_main.h>
#include "hdr/gui/gui_checkBox.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a checkbox element
void gui_drawCheckbox (int whichObject, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	static int positionNotCalledCount = 0;

	float boxSize;
	float radius                      = 4;
	float insideSize                  = 4;
	float textStartX, textStartY;

	if (!guiCheckBoxes[whichObject].ready)
		return;

	if (whichObject > guiCheckBoxes.size ())
		return;

	if (!guiCheckBoxes[whichObject].positionCalled)
		{
			if (positionNotCalledCount < ERROR_REPEAT_NUMBER)
				{
					log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Position has not been set for CheckBox [ %i ]", whichObject));
					positionNotCalledCount++;
				}
			return;
		}

	fnt_setTTF (guiCheckBoxes[whichObject].fontName);

	boxSize = fnt_getHeight () + insideSize;

	textStartX = guiCheckBoxes[whichObject].boundingBox.x + boxSize + insideSize; //guiCheckBoxes[whichObject].gapSize;
	textStartY = guiCheckBoxes[whichObject].boundingBox.y + ((boxSize - fnt_getHeight ()) / 2);

	if (hasFocus)
		{

			al_draw_filled_rounded_rectangle (guiCheckBoxes[whichObject].boundingBox.x, guiCheckBoxes[whichObject].boundingBox.y,
			                                  guiCheckBoxes[whichObject].boundingBox.x + boxSize, guiCheckBoxes[whichObject].boundingBox.y + boxSize,
			                                  radius, radius, al_map_rgba_f (guiCheckBoxes[whichObject].hasFocusColor.r, guiCheckBoxes[whichObject].hasFocusColor.g, guiCheckBoxes[whichObject].hasFocusColor.b, guiCheckBoxes[whichObject].hasFocusColor.a));

			if (!guiCheckBoxes[whichObject].__GUI_element.checkBox.isChecked)
				al_draw_filled_rounded_rectangle (guiCheckBoxes[whichObject].boundingBox.x + (insideSize / 2), guiCheckBoxes[whichObject].boundingBox.y + (insideSize / 2),
				                                  guiCheckBoxes[whichObject].boundingBox.x + (boxSize - (insideSize / 2)), guiCheckBoxes[whichObject].boundingBox.y + (boxSize - (insideSize / 2)),
				                                  radius, radius, al_map_rgba_f (0.0f, 0.0f, 0.0f, 1.0f));

			fnt_setColor_f (guiCheckBoxes[whichObject].hasFocusColor.r, guiCheckBoxes[whichObject].hasFocusColor.g, guiCheckBoxes[whichObject].hasFocusColor.b, guiCheckBoxes[whichObject].hasFocusColor.a);
			fnt_render (b2Vec2{textStartX, textStartY}, guiCheckBoxes[whichObject].text);
		}
	else
		{
			al_draw_filled_rounded_rectangle (guiCheckBoxes[whichObject].boundingBox.x, guiCheckBoxes[whichObject].boundingBox.y,
			                                  guiCheckBoxes[whichObject].boundingBox.x + boxSize, guiCheckBoxes[whichObject].boundingBox.y + boxSize,
			                                  radius, radius, al_map_rgba_f (guiCheckBoxes[whichObject].noFocusColor.r, guiCheckBoxes[whichObject].noFocusColor.g, guiCheckBoxes[whichObject].noFocusColor.b, guiCheckBoxes[whichObject].noFocusColor.a));

			if (!guiCheckBoxes[whichObject].__GUI_element.checkBox.isChecked)
				al_draw_filled_rounded_rectangle (guiCheckBoxes[whichObject].boundingBox.x + (insideSize / 2), guiCheckBoxes[whichObject].boundingBox.y + (insideSize / 2),
				                                  guiCheckBoxes[whichObject].boundingBox.x + (boxSize - (insideSize / 2)), guiCheckBoxes[whichObject].boundingBox.y + (boxSize - (insideSize / 2)),
				                                  radius, radius, al_map_rgba_f (0.0f, 0.0f, 0.0f, 1.0f));

			fnt_setColor_f (guiCheckBoxes[whichObject].noFocusColor.r, guiCheckBoxes[whichObject].noFocusColor.g, guiCheckBoxes[whichObject].noFocusColor.b, guiCheckBoxes[whichObject].noFocusColor.a);
			fnt_render (b2Vec2{textStartX, textStartY}, guiCheckBoxes[whichObject].text);
		}
}

//-----------------------------------------------------------------------------
//
// Set a checkbox to ticked
// Go through other members of the group and untick them
void gui_hostSetCheckboxTick (const std::string objectID, int whichGroup, bool ticked)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting checkBox tick for - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex (GUI_OBJECT_CHECKBOX, objectID);
	if (-1 == objectIndex)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Couldn't find GUI object index [ %s ]", objectID.c_str ()));
			return;
		}

	if (-1 == whichGroup)
		{
			// Not part of a group
			guiCheckBoxes[objectIndex].__GUI_element.checkBox.isChecked = ticked;
			return;
		}

	for (unsigned int i                                         = 0; i != guiCheckBoxes.size (); i++)
		{
			if (whichGroup == guiCheckBoxes[i].__GUI_element.checkBox.whichGroup)
				guiCheckBoxes[i].__GUI_element.checkBox.isChecked = false;
		}
	//
	// Now set the checkbox to ticked
	guiCheckBoxes[objectIndex].__GUI_element.checkBox.isChecked = ticked;
}

//-----------------------------------------------------------------------------
//
// Set the group that this checkbox belongs to
void gui_hostSetCheckboxGroup (const std::string objectID, int whichGroup)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Setting checkBox group for - [ %s ]", objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex (GUI_OBJECT_CHECKBOX, objectID);
	if (-1 == objectIndex)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str ()));
			return;
		}

	guiCheckBoxes[objectIndex].__GUI_element.checkBox.whichGroup = whichGroup;
}

//-----------------------------------------------------------------------------
//
// Set the value for this checkbox
void gui_hostSetCheckboxValue (const std::string objectID, int newValue)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Setting checkBox value for - [ %s ]", objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex (GUI_OBJECT_CHECKBOX, objectID);
	if (-1 == objectIndex)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str ()));
			return;
		}

	guiCheckBoxes[objectIndex].__GUI_element.checkBox.value = newValue;
}

//-----------------------------------------------------------------------------
//
// Get the value from a checkbox
int gui_hostGetCheckboxValue (const std::string objectID)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Getting checkBox value for - [ %s ]", objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
//
// Find the index for this object
	objectIndex = gui_findIndex (GUI_OBJECT_CHECKBOX, objectID);
	if (-1 == objectIndex)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str ()));
			return -1;
		}

	return guiCheckBoxes[objectIndex].__GUI_element.checkBox.value;
}