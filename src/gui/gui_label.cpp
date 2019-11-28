#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_font.h>
#include "hdr/gui/gui_label.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a label element
void gui_drawLabel(int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	static int positionNotCalledCount = 0;
	static int invalidScreenPositionCount = 0;

	float startX, startY;

	if (guiLabels.empty())
		return;

	if (!guiLabels[whichObject].ready)
		return;

	if (whichObject > guiLabels.size())
		return;

	if (!guiLabels[whichObject].positionCalled)
	{
		if (positionNotCalledCount < ERROR_REPEAT_NUMBER)
		{
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Position has not been set for Label [ %i ]", whichObject));
			positionNotCalledCount++;
		}
		return;
	}

	fnt_setTTF (guiLabels[whichObject].fontName);

	switch (guiLabels[whichObject].labelPos)
	{
		case GUI_LABEL_CENTER:
			startX = guiLabels[whichObject].startX;
//			(sys_getLogicalWidth() - fnt_getWidth(guiLabels[whichObject].text)) / 2;
			startY = guiLabels[whichObject].startY;
			break;

		case GUI_LABEL_LEFT:
			startX = guiLabels[whichObject].startX;
			startY = guiLabels[whichObject].startY;
			break;

		case GUI_LABEL_RIGHT:
			startX = sys_getLogicalWidth() - fnt_getWidth(guiLabels[whichObject].text);
			startY = guiLabels[whichObject].startY;
			break;

		default:
			if (invalidScreenPositionCount < ERROR_REPEAT_NUMBER)
			{
				log_logMessage(LOG_LEVEL_ERROR, sys_getString("Invalid screen position passed to label - [ %i ]", whichObject));
				startX = 100;
				startY = 20;
				invalidScreenPositionCount++;
			}
			break;

	}
	fnt_setColor_f (guiLabels[whichObject].noFocusColor.r, guiLabels[whichObject].noFocusColor.g, guiLabels[whichObject].noFocusColor.b, guiLabels[whichObject].noFocusColor.a );
	fnt_render(b2Vec2{startX, startY},guiLabels[whichObject].text);
}
