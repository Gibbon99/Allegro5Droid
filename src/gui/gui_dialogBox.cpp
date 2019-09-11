#include <map>
#include <hdr/system/sys_font.h>
#include "hdr/gui/gui_dialogBox.h"

std::map<int, __GUI_messageBox> messageBox;
int gapSize = 4;

//----------------------------------------------------------------------------------------------------------------------
//
// Setup a message box from the script
void gui_addMessageBox(int boxName, std::string title, std::string text, int posX, int posY, bool modal)
//----------------------------------------------------------------------------------------------------------------------
{
	__GUI_messageBox tempMessageBox;

	tempMessageBox.title     = std::move(title);
	tempMessageBox.text      = std::move(text);
	tempMessageBox.positionX = posX;
	tempMessageBox.positionY = posY;
	tempMessageBox.modal     = modal;

	tempMessageBox.width  = fnt_getWidth(tempMessageBox.text) + (gapSize * 2);
	tempMessageBox.height = fnt_getHeight() * 4;

	messageBox.insert(std::pair<int, __GUI_messageBox>(boxName, tempMessageBox));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a message box
void gui_renderMessageBox(int objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	int radius = 3;
	int tempPosX, tempPosY;

	fnt_setTTF("gui");

	messageBox[objectID].width  = fnt_getWidth(messageBox[objectID].text) + (gapSize * 2);
	messageBox[objectID].height = fnt_getHeight() * 4;

	if (messageBox[objectID].positionX == -1)
		tempPosX = (screenWidth - messageBox[objectID].width) / 2;
	else
		tempPosX = messageBox[objectID].positionX;

	if (messageBox[objectID].positionY == -1)
		tempPosY = (screenHeight - messageBox[objectID].height) / 2;
	else
		tempPosY = messageBox[objectID].positionY;

	al_draw_filled_rounded_rectangle(tempPosX, tempPosY,
	                                 tempPosX + messageBox[objectID].width, tempPosY + messageBox[objectID].height,
			radius, radius, al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f));

	al_draw_line(tempPosX, tempPosY + fnt_getHeight(), tempPosX + messageBox[objectID].width, tempPosY + fnt_getHeight(),
	                  al_map_rgba_f(1.0f, 0.2f, 0.2f, 1.0f), 3);

	fnt_render(b2Vec2{tempPosX + gapSize, tempPosY + gapSize}, messageBox[objectID].title);

	fnt_render(b2Vec2{tempPosX+ gapSize, tempPosY + (fnt_getHeight() * 2)}, messageBox[objectID].text);
}