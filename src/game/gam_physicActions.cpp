#include <queue>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/game/gam_game.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_bullet.h>
#include "hdr/game/gam_physicActions.h"

/*
* Remove a Droid ( by index number ) - mark it as not alive so it's not recreated on level change ( physics )
*
* Have a queue that is processed after physics step to process damage to droids - and remove if required
* Also have bullets and particles in that queue as well for removal
*
* eventType
* - Droid
* - Bullet
* - Particle
*
* Damage from
* - Other droid bullet
* - Player bullet
* - Explosion
* - Bump Damage from other Droid
* - Bump damage from player
*
* Bullet
* - Remove bullet
*
* Particle
* - Remove particle
* - Remove emitter (?)
*/

std::queue<__physicAction> physicActionQueue;

//-------------------------------------------------------------------------------------------------------------------------
//
// Clear the queue between game restart in case there are unprocessed events from previous run
void gam_initPhysicsActionQueue()
//-------------------------------------------------------------------------------------------------------------------------
{
	if (!physicActionQueue.empty())
	{
		for (int i = 0; i != physicActionQueue.size(); i++)
		{
			physicActionQueue.pop();
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------------
//
// Add an event to the queue to be processed out of Physic world step - this function run from main thread
void gam_addPhysicAction(int eventType, int damageType, int eventAction, int targetIndex, int sourceIndex, b2Vec2 worldLocation)
//-------------------------------------------------------------------------------------------------------------------------
{
	__physicAction newAction;

	newAction.eventType = eventType;
	newAction.damageType = damageType;
	newAction.eventAction = eventAction;
	newAction.targetIndex = targetIndex;
	newAction.sourceIndex = sourceIndex;
	newAction.worldLocation = worldLocation;

	physicActionQueue.push(newAction);
}

//-------------------------------------------------------------------------------------------------------------------------
//
// Process the physic action queue
void gam_processPhysicActions()
//-------------------------------------------------------------------------------------------------------------------------
{
	__physicAction  physicEvent;

	if (physicActionQueue.empty())      // No events to action
		return;

	while (!physicActionQueue.empty())
	{
		physicEvent = physicActionQueue.front ();
		physicActionQueue.pop ();

		switch (physicEvent.eventType)
		{
			case PHYSIC_EVENT_TYPE_DROID:
				gam_damageToDroid (physicEvent.targetIndex, DAMAGE_COLLISION, physicEvent.sourceIndex);
				break;

			case PHYSIC_EVENT_TYPE_BULLET:
				gam_damageToDroid (physicEvent.targetIndex, DAMAGE_BULLET, physicEvent.sourceIndex);
				break;

			case PHYSIC_EVENT_TYPE_NEW_BULLET:                  // Needs to be created outside of PhysicWorld Step
				bul_createNewBullet(physicEvent.sourceIndex);     // Pass source of bullet -1 is player
				break;

			case PHYSIC_EVENT_TYPE_REMOVE_BULLET:
				bul_removeBullet(physicEvent.sourceIndex);
				break;

			case PHYSIC_EVENT_TYPE_PARTICLE:

				break;
		}
	}
}
