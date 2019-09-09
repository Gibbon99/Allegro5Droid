
//----------------------------------------------------------------------------------------------------------------------
//
// Setup the values for the intro scrollbox
void script_setupIntroScrollBox()
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

	introScrollBoxBackRed   = 100;
	introScrollBoxBackGreen = 100;
	introScrollBoxBackBlue  = 10;
	introScrollBoxBackAlpha = 100;
	//
	// Font colors are floats to use alpha properly
	introScrollBoxFontRed   = 0.07f; //20;
	introScrollBoxFontGreen = 0.59f; //150;
	introScrollBoxFontBlue  = 0.39f; //100;
	introScrollBoxFontAlpha = 0.94f; //240;

	as_guiSetupScrollBox(SCROLLBOX_INTRO, gui_getString("scrollBoxIntroText"));
}

//-----------------------------------------------------------------------------
//
// Create colors for the sideview graphic
void as_setSideviewColors()
//-----------------------------------------------------------------------------
{
	as_createSideViewColor(SIDEVIEW_SHIP_COLOR, 142, 182, 12, 255);
	as_createSideViewColor(SIDEVIEW_ACTIVE_DECK_COLOR, 71, 199, 232, 255);
	as_createSideViewColor(SIDEVIEW_ENGINE_COLOR, 34, 34, 34, 255);
	as_createSideViewColor(SIDEVIEW_LIFT_COLOR, 18, 76, 88, 255);
	as_createSideViewColor(SIDEVIEW_ACTIVE_LIFT_COLOR, 38, 221, 188, 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init the gui variables
void script_initGUI()
//----------------------------------------------------------------------------------------------------------------------
{
	as_setLanguageStrings();
	as_setSideviewColors();
	script_setupIntroScrollBox();
}
