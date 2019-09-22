#include <hdr/io/io_resourceSprite.h>
#include <hdr/io/io_resources.h>
#include <hdr/system/sys_font.h>
#include <hdr/gui/gui_text.h>
#include "hdr/gui/gui_database.h"

float databaseAnimTime;
int   dbCurrentFrame = 0;

//----------------------------------------------------------------------------------------
//
// Render the database screen
void gui_renderDatabaseScreen ()
//----------------------------------------------------------------------------------------
{
	std::string imageLoading;
	b2Vec2      textPosition;

	if (sprites.at ("db_sprite").common.loaded)
		io_renderSpriteFrame ("db_sprite", dbCurrentFrame, (sys_getLogicalWidth () / 2), (sys_getLogicalHeight () / 2));
	else
		{
			imageLoading = gui_getString ("imageLoading");
			textPosition.y = sys_getLogicalHeight () / 2;
			textPosition.x = (sys_getLogicalWidth () - fnt_getWidth (imageLoading)) / 2;
			fnt_setTTF (databaseScrollBox.__GUI_element.scrollBox.fontName);
			fnt_setColor_f (1.0f, 0.0f, 1.0f, 1.0f);
			fnt_render (textPosition, imageLoading.c_str());
		}
}

//----------------------------------------------------------------------------------------
//
// Animate the droid model in the database view
void gui_animateDroidSprite (float tickTime)
//----------------------------------------------------------------------------------------
{
	static float databaseAnimateDelay = 1.0f;

	databaseAnimateDelay -= (databaseAnimTime * tickTime);
	if (databaseAnimateDelay < 0.0f)
		{
			databaseAnimateDelay = 1.0f;
			dbCurrentFrame++;
			if (dbCurrentFrame == sprites.at ("db_sprite").numFrames)
				{
					dbCurrentFrame = 0;
				}
		}
}