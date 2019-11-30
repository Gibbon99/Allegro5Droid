#include <hdr/game/gam_player.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/io/io_resources.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_game.h>
#include <hdr/io/io_logFile.h>
#include <hdr/game/gam_particles.h>
#include <hdr/system/sys_audio.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_droids.h>
#include "hdr/game/gam_bullet.h"

std::vector<__bulletObject> bullets;
float                       bulletAnimSpeed;          // From script
float                       bulletMoveSpeed;          // From script
float                       bulletDensity;            // From script
int                         numStartingBullets;       // From script
int                         numDisrupterFrames;       // From script
float                       disrupterFadeAmount;      // From script

//-----------------------------------------------------------------------------
//
// Get the angle of rotation for the bullet
auto bul_getBulletAngle (b2Vec2 sourcePos, b2Vec2 destPos) -> double
//-----------------------------------------------------------------------------
{
	b2Vec2 a, b;

	double angle = (atan2 (destPos.y - sourcePos.y, destPos.x - sourcePos.x) * (180 / MY_PI));

	if (angle < 0)
		angle = 360 - (-angle);

	return angle * DEGTORAD;       // Convert from degrees to radians
}

//---------------------------------------------------------------------------------------------------------------
//
// Clear memory for bullets
void bul_clearAllBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	for (auto &bulletItr : bullets)
		{
			bulletItr.inUse = false;

			if (bulletItr.userData != nullptr)
				delete (bulletItr.userData);

			if (bulletItr.body != nullptr)
				sys_getPhysicsWorld ()->DestroyBody (bulletItr.body);
		}
	bullets.clear ();
}

//---------------------------------------------------------------------------------------------------------------
//
// Init bullets for level change
void bul_initBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	bul_clearAllBullets ();
}

//---------------------------------------------------------------------------------------------------------------
//
// Remove a bullet
void bul_removeBullet (int whichBullet)
//---------------------------------------------------------------------------------------------------------------
{
	if (bullets.at (whichBullet).body != nullptr)
		{
			sys_getPhysicsWorld ()->DestroyBody (bullets.at (whichBullet).body);
			bullets.at (whichBullet).body = nullptr;
		}
	bullets.at (whichBullet).inUse = false;
	par_removeEmitter (whichBullet);
}

//---------------------------------------------------------------------------------------------------------------
//
// Init the bullet array size
void bul_initBulletArraySize ()
//---------------------------------------------------------------------------------------------------------------
{
	__bulletObject tempBullet;

	bullets.reserve (numStartingBullets);

	for (int i = 0; i != numStartingBullets; i++)
		{
			tempBullet.inUse = false;
			bullets.push_back (tempBullet);
		}
}

//---------------------------------------------------------------------------------------------------------------
//
// Do damage from disrupter fire
void bul_doDisrupterDamage (int sourceDroid)
//---------------------------------------------------------------------------------------------------------------
{
	std::string currentLevelName;

	currentLevelName = lvl_getCurrentLevelName ();

	if (-1 == sourceDroid)    // Player used disrupter
		{
			for (auto i = 0; i != shipLevel.at (currentLevelName).numDroids; i++)
				{
					if ((shipLevel.at (currentLevelName).droid[i].visibleToPlayer) &&
					    (shipLevel.at (currentLevelName).droid[i].currentMode != DROID_MODE_DEAD) &&
					    (!dataBaseEntry[shipLevel.at (currentLevelName).droid[i].droidType].disrupterImmune))
						{
							gam_damageToDroid (i, DAMAGE_BULLET, sourceDroid, -1);
						}
				}
		}
	else  // Enemy used disrupter
		{

		}
}

