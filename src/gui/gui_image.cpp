#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceImage.h>
#include "hdr/gui/gui_image.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render an image onto the GUI
void gui_drawImage(int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	static int positionNotCalledCount = 0;

	int startX, startY;

	if (!guiImages[whichObject].ready)
		return;

	if (whichObject > guiImages.size())
		return;

	if (!guiImages[whichObject].positionCalled)
	{
		if (positionNotCalledCount < ERROR_REPEAT_NUMBER)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Position has not been set for Image [ %i ]", whichObject));
			positionNotCalledCount++;
		}
		return;
	}

	startX = guiImages[whichObject].startX;
	startY = guiImages[whichObject].startY;

	sys_drawBitmap (guiImages[whichObject].__GUI_element.image.fileName, startX, startY, RENDER_SOURCE);
}
