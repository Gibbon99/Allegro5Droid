#include <hdr/io/io_resources.h>
#include <hdr/io/io_logFile.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_player.h>
#include "hdr/io/io_resourceSprite.h"

int currentFrame = 0;

//---------------------------------------------------------------------------------------------------------------------------
//
// Load a sprite - pass in true for reload to use another bitmap file for same key
void io_loadResourceSprite (const std::string &key, const std::string &fileName, int numFrames, bool reload)
//---------------------------------------------------------------------------------------------------------------------------
{
	__sprite    tempSprite;
	std::string reloadFileName;

	tempSprite.common.fileName = fileName;
	tempSprite.numFrames       = numFrames;
	tempSprite.bitmap          = nullptr;

	if (reload)
		{
			al_destroy_bitmap (sprites.at (key).bitmap);
			reloadFileName = fileName + ".bmp";
			sprites.at (key).bitmap = al_load_bitmap (reloadFileName.c_str ());
			if (nullptr == sprites.at (key).bitmap)
				{
					log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Unable to load database bitmap [ %s ]", reloadFileName.c_str ()));
					sprites.at (key).common.loaded = false;
					return;
				}
			sprites.at (key).common.loaded = true;
			return;
		}

	if (nullptr == tempSprite.bitmap)
		{
			tempSprite.bitmap = al_load_bitmap (fileName.c_str ());
			if (nullptr == tempSprite.bitmap)
				{
					log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Failed to load sprite [ %s ]", fileName.c_str ()));
					tempSprite.common.loaded = false;
					return;
				}
			tempSprite.common.loaded = true;
			tempSprite.frameWidth    = al_get_bitmap_width (tempSprite.bitmap) / numFrames;
			tempSprite.frameHeight   = al_get_bitmap_height (tempSprite.bitmap);
			tempSprite.renderOffsetX = tempSprite.frameWidth / 2;
			tempSprite.renderOffsetY = tempSprite.frameHeight / 2;
			sprites.insert (std::pair<std::string, __sprite> (key, tempSprite));
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Set the transparent pixel color for the sprite
void io_spriteSetAlphaColor (const std::string &key, int red, int green, int blue)
//---------------------------------------------------------------------------------------------------------------------------
{
	try
		{
			al_convert_mask_to_alpha (sprites.at (key).bitmap, al_map_rgb (red, green, blue));
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sprite [ %s ] not found. Unable to set alpha - [ %s ]", key.c_str (), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Render a rotated sprite
void io_renderRotatedSpriteFrame (const std::string &key, int whichFrame, float posX, float posY, float angle)
//---------------------------------------------------------------------------------------------------------------------------
{
	ALLEGRO_BITMAP *singleFrame = nullptr;

	try
		{
			singleFrame = al_create_sub_bitmap (sprites.at (key).bitmap, whichFrame * sprites.at (key).frameWidth, 0, sprites.at (key).frameWidth, sprites.at (key).frameHeight);

			al_draw_rotated_bitmap (singleFrame, sprites.at (key).frameWidth / 2, sprites.at (key).frameHeight / 2, posX, posY, angle, 0);
			al_destroy_bitmap (singleFrame);
			singleFrame = nullptr;
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sprite [ %s ] not found - [ %s ]", key.c_str (), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Render a sprite tinting to a different color
// Position is the center of the sprite - offset by frame width and height
// Position is in world space pixels
void io_renderTintedSpriteFrame (const std::string &key, int whichFrame, float posX, float posY, float red, float green, float blue, float alpha)
//---------------------------------------------------------------------------------------------------------------------------
{
	float newPosX, newPosY;

	try
		{
			newPosX = posX - sprites.at (key).renderOffsetX;
			newPosY = posY - sprites.at (key).renderOffsetY;

			al_draw_tinted_bitmap_region (sprites.at (key).bitmap, al_map_rgba_f (red, green, blue, alpha), sprites.at (key).frameWidth * whichFrame, 0, sprites.at (key).frameWidth, sprites.at (key).frameHeight, newPosX, newPosY, 0);
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sprite [ %s ] not found - [ %s ]", key.c_str (), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Render a frame of a sprite
// Position is the center of the sprite - offset by frame width and height
void io_renderSpriteFrame (const std::string &key, int whichFrame, float posX, float posY)
//---------------------------------------------------------------------------------------------------------------------------
{
	float newPosX, newPosY;

	try
		{
			newPosX = posX - sprites.at (key).renderOffsetX;
			newPosY = posY - sprites.at (key).renderOffsetY;

			al_draw_bitmap_region (sprites.at (key).bitmap, sprites.at (key).frameWidth * whichFrame, 0, sprites.at (key).frameWidth, sprites.at (key).frameHeight, newPosX, newPosY, 0);
		}

	catch (const std::out_of_range &oor)
		{
			log_logMessage (LOG_LEVEL_ERROR, sys_getString ("Sprite [ %s ] not found - [ %s ]", key.c_str (), oor.what ()));
			return;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Animate player sprite
void io_animatePlayerSprite (double tickTime)
//---------------------------------------------------------------------------------------------------------------------------
{
	switch (playerDroid.currentMode)
		{
			case DROID_MODE_NORMAL:
				playerDroid.frameAnimCounter -= (30.0f * playerDroid.frameDelay) * (float) tickTime;
			if (playerDroid.frameAnimCounter < 0.0f)
				{
					playerDroid.frameAnimCounter = 1.0f;
					playerDroid.currentFrame++;
					if (playerDroid.currentFrame == sprites.at ("001").numFrames)
						playerDroid.currentFrame = 0;
				}
			break;

			case DROID_MODE_EXPLODING:
				break;
		}
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Animate droid sprites
void io_animateEnemySprites (const std::string &levelName, double tickTime)
//---------------------------------------------------------------------------------------------------------------------------
{
	io_animatePlayerSprite (tickTime);

	for (int i = 0; i != shipLevel.at (levelName).numDroids; i++)
		{
			switch (shipLevel.at (levelName).droid[i].currentMode)
				{
					case DROID_MODE_NORMAL:
						shipLevel.at (levelName).droid[i].frameAnimCounter -= (droidAnimateSpeed * shipLevel.at (levelName).droid[i].frameDelay) * (float) tickTime;
					if (shipLevel.at (levelName).droid[i].frameAnimCounter < 0.0f)
						{
							shipLevel.at (levelName).droid[i].frameAnimCounter = 1.0f;
							shipLevel.at (levelName).droid[i].currentFrame++;
							if (shipLevel.at (levelName).droid[i].currentFrame == sprites.at ("001").numFrames)
								shipLevel.at (levelName).droid[i].currentFrame = 0;
						}
					break;

					case DROID_MODE_EXPLODING:
						shipLevel.at (levelName).droid[i].frameAnimCounter -= explodeAnimateSpeed * (float) tickTime;
					if (shipLevel.at (levelName).droid[i].frameAnimCounter < 0.0f)
						{
							shipLevel.at (levelName).droid[i].frameAnimCounter = 1.0f;
							shipLevel.at (levelName).droid[i].currentFrame++;
							if (shipLevel.at (levelName).droid[i].currentFrame == sprites.at ("explosion").numFrames)
								{
									shipLevel.at (levelName).droid[i].currentFrame = 0;
									gam_removeDroid (i);
								}
						}
					break;
				}
		}
}