#include <hdr/io/io_resources.h>
#include <hdr/io/io_logFile.h>
#include "hdr/io/io_resourceImage.h"

//---------------------------------------------------------------------------------------------------------------------------
//
// Get width of image
int sys_getImageWidth(const std::string& keyName)
//---------------------------------------------------------------------------------------------------------------------------
{
	return bitmap.at(keyName).width;
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Get height of image
int sys_getImageHeight(const std::string& keyName)
//---------------------------------------------------------------------------------------------------------------------------
{
	return bitmap.at(keyName).height;
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Load a bitmap file
void sys_loadResourceBitmap (const std::string &key, const std::string &fileName)
//---------------------------------------------------------------------------------------------------------------------------
{
	__bitmap tempBitmap;

	tempBitmap.common.fileName = fileName;
	tempBitmap.bitmap          = al_load_bitmap (fileName.c_str ());
	if (nullptr == tempBitmap.bitmap)
	{
		tempBitmap.common.loaded = false;
		bitmap.insert (std::pair<std::string, __bitmap> (key, tempBitmap));
		log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Failed to load [ %s ]", fileName.c_str ()));
		return;
	}

	tempBitmap.width         = al_get_bitmap_width (tempBitmap.bitmap);
	tempBitmap.height        = al_get_bitmap_height (tempBitmap.bitmap);
	tempBitmap.common.loaded = true;
	bitmap.insert (std::pair<std::string, __bitmap> (key, tempBitmap));

	log_logMessage (LOG_LEVEL_DEBUG, sys_getString ("Loaded [ %s ]", fileName.c_str ()));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Draw a bitmap
void sys_drawBitmap (const std::string &keyName, float posX, float posY, int drawMode)
//---------------------------------------------------------------------------------------------------------------------------
{
	switch (drawMode)
	{
		case RENDER_FULLSCREEN:
			al_draw_scaled_bitmap (bitmap.at (keyName).bitmap, 0, 0, bitmap.at (keyName).width, bitmap.at (keyName).height, 0, 0, screenWidth, screenHeight, 0);
			break;

		case RENDER_SOURCE:
			al_draw_bitmap (bitmap.at (keyName).bitmap, 0.0f, 0.0f, 0);
			return;
	}

//	al_draw_bitmap (bitmap.at(keyName).bitmap, 0, 0, 0);
}
