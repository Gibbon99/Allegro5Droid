#pragma once

#include "system/sys_main.h"

enum GUI_ACTIONS
{
	GUI_MOVE_UP = 0,
	GUI_MOVE_DOWN,
	GUI_MOVE_LEFT,
	GUI_MOVE_RIGHT,
	GUI_ACTION,
	GUI_MOUSE_MOTION
};

typedef struct
{
	int              selectedObject;
	std::string      screenID;               // Name of this screen
	std::vector<int> objectIDIndex;          // Index into object array
	std::vector<int> objectType;             // Which object array
} _screenObject;

struct __GUI_scrollBox
{
	int           currentChar;
	float         scrollSpeed;
	float         scrollDelay;
	float         scrollY;
	int           numLinesToPrint;
	float         lineFade;
	float         previousScrollY;
	ALLEGRO_COLOR backgroundColor;
	char          fontName[32];
};

struct __GUI_checkBox
{
	int  whichGroup;
	int  boxSize;       // TODO - set this for bounding box calculation - get font size from script?
	bool isChecked;
};

struct __GUI_image
{
	char keyName[32];
};

struct __BOUNDING_BOX
{
	float w;
	float h;
	float x;
	float y;
};

struct __GUI_object
{
	bool                     ready          = false;
	std::string              objectID       = "";
	int                      screenID       = -1;                      // which screen does this belong to
	bool                     canFocus       = false;
	bool                     positionCalled = false;
	float                    gapSize        = 0;
	int                      lineWidth      = 0;
	float                    startX         = -1;
	float                    startY         = -1;
	float                    width          = -1;
	float                    height         = -1;
	int                      coordType      = -1;
	float                    radius         = -1;
	std::string              text           = "";
	std::string              action         = "";
	ALLEGRO_COLOR            color{};
	ALLEGRO_COLOR            hasFocusColor{};
	ALLEGRO_COLOR            noFocusColor{};
	ALLEGRO_COLOR            cornerFocusColor{};
	ALLEGRO_COLOR            cornerNoFocusColor{};
	ALLEGRO_COLOR            labelHasFocusColor{};
	ALLEGRO_COLOR            labelNoFocusColor;
	float                    fadeCounter    = 0;
	bool                     isHighlited    = false;
	float                    mouseBoxX      = 0;
	float                    mouseBoxY      = 0;
	float                    mouseBoxWidth  = 0;
	float                    mouseBoxHeight = 0;
	int                      labelPos       = 0;
	__BOUNDING_BOX           boundingBox;
	std::vector<std::string> scrollBoxLineText;

	union
	{
		struct __GUI_scrollBox scrollBox;
		struct __GUI_checkBox  checkBox;
		struct __GUI_image     image;
	}                        __GUI_element;
};


extern int                       currentGUIScreen;
extern int                       currentObjectSelected;  // Pass this to script to act on
extern bool                      isGUIStarted;
extern std::vector<__GUI_object> guiButtons;
extern std::vector<__GUI_object> guiCheckBoxes;
extern std::vector<__GUI_object> guiLabels;
extern std::vector<__GUI_object> guiTextBoxes;
extern std::vector<__GUI_object> guiImages;

extern std::vector<_screenObject> guiScreens;

extern __GUI_object introScrollBox;

// Change to a new GUI screen
void gui_changeToGUIScreen (int newScreen);

// Return if an object can be selected or not
bool gui_canObjectBeSelected (int objectType);

// Create a new GUI screen which will contain other GUI objects
void gui_hostCreateNewScreen (std::string screenID);

// Associate the object to a screen, recording it's index and type
void gui_hostAddObjectToScreen (int guiObjectType, std::string objectID, std::string whichScreen);

// Add a gui object to it's relevant vector array
void gui_hostCreateObject (int guiObjectType, std::string objectID);

// Host function called by script to set position values
void gui_hostSetObjectPosition (int guiObjectType, const std::string &objectID, int coordType, int startX, int startY, int width, int height);

// Need to check that setPosition has been called before this - otherwise boundingbox isn't set
void gui_hostSetObjectLabel (int guiObjectType, const std::string &objectID, int labelPos, std::string newLabel);

// Set the script function to run when actioned
void gui_hostSetObjectFunctions (int guiObjectType, std::string objectID, std::string actionFunction);

// Host function called by script to set object colors
void gui_hostSetObjectColor (int guiObjectType, std::string objectID, int whichColor, float red, float green, float blue, float alpha);

// Colors are in range 0..255
void gui_setObjectColorByIndex (int guiObjectType, int objectIndex, int whichColor, float red, float green, float blue, float alpha);

// Host function called by script to set whether the element is ready to use or not
void gui_hostSetReadyState (int guiObjectType, const std::string &objectID, bool newState);

// Find the objectID on the current screen and make it selected
void gui_hostSetObjectFocus (std::string objectID);

// Move focus to next object
void gui_handleFocusMove (int moveDirection, bool takeAction, int eventSource);

// returns index value, or -1 if not found
int gui_findIndex (int guiObjectType, std::string objectID);

// Set an image keyName to the relevant resource image loaded
void gui_setImageKeyName(std::string objectID, std::string keyName);