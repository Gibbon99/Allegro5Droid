#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_scriptEngine.h>
#include <hdr/system/sys_eventsEngine.h>
#include "hdr/gui/gui_main.h"

std::vector<_screenObject> guiScreens;
std::vector<__GUI_object>  guiButtons;
std::vector<__GUI_object>  guiCheckBoxes;
std::vector<__GUI_object>  guiLabels;
std::vector<__GUI_object>  guiTextBoxes;
std::vector<__GUI_object>  guiImages;

int  currentGUIScreen;
int  currentObjectSelected;  // Pass this to script to act on
bool isGUIStarted = false;

//-----------------------------------------------------------------------------
//
// Change to a new GUI screen
void gui_changeToGUIScreen(int newScreen)
//-----------------------------------------------------------------------------
{
	currentGUIScreen = newScreen;
}

//-----------------------------------------------------------------------------
//
// Return if an object can be selected or not
bool gui_canObjectBeSelected(int objectType)
//-----------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
		case GUI_OBJECT_CHECKBOX:
		case GUI_OBJECT_SLIDER:
		case GUI_OBJECT_KEYCODE:
			return true;

		default:
			return false;
	}
}

//-----------------------------------------------------------------------------
//
// Look through the relevant vector to locate the index of the objectID
//
// returns index value, or -1 if not found
int gui_findIndex(int guiObjectType, const std::string objectID)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	indexCount = 0;

	switch (guiObjectType)
	{
		case GUI_OBJECT_SCREEN:
			for (const auto &iter : guiScreens)
			{
				if (iter.screenID == objectID)
				{
					return indexCount;
				}
				indexCount++;
			}
			return -1;
			break;

		case GUI_OBJECT_BUTTON:
			for (const auto &iter : guiButtons)
			{
				if (iter.objectID == objectID)
				{
					return indexCount;
				}
				indexCount++;
			}
			return -1;  // Didn't find a match
			break;

		case GUI_OBJECT_CHECKBOX:
			for (const auto &iter : guiCheckBoxes)
			{
				if (iter.objectID == objectID)
				{
					return indexCount;
				}
				indexCount++;
			}
			return -1;  // Didn't find a match
			break;

		case GUI_OBJECT_LABEL:
			for (const auto &iter : guiLabels)
			{
				if (iter.objectID == objectID)
				{
					return indexCount;
				}
				indexCount++;
			}
			return -1;  // Didn't find a match
			break;

		case GUI_OBJECT_IMAGE:
			for (const auto &iter : guiImages)
			{
				if (iter.objectID == objectID)
				{
					return indexCount;
				}
				indexCount++;
			}
			return -1;
			break;

		case GUI_OBJECT_TEXTBOX:
			for (const auto &iter : guiTextBoxes)
			{
				if (iter.objectID == objectID)
				{
					return indexCount;
				}
				indexCount++;
			}
			return -1;
			break;

		default:
			break;
	}
	return -1;      // Should never get here
}


//-----------------------------------------------------------------------------
//
// Create a new GUI screen which will contain other GUI objects
void gui_hostCreateNewScreen(std::string screenID)
//-----------------------------------------------------------------------------
{
	_screenObject tempScreen;

	tempScreen.screenID       = screenID;
	tempScreen.selectedObject = 0;      // default to first object on screen having focus

	guiScreens.push_back(tempScreen);

	log_logMessage(LOG_LEVEL_GUI, sys_getString("Screen [ %s ] created.", screenID.c_str()));
}

