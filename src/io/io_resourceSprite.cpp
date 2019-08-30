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
	tempSprite.renderOffsetX = tempSprite.frameWidth / 2;
	tempSprite.renderOffsetY = tempSprite.frameHeight / 2;
	sprites.insert(std::pair<std::string, __sprite>(key, tempSprite));


}

//---------------------------------------------------------------------------------------------------------------------------
//
// Set the transparent pixel color for the sprite
void io_spriteSetAlphaColor(std::string key, int red, int green, int blue)
//---------------------------------------------------------------------------------------------------------------------------
{
	try
	{
			al_convert_mask_to_alpha (sprites.at(key).bitmap, al_map_rgb(red, green, blue));
	}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sprite [ %s ] not found. Unable to set alpha - [ %s ]", key.c_str(), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Render a sprite tinting to a different color
// Position is the center of the sprite - offset by frame width and height
void io_renderTintedSpriteFrame(std::string key, int whichFrame, float posX, float posY, int red, int green, int blue)
//---------------------------------------------------------------------------------------------------------------------------
{
	float newPosX, newPosY;

	try
		{
			newPosX = posX - sprites.at(key).renderOffsetX;
			newPosY = posY - sprites.at(key).renderOffsetY;

			al_draw_tinted_bitmap_region (sprites.at(key).bitmap,
					al_map_rgb(red, green, blue),
					sprites.at(key).frameWidth * whichFrame, 0,
					sprites.at(key).frameWidth, sprites.at(key).frameHeight,
					newPosX, newPosY, 0);
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sprite [ %s ] not found - [ %s ]", key.c_str(), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Render a frame of a sprite
// Position is the center of the sprite - offset by frame width and height
void io_renderSpriteFrame(std::string key, int whichFrame, float posX, float posY)
//---------------------------------------------------------------------------------------------------------------------------
{
	float newPosX, newPosY;

	try
		{
			newPosX = posX - sprites.at(key).renderOffsetX;
			newPosY = posY - sprites.at(key).renderOffsetY;

			al_draw_bitmap_region (sprites.at (key).bitmap,
			                       sprites.at(key).frameWidth * whichFrame, 0,
			                       sprites.at(key).frameWidth, sprites.at(key).frameHeight,
			                       newPosX, newPosY, 0);
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sprite [ %s ] not found - [ %s ]", key.c_str(), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Test animate sprite
void io_testAnimateSprite()
//---------------------------------------------------------------------------------------------------------------------------
{
	currentFrame++;
	if (currentFrame == sprites.at("123").numFrames)
		currentFrame = 0;
}

int io_getFrame()
{
	return currentFrame;
}