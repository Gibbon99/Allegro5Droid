#include <hdr/game/gam_droids.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_game.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_physicActions.h>
#include <hdr/game/gam_bullet.h>
#include "hdr/game/gam_physicsCollisions.h"

contactListener myContactListenerInstance;

//----------------------------------------------------------------------------------------------------------------------
//
// Called when a contact is first made
void contactListener::BeginContact (b2Contact *contact)
//----------------------------------------------------------------------------------------------------------------------
{
	_userData *bodyUserData_A;
	_userData *bodyUserData_B;

	bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA ()->GetBody ()->GetUserData ());
	bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB ()->GetBody ()->GetUserData ());

	switch (bodyUserData_A->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_DROID, PHYSIC_DAMAGE_BUMP, -1, bodyUserData_B->dataValue, -1, {0, 0});
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_PLAYER, PHYSIC_DAMAGE_BUMP, -1, -1, bodyUserData_B->dataValue, {0, 0});
				return;
			}
			break;

		case PHYSIC_TYPE_ENEMY:
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_DROID, PHYSIC_DAMAGE_BUMP, -1, bodyUserData_A->dataValue, bodyUserData_B->dataValue, {0, 0});
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_DROID, PHYSIC_DAMAGE_BUMP, -1, bodyUserData_B->dataValue, bodyUserData_A->dataValue, {0, 0});
				return;
			}
			break;

		case PHYSIC_TYPE_DOOR:
			if ((bodyUserData_B->userType == PHYSIC_TYPE_PLAYER) || (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY))
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_DOOR, bodyUserData_A->dataValue, GAME_DOOR_STATE_ENTER, "");
				return;
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = true;
				playerDroid.liftIndex    = bodyUserData_A->dataValue;
				return;
			}
			break;

		case PHYSIC_TYPE_HEALING:
			if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_HEALING_START, -1, bodyUserData_A->dataValue, "");
				return;
			}
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_HEALING_START, bodyUserData_B->dataValue, bodyUserData_A->dataValue, "");
				return;
			}
			break;

		case PHYSIC_TYPE_BULLET:
			if (bodyUserData_B->userType == PHYSIC_TYPE_WALL)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_A->dataValue, {0, 0});
				return;
			}

			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_A->dataValue, {0, 0});
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_BULLET, PHYSIC_DAMAGE_BULLET, -1, bodyUserData_B->dataValue, bullets[bodyUserData_A->dataValue].sourceIndex, {0, 0});
				return;
			}

			if (bodyUserData_B->userType == PHYSIC_TYPE_DOOR_BULLET)
			{
				switch (doorBulletSensor[bodyUserData_B->dataValue].direction)
				{
					case DIRECTION_UP:
						if (doorBulletSensor[bodyUserData_B->dataValue].currentFrame != DOOR_UP_OPENED)
							gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_A->dataValue, {0, 0});
						break;

					case DIRECTION_ACROSS:
						if (doorBulletSensor[bodyUserData_B->dataValue].currentFrame != DOOR_ACROSS_OPENED)
							gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_A->dataValue, {0, 0});
						break;
				}
			}
			break;
	}

	switch (bodyUserData_B->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_DROID, PHYSIC_DAMAGE_BUMP, -1, -1, bodyUserData_B->dataValue, {0, 0});
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_PLAYER, PHYSIC_DAMAGE_BUMP, -1, bodyUserData_B->dataValue, -1, {0, 0});
			}
			break;

		case PHYSIC_TYPE_ENEMY:
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_DROID, PHYSIC_DAMAGE_BUMP, -1, bodyUserData_B->dataValue, bodyUserData_A->dataValue, {0, 0});
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_DROID, PHYSIC_DAMAGE_BUMP, -1, bodyUserData_A->dataValue, bodyUserData_B->dataValue, {0, 0});
			}
			break;

		case PHYSIC_TYPE_DOOR:
			if ((bodyUserData_A->userType == PHYSIC_TYPE_PLAYER) || (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY))
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_DOOR, bodyUserData_B->dataValue, GAME_DOOR_STATE_ENTER, "");
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = true;
				playerDroid.liftIndex    = bodyUserData_B->dataValue;
				return;
			}
			break;

		case PHYSIC_TYPE_HEALING:
			if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_HEALING_START, -1, bodyUserData_B->dataValue, "");
				return;
			}
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_HEALING_START, bodyUserData_A->dataValue, bodyUserData_B->dataValue, "");
				return;
			}
			break;

		case PHYSIC_TYPE_BULLET:
			if (bodyUserData_A->userType == PHYSIC_TYPE_WALL)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_B->dataValue, {0, 0});
				return;
			}

			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_B->dataValue, {0, 0});
				gam_addPhysicAction (PHYSIC_EVENT_TYPE_BULLET, PHYSIC_DAMAGE_BULLET, -1, bodyUserData_A->dataValue, bullets[bodyUserData_A->dataValue].sourceIndex, {0, 0});
				return;
			}

			if (bodyUserData_A->userType == PHYSIC_TYPE_DOOR_BULLET)
			{
				switch (doorBulletSensor[bodyUserData_A->dataValue].direction)
				{
					case DIRECTION_UP:
						if (doorBulletSensor[bodyUserData_A->dataValue].currentFrame != DOOR_UP_OPENED)
							gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_B->dataValue, {0, 0});
						break;

					case DIRECTION_ACROSS:
						if (doorBulletSensor[bodyUserData_A->dataValue].currentFrame != DOOR_ACROSS_OPENED)
							gam_addPhysicAction (PHYSIC_EVENT_TYPE_REMOVE_BULLET, 0, 0, 0, bodyUserData_B->dataValue, {0, 0});
						break;
				}
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called when a contact is broken - no contact anymore
void contactListener::EndContact (b2Contact *contact)
//----------------------------------------------------------------------------------------------------------------------
{
	_userData *bodyUserData_A;
	_userData *bodyUserData_B;

	bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA ()->GetBody ()->GetUserData ());
	bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB ()->GetBody ()->GetUserData ());

	switch (bodyUserData_A->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				printf ("Player collided with droid [ %i ]\n", bodyUserData_B->dataValue);
			}
			break;

		case PHYSIC_TYPE_DOOR:
			if ((bodyUserData_B->userType == PHYSIC_TYPE_PLAYER) || (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY))
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_DOOR, bodyUserData_A->dataValue, GAME_DOOR_STATE_EXIT, "");
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = false;
				playerDroid.liftIndex    = -1;
				return;
			}
			break;

		case PHYSIC_TYPE_HEALING:
			evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_HEALING_STOP, bodyUserData_A->dataValue, 0, "");
			return;
			break;
	}

	switch (bodyUserData_B->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				printf ("Player collided with droid [ %i ]\n", bodyUserData_A->dataValue);
			}
			break;

		case PHYSIC_TYPE_DOOR:
			if ((bodyUserData_A->userType == PHYSIC_TYPE_PLAYER) || (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY))
			{
				evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_DOOR, bodyUserData_B->dataValue, GAME_DOOR_STATE_EXIT, "");
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = false;
				playerDroid.liftIndex    = -1;
				return;
			}
			break;

		case PHYSIC_TYPE_HEALING:
			evt_pushEvent (0, PARA_EVENT_GAME, GAME_EVENT_HEALING_STOP, bodyUserData_B->dataValue, 0, "");
			return;
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called before the collision resolution is run
void contactListener::PreSolve (b2Contact *contact, const b2Manifold *manifold)
//----------------------------------------------------------------------------------------------------------------------
{
	_userData *bodyUserData_A;
	_userData *bodyUserData_B;

	bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA ()->GetBody ()->GetUserData ());
	bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB ()->GetBody ()->GetUserData ());

	switch (bodyUserData_A->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_B->userType == PHYSIC_TYPE_WALL)
			{
				contact->SetEnabled (false);
			}

			if (bodyUserData_B->userType == PHYSIC_TYPE_BULLET)     // Let bullet pass through player when it starts - need a type for ENEMY_BULLET
			{
				//if (bodyUserData_B->dataValue == -1)
				contact->SetEnabled (false);
			}
			break;
	}

	switch (bodyUserData_B->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_A->userType == PHYSIC_TYPE_WALL)
			{
				contact->SetEnabled (false);
			}

			if (bodyUserData_A->userType == PHYSIC_TYPE_BULLET)
			{
				//if (bodyUserData_A->dataValue == -1)
				contact->SetEnabled (false);
			}
			break;
	}
}