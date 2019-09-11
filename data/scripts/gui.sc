int buttonHeight = 7;

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
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "briefingButton"))
				{
					sys_changeCurrentMode (MODE_GUI_INTRO, true);
					return;
				}

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "optionsButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "guiOptionsMain"));
					sys_changeCurrentMode (MODE_GUI_OPTIONS, true);
					as_guiSetObjectFocus ("guiOptionsMainCancelButton");
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

			if (currentObjectSelected == as_guiFindIndex (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton"))
				{
					as_guiChangeCurrentScreen (as_guiFindIndex (GUI_OBJECT_SCREEN, "mainGUIScreen"));
					sys_changeCurrentMode (MODE_GUI, true);
					as_guiSetObjectFocus ("startGameButton");
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
					as_guiSetCheckboxTick ("optionsVideoCheckFullScreen", -1, !fullScreen);
					fullScreen = !fullScreen;
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
// Setup up Options - Video screen
void as_setupOptionsVideoScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_guiCreateNewScreen ("optionsVideo");

	as_guiCreateObject (GUI_OBJECT_LABEL, "optionVideoLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "optionVideoLabel", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "optionVideoLabel", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "optionVideoLabel", GUI_LABEL_CENTER, gui_getString ("optionVideoLabel"));
	as_guiSetObjectColor (GUI_OBJECT_LABEL, "optionVideoLabel", GUI_INACTIVE_COL, 250, 250, 200, 250);
	as_guiSetReadyState (GUI_OBJECT_LABEL, "optionVideoLabel", true);

	as_guiCreateObject (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen");
	as_guiAddObjectToScreen (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", GUI_COORD_TYPE_PERCENT, 50, 30, 25, 25);
	as_guiSetObjectLabel (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", GUI_LABEL_CENTER, gui_getString ("optionsVideoCheckFullScreen"));
	as_guiSetObjectFunction (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", "as_guiHandleElementAction");
	as_guiSetCheckboxGroup ("optionsVideoCheckFullScreen", -1);
	as_guiSetCheckboxTick ("optionsVideoCheckFullScreen", -1, fullScreen);
	as_guiSetReadyState (GUI_OBJECT_CHECKBOX, "optionsVideoCheckFullScreen", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "optionsVideoCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", "optionsVideo");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "optionsVideoCancelButton", true);

//	as_guiSetCheckboxTick ( "optionsVideoCheckFullScreen", -1, fullScreenValue == 1 ? true : false );
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the options screen
void as_setupOptionsScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_guiCreateNewScreen ("guiOptionsMain");

	as_guiCreateObject (GUI_OBJECT_LABEL, "guiOptionsMainLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "guiOptionsMainLabel", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "guiOptionsMainLabel", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "guiOptionsMainLabel", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainLabel"));
	as_guiSetObjectColor (GUI_OBJECT_LABEL, "guiOptionsMainLabel", GUI_INACTIVE_COL, 250, 250, 200, 250);
	as_guiSetReadyState (GUI_OBJECT_LABEL, "guiOptionsMainLabel", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", GUI_COORD_TYPE_PERCENT, 50, 40, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainVideoButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainVideoButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", GUI_COORD_TYPE_PERCENT, 50, 51, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainAudioButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainAudioButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", GUI_COORD_TYPE_PERCENT, 50, 62, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainGraphicsButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainGraphicsButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", GUI_COORD_TYPE_PERCENT, 50, 73, 65, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", GUI_LABEL_CENTER, gui_getString ("guiOptionsMainControlsButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainControlsButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", "guiOptionsMain");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", GUI_COORD_TYPE_PERCENT, 20, 84, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "guiOptionsMainCancelButton", true);
}

//------------------------------------------------------------
//
// Setup tutorial screen
void as_guiSetupTutorial ()
//------------------------------------------------------------
{
	as_guiCreateNewScreen ("scrTutorial");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutMove");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutMove", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutMove", GUI_LABEL_CENTER, gui_getString ("tutMoveLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutMove", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutMove", true);

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTextBox", GUI_LABEL_LEFT, gui_getString ("tutMoveText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTextBox", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTextBox", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutMoveNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutMoveNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutMoveNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutMoveNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutMoveNext", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutMoveNext", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutorialCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutorialCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutorialCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutorialCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutorialCancel", "scrTutorial");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutorialCancel", true);

	//
	// Screen how to start transfer mode
	//
	as_guiCreateNewScreen ("scrTutTransfer");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTransMove");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTransMove", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTransMove", GUI_LABEL_CENTER, gui_getString ("tutTransferLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTransMove", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTransMove", true);

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTransTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTransTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTransTextBox", GUI_LABEL_LEFT, gui_getString ("tutTransText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTransTextBox", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTransTextBox", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransNext", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransNext", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransCancel", "scrTutTransfer");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransCancel", true);

	//
	// Screen explaining transfer game
	//
	as_guiCreateNewScreen ("scrTutTransGame");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTransGame");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTransGame", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTransGame", GUI_LABEL_CENTER, gui_getString ("tutTransGameLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTransGame", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTransGame", true);

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", GUI_LABEL_LEFT, gui_getString ("tutTransGameText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTransGameTextBox", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransGameNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransGameNext", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", "scrTutTransGame");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTransGameCancel", true);

	//
	// Screen showing how to use lifts
	//
	as_guiCreateNewScreen ("scrTutLifts");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutLifts");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutLifts", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutLifts", GUI_LABEL_CENTER, gui_getString ("tutLiftLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutLifts", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutLifts", true);

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutLiftTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", GUI_COORD_TYPE_PERCENT, 5, 50, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", GUI_LABEL_LEFT, gui_getString ("tutLiftText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutLiftTextBox", true);

	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageLift");
	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageLift", "scrTutLifts");
	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageLift", GUI_COORD_TYPE_ABSOLUTE, 192, 100, gam_getTextureWidth ("tut_lift"), gam_getTextureHeight ("tut_lift"));
	as_guiSetImageKeyName ("tutImageLift", "tut_lift");
	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageLift", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutLiftNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutLiftNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutLiftNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutLiftNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutLiftNext", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutLiftNext", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutLiftCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", "scrTutLifts");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutLiftCancel", true);

	//
	// Screen showing terminal use
	//
	as_guiCreateNewScreen ("scrTutTerminals");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTerminals");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTerminals", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTerminals", GUI_LABEL_CENTER, gui_getString ("tutTerminalLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTerminals", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTerminals", true);

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", GUI_COORD_TYPE_PERCENT, 5, 50, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", GUI_LABEL_LEFT, gui_getString ("tutTerminalText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTerminalTextBox", true);

	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageTerminal");
	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageTerminal", "scrTutTerminals");
	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageTerminal", GUI_COORD_TYPE_ABSOLUTE, 192, 100, gam_getTextureWidth ("tut_terminal"), gam_getTextureHeight ("tut_terminal"));
	as_guiSetImageKeyName ("tutImageTerminal", "tut_terminal");
	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageTerminal", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTerminalNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTerminalNext", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", "scrTutTerminals");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTerminalCancel", true);

	//
	// Screen showing healing use
	//
	as_guiCreateNewScreen ("scrTutHealing");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutHealing");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutHealing", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutHealing", GUI_LABEL_CENTER, gui_getString ("tutHealingLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutHealing", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutHealing", true);

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutHealingTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", GUI_COORD_TYPE_PERCENT, 5, 50, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", GUI_LABEL_LEFT, gui_getString ("tutHealingText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutHealingTextBox", true);

	as_guiCreateObject (GUI_OBJECT_IMAGE, "tutImageHealing");
	as_guiAddObjectToScreen (GUI_OBJECT_IMAGE, "tutImageHealing", "scrTutHealing");
	as_guiSetObjectPosition (GUI_OBJECT_IMAGE, "tutImageHealing", GUI_COORD_TYPE_ABSOLUTE, 192, 100, gam_getTextureWidth ("tut_healing"), gam_getTextureHeight ("tut_healing"));
	as_guiSetImageKeyName ("tutImageHealing", "tut_healing");
	as_guiSetReadyState (GUI_OBJECT_IMAGE, "tutImageHealing", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutHealinglNext");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", GUI_COORD_TYPE_PERCENT, 20, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutHealinglNext", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutHealingCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", "scrTutHealing");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutHealingCancel", true);

	//
	// Screen showing some gameplay tips
	//
	as_guiCreateNewScreen ("scrTutTips");

	as_guiCreateObject (GUI_OBJECT_LABEL, "textLabelTutTips");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "textLabelTutTips", GUI_COORD_TYPE_PERCENT, 8, 19, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "textLabelTutTips", GUI_LABEL_CENTER, gui_getString ("tutTipsLabel"));
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "textLabelTutTips", "scrTutTips");
	as_guiSetReadyState (GUI_OBJECT_LABEL, "textLabelTutTips", true);

	as_guiCreateObject (GUI_OBJECT_TEXTBOX, "tutTipsTextBox");
	as_guiSetObjectPosition (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", GUI_COORD_TYPE_PERCENT, 5, 35, 90, 90);
	as_guiSetObjectLabel (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", GUI_LABEL_LEFT, gui_getString ("tutTipsText"));
	as_guiAddObjectToScreen (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", "scrTutTips");
	as_guiSetReadyState (GUI_OBJECT_TEXTBOX, "tutTipsTextBox", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "buttonTutTipsCancel");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", GUI_COORD_TYPE_PERCENT, 80, 90, 25, buttonHeight);
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", "as_guiHandleElementAction");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", "scrTutTips");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "buttonTutTipsCancel", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the main GUI screen with buttons
void as_setupMainScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	float buttonStartY = 40;
	float buttonSpacing = 13;

	as_guiCreateNewScreen ("mainGUIScreen");

	as_guiCreateObject (GUI_OBJECT_LABEL, "mainScreenLabel");
	as_guiAddObjectToScreen (GUI_OBJECT_LABEL, "mainScreenLabel", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_LABEL, "mainScreenLabel", GUI_COORD_TYPE_PERCENT, 8, 30, 10, 10);
	as_guiSetObjectLabel (GUI_OBJECT_LABEL, "mainScreenLabel", GUI_LABEL_CENTER, gui_getString ("mainScreenLabel"));
	as_guiSetReadyState (GUI_OBJECT_LABEL, "mainScreenLabel", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "startGameButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "startGameButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "startGameButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "startGameButton", GUI_LABEL_CENTER, gui_getString ("startGameButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "startGameButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "startGameButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "optionsButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "optionsButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "optionsButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "optionsButton", GUI_LABEL_CENTER, gui_getString ("optionsButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "optionsButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "optionsButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "tutorialButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "tutorialButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "tutorialButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "tutorialButton", GUI_LABEL_CENTER, gui_getString ("tutorialButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "tutorialButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "tutorialButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "briefingButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "briefingButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "briefingButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "briefingButton", GUI_LABEL_CENTER, gui_getString ("briefingButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "briefingButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "briefingButton", true);

	as_guiCreateObject (GUI_OBJECT_BUTTON, "exitGameButton");
	as_guiAddObjectToScreen (GUI_OBJECT_BUTTON, "exitGameButton", "mainGUIScreen");
	as_guiSetObjectPosition (GUI_OBJECT_BUTTON, "exitGameButton", GUI_COORD_TYPE_PERCENT, 50, buttonStartY, 65, buttonHeight);
	buttonStartY += buttonSpacing;
	as_guiSetObjectLabel (GUI_OBJECT_BUTTON, "exitGameButton", GUI_LABEL_CENTER, gui_getString ("exitGameButton"));
	as_guiSetObjectFunction (GUI_OBJECT_BUTTON, "exitGameButton", "as_guiHandleElementAction");
	as_guiSetReadyState (GUI_OBJECT_BUTTON, "exitGameButton", true);
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
	as_guiSetupTutorial ();
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

	isGUIStarted = true;
}