//-----------------------------------------------------------------------------
//
// Associate the object to a screen, recording it's index and type
void gui_hostAddObjectToScreen(int guiObjectType, std::string objectID, std::string whichScreen)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Adding [ %s ] to screen [ %s ]", objectID.c_str(), whichScreen.c_str()));
#endif // DEBUG_GUI_SETUP

	int screenIndex = 0;
	int objectIndex = 0;
	//
	// Find the index for this screen
	screenIndex = gui_findIndex(GUI_OBJECT_SCREEN, whichScreen);
	if (-1 == screenIndex)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("ERROR: Couldn't find GUI screen index [ %s ]", whichScreen.c_str()));
		return;
	}
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str()));
		return;
	}

	switch (guiObjectType)
	{
		case GUI_OBJECT_BUTTON:
		case GUI_OBJECT_CHECKBOX:
		case GUI_OBJECT_TEXTBOX:
		case GUI_OBJECT_LABEL:
		case GUI_OBJECT_IMAGE:
//		case GUI_OBJECT_SLIDER:
//		case GUI_OBJECT_KEYCODE:
			guiScreens[screenIndex].objectIDIndex.push_back(objectIndex);    // Add objectIndex to the end
			guiScreens[screenIndex].objectType.push_back(guiObjectType);

#ifdef DEBUG_GUI_SETUP
		io_logToFile("Screen [ %s ] objectID [ %i ] - Added", guiScreens[screenIndex].screenID.c_str(), guiScreens[screenIndex].objectIDIndex.back());
			  io_logToFile("-------------");
#endif
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Add a gui object to it's relevant vector array
void gui_hostCreateObject(int guiObjectType, const std::string objectID)
//-----------------------------------------------------------------------------
{
	__GUI_object tmpGuiButton;
	__GUI_object tmpGuiCheckBox;
	__GUI_object tmpGuiTextBox;
	__GUI_object tmpLabel;
	__GUI_object tmpObject;


#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Added new object - [ %s ]", objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	switch (guiObjectType)
	{
		case GUI_OBJECT_BUTTON:
			tmpGuiButton.objectID       = objectID;
			tmpGuiButton.canFocus       = true;
			tmpGuiButton.positionCalled = false;
			tmpGuiButton.gapSize        = 20;
			tmpGuiButton.lineWidth      = 3;
			tmpGuiButton.fadeCounter    = 0;
			tmpGuiButton.isHighlited    = false;
			tmpGuiButton.text           = "Init done";
			tmpGuiButton.ready          = false;

			if (guiButtons.empty())
			{
				guiButtons.push_back(tmpGuiButton);
				return;
			}
			//
			// See if it exists already
			for (auto guiItr : guiButtons)
			{
				if (guiItr.objectID == objectID)
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("GUI Button already exists [ %s ]", objectID.c_str()));
					return;
				}
			}
			//
			// Button does not exist - add it
			guiButtons.push_back(tmpGuiButton);
			break;


		case GUI_OBJECT_CHECKBOX:
			tmpGuiCheckBox.objectID       = objectID;
			tmpGuiCheckBox.canFocus       = true;
			tmpGuiCheckBox.positionCalled = false;
			tmpGuiCheckBox.gapSize        = 8;
			tmpGuiCheckBox.lineWidth      = 3;
			tmpGuiCheckBox.fadeCounter    = 0;
			tmpGuiCheckBox.isHighlited    = false;
			tmpGuiCheckBox.text           = "Init done";
			tmpGuiCheckBox.ready          = false;

			if (guiCheckBoxes.empty())
			{
				guiCheckBoxes.push_back(tmpGuiCheckBox);
				return;
			}

			for (auto guiItr : guiCheckBoxes)
			{
				if (guiItr.objectID == objectID)
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("GUI checkBox already exists [ %s ]", objectID.c_str()));
					return;
				}
			}
			guiCheckBoxes.push_back(tmpGuiCheckBox);
			break;

		case GUI_OBJECT_TEXTBOX:
			tmpGuiTextBox.objectID       = objectID;
			tmpGuiTextBox.canFocus       = true;
			tmpGuiTextBox.positionCalled = false;
			tmpGuiTextBox.gapSize        = 8;
			tmpGuiTextBox.lineWidth      = 3;
			tmpGuiTextBox.fadeCounter    = 0;
			tmpGuiTextBox.isHighlited    = false;
			tmpGuiTextBox.text           = "Init done";
			tmpGuiTextBox.ready          = false;

			if (guiTextBoxes.empty())
			{
				guiTextBoxes.push_back(tmpGuiTextBox);
				return;
			}

			for (auto guiItr : guiTextBoxes)
			{
				if (guiItr.objectID == objectID)
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("GUI textBox already exists [ %s ]", objectID.c_str()));
					return;
				}
			}
			guiTextBoxes.push_back(tmpGuiTextBox);
			break;

		case GUI_OBJECT_LABEL:
			tmpLabel.objectID       = objectID;
			tmpLabel.canFocus       = false;
			tmpLabel.positionCalled = false;
			tmpLabel.gapSize        = 0;
			tmpLabel.lineWidth      = 0;
			tmpLabel.fadeCounter    = 0;
			tmpLabel.isHighlited    = false;
			tmpLabel.text           = "Init done";
			tmpLabel.ready          = false;

			if (guiLabels.empty())
			{
				guiLabels.push_back(tmpLabel);
				return;
			}

			for (auto guiItr : guiLabels)
			{
				if (guiItr.objectID == objectID)
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("GUI Label already exists [ %s ]", objectID.c_str()));
					return;
				}
			}
			guiLabels.push_back(tmpLabel);
			break;

		case GUI_OBJECT_IMAGE:
			tmpObject.objectID       = objectID;
			tmpObject.canFocus       = false;
			tmpObject.positionCalled = false;
			tmpObject.gapSize        = 0;
			tmpObject.lineWidth      = 0;
			tmpObject.fadeCounter    = 0;
			tmpObject.isHighlited    = false;
			tmpObject.text           = "";
			tmpObject.ready          = false;
			strcpy(tmpObject.__GUI_element.image.keyName, "");

			if (guiImages.empty())      // WHY ? TODO
			{
				guiImages.push_back(tmpObject);
				return;
			}

			for (auto imageItr : guiImages)
			{
				if (imageItr.objectID == objectID)
				{
					log_logMessage(LOG_LEVEL_INFO, sys_getString("Image element already exists [ %s ]", objectID.c_str()));
					return;
				}
			}
			guiImages.push_back(tmpObject);
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Host function called by script to set position values
void gui_hostSetObjectPosition(int guiObjectType, const std::string &objectID, int coordType, int startX, int startY, int width, int height)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Setting position for - [ %s ]", objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;

	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str()));
		return;
	}

	switch (guiObjectType)
	{
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].coordType      = coordType;

			if (GUI_COORD_TYPE_PERCENT == guiButtons[objectIndex].coordType)
			{
				guiButtons[objectIndex].boundingBox.w = (sys_getLogicalWidth() * ((float) width / 100.0f));
				guiButtons[objectIndex].boundingBox.h = (sys_getLogicalHeight() * ((float) height / 100.0f));

				guiButtons[objectIndex].boundingBox.x = ((sys_getLogicalWidth() * ((float) startX / 100.0f)) - (guiButtons[objectIndex].boundingBox.w / 2));
				guiButtons[objectIndex].boundingBox.y = ((sys_getLogicalHeight() * ((float) startY / 100.0f)) - (guiButtons[objectIndex].boundingBox.h / 2));

				guiButtons[objectIndex].startX = guiButtons[objectIndex].boundingBox.x;
				guiButtons[objectIndex].startY = guiButtons[objectIndex].boundingBox.y;
				guiButtons[objectIndex].width  = guiButtons[objectIndex].boundingBox.w;
				guiButtons[objectIndex].height = guiButtons[objectIndex].boundingBox.h;
			}
			else
			{
				guiButtons[objectIndex].boundingBox.x = startX;
				guiButtons[objectIndex].boundingBox.y = startY;
				guiButtons[objectIndex].boundingBox.w = width;
				guiButtons[objectIndex].boundingBox.h = height;
			}
			guiButtons[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].coordType = coordType;
			if (GUI_COORD_TYPE_PERCENT == guiCheckBoxes[objectIndex].coordType)
			{
				guiCheckBoxes[objectIndex].boundingBox.w = (sys_getLogicalWidth() * ((float) width / 100.0f));
				guiCheckBoxes[objectIndex].boundingBox.h = (sys_getLogicalHeight() * ((float) height / 100.0f));

				guiCheckBoxes[objectIndex].boundingBox.x = ((sys_getLogicalWidth() * ((float) startX / 100.0f))); // + (guiCheckBoxes[objectIndex].boundingBox.w / 2));
				guiCheckBoxes[objectIndex].boundingBox.y = ((sys_getLogicalHeight() * ((float) startY / 100.0f))); // + (guiCheckBoxes[objectIndex].boundingBox.h / 2));
			}
			else
			{
				guiCheckBoxes[objectIndex].boundingBox.x = startX;
				guiCheckBoxes[objectIndex].boundingBox.y = startY;
				guiCheckBoxes[objectIndex].boundingBox.w = width;
				guiCheckBoxes[objectIndex].boundingBox.h = height;

			}
			guiCheckBoxes[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_TEXTBOX:
			guiTextBoxes[objectIndex].coordType      = coordType;
			if (GUI_COORD_TYPE_PERCENT == guiTextBoxes[objectIndex].coordType)
			{
				guiTextBoxes[objectIndex].boundingBox.w = (sys_getLogicalWidth() * ((float) width / 100.0f));
				guiTextBoxes[objectIndex].boundingBox.h = (sys_getLogicalHeight() * ((float) height / 100.0f));

				guiTextBoxes[objectIndex].boundingBox.x = ((sys_getLogicalWidth() * ((float) startX / 100.0f))); // + (guiTextBoxes[objectIndex].boundingBox.w / 2));
				guiTextBoxes[objectIndex].boundingBox.y = ((sys_getLogicalHeight() * ((float) startY / 100.0f))); // + (guiTextBoxes[objectIndex].boundingBox.h / 2));

			}
			else
			{
				guiTextBoxes[objectIndex].boundingBox.x = startX;
				guiTextBoxes[objectIndex].boundingBox.y = startY;
				guiTextBoxes[objectIndex].boundingBox.w = width;
				guiTextBoxes[objectIndex].boundingBox.h = height;
			}
			guiTextBoxes[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_LABEL:
			guiLabels[objectIndex].coordType      = coordType;
			if (GUI_COORD_TYPE_PERCENT == guiLabels[objectIndex].coordType)
			{
				guiLabels[objectIndex].boundingBox.w = (sys_getLogicalWidth() * ((float) width / 100.0f));
				guiLabels[objectIndex].boundingBox.h = (sys_getLogicalHeight() * ((float) height / 100.0f));

				guiLabels[objectIndex].boundingBox.x = ((sys_getLogicalWidth() * ((float) startX / 100.0f)) - (guiLabels[objectIndex].boundingBox.w / 2));
				guiLabels[objectIndex].boundingBox.y = ((sys_getLogicalHeight() * ((float) startY / 100.0f)) - (guiLabels[objectIndex].boundingBox.h / 2));

				guiLabels[objectIndex].startX = guiLabels[objectIndex].boundingBox.x;
				guiLabels[objectIndex].startY = guiLabels[objectIndex].boundingBox.y;
			}
			else
			{
				guiLabels[objectIndex].boundingBox.x = startX;
				guiLabels[objectIndex].boundingBox.y = startY;
				guiLabels[objectIndex].boundingBox.w = width;
				guiLabels[objectIndex].boundingBox.h = height;
			}
			guiLabels[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_IMAGE:
			guiImages[objectIndex].coordType      = coordType;
			if (GUI_COORD_TYPE_PERCENT == guiImages[objectIndex].coordType)
			{
// TODO get percent when image size is greater than 100
				guiImages[objectIndex].startX = (sys_getLogicalWidth() * ((float) width / 100.0f));
				guiImages[objectIndex].startY = (sys_getLogicalHeight() * ((float) height / 100.0f));
				guiImages[objectIndex].width  = width;
				guiImages[objectIndex].height = height;
			}
			else
			{
				guiImages[objectIndex].startX = startX;
				guiImages[objectIndex].startY = startY;
				guiImages[objectIndex].width  = width;
				guiImages[objectIndex].height = height;
			}
			guiImages[objectIndex].positionCalled = true;
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Set label string and position for an object
//
// Need to check that setPosition has been called before this - otherwise boundingbox isn't set
void gui_hostSetObjectLabel(int guiObjectType, const std::string &objectID, int labelPos, std::string newLabel)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Setting label for - [ %s ]", objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str()));
		return;
	}

	switch (guiObjectType)
	{
		case GUI_OBJECT_BUTTON:
			if (!guiButtons[objectIndex].positionCalled)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("GUI object position has not been set [ %s ]", objectID.c_str()));
				return;
			}
			guiButtons[objectIndex].text     = std::move(newLabel);
			guiButtons[objectIndex].labelPos = labelPos;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (!guiCheckBoxes[objectIndex].positionCalled)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("GUI object position has not been set [ %s ]", objectID.c_str()));
				return;
			}
			guiCheckBoxes[objectIndex].text     = std::move(newLabel);
			guiCheckBoxes[objectIndex].labelPos = labelPos;
			break;

		case GUI_OBJECT_TEXTBOX:
			if (!guiTextBoxes[objectIndex].positionCalled)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("GUI object position has not been set [ %s ]", objectID.c_str()));
				return;
			}
			guiTextBoxes[objectIndex].text     = std::move(newLabel);
			guiTextBoxes[objectIndex].labelPos = labelPos;
			break;

		case GUI_OBJECT_LABEL:
			if (!guiLabels[objectIndex].positionCalled)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("GUI object position has not been set [ %s ]", objectID.c_str()));
				return;
			}
			guiLabels[objectIndex].text     = std::move(newLabel);
			guiLabels[objectIndex].labelPos = labelPos;
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Set the script function to run when actioned
void gui_hostSetObjectFunctions(int guiObjectType, const std::string objectID, const std::string actionFunction)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Setting the function call for [ %s ] - [ %s ]", objectID.c_str(), clickFunction.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("Couldn't find GUI object index [ %s ]", objectID.c_str()));
		return;
	}

	switch (guiObjectType)
	{
		//
		// Actions for a object
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].action = actionFunction;
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].action = actionFunction;
			break;

		case GUI_OBJECT_TEXTBOX:
			guiTextBoxes[objectIndex].action = actionFunction;
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Set the color for the passed in element for this object
//
// Colors are in range 0..255
void gui_setObjectColorByIndex(int guiObjectType, int objectIndex, int whichColor, float red, float green, float blue, float alpha)
//-----------------------------------------------------------------------------
{
	switch (guiObjectType)
	{
		//
		// Colors for guiButton
		case GUI_OBJECT_BUTTON:
		{
			if (objectIndex > guiButtons.size() - 1)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("Index used to access guiButtons is too large."));
				return;
			}

			switch (whichColor)
			{
				case GUI_ACTIVE_COL:
					guiButtons[objectIndex].hasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_COL:
					guiButtons[objectIndex].noFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_ACTIVE_CORNER_COL:
					guiButtons[objectIndex].cornerFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_CORNER_COL:
					guiButtons[objectIndex].cornerNoFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_ACTIVE_LABEL_COL:
					guiButtons[objectIndex].labelHasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_LABEL_COL:
					guiButtons[objectIndex].labelNoFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				default:
					break;
			}
		}
			break;

		case GUI_OBJECT_CHECKBOX:
		{
			if (objectIndex > guiCheckBoxes.size() - 1)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("Index used to access guiCheckboxes is too large."));
				return;
			}

			switch (whichColor)
			{
				case GUI_ACTIVE_COL:
					guiCheckBoxes[objectIndex].hasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_COL:
					guiCheckBoxes[objectIndex].noFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_ACTIVE_CORNER_COL:
					guiCheckBoxes[objectIndex].cornerFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_CORNER_COL:
					guiCheckBoxes[objectIndex].cornerNoFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_ACTIVE_LABEL_COL:
					guiCheckBoxes[objectIndex].labelHasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_LABEL_COL:
					guiCheckBoxes[objectIndex].labelNoFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				default:
					break;
			}
		}
			break;

		case GUI_OBJECT_TEXTBOX:
		{
			if (objectIndex > guiTextBoxes.size() - 1)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("Index used to access guiTextBoxes is too large."));
				return;
			}

			switch (whichColor)
			{
				case GUI_ACTIVE_COL:
					guiTextBoxes[objectIndex].hasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_COL:
					guiTextBoxes[objectIndex].noFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_ACTIVE_CORNER_COL:
					guiTextBoxes[objectIndex].cornerFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_CORNER_COL:
					guiTextBoxes[objectIndex].cornerNoFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_ACTIVE_LABEL_COL:
					guiTextBoxes[objectIndex].labelHasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_LABEL_COL:
					guiTextBoxes[objectIndex].labelNoFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				default:
					break;
			}
		}
			break;

		case GUI_OBJECT_LABEL:
		{
			if (objectIndex > guiLabels.size() - 1)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("Index used to access guiLabels is too large."));
				return;
			}

			switch (whichColor)
			{
				case GUI_ACTIVE_COL:
					guiLabels[objectIndex].hasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_COL:
					guiLabels[objectIndex].noFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_ACTIVE_LABEL_COL:
					guiLabels[objectIndex].labelHasFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				case GUI_INACTIVE_LABEL_COL:
					guiLabels[objectIndex].labelNoFocusColor = al_map_rgba_f(red, green, blue, alpha);
					break;

				default:
					break;
			}
		}
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Host function called by script to set object colors
void gui_hostSetObjectColor(int guiObjectType, const std::string objectID, int whichColor, float red, float green, float blue, float alpha)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	io_logToFile("Setting colors for - [ %s ]", objectID.c_str());
#endif // DEBUG_GUI_SETUP

	int           objectIndex = 0;
	unsigned long numObjects  = 0;

	if (objectID == "ALL")    // Are we setting the colors for all of this type of object
	{
		switch (guiObjectType)    // Find out how many of this type there are
		{
			case GUI_OBJECT_BUTTON:
				numObjects = guiButtons.size();
				break;

			case GUI_OBJECT_CHECKBOX:
				numObjects = guiCheckBoxes.size();
				break;

			case GUI_OBJECT_TEXTBOX:
				numObjects = guiTextBoxes.size();
				break;

			case GUI_OBJECT_LABEL:
				numObjects = guiLabels.size();
				break;

			default:
				break;
		}

		if (numObjects == 0)
		{
			return;
		}

		for (int i = 0; i < numObjects; i++)
		{    // Loop through each object and set it's color
			gui_setObjectColorByIndex(guiObjectType, i, whichColor, red, green, blue, alpha);
		}
	}
	else    // Just setting color for one object
	{
		//
		// Find the index for this object
		objectIndex = gui_findIndex(guiObjectType, objectID);
		if (-1 == objectIndex)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str()));
			return;
		}
		gui_setObjectColorByIndex(guiObjectType, objectIndex, whichColor, red, green, blue, alpha);
	}
}

