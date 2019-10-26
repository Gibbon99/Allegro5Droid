#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_doors.h>
#include "hdr/game/gam_lineOfSight.h"

float visibleFadeValue = 3.0f;

class RayCastAnyCallback : public b2RayCastCallback {
	public:
	RayCastAnyCallback ()
	{
		m_hit = false;
	}

	float32 ReportFixture (b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32) override
	{

		b2Body *body     = fixture->GetBody ();
		auto   *userData = static_cast<_userData *>(body->GetUserData ());

		switch (userData->userType)
			{
				case PHYSIC_TYPE_WALL:
					m_hit = true;
				return 0.0f;
				break;

				case PHYSIC_TYPE_DOOR_BULLET:
					{
						//
						// Look through open doors, ignoring the bullet sensor
						if ((DOOR_ACROSS_OPENED == doorBulletSensor[userData->dataValue].currentFrame) ||
						    (DOOR_UP_OPENED == doorBulletSensor[userData->dataValue].currentFrame))
							{
								m_hit = false;
								return 1.0f;
							}
						else
							{
								m_hit = true;
								// At this point we have a hit, so we know the ray is obstructed.
								// By returning 0, we instruct the calling code to terminate the ray-cast.
								return 0.0f;
							}
					}
				break;
			}
		m_hit            = false;
		return 1.0f;
	}

	bool   m_hit;
	b2Vec2 m_point;
};

//----------------------------------------------------------------------------------------------------------------------
//
// Test each droid against the player droid to see if the ray intersects a wall or door
void gam_doLineOfSight ()
//----------------------------------------------------------------------------------------------------------------------
{
	std::string        cacheLevelName;
	RayCastAnyCallback callback;

	b2Vec2 sourcePosition;
	b2Vec2 destPosition;

	cacheLevelName = lvl_getCurrentLevelName ();

	sourcePosition = playerDroid.worldPos;
	sourcePosition.x /= pixelsPerMeter;
	sourcePosition.y /= pixelsPerMeter;

	for (int i = 0; i != shipLevel.at (cacheLevelName).droid.size (); ++i)
		{
			destPosition.x = shipLevel.at (cacheLevelName).droid[i].worldPos.x / pixelsPerMeter;
			destPosition.y = shipLevel.at (cacheLevelName).droid[i].worldPos.y / pixelsPerMeter;

			sys_getPhysicsWorld ()->RayCast (&callback, sourcePosition, destPosition);

			if (callback.m_hit)
				{
					shipLevel.at (cacheLevelName).droid[i].visibleToPlayer = false;
					shipLevel.at (cacheLevelName).droid[i].visibleStatus   = VISIBLE_STATE_GO_NOT_VISIBLE;
				}
			else
				{
					shipLevel.at (cacheLevelName).droid[i].visibleToPlayer = true;
					shipLevel.at (cacheLevelName).droid[i].visibleStatus   = VISIBLE_STATE_GO_VISIBLE;
				}

			if (shipLevel.at (cacheLevelName).droid[i].visibleStatus == VISIBLE_STATE_GO_VISIBLE)
				{
					if (shipLevel.at (cacheLevelName).droid[i].visibleValue < 1.0f)
						{
							shipLevel.at (cacheLevelName).droid[i].visibleValue += visibleFadeValue * sys_getTickTime ();
							if (shipLevel.at (cacheLevelName).droid[i].visibleValue > 1.0f)
								shipLevel.at (cacheLevelName).droid[i].visibleValue = 1.0f;
						}
				}

			if (shipLevel.at (cacheLevelName).droid[i].visibleStatus == VISIBLE_STATE_GO_NOT_VISIBLE)
				{
					shipLevel.at (cacheLevelName).droid[i].visibleValue -= visibleFadeValue * sys_getTickTime ();
					if (shipLevel.at (cacheLevelName).droid[i].visibleValue < 0.0f)
						shipLevel.at (cacheLevelName).droid[i].visibleValue = 0.0f;
				}
		}
}