//---------------------------------------------------------------------------------------------------------------
//
// Setup information for new bullet
auto bul_setupNewBullet (int bulletSourceIndex, int arrayIndex) -> __bulletObject
//---------------------------------------------------------------------------------------------------------------
{
	__bulletObject tempBullet;
	b2Vec2         tempPos;
	int            bulletType;

	if (bulletSourceIndex == -1)        // Bullet came from player
		{
			tempBullet.worldPos.x = playerDroid.worldPos.x;     // Physics coords
			tempBullet.worldPos.y = playerDroid.worldPos.y;
			tempBullet.velocity   = playerDroid.velocity;

			tempPos = playerDroid.velocity;
			tempPos.Normalize ();
			tempPos.operator*= (12.0f);
			tempBullet.worldPos += tempPos;

			tempBullet.velocity.operator*= (2.0f);
			tempBullet.destPos = tempBullet.worldPos + tempBullet.velocity;

			bulletType = dataBaseEntry[playerDroid.droidType].bulletType;
		}
	else                                // Bullet came from enemy droid - index on level used
		{
			tempBullet.worldPos = shipLevel.at (lvl_getCurrentLevelName ()).droid[bulletSourceIndex].worldPos;
			tempBullet.destPos  = playerDroid.worldPos;
			tempBullet.velocity = tempBullet.destPos - tempBullet.worldPos;
			tempBullet.velocity.Normalize ();
			tempBullet.velocity *= bulletMoveSpeed;

			tempPos = tempBullet.velocity;
			tempPos.Normalize ();
			tempPos.operator*= (24.0f);
			tempBullet.worldPos += tempPos;

			bulletType = dataBaseEntry[shipLevel.at (lvl_getCurrentLevelName ()).droid[bulletSourceIndex].droidType].bulletType;
		}

// TODO Test
// bulletType = BULLET_TYPE_DISRUPTER;

	if (bulletType != BULLET_TYPE_DISRUPTER)
		{
			tempBullet.angle = bul_getBulletAngle (tempBullet.worldPos, tempBullet.destPos);

			tempBullet.bodyDef.type = b2_dynamicBody;
			tempBullet.bodyDef.position.Set (tempBullet.worldPos.x / pixelsPerMeter, tempBullet.worldPos.y / pixelsPerMeter);
			tempBullet.bodyDef.angle  = 0;
			tempBullet.bodyDef.bullet = true;
			tempBullet.body           = sys_getPhysicsWorld ()->CreateBody (&tempBullet.bodyDef);
			if (tempBullet.body == nullptr)
				{
					tempBullet.inUse = false;
					return tempBullet;
				}

			tempBullet.userData             = new _userData;
			if (bulletSourceIndex == -1)
				tempBullet.userData->userType = PHYSIC_TYPE_BULLET_PLAYER;
			else
				tempBullet.userData->userType = PHYSIC_TYPE_BULLET_ENEMY;

			tempBullet.userData->dataValue = arrayIndex;
			tempBullet.body->SetUserData (tempBullet.userData);
		}

	switch (bulletType)
		{
			case BULLET_TYPE_NORMAL: // small double laser
				tempBullet.shape.m_radius = (float) (sprites.at ("bullet_001").frameHeight * 0.5f) / pixelsPerMeter;
			tempBullet.shape.m_p.Set (0, 0);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 50, ALLEGRO_PLAYMODE_ONCE, "laser");
			break;

			case BULLET_TYPE_SINGLE: // Large single laser
				tempBullet.boxShape.SetAsBox ((sprites.at ("bullet_476").frameWidth / 2) / pixelsPerMeter, (sprites.at ("bullet_476").frameHeight / 2) / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.boxShape;
			evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 50, ALLEGRO_PLAYMODE_ONCE, "laser");
			break;

			case BULLET_TYPE_DOUBLE: // Large double laser
				tempBullet.shape.m_radius = (float) (sprites.at ("bullet_821").frameHeight * 0.5f) / pixelsPerMeter;
			tempBullet.shape.m_p.Set (0, 0);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 50, ALLEGRO_PLAYMODE_ONCE, "laser");
			break;

			case BULLET_TYPE_DISRUPTER: // Disrupter
				tempBullet.disrupterFadeAmount = disrupterFadeAmount / (float) numDisrupterFrames;
			tempBullet.disrupterFade         = disrupterFadeAmount;
			evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 50, ALLEGRO_PLAYMODE_ONCE, "disrupter");
			break;

			default:
				log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Invalid bullet type used."));
			break;
		}

	if (bulletType != BULLET_TYPE_DISRUPTER)
		{

			if (bulletSourceIndex == -1)
				{
					tempBullet.fixtureDef.filter.categoryBits = PHYSIC_TYPE_BULLET_PLAYER;
					tempBullet.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_DOOR_CLOSED;
				}
			else
				{
					tempBullet.fixtureDef.filter.categoryBits = PHYSIC_TYPE_BULLET_ENEMY;
					tempBullet.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_DOOR_CLOSED;
				}

			tempBullet.fixtureDef.density     = bulletDensity;
			tempBullet.fixtureDef.friction    = 0.3f;
			tempBullet.fixtureDef.restitution = 0.0f;
			tempBullet.body->CreateFixture (&tempBullet.fixtureDef);

			if (dataBaseEntry[playerDroid.droidType].bulletType == BULLET_TYPE_SINGLE)
				tempBullet.body->SetTransform (tempBullet.body->GetPosition (), tempBullet.angle);

			par_addEmitter (tempBullet.worldPos, PARTICLE_TYPE_TRAIL, arrayIndex);
		}

	tempBullet.currentFrame     = 0;
	tempBullet.frameAnimCounter = 1.0f;
	tempBullet.sourceIndex      = bulletSourceIndex;
	tempBullet.inUse            = true;
	tempBullet.type             = bulletType;

	return tempBullet;
}

