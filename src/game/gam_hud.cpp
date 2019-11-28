#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceImage.h>
#include <hdr/system/sys_font.h>
#include <hdr/game/gam_game.h>
#include <hdr/game/gam_transferRender.h>
#include <hdr/gui/gui_text.h>
#include "hdr/game/gam_hud.h"

std::string hudStatusText;

//--------------------------------------------------------------------------------------------------------------------------------------------
//
// Set the text for the HUD status
void hud_setText (bool useDirectValue, std::string newText)
//--------------------------------------------------------------------------------------------------------------------------------------------
{
	if (!useDirectValue)
		hudStatusText = gui_getString(newText);
	else
		hudStatusText = newText;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
//
// Render the HUD - scale to logical screen width
void hud_renderHUD ()
//--------------------------------------------------------------------------------------------------------------------------------------------
{
	float hudScoreFontX;
	float hudStatusFontX;
	float hudFontY;

	try
		{
			if (!bitmap.at ("hud").common.loaded)
				return;
		}

	catch (const std::out_of_range &oor)
		{
			return;
		}

	fnt_setColor_f (0.0f, 0.0f, 0.0f, 1.0f);
	fnt_setTTF ("gui");

	switch (currentMode)
		{
			case MODE_GUI_TRANSFER_CHOOSE_SIDE:
			case MODE_PLAY_TRANSFER_GAME:
				sys_drawBitmap ("hud", 0.0f, 0.0f, RENDER_SCALE, trn_getTransferBitmapWidth (), sys_getImageHeight ("hud"));
			hudScoreFontX = trn_getTransferBitmapWidth () - fnt_getWidth (sys_getString ("%s", gam_getScore ().c_str ()));
			hudScoreFontX -= fnt_getWidth("--");
			hudStatusFontX = fnt_getWidth("-");
			break;

			default:
				sys_drawBitmap ("hud", 0.0f, 0.0f, RENDER_SCALE, sys_getLogicalWidth (), sys_getImageHeight ("hud"));
			hudScoreFontX = sys_getLogicalWidth () - fnt_getWidth (sys_getString ("%s", gam_getScore ().c_str ()));
			hudScoreFontX -= fnt_getWidth("--");
			hudStatusFontX = fnt_getWidth("-");
			break;
		}

	hudFontY = (sys_getImageHeight ("hud") / 2) - (fnt_getHeight () / 2);

	fnt_render (b2Vec2{hudScoreFontX, hudFontY}, sys_getString ("%s", gam_getScore ().c_str ()));

	fnt_render (b2Vec2{hudStatusFontX, hudFontY}, sys_getString ("%s", hudStatusText.c_str()));
}

