//-----------------------------------------------------------------------------
//
// Setup the values for the transfer game
void as_initTransferValues ()
//-----------------------------------------------------------------------------
{
	int backgroundPadding = 10;

	transferBitmapWidth  = 384;
	transferBitmapHeight = 384;

	trn_setTransferColor (TRANSFER_COLOR_LEFT, 1.0, 1.0, 0.0, 1.0);
	trn_setTransferColor (TRANSFER_COLOR_RIGHT, 0.0, 1.0, 1.0, 1.0);
	trn_setTransferColor (TRANSFER_COLOR_BACKGROUND, 0.5, 0.3, 0.1, 1.0);
	trn_setTransferColor (TRANSFER_COLOR_BORDER, 0.0, 0.0, 0.0, 1.0);

	transferImageFade       = 0.8f;
	numberTransferRows      = 12;
	transferRowHeight       = 16;
	transferRowStartY       = 135.0f;
	transferRowCellWidth    = 30.0f;
	transferBorderThickness = 2;
	transferStatusTabWidth  = transferRowCellWidth / 3;
	transferStatusTabHeight = transferRowHeight / 2;

	transferSidebarGap    = 30;
	transferSidebarWidth  = 8;
	transferLineThickness = 3;

	transferBackgroundStartX = backgroundPadding;
	transferBackgroundStartY = 55;
	transferBackgroundWidth  = transferBitmapWidth - (backgroundPadding * 2);
	transferBackgroundHeight = transferBitmapHeight - (backgroundPadding) - transferBackgroundStartY;
}