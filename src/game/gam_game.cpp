#include <hdr/game/gam_physicActions.h>
#include "hdr/game/gam_game.h"

int currentAlertLevel;

//-------------------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame()
//-------------------------------------------------------------------------------------------------------------------------
{
	gam_initPhysicsActionQueue();
	currentAlertLevel = ALERT_GREEN_TILE;
}