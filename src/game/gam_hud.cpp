#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceImage.h>
#include <hdr/system/sys_font.h>
#include <hdr/game/gam_game.h>
#include <hdr/game/gam_transferRender.h>
#include "hdr/game/gam_hud.h"

//---------------------------------------------------------------------------------
//
// Render the HUD - scale to logical screen width
void hud_renderHUD()
//---------------------------------------------------------------------------------
{
	float hudFontX;
	float hudFontY;

	try
		{
			if (!bitmap.at("hud").common.loaded)
				return;
		}

	catch (const std::out_of_range &oor)
		{
			return;
		}

		switch (currentMode)
			{
				case MODE_GUI_TRANSFER_CHOOSE_SIDE:
					sys_drawBitmap ("hud", 0.0f, 0.0f, RENDER_SCALE, trn_getTransferBitmapWidth (), sys_getImageHeight ("hud"));
					break;

				default:
					sys_drawBitmap ("hud", 0.0f, 0.0f, RENDER_SCALE, sys_getLogicalWidth (), sys_getImageHeight ("hud"));
					break;
			}


	fnt_setTTF ("gui");

	hudFontX = 30.0f;
	hudFontY = (sys_getImageHeight ("hud") / 2) - (fnt_getHeight () / 2);

	fnt_render (b2Vec2{hudFontX, hudFontY}, sys_getString("%s",gam_getScore().c_str()));
}

