#include <map>
#include <hdr/system/sys_font.h>
#include <hdr/gui/gui_render.h>
#include <hdr/game/gam_pauseMode.h>
#include "hdr/gui/gui_dialogBox.h"

std::map<std::string, __GUI_dialogBox> dialogBox;
int                                    gapSize         = 4;
bool                                    dialogBoxActive = false;
std::string                             currentDialogBoxName;

//
// Add other elements to a messagebox
//

//----------------------------------------------------------------------------------------------------------------------
//
// Setup a message box from the script
void gui_addDialogBox (const std::string& dialogName, std::string title, std::string text, int posX, int posY, bool modal)
//----------------------------------------------------------------------------------------------------------------------
{
	__GUI_dialogBox tempMessageBox;

	fnt_setTTF ("gui");

	tempMessageBox.title     = std::move (title);
	tempMessageBox.text      = std::move (text);
	tempMessageBox.positionX = posX;
	tempMessageBox.positionY = posY;
	tempMessageBox.modal     = modal;

	tempMessageBox.width  = fnt_getWidth (tempMessageBox.text) + (gapSize * 2);
	tempMessageBox.height = fnt_getHeight () * 4;

	dialogBox.insert (std::pair<std::string, __GUI_dialogBox> (dialogName, tempMessageBox));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a message box
void gui_renderDialogBox ()
//----------------------------------------------------------------------------------------------------------------------
{
	int radius = 3;
	int tempPosX, tempPosY;
	int indexCount = 0;

	fnt_setTTF ("gui");

	dialogBox.at (currentDialogBoxName).width  = fnt_getWidth (dialogBox.at (currentDialogBoxName).text) + (gapSize * 2);
	dialogBox.at (currentDialogBoxName).height = fnt_getHeight () * 4;

	if (dialogBox.at (currentDialogBoxName).positionX == -1)
		tempPosX = (screenWidth - dialogBox.at (currentDialogBoxName).width) / 2;
	else
		tempPosX = dialogBox.at (currentDialogBoxName).positionX;

	if (dialogBox.at (currentDialogBoxName).positionY == -1)
		tempPosY = (screenHeight - dialogBox.at (currentDialogBoxName).height) / 2;
	else
		tempPosY = dialogBox.at (currentDialogBoxName).positionY;

	al_draw_filled_rounded_rectangle (tempPosX, tempPosY, tempPosX + dialogBox.at (currentDialogBoxName).width, tempPosY + dialogBox.at (currentDialogBoxName).height, radius, radius, al_map_rgba_f (1.0f, 1.0f, 1.0f, 1.0f));

	al_draw_line (tempPosX, tempPosY + fnt_getHeight (), tempPosX + dialogBox.at (currentDialogBoxName).width, tempPosY + fnt_getHeight (), al_map_rgba_f (1.0f, 0.2f, 0.2f, 1.0f), 3);

	fnt_render (b2Vec2{static_cast<float32>(tempPosX + gapSize), static_cast<float32>(tempPosY + gapSize)}, dialogBox.at (currentDialogBoxName).title);

	fnt_render (b2Vec2{static_cast<float32>(tempPosX + gapSize), tempPosY + (fnt_getHeight () * 2)}, dialogBox.at (currentDialogBoxName).text);

	for (indexCount = 0; indexCount != dialogBox.at(currentDialogBoxName).objectIDIndex.size (); indexCount++)
	{
		if (dialogBox.at(currentDialogBoxName).selectedObject == indexCount)
			gui_drawObject (dialogBox.at(currentDialogBoxName).objectType[indexCount], dialogBox.at(currentDialogBoxName).objectIDIndex[indexCount], true);
		else
			gui_drawObject (dialogBox.at(currentDialogBoxName).objectType[indexCount], dialogBox.at(currentDialogBoxName).objectIDIndex[indexCount], false);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Activate a dialog box
void gui_activateDialogBox (const std::string &dialogName)
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCount = 0;

	dialogBoxActive = true;
	gam_handlePauseMode(true);

	indexCount = 0;

	for (auto itr : dialogBox)
	{
		if (itr.first == dialogName)    // Match the map key
		{
			currentDialogBox = indexCount;
			currentDialogBoxName = dialogName;
			return;
		}
		indexCount++;
	}
}