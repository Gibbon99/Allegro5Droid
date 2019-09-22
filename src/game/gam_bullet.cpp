#include <hdr/game/gam_player.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/io/io_resources.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_game.h>
#include <hdr/io/io_logFile.h>
#include "hdr/game/gam_bullet.h"

#define DEGTORAD 0.0174532925199432957f
#define MY_PI 3.1415926535f

std::vector<__bulletObject> bullets;
float                       bulletAnimSpeed;          // From script
float                       bulletMoveSpeed;          // From script
float                       bulletDensity;            // From script

//-----------------------------------------------------------------------------
//
// Get the angle of rotation for the bullet
double bul_getBulletAngle (b2Vec2 sourcePos, b2Vec2 destPos)
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
}

//---------------------------------------------------------------------------------------------------------------
//
// Create a new bullet - pass in the source of the bullet
void bul_createNewBullet (int bulletSourceIndex)
//---------------------------------------------------------------------------------------------------------------
{
	__bulletObject tempBullet;

	if (bulletSourceIndex == -1)        // Player bullet
		{
			tempBullet.worldPos = playerDroid.worldPos;
			tempBullet.velocity = playerDroid.velocity;

			tempBullet.velocity.operator*= (2.0f);
			tempBullet.destPos = tempBullet.worldPos + tempBullet.velocity;
		}
	else
		{
			tempBullet.worldPos = shipLevel.at (lvl_getCurrentLevelName ()).droid[bulletSourceIndex].worldPos;
			tempBullet.destPos  = playerDroid.worldPos;
		}

	tempBullet.angle = bul_getBulletAngle (tempBullet.worldPos, tempBullet.destPos);

	tempBullet.bodyDef.type = b2_dynamicBody;
	tempBullet.bodyDef.position.Set (tempBullet.worldPos.x / pixelsPerMeter, tempBullet.worldPos.y / pixelsPerMeter);
	tempBullet.bodyDef.angle  = 0;
	tempBullet.bodyDef.bullet = true;
	tempBullet.body           = sys_getPhysicsWorld ()->CreateBody (&tempBullet.bodyDef);
	if (tempBullet.body == nullptr)
		return;

	tempBullet.userData            = new _userData;
	tempBullet.userData->userType  = PHYSIC_TYPE_BULLET;
	tempBullet.userData->dataValue = bullets.size ();
	tempBullet.body->SetUserData (tempBullet.userData);

	switch (dataBaseEntry[playerDroid.droidType].bulletType)
		{
			case BULLET_TYPE_NORMAL: // small double laser
				tempBullet.shape.m_radius = (float) (sprites.at ("bullet_001").frameHeight * 0.5f) / pixelsPerMeter;
			tempBullet.shape.m_p.Set (0, 0);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			break;

			case BULLET_TYPE_SINGLE: // Large single laser
				tempBullet.boxShape.SetAsBox ((sprites.at ("bullet_476").frameWidth / 2) / pixelsPerMeter, (sprites.at ("bullet_476").frameHeight / 2) / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.boxShape;
			break;

			case BULLET_TYPE_DOUBLE: // Large double laser
				tempBullet.shape.m_radius = (float) (sprites.at ("bullet_821").frameHeight * 0.5f) / pixelsPerMeter;
			tempBullet.shape.m_p.Set (0, 0);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			break;

			case BULLET_TYPE_DISRUPTER: // Disrupter
				break;

			default:
				log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Invalid bullet type used."));
			break;
		}

	tempBullet.fixtureDef.density     = bulletDensity;
	tempBullet.fixtureDef.friction    = 0.3f;
	tempBullet.fixtureDef.restitution = 0.0f;
	tempBullet.body->CreateFixture (&tempBullet.fixtureDef);

	if (dataBaseEntry[playerDroid.droidType].bulletType == BULLET_TYPE_SINGLE)
		tempBullet.body->SetTransform (tempBullet.body->GetPosition (), tempBullet.angle);

	tempBullet.currentFrame     = 0;
	tempBullet.frameAnimCounter = 1.0f;
	tempBullet.sourceIndex      = bulletSourceIndex;
	bullets.push_back (tempBullet);

	printf ("New bullet [ %f ] Source [ %i ] Start [ %f %f ]\n", tempBullet.angle, bulletSourceIndex, tempBullet.destPos.x, tempBullet.destPos.y);
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
			if (bulletItr.currentFrame == sprites.at ("bullet_001").numFrames)
				bulletItr.currentFrame = 0;
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
			if (bulletItr.body != nullptr)
				{
					bulletItr.velocity.Normalize ();
					bulletItr.velocity.operator*= (bulletMoveSpeed);
					bulletItr.body->ApplyLinearImpulseToCenter (bulletItr.velocity, true);

					bul_animateBullets (bulletItr, tickTime);
				}
		}
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
			if (bulletItr.body != nullptr)
				{
					tempPosition = bulletItr.body->GetPosition ();      // Get position in meters
					tempPosition.x *= pixelsPerMeter;                   // Change to pixels
					tempPosition.y *= pixelsPerMeter;

					tempPosition = sys_worldToScreen (tempPosition, SPRITE_SIZE);

					io_renderRotatedSpriteFrame (playerDroid.bulletName, bulletItr.currentFrame, tempPosition.x, tempPosition.y, bulletItr.angle);
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