#include <hdr/io/io_resources.h>
#include <hdr/io/io_logFile.h>
#include "hdr/io/io_resourceImage.h"

bool useHoldBitmap;

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

	try
	{
		if (bitmap.at (key).common.loaded)  // Already loaded
		{
			al_destroy_bitmap(bitmap.at(key).bitmap);
			bitmap.at(key).common.fileName = fileName;
			bitmap.at(key).bitmap = al_load_bitmap(fileName.c_str());
			if (nullptr == bitmap.at(key).bitmap)
			{
				bitmap.at(key).common.loaded = false;
				log_logMessage (LOG_LEVEL_ERROR, sys_getString("Failed to load [ %s ]", fileName.c_str()));
				return;
			}
			bitmap.at(key).width = al_get_bitmap_width(bitmap.at(key).bitmap);
			bitmap.at(key).height = al_get_bitmap_height(bitmap.at(key).bitmap);
			bitmap.at(key).common.loaded = true;

			log_logMessage (LOG_LEVEL_DEBUG, sys_getString("Loaded [ %s ]", fileName.c_str()));
			return;
		}
	}

	catch (const std::out_of_range &oor)
	{
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
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Draw a bitmap
void sys_drawBitmap (const std::string &keyName, float posX, float posY, int drawMode, float scaleX, float scaleY)
//---------------------------------------------------------------------------------------------------------------------------
{
	try
		{
			switch (drawMode)
				{
					case RENDER_FULLSCREEN:
						al_draw_scaled_bitmap (bitmap.at (keyName).bitmap, 0, 0, bitmap.at (keyName).width, bitmap.at (keyName).height, 0, 0, screenWidth, screenHeight, 0);
					break;

					case RENDER_SOURCE:
						al_draw_bitmap (bitmap.at (keyName).bitmap, posX, posY, 0);
					return;

					case RENDER_SCALE:
						al_draw_scaled_bitmap (bitmap.at (keyName).bitmap, 0, 0, bitmap.at (keyName).width, bitmap.at (keyName).height, posX, posY, scaleX, scaleY, 0);
						break;
				}
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Image [ %s ] not found - [ %s ]", keyName.c_str (), oor.what ()));
			return;
		}
}
