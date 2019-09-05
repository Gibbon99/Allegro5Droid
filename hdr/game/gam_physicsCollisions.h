#pragma once

#include "system/sys_main.h"
#include "gam_doors.h"

class MyContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) override
	{
		_userData* bodyUserData_A;
		_userData* bodyUserData_B;

		bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA()->GetBody()->GetUserData());
		bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB()->GetBody()->GetUserData());

		switch (bodyUserData_A->userType)
		{
			case PHYSIC_TYPE_PLAYER:
				if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
					printf("Player collided with droid [ %i ]\n", bodyUserData_B->dataValue);
				break;

			case PHYSIC_TYPE_DOOR:
				if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
					gam_handleDoorTrigger(bodyUserData_A->dataValue, 0);
				break;
		}

		switch (bodyUserData_B->userType)
		{
			case PHYSIC_TYPE_DOOR:
				if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
					gam_handleDoorTrigger(bodyUserData_B->dataValue, 0);
				break;
		}
	}

	void EndContact(b2Contact* contact) override
	{
		_userData* bodyUserData_A;
		_userData* bodyUserData_B;

		bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA()->GetBody()->GetUserData());
		bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB()->GetBody()->GetUserData());

		switch (bodyUserData_A->userType)
		{
			case PHYSIC_TYPE_PLAYER:
				if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
					printf("Player collided with droid [ %i ]\n", bodyUserData_B->dataValue);
				break;

			case PHYSIC_TYPE_DOOR:
				if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
					gam_handleDoorTrigger(bodyUserData_A->dataValue, 1);
				break;
		}

		switch (bodyUserData_B->userType)
		{
			case PHYSIC_TYPE_DOOR:
				if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
					gam_handleDoorTrigger(bodyUserData_B->dataValue, 1);
				break;
		}
	}

	void PreSolve(b2Contact *contact, const b2Manifold *manifold) override
	{
		_userData* bodyUserData_A;
		_userData* bodyUserData_B;

		bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA()->GetBody()->GetUserData());
		bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB()->GetBody()->GetUserData());

		switch (bodyUserData_A->userType)
		{
			case PHYSIC_TYPE_PLAYER:
				if (bodyUserData_B->userType == PHYSIC_TYPE_WALL)
					contact->SetEnabled(false);
				break;
		}

		switch (bodyUserData_B->userType)
		{
			case PHYSIC_TYPE_PLAYER:
				if (bodyUserData_A->userType == PHYSIC_TYPE_WALL)
					contact->SetEnabled(false);
		}
	}
};

extern MyContactListener   myContactListenerInstance;