//---------------------------------------------------------------------------------------------------------------
//
// Create a new bullet - pass in the source of the bullet
void bul_createNewBullet (int bulletSourceIndex)
//---------------------------------------------------------------------------------------------------------------
{
	__bulletObject tempBullet;

	for (int i = 0; i != bullets.size (); i++)
		{
			if (!bullets.at (i).inUse)
				{
					bullets.at (i) = bul_setupNewBullet (bulletSourceIndex, i);
					return;
				}
		}
	bullets.push_back (bul_setupNewBullet (bulletSourceIndex, bullets.size () - 1));
}

//---------------------------------------------------------------------------------------------------------------------------
//
// Animate any current bullets
void bul_animateBullets (__bulletObject &bulletItr, double tickTime)
//---------------------------------------------------------------------------------------------------------------------------
{
	bulletItr.frameAnimCounter -= bulletAnimSpeed * (float) tickTime;
	if (bulletItr.frameAnimCounter < 0.0f)
		{
			bulletItr.frameAnimCounter = 1.0f;
			bulletItr.currentFrame++;
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
				{
					if (bulletItr.currentFrame == sprites.at ("bullet_001").numFrames)
						bulletItr.currentFrame = 0;
				}
			else
				{
					bulletItr.disrupterFade -= bulletItr.disrupterFadeAmount;
					if (bulletItr.disrupterFade < 0.0f)
						bulletItr.disrupterFade = 0.0f;

					if (bulletItr.currentFrame > numDisrupterFrames)
						{
							bulletItr.currentFrame  = 0;
							bulletItr.inUse         = false;
							bulletItr.disrupterFade = 0.0f;
						}
				}
		}
}

//---------------------------------------------------------------------------------------------------------------
//
// Apply physics to bullets
void bul_applyPhysics (double tickTime)
//---------------------------------------------------------------------------------------------------------------
{
	for (auto &bulletItr : bullets)
		{
//			if ((bulletItr.body != nullptr) && (bulletItr.inUse))
			if (bulletItr.inUse)
				{
					if (bulletItr.type != BULLET_TYPE_DISRUPTER)
						{
							bulletItr.velocity.Normalize ();
							bulletItr.velocity.operator*= (bulletMoveSpeed);
							bulletItr.body->ApplyLinearImpulseToCenter (bulletItr.velocity, true);

							bulletItr.worldPos = bulletItr.body->GetPosition ();
						}
					bul_animateBullets (bulletItr, tickTime);
				}
		}
}

//---------------------------------------------------------------------------------------------------------------
//
// Render a disrupter bullet effect
void bul_renderDisrupter (__bulletObject whichBullet)
//---------------------------------------------------------------------------------------------------------------
{
	al_draw_filled_rectangle (0, 0, sys_getLogicalWidth (), sys_getLogicalHeight (),
	                          al_map_rgba_f (whichBullet.disrupterFade, whichBullet.disrupterFade,
	                                         whichBullet.disrupterFade, whichBullet.disrupterFade));
}

//---------------------------------------------------------------------------------------------------------------
//
// Render the bullets
void bul_renderBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempPosition;

	for (auto bulletItr : bullets)
		{
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
				{
					if ((bulletItr.body != nullptr) && (bulletItr.inUse))
						{
							tempPosition = bulletItr.body->GetPosition ();      // Get position in meters
							tempPosition.x *= pixelsPerMeter;                   // Change to pixels
							tempPosition.y *= pixelsPerMeter;

							tempPosition = sys_worldToScreen (tempPosition, SPRITE_SIZE);

							io_renderRotatedSpriteFrame (playerDroid.bulletName, bulletItr.currentFrame, tempPosition.x, tempPosition.y, bulletItr.angle);
						}
				}
			else
				{   // Draw disrupter effect
					if (bulletItr.inUse)
						bul_renderDisrupter (bulletItr);
				}
		}
}

//---------------------------------------------------------------------------------------------------------------
//
// Return a string with the current bullet sprite key according to the sprite type
std::string bul_getBulletName (int droidType)
//---------------------------------------------------------------------------------------------------------------
{
	switch (dataBaseEntry[droidType].bulletType)
		{
			case BULLET_TYPE_NORMAL: // small double laser
				return "bullet_001";
			break;

			case BULLET_TYPE_SINGLE: // Large single laser
				return "bullet_476";
			break;

			case BULLET_TYPE_DOUBLE: // Large double laser
				return "bullet_821";
			break;

			case BULLET_TYPE_DISRUPTER: // Disrupter
				break;

			default:
				log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Invalid bullet type used in bul_getBulletName."));
			break;
		}
	return "Never get here";
}