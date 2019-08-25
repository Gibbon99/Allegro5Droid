#include <hdr/io/io_resources.h>
#include <hdr/io/io_logFile.h>
#include "hdr/io/io_resourceSprite.h"

int currentFrame = 0;

//---------------------------------------------------------------------------------------------------------------------------
//
// Load a sprite
void io_loadResourceSprite(std::string key, std::string fileName, int numFrames)
//---------------------------------------------------------------------------------------------------------------------------
{
	__sprite tempSprite;

	tempSprite.common.fileName = fileName;
	tempSprite.numFrames       = numFrames;
	tempSprite.bitmap          = al_load_bitmap(fileName.c_str());

	if (nullptr == tempSprite.bitmap)
		{
			tempSprite.common.loaded = false;
			sprites.insert(std::pair<std::string, __sprite>(key, tempSprite));
			log_logMessage(LOG_LEVEL_ERROR, sys_getString("Failed to load sprite [ %s ]", fileName.c_str()));
			return;
		}

	tempSprite.frameWidth  = al_get_bitmap_width(tempSprite.bitmap) / numFrames;
	tempSprite.frameHeight = al_get_bitmap_height(tempSprite.bitmap);
	sprites.insert(std::pair<std::string, __sprite>(key, tempSprite));

	al_convert_mask_to_alpha (sprites.at("001").bitmap, al_map_rgb(0, 0, 0));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Render a frame of a sprite
void io_renderSpriteFrame(std::string key, int whichFrame, float posX, float posY)
//---------------------------------------------------------------------------------------------------------------------------
{
	al_draw_bitmap_region (sprites.at (key).bitmap,
	                       sprites.at(key).frameWidth * whichFrame, 0,
	                       sprites.at(key).frameWidth, sprites.at(key).frameHeight,
	                       posX, posY, 0);

}

//---------------------------------------------------------------------------------------------------------------------------
//
// Test animate sprite
void io_testAnimateSprite()
//---------------------------------------------------------------------------------------------------------------------------
{
	currentFrame++;
	if (currentFrame == sprites.at("001").numFrames)
		currentFrame = 0;
}

int io_getFrame()
{
	return currentFrame;
}