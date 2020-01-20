float buttonHeight = 7.0f;
float hudPadding   = 10.0f;

int ALLEGRO_PLAYMODE_ONCE = 0x100;
int ALLEGRO_PLAYMODE_LOOP = 0x101;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle the actions for the transfer game
void as_guiHandleTransferAction (string &in objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "guiTransferOne"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTransferOneNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "guiTransferTwo"));
					as_guiSetObjectFocus ("buttonTransferTwoNext");
					sys_changeCurrentMode (MODE_GUI_TRANSFER_SCREEN_TWO_LOAD, true);
					return;
				}
		}

	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "guiTransferTwo"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTransferTwoNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "guiTransferChooseSide"));
					sys_changeCurrentMode (MODE_GUI_TRANSFER_INIT_GAME, true);
					hud_setText (false, "hudChooseSide");
					return;
				}
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle the actions for the database screen
void as_guiHandleDatabaseAction (string &in objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "databaseScreen"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "databasePrevButton"))
				{
					gam_previousDatabaseDroid ();   // Go to previous droid
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "databaseCancelButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainTerminalScreen"));
					as_guiSetObjectFocus ("terminalDatabaseButton");
					sys_changeCurrentMode (MODE_GUI_TERMINAL, true);
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "databaseNextButton"))
				{
					gam_nextDatabaseDroid ();   // Go to next droid
					return;
				}
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle the actions for the terminal
void as_guiHandleTerminalAction (string &in objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Main terminal screen
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "mainTerminalScreen"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "terminalLogoffButton"))
				{
					sys_changeCurrentMode (MODE_GAME, true);
					hud_setText (false, "hudMoving");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "terminalDatabaseButton"))
				{
					gam_setLocalDroidType ();
					gam_enterDatabaseMode ();
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "databaseScreen"));
					as_guiSetObjectFocus ("databaseCancelButton");
					sys_changeCurrentMode (MODE_GUI_DATABASE, true);
					hud_setText (false, "hudDatabase");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "terminalDeckviewButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "terminalDeckviewScreen"));
					as_guiSetObjectFocus ("deckviewCancelButton");
					sys_changeCurrentMode (MODE_GUI_TERMINAL_DECKVIEW, true);
					hud_setText (false, "hudDeckView");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "terminalShipviewButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "terminalShipviewScreen"));
					as_guiSetObjectFocus ("shipviewCancelButton");
					sys_changeCurrentMode (MODE_GUI_TERMINAL_SHIPVIEW, true);
					hud_setText (false, "hudShipView");
					return;
				}
		}
	//
	// For each display screen - shipview and deckview
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "terminalShipviewScreen"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "shipviewCancelButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainTerminalScreen"));
					as_guiSetObjectFocus ("terminalShipviewButton");
					sys_changeCurrentMode (MODE_GUI_TERMINAL, true);
					hud_setText (false, "hudTerminal");
					return;
				}
		}

	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "terminalDeckviewScreen"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "deckviewCancelButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainTerminalScreen"));
					as_guiSetObjectFocus ("terminalDeckviewButton");
					sys_changeCurrentMode (MODE_GUI_TERMINAL, true);
					hud_setText (false, "hudTerminal");
					return;
				}
		}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle doing an action for an element
