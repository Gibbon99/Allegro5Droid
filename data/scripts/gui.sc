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
}
