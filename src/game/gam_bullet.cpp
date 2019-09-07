#include <hdr/game/gam_player.h>
#include <hdr/io/io_resourceSprite.h>
#include "hdr/game/gam_bullet.h"

std::vector<__bulletObject> bullets;

//-----------------------------------------------------------------------------
//
// Get the angle of rotation for the bullet
double bul_getBulletAngle (b2Vec2 sourcePos, b2Vec2 destPos)
//-----------------------------------------------------------------------------
{
	b2Vec2 a, b;

	double angle = (atan2 (destPos.y - sourcePos.y, destPos.x - sourcePos.x) * (180 / 3.1415f));

	if (angle < 0)
		angle = 360 - (-angle);

	return angle * 3.14135 / 180;       // Convert from degrees to radians
}

//---------------------------------------------------------------------------------------------------------------
//
// Init bullets for level change
void bul_initBullets()
//---------------------------------------------------------------------------------------------------------------
{
	for (auto bulletItr : bullets)
	{
		if (bulletItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (bulletItr.body);
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Remove a bullet
void bul_removeBullet(int whichBullet)
//---------------------------------------------------------------------------------------------------------------
{
	if (bullets.at(whichBullet).body != nullptr)
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
	b2Vec2         destVector;

	if (bulletSourceIndex == -1)        // Player bullet
	{
		tempBullet.worldPos = playerDroid.worldPos;
		tempBullet.velocity = playerDroid.velocity;

		tempBullet.velocity.operator*= (2.0f);
		tempBullet.destPos = tempBullet.worldPos + tempBullet.velocity;

		tempBullet.angle = bul_getBulletAngle (tempBullet.worldPos, tempBullet.destPos);


		tempBullet.bodyDef.type = b2_dynamicBody;
		tempBullet.bodyDef.position.Set (tempBullet.worldPos.x / pixelsPerMeter, tempBullet.worldPos.y / pixelsPerMeter);
		tempBullet.bodyDef.angle = 0;
		tempBullet.bodyDef.bullet = true;
		tempBullet.body          = sys_getPhysicsWorld ()->CreateBody (&tempBullet.bodyDef);
		if (tempBullet.body == nullptr)
			return;

		tempBullet.userData            = new _userData;
		tempBullet.userData->userType  = PHYSIC_TYPE_BULLET;
		tempBullet.userData->dataValue = bullets.size ();
		tempBullet.body->SetUserData (tempBullet.userData);

		tempBullet.shape.m_radius = (float) (SPRITE_SIZE * 0.5f) / pixelsPerMeter;
		tempBullet.shape.m_p.Set (0, 0);

		tempBullet.fixtureDef.shape       = &tempBullet.shape;
		tempBullet.fixtureDef.density     = 0.2f;
		tempBullet.fixtureDef.friction    = 0.3f;
		tempBullet.fixtureDef.restitution = 1.0f;
		tempBullet.body->CreateFixture (&tempBullet.fixtureDef);

		bullets.push_back (tempBullet);

		printf ("New bullet from player [ %f ] Start [ %f %f ]\n", tempBullet.angle, tempBullet.destPos.x, tempBullet.destPos.y);
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Apply physics to bullets
void bul_applyPhysics()
//---------------------------------------------------------------------------------------------------------------
{
	for (auto bulletItr : bullets)
	{
		if (bulletItr.body != nullptr)
		{
			bulletItr.velocity.Normalize ();
			bulletItr.velocity.operator*= (2.0f);
			bulletItr.body->ApplyLinearImpulseToCenter (bulletItr.velocity, true);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Render the bullets
void bul_renderBullets()
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

			io_renderRotatedSpriteFrame ("bullet_821", 1, tempPosition.x, tempPosition.y, bulletItr.angle);
		}
	}
}