void as_guiHandleElementAction (string &in objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Main Screen
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "startGameButton"))
				{
					//gam_changeToLevel("Bridge");
					sys_changeCurrentMode (MODE_PRE_GAME, true);
					hud_setText (false, "hudMoving");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "briefingButton"))
				{
					sys_changeCurrentMode (MODE_GUI_INTRO, true);
					hud_setText (false, "hudBriefing");
					evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_PLAY_AUDIO, volumeLevel, ALLEGRO_PLAYMODE_LOOP, "introSound");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "optionsButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "guiOptionsMain"));
					sys_changeCurrentMode (MODE_GUI_OPTIONS, true);
					as_guiSetObjectFocus ("guiOptionsMainCancelButton");
					hud_setText (false, "hudOptions");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "tutorialButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutorial"));
					sys_changeCurrentMode (MODE_GUI_TUT_MOVE, true);
					as_guiSetObjectFocus ("buttonTutMoveNext");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "exitGameButton"))
				{
					quitProgram = 1;
					return;
				}
			return;
		}
	//
	// Main Option Screen
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "guiOptionsMain"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "optionsVideo"));
					sys_changeCurrentMode (MODE_GUI_OPTIONS_VIDEO, true);
					as_guiSetObjectFocus ("optionsVideoCheckFullScreen");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "optionsAudio"));
					sys_changeCurrentMode (MODE_GUI_OPTIONS_AUDIO, true);
					as_guiSetObjectFocus ("optionsAudioUseSound");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton"))
			{
				as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "optionsGraphics"));
				sys_changeCurrentMode (MODE_GUI_OPTIONS_GRAPHICS, true);
				as_guiSetObjectFocus ("optionsGraphicsCheckBackdrop");
				return;
			}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}
		}
	//
	// Audio Options screen
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "optionsAudio"))
		{
			//
			// Back button - back one screen
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "optionsAudioBackButton"))
				{
				//
				// Save values to config file
					cfg_setConfigValue ("enableSound", sys_boolToString (enableSound));
					cfg_setConfigValue ("volumeLevel", as_guiGetSliderValue("optionsAudioVolume"));
					volumeLevel = sys_stringToInt(as_guiGetSliderValue("optionsAudioVolume"));

					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "guiOptionsMain"));
					sys_changeCurrentMode (MODE_GUI_OPTIONS, true);
					as_guiSetObjectFocus ("guiOptionsMainCancelButton");
					return;
				}
			//
			// Use Audio checkbox
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound"))
				{
					enableSound = !enableSound;
					as_guiSetCheckboxTick ("optionsAudioUseSound", -1, enableSound);
					return;
				}
		}
	//
	// Graphics Options screen
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "optionsGraphics"))
	{
		//
		// Back Button - back one screen
		// TODO : Save any changes
		if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton"))
		{
			string tileTypeName;
			string tileColorName;
			string tileFileName;

			tileTypeName = as_guiGetSliderValue("optionsGraphicsTileType");
			tileColorName = as_guiGetSliderValue("optionsGraphicsTileColor");
			tileFileName = tileTypeName + "_" + tileColorName + ".bmp";
			sys_loadResource ("alltiles", tileFileName, RESOURCE_BITMAP, 0, 0);

			tileFileName = tileTypeName + "_" + "dark.bmp";
			sys_loadResource ("alltiles_dark", tileFileName, RESOURCE_BITMAP, 0, 0);

			cfg_setConfigValue ("tileType", tileTypeName);
			cfg_setConfigValue ("tileColor", tileColorName);

			as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "guiOptionsMain"));
			sys_changeCurrentMode (MODE_GUI_OPTIONS, true);
			as_guiSetObjectFocus ("guiOptionsMainCancelButton");
			return;
		}
		//
		// Use backdrop checkbox
		if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop"))
		{
			renderBackdrop = !renderBackdrop;
			as_guiSetCheckboxTick ("optionsGraphicsCheckBackdrop", -1, renderBackdrop);
			cfg_setConfigValue ("renderBackdrop", sys_boolToString (renderBackdrop));
			return;
		}
	}
	//
	// Video Options screen
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "optionsVideo"))
		{
			//
			// Cancel button - back one screen
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "optionsVideoCancelButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "guiOptionsMain"));
					sys_changeCurrentMode (MODE_GUI_OPTIONS, true);
					as_guiSetObjectFocus ("guiOptionsMainCancelButton");
					return;
				}
			//
			// Fullscreen checkbox
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen"))
				{
					fullScreen = !fullScreen;
					as_guiSetCheckboxTick ("optionsVideoCheckFullScreen", 1, true);
					cfg_setConfigValue ("screenType", sys_intToString (screenType));
					return;
				}
			//
			// Windowed checkbox
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed"))
				{
					fullScreen = !fullScreen;
					as_guiSetCheckboxTick ("optionsVideoCheckWindowed", 1, true);
					cfg_setConfigValue ("screenType", sys_intToString (screenType));
					return;
				}
			//
			// Windowed fullscreen checkbox
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed"))
				{
					fullScreen = !fullScreen;
					as_guiSetCheckboxTick ("optionsVideoCheckFullScreenWindowed", 1, true);
					cfg_setConfigValue ("screenType", sys_intToString (screenType));
					return;
				}
		}

	//
	// Start of the tutorial screens - how to move
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutorial"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutorialCancel"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutMoveNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTransfer"));
					sys_changeCurrentMode (MODE_GUI_TUT_TRANSFER_START, true);
					as_guiSetObjectFocus ("buttonTutTransNext");
					return;
				}
		}

	//
	// Tutorial on doing a transfer
	if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTransfer"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutTransCancel"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutTransNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTransGame"));
					sys_changeCurrentMode (MODE_GUI_TUT_TRANSFER_GAME, true);
					as_guiSetObjectFocus ("buttonTutTransGameNext");
					return;
				}
		}

		//
		// Tutorial on transfer game
	else if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTransGame"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutTransGameNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutLifts"));
					sys_changeCurrentMode (MODE_GUI_TUT_LIFTS, true);
					as_guiSetObjectFocus ("buttonTutLiftNext");
					return;
				}
		}

		//
		// How to use the lifts
	else if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutLifts"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutLiftCancel"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutLiftNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTerminals"));
					sys_changeCurrentMode (MODE_GUI_TUT_TERMINALS, true);
					as_guiSetObjectFocus ("buttonTutTerminalNext");
					return;
				}
		}

		//
		// How to use the terminals
	else if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTerminals"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutTerminalNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutHealing"));
					sys_changeCurrentMode (MODE_GUI_TUT_HEALING, true);
					as_guiSetObjectFocus ("buttonTutHealinglNext");
					return;
				}
		}

		//
		// How to use the healing tiles
	else if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutHealing"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutHealingCancel"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutHealinglNext"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTips"));
					sys_changeCurrentMode (MODE_GUI_TUT_TIPS, true);
					as_guiSetObjectFocus ("buttonTutTipsCancel");
					return;
				}
		}

		//
		// Gameplay tips
	else if (currentGUIScreen == as_guiFindIndex (GUI_OBJECT_SCREEN, "scrTutTips"))
		{
			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "buttonTutTipsCancel"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
					return;
				}
		}

}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the values for the database scrollbox
void script_setupDatabaseScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY                 = 94;
	int   databaseScrollBoxBorderWidth = 10;

	databaseScrollBoxWidth  = logicalWidth - databaseScrollBoxBorderWidth;
	databaseScrollBoxHeight = (logicalHeight - 54) - (logicalHeight * (buttonHeight / 100.0f));

	databaseScrollBoxStartX = databaseScrollBoxBorderWidth / 2;
	databaseScrollBoxStartY = (logicalHeight - databaseScrollBoxHeight) - (databaseScrollBoxBorderWidth / 2) - (logicalHeight * (buttonHeight / 100.0f));

	databaseScrollBoxSpeed   = 15.0f;
	databaseScrollBoxGapSize = 6;
	databaseScrollBoxRadius  = 12;

	databaseScrollBoxBackRed   = 0.39f;
	databaseScrollBoxBackGreen = 0.39f;
	databaseScrollBoxBackBlue  = 0.03f;
	databaseScrollBoxBackAlpha = 0.79f;
	//
	// Font colors are floats to use alpha properly
	databaseScrollBoxFontRed   = 0.07f; //20;
	databaseScrollBoxFontGreen = 0.59f; //150;
	databaseScrollBoxFontBlue  = 0.39f; //100;
	databaseScrollBoxFontAlpha = 0.99f; //240;

	as_guiSetupScrollBox (SCROLLBOX_DB, gui_getString ("db_476"));

	as_guiCreateNewScreen ("databaseScreen");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "databasePrevButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "databasePrevButton", "databaseScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "databasePrevButton", GUI_COORD_TYPE_PERCENT, 15, buttonStartY, 30, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "databasePrevButton", GUI_LABEL_CENTER, gui_getString ("previousButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "databasePrevButton", "as_guiHandleDatabaseAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "databasePrevButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "databasePrevButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "databaseCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "databaseCancelButton", "databaseScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "databaseCancelButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 30, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "databaseCancelButton", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "databaseCancelButton", "as_guiHandleDatabaseAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "databaseCancelButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "databaseCancelButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "databaseNextButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "databaseNextButton", "databaseScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "databaseNextButton", GUI_COORD_TYPE_PERCENT, 85, buttonStartY, 30, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "databaseNextButton", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "databaseNextButton", "as_guiHandleDatabaseAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "databaseNextButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "databaseNextButton", "gui");
}
//----------------------------------------------------------------------------------------------------------------------
//
// Setup the values for the intro scrollbox
void script_setupIntroScrollBox ()
//----------------------------------------------------------------------------------------------------------------------
{
	int introScrollBoxBorderWidth = 10;

	introScrollBoxWidth  = logicalWidth - introScrollBoxBorderWidth;
	introScrollBoxHeight = logicalHeight - 54;

	introScrollBoxStartX = introScrollBoxBorderWidth / 2;
	introScrollBoxStartY = (logicalHeight - introScrollBoxHeight) - (introScrollBoxBorderWidth / 2);

	introScrollBoxSpeed   = 30.0f;
	introScrollBoxGapSize = 6;
	introScrollBoxRadius  = 12;

	introScrollBoxBackRed   = 0.39f;
	introScrollBoxBackGreen = 0.39f;
	introScrollBoxBackBlue  = 0.03f;
	introScrollBoxBackAlpha = 0.39f;
	//
	// Font colors are floats to use alpha properly
	introScrollBoxFontRed   = 0.07f; //20;
	introScrollBoxFontGreen = 0.59f; //150;
	introScrollBoxFontBlue  = 0.39f; //100;
	introScrollBoxFontAlpha = 0.94f; //240;

	as_guiSetupScrollBox (SCROLLBOX_INTRO, gui_getString ("scrollBoxIntroText"));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the Audio options screen
void as_setupOptionsAudioScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY = gam_getTextureHeight ("hud") + buttonHeight;

	as_guiCreateNewScreen ("optionsAudio");

	as_guiCreateObject (GUI_OBJECT_LABEL, "optionAudioLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "optionAudioLabel", "optionsAudio");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "optionAudioLabel", GUI_COORD_TYPE_ABSOLUTE, 8, buttonStartY, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "optionAudioLabel", GUI_LABEL_CENTER, gui_getString ("optionAudioLabel"));
	as_guiSetObjectColor (GUI_OBJECT_LABEL, "optionAudioLabel", GUI_INACTIVE_COL, 250, 250, 200, 250);
	as_guiSetReadyState (GUI_OBJECT_LABEL, "optionAudioLabel", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "optionAudioLabel", "gui");

	as_guiCreateObject (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound");
	as_guiAddObjectToScreen (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound", "optionsAudio");
	as_guiSetObjectPosition (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound", GUI_COORD_TYPE_PERCENT, 1, 30, 25, 25);
	as_guiSetObjectLabel (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound", GUI_LABEL_CENTER, gui_getString ("optionsAudioUseSound"));
	as_guiSetObjectFunction (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound", "as_guiHandleElementAction");
	as_guiSetCheckboxGroup ("optionsAudioUseSound", -1);
	as_guiSetCheckboxTick ("optionsAudioUseSound", -1, enableSound);
	as_guiSetReadyState (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound", true);
	as_guiSetObjectFontName (GUI_OBJECT_CHECKBOX, "optionsAudioUseSound", "gui_small");


	as_guiCreateObject (GUI_OBJECT_SLIDER, "optionsAudioVolume");
	as_guiAddObjectToScreen (GUI_OBJECT_SLIDER, "optionsAudioVolume", "optionsAudio");
	as_guiSetObjectPosition (GUI_OBJECT_SLIDER, "optionsAudioVolume", GUI_COORD_TYPE_PERCENT, 1, 50, 40, 2);
	as_guiSetObjectLabel (GUI_OBJECT_SLIDER, "optionsAudioVolume", GUI_LABEL_CENTER, gui_getString ("optionsAudioVolume"));

	string tempNum;

	for ( int i = 0; i != 10; i++ )
	{
		tempNum = sys_intToString ( i + 1 );
		as_guiAddNewElement ( "optionsAudioVolume", tempNum, tempNum, SLIDER_TYPE_INT );
	}

	as_guiSetSliderValue ( "optionsAudioVolume", volumeLevelStr );

	as_guiSetReadyState (GUI_OBJECT_SLIDER, "optionsAudioVolume", true);
	as_guiSetObjectFontName (GUI_OBJECT_SLIDER, "optionsAudioVolume", "gui_small");
//as_guiAddNewElement (const std::string objectID, const std::string newLabel, const std::string newValue, int type)

	as_guiCreateObject (GUI_OBJECT_BUTTON, "optionsAudioBackButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "optionsAudioBackButton", "optionsAudio");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "optionsAudioBackButton", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "optionsAudioBackButton", GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "optionsAudioBackButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "optionsAudioBackButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "optionsAudioBackButton", "gui");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup Options - Graphics screen
void as_setupOptionsGraphicsScreen()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY = gam_getTextureHeight ("hud") + buttonHeight;

	as_guiCreateNewScreen ("optionsGraphics");

	as_guiCreateObject (GUI_OBJECT_LABEL, "optionGraphicsLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "optionGraphicsLabel", "optionsGraphics");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "optionGraphicsLabel", GUI_COORD_TYPE_ABSOLUTE, 8, buttonStartY, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "optionGraphicsLabel", GUI_LABEL_CENTER, gui_getString ("optionGraphicsLabel"));
	as_guiSetObjectColor (GUI_OBJECT_LABEL, "optionGraphicsLabel", GUI_INACTIVE_COL, 250, 250, 200, 250);
	as_guiSetReadyState (GUI_OBJECT_LABEL, "optionGraphicsLabel", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "optionGraphicsLabel", "gui");

	as_guiCreateObject (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop");
	as_guiAddObjectToScreen (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop", "optionsGraphics");
	as_guiSetObjectPosition (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop", GUI_COORD_TYPE_PERCENT, 50, 30, 25, 25);
	as_guiSetObjectLabel (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop", GUI_LABEL_CENTER, gui_getString ("optionsGraphicsCheckBackdrop"));
	as_guiSetObjectFunction (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop", "as_guiHandleElementAction");
	as_guiSetCheckboxGroup ("optionsGraphicsCheckBackdrop", -1);
	as_guiSetCheckboxTick ("optionsGraphicsCheckBackdrop", -1, renderBackdrop);
	as_guiSetReadyState (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop", true);
	as_guiSetObjectFontName (GUI_OBJECT_CHECKBOX, "optionsGraphicsCheckBackdrop", "gui_small");
	//
	// Which Tile Color
	as_guiCreateObject (GUI_OBJECT_SLIDER, "optionsGraphicsTileColor");
	as_guiAddObjectToScreen (GUI_OBJECT_SLIDER, "optionsGraphicsTileColor", "optionsGraphics");
	as_guiSetObjectPosition (GUI_OBJECT_SLIDER, "optionsGraphicsTileColor", GUI_COORD_TYPE_PERCENT, 1, 50, 30, 2);
	as_guiSetObjectLabel (GUI_OBJECT_SLIDER, "optionsGraphicsTileColor", GUI_LABEL_CENTER, gui_getString ("optionsGraphicsTileColor"));

	as_guiAddNewElement ( "optionsGraphicsTileColor", gui_getString("colorBlue"), "blue", SLIDER_TYPE_STRING );
	as_guiAddNewElement ( "optionsGraphicsTileColor", gui_getString("colorGray"), "gray", SLIDER_TYPE_STRING );
	as_guiAddNewElement ( "optionsGraphicsTileColor", gui_getString("colorGreen"), "green", SLIDER_TYPE_STRING );
	as_guiAddNewElement ( "optionsGraphicsTileColor", gui_getString("colorRed"), "red", SLIDER_TYPE_STRING );
	as_guiAddNewElement ( "optionsGraphicsTileColor", gui_getString("colorTurquoise"), "turquoise", SLIDER_TYPE_STRING );
	as_guiAddNewElement ( "optionsGraphicsTileColor", gui_getString("colorYellow"), "yellow", SLIDER_TYPE_STRING );

	as_guiSetReadyState (GUI_OBJECT_SLIDER, "optionsGraphicsTileColor", true);
	as_guiSetObjectFontName (GUI_OBJECT_SLIDER, "optionsGraphicsTileColor", "gui_small");

	as_guiSetSliderValue ( "optionsGraphicsTileColor", "blue" );
	//
	// Which tile style
	as_guiCreateObject (GUI_OBJECT_SLIDER, "optionsGraphicsTileType");
	as_guiAddObjectToScreen (GUI_OBJECT_SLIDER, "optionsGraphicsTileType", "optionsGraphics");
	as_guiSetObjectPosition (GUI_OBJECT_SLIDER, "optionsGraphicsTileType", GUI_COORD_TYPE_PERCENT, 69, 50, 30, 2);
	as_guiSetObjectLabel (GUI_OBJECT_SLIDER, "optionsGraphicsTileType", GUI_LABEL_CENTER, gui_getString ("optionsGraphicsTileType"));

	as_guiAddNewElement ( "optionsGraphicsTileType", gui_getString("tileTypeFuture"), "future", SLIDER_TYPE_STRING );
	as_guiAddNewElement ( "optionsGraphicsTileType", gui_getString("tileTypeRetro"), "retro", SLIDER_TYPE_STRING );

	as_guiSetReadyState (GUI_OBJECT_SLIDER, "optionsGraphicsTileType", true);
	as_guiSetObjectFontName (GUI_OBJECT_SLIDER, "optionsGraphicsTileType", "gui_small");

	as_guiSetSliderValue ( "optionsGraphicsTileType", "future" );


	as_guiCreateObject (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton", "optionsGraphics");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton", GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "optionsGraphicsCancelButton", "gui");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup Options - Video screen
void as_setupOptionsVideoScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY = gam_getTextureHeight ("hud") + buttonHeight;

	as_guiCreateNewScreen ("optionsVideo");

	as_guiCreateObject (GUI_OBJECT_LABEL, "optionVideoLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "optionVideoLabel", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "optionVideoLabel", GUI_COORD_TYPE_ABSOLUTE, 8, buttonStartY, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "optionVideoLabel", GUI_LABEL_CENTER, gui_getString ("optionVideoLabel"));
	as_guiSetObjectColor (GUI_OBJECT_LABEL, "optionVideoLabel", GUI_INACTIVE_COL, 250, 250, 200, 250);
	as_guiSetReadyState (GUI_OBJECT_LABEL, "optionVideoLabel", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "optionVideoLabel", "gui");

	as_guiCreateObject (GUI_OBJECT_LABEL, "optionVideoLabelWindowType");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "optionVideoLabelWindowType", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "optionVideoLabelWindowType", GUI_COORD_TYPE_ABSOLUTE, 1, 78, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "optionVideoLabelWindowType", GUI_LABEL_CENTER, gui_getString ("optionVideoLabelWindowType"));
	as_guiSetObjectColor (GUI_OBJECT_LABEL, "optionVideoLabelWindowType", GUI_INACTIVE_COL, 250, 250, 200, 250);
	as_guiSetReadyState (GUI_OBJECT_LABEL, "optionVideoLabelWindowType", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "optionVideoLabelWindowType", "gui_small");

	as_guiCreateObject (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed");
	as_guiAddObjectToScreen (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed", GUI_COORD_TYPE_PERCENT, 2, 40, 25, 25);
	as_guiSetObjectLabel (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed", GUI_LABEL_CENTER, gui_getString ("optionsVideoCheckWindowed"));
	as_guiSetObjectFunction (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed", "as_guiHandleElementAction");
	as_guiSetCheckboxGroup ("optionsVideoCheckWindowed", 1);
	as_guiSetCheckboxValue ("optionsVideoCheckWindowed", 0);  // Windowed
	as_guiSetReadyState (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed", true);
	as_guiSetObjectFontName (GUI_OBJECT_CHECKBOX, "optionsVideoCheckWindowed", "gui_small");

	as_guiCreateObject (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen");
	as_guiAddObjectToScreen (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", GUI_COORD_TYPE_PERCENT, 2, 48, 25, 25);
	as_guiSetObjectLabel (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", GUI_LABEL_CENTER, gui_getString ("optionsVideoCheckFullScreen"));
	as_guiSetObjectFunction (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", "as_guiHandleElementAction");
	as_guiSetCheckboxGroup ("optionsVideoCheckFullScreen", 1);
	as_guiSetCheckboxValue ("optionsVideoCheckFullScreen", 2);  // Fullscreen
	as_guiSetReadyState (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", true);
	as_guiSetObjectFontName (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", "gui_small");

	as_guiCreateObject (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed");
	as_guiAddObjectToScreen (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed", GUI_COORD_TYPE_PERCENT, 2, 56, 25, 25);
	as_guiSetObjectLabel (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed", GUI_LABEL_CENTER, gui_getString ("optionsVideoCheckFullScreenWindowed"));
	as_guiSetObjectFunction (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed", "as_guiHandleElementAction");
	as_guiSetCheckboxGroup ("optionsVideoCheckFullScreenWindowed", 1);
	as_guiSetCheckboxValue ("optionsVideoCheckFullScreenWindowed", 1);  // Fullscreen windowed
	as_guiSetReadyState (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed", true);
	as_guiSetObjectFontName (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreenWindowed", "gui_small");

	switch (screenType)
		{
			case 0:
				as_guiSetCheckboxTick ("optionsVideoCheckFullScreen", -1, true);
			break;

			case 1:
				as_guiSetCheckboxTick ("optionsVideoCheckFullScreenWindowed", -1, true); // Repeat for off for others??
			break;

			case 2:
				as_guiSetCheckboxTick ("optionsVideoCheckWindowed", -1, true); // Repeat for off for others??
			break;
		}

	as_guiCreateObject (GUI_OBJECT_BUTTON, "optionsVideoCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", "gui");

//	as_guiSetCheckboxTick ( "optionsVideoCheckFullScreen", -1, fullScreenValue == 1 ? true : false );
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the options screen
void as_setupOptionsScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float hudSize = gam_getTextureHeight ("hud") + hudPadding;

	as_guiCreateNewScreen ("guiOptionsMain");

	as_guiCreateObject (GUI_OBJECT_LABEL, "guiOptionsMainLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "guiOptionsMainLabel", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "guiOptionsMainLabel", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "guiOptionsMainLabel", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainLabel"));
	as_guiSetObjectColor (GUI_OBJECT_LABEL, "guiOptionsMainLabel", GUI_INACTIVE_COL, 250, 250, 200, 250);
	as_guiSetReadyState (GUI_OBJECT_LABEL, "guiOptionsMainLabel", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "guiOptionsMainLabel", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", GUI_COORD_TYPE_PERCENT, 50, 40, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainVideoButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", GUI_COORD_TYPE_PERCENT, 50, 51, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainAudioButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", GUI_COORD_TYPE_PERCENT, 50, 62, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainGraphicsButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", GUI_COORD_TYPE_PERCENT, 50, 73, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainControlsButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", GUI_COORD_TYPE_PERCENT, 20, 84, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", "gui");
}

//------------------------------------------------------------
//
// Setup tutorial screen
void as_guiSetupTutorial ()
//------------------------------------------------------------
{
	float hudSize      = gam_getTextureHeight ("hud") + hudPadding;
	float tutImageSize = 48.0f;

	as_guiCreateNewScreen ("scrTutorial");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutMove");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutMove", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutMove", GUI_LABEL_CENTER, gui_getString ("tutMoveLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutMove", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutMove", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "textLabelTutMove", "gui_label");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTextBox", GUI_LABEL_LEFT, gui_getString ("tutMoveText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTextBox", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTextBox", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "tutTextBox", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutMoveNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutMoveNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutMoveNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutMoveNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutMoveNext", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutMoveNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutMoveNext", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutorialCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutorialCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutorialCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutorialCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutorialCancel", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutorialCancel", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutorialCancel", "gui");

	//
	// Screen how to start transfer mode
	//
	as_guiCreateNewScreen ("scrTutTransfer");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTransMove");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTransMove", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTransMove", GUI_LABEL_CENTER, gui_getString ("tutTransferLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTransMove", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTransMove", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "textLabelTutTransMove", "gui_label");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTransTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTransTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTransTextBox", GUI_LABEL_LEFT, gui_getString ("tutTransText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTransTextBox", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTransTextBox", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "tutTransTextBox", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransNext", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutTransNext", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransCancel", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransCancel", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutTransCancel", "gui");

	//
	// Screen explaining transfer game
	//
	as_guiCreateNewScreen ("scrTutTransGame");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTransGame");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTransGame", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTransGame", GUI_LABEL_CENTER, gui_getString ("tutTransGameLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTransGame", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTransGame", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "textLabelTutTransGame", "gui_label");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", GUI_LABEL_LEFT, gui_getString ("tutTransGameText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransGameNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", "gui");

	//
	// Screen showing how to use lifts
	//
	as_guiCreateNewScreen ("scrTutLifts");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutLifts");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutLifts", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutLifts", GUI_LABEL_CENTER, gui_getString ("tutLiftLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutLifts", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutLifts", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "textLabelTutLifts", "gui_label");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutLiftTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", GUI_COORD_TYPE_PERCENT, 5, 50, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", GUI_LABEL_LEFT, gui_getString ("tutLiftText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", "gui_small");

	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageLift");
	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageLift", "scrTutLifts");
	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageLift", GUI_COORD_TYPE_ABSOLUTE, (logicalWidth - tutImageSize) / 2, hudSize + (hudPadding * 2), tutImageSize, tutImageSize);
	as_guiSetImageKeyName ("tutImageLift", "tut_lift");
	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageLift", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutLiftNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutLiftNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutLiftNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutLiftNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutLiftNext", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutLiftNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutLiftNext", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutLiftCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", "gui");

	//
	// Screen showing terminal use
	//
	as_guiCreateNewScreen ("scrTutTerminals");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTerminals");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTerminals", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTerminals", GUI_LABEL_CENTER, gui_getString ("tutTerminalLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTerminals", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTerminals", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "textLabelTutTerminals", "gui_label");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", GUI_COORD_TYPE_PERCENT, 5, 50, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", GUI_LABEL_LEFT, gui_getString ("tutTerminalText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", "gui_small");

	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageTerminal");
	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageTerminal", "scrTutTerminals");
	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageTerminal", GUI_COORD_TYPE_ABSOLUTE, (logicalWidth - tutImageSize) / 2, hudSize + (hudPadding * 2), tutImageSize, tutImageSize);
	as_guiSetImageKeyName ("tutImageTerminal", "tut_terminal");
	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageTerminal", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTerminalNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", "gui");

	//
	// Screen showing healing use
	//
	as_guiCreateNewScreen ("scrTutHealing");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutHealing");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutHealing", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutHealing", GUI_LABEL_CENTER, gui_getString ("tutHealingLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutHealing", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutHealing", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "textLabelTutHealing", "gui_label");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutHealingTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", GUI_COORD_TYPE_PERCENT, 5, 50, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", GUI_LABEL_LEFT, gui_getString ("tutHealingText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", "gui_small");

	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageHealing");
	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageHealing", "scrTutHealing");
	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageHealing", GUI_COORD_TYPE_ABSOLUTE, (logicalWidth - tutImageSize) / 2, hudSize + (hudPadding * 2), tutImageSize, tutImageSize);
	as_guiSetImageKeyName ("tutImageHealing", "tut_healing");
	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageHealing", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutHealinglNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutHealingCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", "gui");

	//
	// Screen showing some gameplay tips
	//
	as_guiCreateNewScreen ("scrTutTips");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTips");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTips", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTips", GUI_LABEL_CENTER, gui_getString ("tutTipsLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTips", "scrTutTips");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTips", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "textLabelTutTips", "gui_label");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTipsTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", GUI_LABEL_LEFT, gui_getString ("tutTipsText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", "scrTutTips");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTipsCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", "scrTutTips");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", "gui");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the screen to display the current sideview from the terminal
void as_setupTerminalDeckviewScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY = 80; //logicalHeight - (buttonHeight * 2);

	as_guiCreateNewScreen ("terminalDeckviewScreen");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "deckviewCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "deckviewCancelButton", "terminalDeckviewScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "deckviewCancelButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "deckviewCancelButton", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "deckviewCancelButton", "as_guiHandleTerminalAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "deckviewCancelButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "deckviewCancelButton", "gui");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the screen to display the current shipview from the terminal
void as_setupTerminalShipviewScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY = 80; //logicalHeight - (buttonHeight * 2);

	as_guiCreateNewScreen ("terminalShipviewScreen");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "shipviewCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "shipviewCancelButton", "terminalShipviewScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "shipviewCancelButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "shipviewCancelButton", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "shipviewCancelButton", "as_guiHandleTerminalAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "shipviewCancelButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "shipviewCancelButton", "gui");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the main terminal screen with buttons
void as_setupMainTerminalScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY  = 40;
	float buttonSpacing = 13;
	float hudSize       = gam_getTextureHeight ("hud") + hudPadding;

	as_guiCreateNewScreen ("mainTerminalScreen");

	as_guiCreateObject (GUI_OBJECT_LABEL, "terminalScreenLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "terminalScreenLabel", "mainTerminalScreen");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "terminalScreenLabel", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "terminalScreenLabel", GUI_LABEL_CENTER, gui_getString ("terminalScreenLabel"));
	as_guiSetReadyState (GUI_OBJECT_LABEL, "terminalScreenLabel", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "terminalScreenLabel", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "terminalLogoffButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "terminalLogoffButton", "mainTerminalScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "terminalLogoffButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "terminalLogoffButton", GUI_LABEL_CENTER, gui_getString ("terminalLogoffButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "terminalLogoffButton", "as_guiHandleTerminalAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "terminalLogoffButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "terminalLogoffButton", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "terminalDatabaseButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "terminalDatabaseButton", "mainTerminalScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "terminalDatabaseButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "terminalDatabaseButton", GUI_LABEL_CENTER, gui_getString ("terminalDatabaseButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "terminalDatabaseButton", "as_guiHandleTerminalAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "terminalDatabaseButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "terminalDatabaseButton", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "terminalDeckviewButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "terminalDeckviewButton", "mainTerminalScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "terminalDeckviewButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "terminalDeckviewButton", GUI_LABEL_CENTER, gui_getString ("terminalDeckviewButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "terminalDeckviewButton", "as_guiHandleTerminalAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "terminalDeckviewButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "terminalDeckviewButton", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "terminalShipviewButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "terminalShipviewButton", "mainTerminalScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "terminalShipviewButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "terminalShipviewButton", GUI_LABEL_CENTER, gui_getString ("terminalShipviewButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "terminalShipviewButton", "as_guiHandleTerminalAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "terminalShipviewButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "terminalShipviewButton", "gui_small");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the first transfer screen
void as_setupTransferOne ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_guiCreateNewScreen ("guiTransferOne");

	as_guiCreateObject (GUI_OBJECT_LABEL, "labelTransferOne");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "labelTransferOne", "guiTransferOne");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "labelTransferOne", GUI_COORD_TYPE_PERCENT, 8, 30, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "labelTransferOne", GUI_LABEL_CENTER, gui_getString ("labelTransferOne"));
	as_guiSetReadyState (GUI_OBJECT_LABEL, "labelTransferOne", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "labelTransferOne", "gui");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "textBoxTransferOne");
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "textBoxTransferOne", "guiTransferOne");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "textBoxTransferOne", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "textBoxTransferOne", GUI_LABEL_LEFT, gui_getString ("textBoxTransferOne"));
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "textBoxTransferOne", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "textBoxTransferOne", "gui");

//	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageLift");
//	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageLift", "scrTutLifts");
//	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageLift", GUI_COORD_TYPE_ABSOLUTE, 192, 100, gam_getTextureWidth ("tut_lift"), gam_getTextureHeight ("tut_lift"));
//	as_guiSetImageKeyName ("tutImageLift", "tut_lift");
//	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageLift", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTransferOneNext");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTransferOneNext", "guiTransferOne");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTransferOneNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTransferOneNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTransferOneNext", "as_guiHandleTransferAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTransferOneNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTransferOneNext", "gui");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the second transfer screen
void as_setupTransferTwo ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_guiCreateNewScreen ("guiTransferTwo");

	as_guiCreateObject (GUI_OBJECT_LABEL, "labelTransferTwo");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "labelTransferTwo", "guiTransferTwo");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "labelTransferTwo", GUI_COORD_TYPE_PERCENT, 8, 30, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "labelTransferTwo", GUI_LABEL_CENTER, gui_getString ("labelTransferTwo"));
	as_guiSetReadyState (GUI_OBJECT_LABEL, "labelTransferTwo", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "labelTransferTwo", "gui");

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "textBoxTransferTwo");
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "textBoxTransferTwo", "guiTransferTwo");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "textBoxTransferTwo", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "textBoxTransferTwo", GUI_LABEL_LEFT, gui_getString ("textBoxTransferTwo"));
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "textBoxTransferTwo", true);
	as_guiSetObjectFontName (GUI_OBJECT_TEXTBOX, "textBoxTransferTwo", "gui");

//	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageLift");
//	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageLift", "scrTutLifts");
//	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageLift", GUI_COORD_TYPE_ABSOLUTE, 192, 100, gam_getTextureWidth ("tut_lift"), gam_getTextureHeight ("tut_lift"));
//	as_guiSetImageKeyName ("tutImageLift", "tut_lift");
//	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageLift", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTransferTwoNext");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTransferTwoNext", "guiTransferTwo");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTransferTwoNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTransferTwoNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTransferTwoNext", "as_guiHandleTransferAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTransferTwoNext", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "buttonTransferTwoNext", "gui");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the main GUI screen with buttons
void as_setupMainScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY = 40;
//	float labelStartY                 = 32;
	float labelStartY  = gam_getTextureHeight ("hud") - buttonHeight;
	float hudSize      = gam_getTextureHeight ("hud") + hudPadding;

	float buttonSpacing = 13;

	as_guiCreateNewScreen ("mainGUIScreen");

	as_guiCreateObject (GUI_OBJECT_LABEL, "mainScreenLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "mainScreenLabel", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "mainScreenLabel", GUI_COORD_TYPE_ABSOLUTE, 8, hudSize, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "mainScreenLabel", GUI_LABEL_CENTER, gui_getString ("mainScreenLabel"));
	as_guiSetReadyState (GUI_OBJECT_LABEL, "mainScreenLabel", true);
	as_guiSetObjectFontName (GUI_OBJECT_LABEL, "mainScreenLabel", "gui_small");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "startGameButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "startGameButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "startGameButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "startGameButton", GUI_LABEL_CENTER, gui_getString ("startGameButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "startGameButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "startGameButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "startGameButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "optionsButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "optionsButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "optionsButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "optionsButton", GUI_LABEL_CENTER, gui_getString ("optionsButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "optionsButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "optionsButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "optionsButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "tutorialButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "tutorialButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "tutorialButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "tutorialButton", GUI_LABEL_CENTER, gui_getString ("tutorialButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "tutorialButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "tutorialButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "tutorialButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "briefingButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "briefingButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "briefingButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "briefingButton", GUI_LABEL_CENTER, gui_getString ("briefingButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "briefingButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "briefingButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "briefingButton", "gui");

	as_guiCreateObject (GUI_OBJECT_BUTTON, "exitGameButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "exitGameButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "exitGameButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "exitGameButton", GUI_LABEL_CENTER, gui_getString ("exitGameButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "exitGameButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "exitGameButton", true);
	as_guiSetObjectFontName (GUI_OBJECT_BUTTON, "exitGameButton", "gui");
}

//-----------------------------------------------------------------------------
//
// Create colors for the sideview graphic
void as_setSideviewColors ()
//-----------------------------------------------------------------------------
{
	as_createSideViewColor (SIDEVIEW_SHIP_COLOR, 142, 182, 12, 255);
	as_createSideViewColor (SIDEVIEW_ACTIVE_DECK_COLOR, 71, 199, 232, 255);
	as_createSideViewColor (SIDEVIEW_ENGINE_COLOR, 34, 34, 34, 255);
	as_createSideViewColor (SIDEVIEW_LIFT_COLOR, 18, 76, 88, 255);
	as_createSideViewColor (SIDEVIEW_ACTIVE_LIFT_COLOR, 38, 221, 188, 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup any message box instances
void as_setupMessageBoxes ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_guiAddMessageBox (MESSAGE_BOX_PAUSED, gui_getString ("pausedTitle"), gui_getString ("pausedText"), -1, -1, false);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init the gui variables
void script_initGUI ()
//----------------------------------------------------------------------------------------------------------------------
{
	buttonHeight = 9;

	as_setLanguageStrings ();
	as_setSideviewColors ();
	script_setupIntroScrollBox ();
	as_setupMainScreen ();
	as_setupOptionsScreen ();
	as_setupOptionsVideoScreen ();
	as_setupOptionsAudioScreen ();
	as_setupOptionsGraphicsScreen ();
	as_guiSetupTutorial ();
	as_setupMainTerminalScreen ();
	as_setupTerminalDeckviewScreen ();
	as_setupTerminalShipviewScreen ();
	script_setupDatabaseScreen ();
	as_setupTransferOne ();
	as_setupTransferTwo ();

//	as_setupMessageBoxes();

	currentGUIScreen      = as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen");
	currentObjectSelected = as_guiFindIndex (GUI_OBJECT_BUTTON, "startGameButton");
	as_guiSetObjectFocus ("startGameButton");

	//
	// --------------- SET COLORS FOR BUTTONS -------------------------
	as_guiSetObjectColor (GUI_OBJECT_BUTTON, "ALL", GUI_ACTIVE_COL, 0.19f, 0.58f, 0.58f, 0.98f);
	as_guiSetObjectColor (GUI_OBJECT_BUTTON, "ALL", GUI_INACTIVE_COL, 0.0f, 0.19f, 0.19f, 0.98f);
	as_guiSetObjectColor (GUI_OBJECT_BUTTON, "ALL", GUI_ACTIVE_LABEL_COL, 1.0f, 1.0f, 1.0f, 1.0f);
	as_guiSetObjectColor (GUI_OBJECT_BUTTON, "ALL", GUI_INACTIVE_LABEL_COL, 0.0f, 0.0f, 0.0f, 1.0f);

	as_guiSetObjectColor (GUI_OBJECT_CHECKBOX, "ALL", GUI_ACTIVE_COL, 0.19f, 0.58f, 0.58f, 0.98f);
	as_guiSetObjectColor (GUI_OBJECT_CHECKBOX, "ALL", GUI_INACTIVE_COL, 0.0f, 0.19f, 0.19f, 0.98f);

	as_guiSetObjectColor (GUI_OBJECT_TEXTBOX, "ALL", GUI_ACTIVE_COL, 0.19f, 0.58f, 0.58f, 0.98f);

	as_guiSetObjectColor (GUI_OBJECT_LABEL, "ALL", GUI_INACTIVE_COL, 0.98f, 0.98f, 0.78f, 0.98f);

	as_guiSetObjectColor (GUI_OBJECT_SLIDER, "ALL", GUI_ACTIVE_COL, 0.19f, 0.58f, 0.58f, 0.98f);
	as_guiSetObjectColor (GUI_OBJECT_SLIDER, "ALL", GUI_INACTIVE_COL, 0.0f, 0.19f, 0.19f, 0.98f);

	isGUIStarted = true;
}