//-----------------------------------------------------------------------------
//
// Host function called by script to set whether the element is ready to use or not
void gui_hostSetReadyState(int guiObjectType, const std::string &objectID, bool newState)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Setting position for - [ %s ]", objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(guiObjectType, objectID);
	if (-1 == objectIndex)
	{
		log_logMessage(LOG_LEVEL_ERROR, sys_getString("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str()));
		return;
	}

	switch (guiObjectType)
	{
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_TEXTBOX:
			guiTextBoxes[objectIndex].ready = newState;

		case GUI_OBJECT_LABEL:
			guiLabels[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_IMAGE:
			guiImages[objectIndex].ready = newState;
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Find the objectID on the current screen and make it selected
void gui_hostSetObjectFocus(std::string objectID)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	for (indexCount = 0; indexCount != (int) guiScreens[currentGUIScreen].objectIDIndex.size(); indexCount++)
	{
		switch (guiScreens[currentGUIScreen].objectType[indexCount])
		{
			case GUI_OBJECT_BUTTON:
				if (guiButtons[guiScreens[currentGUIScreen].objectIDIndex[indexCount]].objectID == objectID)
				{
					guiScreens[currentGUIScreen].selectedObject = indexCount;
					return;
				}
				break;

			case GUI_OBJECT_CHECKBOX:
				if (guiCheckBoxes[guiScreens[currentGUIScreen].objectIDIndex[indexCount]].objectID == objectID)
				{
					guiScreens[currentGUIScreen].selectedObject = indexCount;
					return;
				}
				break;

			case GUI_OBJECT_TEXTBOX:
				if (guiTextBoxes[guiScreens[currentGUIScreen].objectIDIndex[indexCount]].objectID == objectID)
				{
					guiScreens[currentGUIScreen].selectedObject = indexCount;
					return;
				}
				break;
		}
	}
	log_logMessage(LOG_LEVEL_ERROR, sys_getString("Unable to locate GUI element [ %s ]", objectID.c_str()));
}


//-----------------------------------------------------------------------------
//
// Move focus to next object
void gui_handleFocusMove(int moveDirection, bool takeAction, int eventSource)
//-----------------------------------------------------------------------------
{
	int         indexCount = 0;
//	int selectedSlider, selectedKeyCode;
	b2Vec2 mouseLocation;

//	indexCount = 0;

//	selectedSlider = guiScreens[currentGUIScreen].objectIDIndex[guiScreens[currentGUIScreen].selectedObject];
//	selectedKeyCode = guiScreens[currentGUIScreen].objectIDIndex[guiScreens[currentGUIScreen].selectedObject];

	if (!isGUIStarted)
	{
		return;
	}

//	mouseLocation = io_getMousePosition();

	if (takeAction)
	{
		currentObjectSelected = guiScreens[currentGUIScreen].objectIDIndex[guiScreens[currentGUIScreen].selectedObject];

		evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 20, ALLEGRO_PLAYMODE_ONCE, "keyPressGood");

		switch (guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject])
		{
			case GUI_OBJECT_BUTTON:
				if (!eventSource)
				{   // not a mouse action
					sys_runScriptFunction(guiButtons[currentObjectSelected].action, guiButtons[currentObjectSelected].objectID);
				}
				else
				{   // Event is from mouse - check it's inside the bounds of this button to be activated
					if (sys_isPointInRect(mouseLocation, guiButtons[currentObjectSelected].boundingBox))
					{
						sys_runScriptFunction(guiButtons[currentObjectSelected].action, guiButtons[currentObjectSelected].objectID);
					}
				}
				break;

			case GUI_OBJECT_CHECKBOX:
				if (!eventSource)
				{
					sys_runScriptFunction(guiCheckBoxes[currentObjectSelected].action, guiCheckBoxes[currentObjectSelected].objectID);
				}
				else
				{
					if (sys_isPointInRect(mouseLocation, guiCheckBoxes[currentObjectSelected].boundingBox))
					{
						sys_runScriptFunction(guiCheckBoxes[currentObjectSelected].action, guiCheckBoxes[currentObjectSelected].objectID);
					}
				}

			default:
				break;
		}
		return;
	}

	switch (moveDirection)
	{
		case GUI_MOUSE_MOTION:      // Check each object on this screen to see if the mouse is on it - make selected if it is
			for (int i = 0; i != guiScreens[currentGUIScreen].objectIDIndex.size(); i++)
			{
				if (gui_canObjectBeSelected(guiScreens[currentGUIScreen].objectType[i]))
				{
					switch (guiScreens[currentGUIScreen].objectType[i])
					{
						case GUI_OBJECT_BUTTON:
							if (sys_isPointInRect(mouseLocation, guiButtons[guiScreens[currentGUIScreen].objectIDIndex[i]].boundingBox))
							{
								guiScreens[currentGUIScreen].selectedObject = i;
							}
							break;

						case GUI_OBJECT_CHECKBOX:
							if (sys_isPointInRect(mouseLocation, guiCheckBoxes[guiScreens[currentGUIScreen].objectIDIndex[i]].boundingBox))
							{
								guiScreens[currentGUIScreen].selectedObject = i;
							}
							break;
					}

				}
			}
			break;

		case GUI_MOVE_DOWN:
			indexCount = 1;
			if (guiScreens[currentGUIScreen].selectedObject != (int) guiScreens[currentGUIScreen].objectIDIndex.size() -
			                                                   1)    // Don't go past number of objects on the screen
			{
				while (!gui_canObjectBeSelected(guiScreens[currentGUIScreen].objectType[
						                                guiScreens[currentGUIScreen].selectedObject + indexCount]))
				{
					indexCount++;
				}
				guiScreens[currentGUIScreen].selectedObject += indexCount;

				evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 20, ALLEGRO_PLAYMODE_ONCE, "keyPressGood");

				if (indexCount > (int) guiScreens[currentGUIScreen].objectIDIndex.size())
				{
					indexCount = static_cast<int>(guiScreens[currentGUIScreen].objectIDIndex.size());
					evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 20, ALLEGRO_PLAYMODE_ONCE, "keyPressBad");
				}

				currentObjectSelected = guiScreens[currentGUIScreen].selectedObject;
			}
			break;

		case GUI_MOVE_UP:
			indexCount = 1;
			if (guiScreens[currentGUIScreen].selectedObject > 0)
			{
				while (!gui_canObjectBeSelected(guiScreens[currentGUIScreen].objectType[
						                                guiScreens[currentGUIScreen].selectedObject - indexCount]))
				{
					indexCount++;
					if (guiScreens[currentGUIScreen].selectedObject - indexCount < 0)
					{
							evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 20, ALLEGRO_PLAYMODE_ONCE, "keyPressBad");
						return;
					}
				}
				guiScreens[currentGUIScreen].selectedObject -= indexCount;
				evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 20, ALLEGRO_PLAYMODE_ONCE, "keyPressGood");

				if (guiScreens[currentGUIScreen].selectedObject < 0)
				{
					guiScreens[currentGUIScreen].selectedObject = 0;
				}

				currentObjectSelected = guiScreens[currentGUIScreen].selectedObject;
			}
			break;

		case GUI_MOVE_LEFT:
			switch (guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject])
			{
			}
			break;

		case GUI_MOVE_RIGHT:
			switch (guiScreens[currentGUIScreen].objectType[guiScreens[currentGUIScreen].selectedObject])
			{
			}
			break;

		default:
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Set an image keyName to the relevant resource image loaded
void gui_setImageKeyName(const std::string objectID, const std::string keyName)
//-----------------------------------------------------------------------------
{
#ifdef DEBUG_GUI_SETUP
	log_logMessage(LOG_LEVEL_DEBUG, sys_getString("Setting image keyName [ %s ] for - [ %s ]", keyName.c_str(), objectID.c_str()));
#endif // DEBUG_GUI_SETUP

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = gui_findIndex(GUI_OBJECT_IMAGE, objectID);
	if (-1 == objectIndex)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str()));
			return;
		}

	strcpy(guiImages[objectIndex].__GUI_element.image.keyName, keyName.c_str());
}