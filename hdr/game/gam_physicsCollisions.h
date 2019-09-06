#pragma once

#include "system/sys_main.h"
#include "gam_doors.h"
#include <system/sys_eventsEngine.h>

class contactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void PreSolve(b2Contact *contact, const b2Manifold *manifold) override;

};

extern contactListener myContactListenerInstance;