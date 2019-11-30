#include <hdr/game/gam_physicActions.h>
#include <hdr/game/gam_particles.h>
#include <hdr/game/gam_hud.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_render.h>
#include "hdr/game/gam_game.h"

int         currentAlertLevel;
int         currentScore;
int         displayScore;
int         yellowAlertLevel;
int         redAlertLevel;
float       scoreDelay;
std::string currentAlertLevelSoundName;
char        displayScoreString[8];

//-------------------------------------------------------------------------------------------------------------------------
//
// Return the current score as a string for displaying on the HUD
std::string gam_getScore()
//-------------------------------------------------------------------------------------------------------------------------
{
	return displayScoreString;
}

//-------------------------------------------------------------------------------------------------------------------------
//
// Power down a level - all droids are dead
void gam_powerDownLevel(std::string whichLevel)
//-------------------------------------------------------------------------------------------------------------------------
{
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, 70, ALLEGRO_PLAYMODE_ONCE, "levelPowerDown");
	gam_drawAllTiles ();
	//
	// check to see if all levels are dead
	for (auto &levelItr : shipLevel )
		{
			if (levelItr.second.numEnemiesAlive > 0)
				return;
		}
		//
		// All Levels are dead
		// End game
		printf("END GAME\n");
}

//-------------------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame ()
//-------------------------------------------------------------------------------------------------------------------------
{
	gam_initPhysicsActionQueue ();
	currentAlertLevel = ALERT_GREEN_TILE;
	currentAlertLevelSoundName = "greenAlert";
	par_resetAllEmitters ();
	hud_setText (false, "hudGameOn");
	for (auto &shipItr : shipLevel)
		{
			shipItr.second.numEnemiesAlive = shipItr.second.numDroids;
		}
		gam_drawAllTiles();
}

//-------------------------------------------------------------------------------------------------------------------------
//
// Process score and set current Alert level
void gam_processScore (float tickTime)
//-------------------------------------------------------------------------------------------------------------------------
{
	static float scoreDelayCount = 0.0f;

	switch (currentAlertLevel)
		{
			case ALERT_GREEN_TILE:
				scoreDelayCount -= scoreDelay * tickTime;
			break;

			case ALERT_YELLOW_TILE:
				scoreDelayCount -= (scoreDelay * 2) * tickTime;
			break;

			case ALERT_RED_TILE:
				scoreDelayCount -= (scoreDelay * 3) * tickTime;
			break;
		}

	if (scoreDelayCount < 0)
		{
			scoreDelayCount = 1.0f;

			if (currentScore > 0)
				{
					currentScore--;
					displayScore++;
				}
		}

	sprintf (displayScoreString, "%7i", displayScore);

	currentAlertLevelSoundName = "greenAlert";

	if ((currentScore - displayScore) > yellowAlertLevel)
		currentAlertLevelSoundName = "yellowAlert";

	if ((currentScore - displayScore) > redAlertLevel)
		currentAlertLevelSoundName = "redAlert";
}

//---------------------------------------------------------
//
// Add passed in value to currentScore
void gam_addToScore (int scoreValue)
//---------------------------------------------------------
{
	currentScore += scoreValue;

	printf("Adding [ %i ] to score\n", scoreValue);
}

//-----------------------------------------------------------------------------
//
// Reset the score values
void gam_resetScore ()
//-----------------------------------------------------------------------------
{
	//
	// Reset score
	//
	currentScore = 0;
	displayScore = 101010;
}
