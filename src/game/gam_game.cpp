#include <hdr/game/gam_physicActions.h>
#include <hdr/game/gam_particles.h>
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
// Start a new game
void gam_startNewGame ()
//-------------------------------------------------------------------------------------------------------------------------
{
	gam_initPhysicsActionQueue ();
	currentAlertLevel = ALERT_GREEN_TILE;
	par_resetAllEmitters ();
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
	displayScore = 0;
}
