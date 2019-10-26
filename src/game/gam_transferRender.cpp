#include <hdr/game/gam_player.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_transfer.h>
#include <hdr/io/io_logFile.h>
#include <hdr/system/sys_init.h>
#include <hdr/game/gam_database.h>
#include "hdr/game/gam_transferRender.h"

int   transferBorderThickness;
int   numberTransferRows;
float transferRowHeight;
float transferRowCellWidth;
float transferRowStartY;
float transferBackgroundStartX;
float transferBackgroundStartY;
float transferBackgroundWidth;
float transferBackgroundHeight;
float transferStatusTabWidth;
float transferStatusTabHeight;
float transferSidebarGap;
float transferSidebarWidth;

int   transferPlayerWhichSide;
float transferLineThickness;

int transferBitmapWidth;
int transferBitmapHeight;

ALLEGRO_COLOR transferColorLeft;
ALLEGRO_COLOR transferColorRight;
ALLEGRO_COLOR transferColorBackground;
ALLEGRO_COLOR transferColorBorder;
ALLEGRO_COLOR transferColorStatusCell;

ALLEGRO_BITMAP *transferGameBitmap = nullptr;

float gapWidth;

std::vector<__transferRow> transferRows;

#define BLOCK_HEIGHT             8
#define BLOCK_WIDTH              4
#define BLOCK_STEP               2

//
// Create another backing bitmap large enough to hold the transfer screen
//
// Blit onto the display window scaled

//-------------------------------------------------------------------------------------------------------------------
//
// Return the row type by string
std::string trn_getRowName(int rowType)
//-------------------------------------------------------------------------------------------------------------------
{
	switch (rowType)
		{
			case TRANSFER_ROW_FULL_LINE:
				return "TRANSFER_ROW_FULL_LINE";
				break;

			case TRANSFER_ROW_HALF_LINE:
				return "TRANSFER_ROW_HALF_LINE";
				break;

			case TRANSFER_ROW_3_4_LINE:
				return "TRANSFER_ROW_3_4_LINE";
				break;

			case TRANSFER_ROW_FULL_LINE_1:
				return "TRANSFER_ROW_FULL_LINE_1";
				break;

			case TRANSFER_ROW_QUARTER_LINE:
				return "TRANSFER_ROW_QUARTER_LINE";
				break;

			case TRANSFER_ROW_REPEAT_HALF:
				return "TRANSFER_ROW_REPEAT_HALF";
				break;

			case TRANSFER_ROW_REPEAT_QUARTER:
				return "TRANSFER_ROW_REPEAT_QUARTER";
				break;

			case TRANSFER_ROW_FULL_LINE_2:
				return "TRANSFER_ROW_FULL_LINE_2";
				break;

			case TRANSFER_ROW_REVERSE_HALF:
				return "TRANSFER_ROW_REVERSE_HALF";
				break;

			case TRANSFER_ROW_REVERSE_QUARTER:
				return "TRANSFER_ROW_REVERSE_QUARTER";
				break;

			case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
				return "TRANSFER_ROW_TWO_INTO_ONE_MIDDLE";
				break;

			case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
				return "TRANSFER_ROW_ONE_INTO_TWO_MIDDLE";
				break;

			case TRANSFER_ROW_FULL_LINE_3:
				return "TRANSFER_ROW_FULL_LINE_3";
				break;

			case TRANSFER_ROW_ONE_INTO_TWO_TOP:
			case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
			case TRANSFER_ROW_TWO_INTO_ONE_TOP:
			case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
				return "Split row";
				break;

			default:
				return sys_getString("Unknown Row Line [ %i ]", rowType);
				break;
		}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Return the transfer bitmap width
float trn_getTransferBitmapWidth ()
//-------------------------------------------------------------------------------------------------------------------
{
	return transferBitmapWidth;
}

//-------------------------------------------------------------------------------------------------------------------
//
// Return the transfer bitmap height
float trn_getTransferBitmapHeight ()
//-------------------------------------------------------------------------------------------------------------------
{
	return transferBitmapHeight;
}

//-------------------------------------------------------------------------------------------------------------------
//
// Return the bitmap for the tranfer game screen
ALLEGRO_BITMAP *trn_getTransferScreen ()
//-------------------------------------------------------------------------------------------------------------------
{
	return transferGameBitmap;
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render a transfer row bar
void trn_renderRowBar (float startX, float middleY, ALLEGRO_COLOR barColor)
//-------------------------------------------------------------------------------------------------------------------
{
	middleY++;

	al_draw_filled_rectangle (startX, middleY - transferRowHeight, startX + (BLOCK_WIDTH * 2), (middleY - 1) + transferRowHeight, barColor);

	al_draw_rectangle (startX, middleY - transferRowHeight, startX + (BLOCK_WIDTH * 2), (middleY - 1) + transferRowHeight + transferBorderThickness, transferColorBorder, 2.0f);
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render a token
void trn_renderToken (int tokenDirection, float startX, float startY, ALLEGRO_COLOR tokenColor)
//-------------------------------------------------------------------------------------------------------------------
{

	if (tokenDirection == TOKEN_DIRECTION_RIGHT)
		{
			al_draw_filled_rectangle (startX, startY, startX + BLOCK_WIDTH, startY + BLOCK_HEIGHT, tokenColor);

			al_draw_filled_rectangle (startX + BLOCK_WIDTH, startY + BLOCK_STEP, startX + (BLOCK_WIDTH * 2), startY + (BLOCK_HEIGHT - BLOCK_STEP), tokenColor);

			al_draw_rectangle (startX, startY, startX + BLOCK_WIDTH, startY + BLOCK_HEIGHT, transferColorBorder, 2.0f);

			al_draw_rectangle (startX + BLOCK_WIDTH, startY + BLOCK_STEP, startX + (BLOCK_WIDTH * 2), startY + (BLOCK_HEIGHT - BLOCK_STEP), transferColorBorder, 2.0f);
		}

	if (tokenDirection == TOKEN_DIRECTION_LEFT)
		{

			al_draw_filled_rectangle (startX, startY, startX - BLOCK_WIDTH, startY + BLOCK_HEIGHT, tokenColor);

			al_draw_filled_rectangle (startX - BLOCK_WIDTH, startY + BLOCK_STEP, startX - (BLOCK_WIDTH * 2), startY + (BLOCK_HEIGHT - BLOCK_STEP), tokenColor);

			al_draw_rectangle (startX, startY, startX - BLOCK_WIDTH, startY + BLOCK_HEIGHT, transferColorBorder, 2.0f);

			al_draw_rectangle (startX - BLOCK_WIDTH, startY + BLOCK_STEP, startX - (BLOCK_WIDTH * 2), startY + (BLOCK_HEIGHT - BLOCK_STEP), transferColorBorder, 2.0f);

		}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render the types of lines for each row
void trn_renderLineForRow (__transferRow whichRow, float lineLength, int special)
//-------------------------------------------------------------------------------------------------------------------
{
	float leftLineStartX  = 0.0f;
	float rightLineStartX = 0.0f;
	float lineStartY      = 0.0f;

	leftLineStartX  = transferSidebarGap + transferSidebarWidth;
	rightLineStartX = transferBitmapWidth - (transferSidebarGap + transferSidebarWidth);
	lineStartY      = whichRow.startY + ((transferRowHeight - transferLineThickness) / 2);

	if (TRANSFER_ROW_LAUNCHER_LEFT == special)
		{
			lineStartY -= transferRowHeight;
			al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + (lineLength / 4), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength / 4), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorBorder);
			return;
		}

	if (TRANSFER_ROW_LAUNCHER_RIGHT == special)
		{
			lineStartY -= transferRowHeight;
			al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - (lineLength / 4), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength / 4), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorBorder);
			return;
		}

	switch (whichRow.leftSideType)
		{
			case TRANSFER_ROW_FULL_LINE:
			case TRANSFER_ROW_FULL_LINE_1:
			case TRANSFER_ROW_FULL_LINE_2:
			case TRANSFER_ROW_FULL_LINE_3:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder);
			break;

			case TRANSFER_ROW_HALF_LINE:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + (lineLength / 2), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength / 2), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_3_4_LINE:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.75f), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.75f), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_QUARTER_LINE:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_REPEAT_HALF:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength / 2), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_REPEAT_QUARTER:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength * 0.25f), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_REVERSE_HALF:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength / 2), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_REVERSE_QUARTER:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength * 0.25f), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
				al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder);
			al_draw_filled_rectangle (leftLineStartX + (lineLength / 2), lineStartY, leftLineStartX + (lineLength), lineStartY + transferLineThickness, transferColorBorder);
			//
			// Top Line
			al_draw_filled_rectangle (leftLineStartX, lineStartY - transferRowHeight, leftLineStartX + (lineLength / 2), (lineStartY - transferRowHeight) + transferLineThickness, transferColorBorder);
			//
			// Bottom Line
			al_draw_filled_rectangle (leftLineStartX, lineStartY + transferRowHeight, leftLineStartX + (lineLength / 2), (lineStartY + transferRowHeight) + transferLineThickness, transferColorBorder);
			trn_renderRowBar (leftLineStartX + (lineLength / 2), lineStartY, transferColorLeft);
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
				//
				// Top Line
				al_draw_filled_rectangle (leftLineStartX + (lineLength * 0.5f), lineStartY - transferRowHeight, leftLineStartX + (lineLength * 0.5f) + (lineLength * 0.25f) + (BLOCK_WIDTH * 4), (lineStartY - transferRowHeight) + transferLineThickness, transferColorBorder);
			//
			// Bottom line
			al_draw_filled_rectangle (leftLineStartX + (lineLength * 0.5f), lineStartY + transferRowHeight, leftLineStartX + (lineLength * 0.5f) + (lineLength * 0.5f), (lineStartY + transferRowHeight) + transferLineThickness, transferColorBorder);
			//
			// Middle line
			al_draw_filled_rectangle (leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorBorder);

			trn_renderRowBar (leftLineStartX + (lineLength * 0.5f), lineStartY, transferColorLeft);

			al_draw_filled_rectangle (leftLineStartX, lineStartY - transferRowHeight, leftLineStartX + (lineLength * 0.25f), (lineStartY - transferRowHeight) + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), (lineStartY - transferRowHeight) - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);

			al_draw_filled_rectangle (leftLineStartX, lineStartY + transferRowHeight, leftLineStartX + (lineLength * 0.25f), (lineStartY + transferRowHeight) + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), (lineStartY + transferRowHeight) - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);

			break;

			default:
//				printf ("Unknown left row [ %f ] value [ %s ]\n", whichRow.startY / transferRowHeight, trn_getRowName (whichRow.leftSideType).c_str());
			break;
		}

	switch (whichRow.rightSideType)
		{
			case TRANSFER_ROW_FULL_LINE:
			case TRANSFER_ROW_FULL_LINE_1:
			case TRANSFER_ROW_FULL_LINE_2:
			case TRANSFER_ROW_FULL_LINE_3:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder);
			break;

			case TRANSFER_ROW_HALF_LINE:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - (lineLength / 2), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength / 2) - transferBorderThickness, lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_3_4_LINE:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - (lineLength * 0.75f), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.75f) - transferBorderThickness, lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_QUARTER_LINE:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_REPEAT_HALF:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength / 2) - transferBorderThickness, lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_REPEAT_QUARTER:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_REVERSE_HALF:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength / 2) - transferBorderThickness, lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_REVERSE_QUARTER:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorLeft);
			break;

			case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
				al_draw_filled_rectangle (rightLineStartX, lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder);
			al_draw_filled_rectangle (rightLineStartX - (lineLength / 2), lineStartY, rightLineStartX - (lineLength), lineStartY + transferLineThickness, transferColorBorder);
			//
			// Top Line
			al_draw_filled_rectangle (rightLineStartX, lineStartY - transferRowHeight, rightLineStartX - (lineLength / 2), (lineStartY - transferRowHeight) + transferLineThickness, transferColorBorder);
			//
			// Bottom Line
			al_draw_filled_rectangle (rightLineStartX, lineStartY + transferRowHeight, rightLineStartX - (lineLength / 2), (lineStartY + transferRowHeight) + transferLineThickness, transferColorBorder);
			trn_renderRowBar (rightLineStartX - (lineLength / 2), lineStartY, transferColorRight);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f), lineStartY - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);
			break;

			case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
				//
				// Top Line
				al_draw_filled_rectangle (rightLineStartX - lineLength + (BLOCK_WIDTH * 6), lineStartY - transferRowHeight, rightLineStartX - (lineLength * 0.5f), (lineStartY - transferRowHeight) + transferLineThickness, transferColorBorder);
			//
			// Bottom line
			al_draw_filled_rectangle (rightLineStartX - lineLength, lineStartY + transferRowHeight, rightLineStartX - (lineLength * 0.5f), (lineStartY + transferRowHeight) + transferLineThickness, transferColorBorder);
			//
			// Middle line
			al_draw_filled_rectangle (rightLineStartX - (lineLength * 0.5f), lineStartY, rightLineStartX, lineStartY + transferLineThickness, transferColorBorder);

			trn_renderRowBar (rightLineStartX - (lineLength * 0.5f), lineStartY, transferColorRight);

			al_draw_filled_rectangle (rightLineStartX, lineStartY - transferRowHeight, rightLineStartX - (lineLength * 0.25f), (lineStartY - transferRowHeight) + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, (lineStartY - transferRowHeight) - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);

			al_draw_filled_rectangle (rightLineStartX, lineStartY + transferRowHeight, rightLineStartX - (lineLength * 0.25f), (lineStartY + transferRowHeight) + transferLineThickness, transferColorBorder);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, (lineStartY + transferRowHeight) - (transferStatusTabHeight / 2) + (transferBorderThickness / 2), transferColorRight);

			break;

			default:
//				printf("Unknown right row value [ %i ]\n", whichRow.rightSideType);
				break;
		}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render the current transfer game elements
void trn_renderTransferGame ()
//-------------------------------------------------------------------------------------------------------------------
{
	ALLEGRO_COLOR rowColor;

	if (transferRows.empty ())
		return;

	al_set_target_bitmap (transferGameBitmap);
	al_clear_to_color (al_map_rgb_f (0, 0, 0));

	al_draw_filled_rectangle (transferBackgroundStartX, transferBackgroundStartY,
	                          transferBackgroundStartX + transferBackgroundWidth, transferBackgroundStartY + transferBackgroundHeight,
	                          transferColorBackground);

	for (auto transferIndex : transferRows)
		{
			trn_renderLineForRow (transferIndex, gapWidth * 2, 0);
			trn_renderToken (TOKEN_DIRECTION_RIGHT, transferIndex.startX + transferRowCellWidth - (BLOCK_WIDTH * 3) - transferBorderThickness, transferIndex.startY + 4, transferColorRight);
			trn_renderToken (TOKEN_DIRECTION_LEFT, transferIndex.startX - (BLOCK_WIDTH * 4), transferIndex.startY + 4, transferColorLeft);

			if (transferIndex.currentColor == TRANSFER_COLOR_LEFT)
				{
					rowColor = transferColorLeft;
				}
			else
				{
					rowColor = transferColorRight;
				}

			al_draw_filled_rectangle (transferIndex.startX - (transferRowCellWidth / 2), transferIndex.startY,
			                          transferIndex.startX + (transferRowCellWidth / 2),
			                          transferIndex.startY + transferRowHeight,
			                          rowColor);

			al_draw_rectangle (transferIndex.startX - (transferRowCellWidth / 2), transferIndex.startY,
			                   transferIndex.startX + (transferRowCellWidth / 2), transferIndex.startY + transferRowHeight,
			                   transferColorBorder, transferBorderThickness);
		}
	//
	// Render the status cell
	al_draw_filled_rectangle (transferRows[0].startX - (transferRowCellWidth / 2), transferRows[0].startY - (transferRowHeight * 2),
	                          transferRows[0].startX + (transferRowCellWidth / 2), transferRows[0].startY,
	                          transferColorStatusCell);

	al_draw_rectangle (transferRows[0].startX - (transferRowCellWidth / 2), transferRows[0].startY - (transferRowHeight * 2),
	                   transferRows[0].startX + (transferRowCellWidth / 2), transferRows[0].startY,
	                   transferColorBorder, transferBorderThickness);

	al_draw_rectangle (transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth, transferRows[0].startY - (transferRowHeight * 2) - 2,
	                   transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth + transferStatusTabWidth, transferRows[0].startY - (transferRowHeight * 2),
	                   transferColorBorder, transferBorderThickness);
	//
	// Draw tabs around status cell
	al_draw_rectangle (transferRows[0].startX - (transferRowCellWidth / 2) - transferBorderThickness,
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2),
	                   transferRows[0].startX - (transferRowCellWidth / 2),
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferStatusTabHeight,
	                   transferColorBorder, transferBorderThickness);

	al_draw_rectangle (transferRows[0].startX - (transferRowCellWidth / 2) - transferBorderThickness,
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferRowHeight,
	                   transferRows[0].startX - (transferRowCellWidth / 2),
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferStatusTabHeight + transferRowHeight,
	                   transferColorBorder, transferBorderThickness);

	al_draw_rectangle (transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth,
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2),
	                   transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth + transferBorderThickness,
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferStatusTabHeight,
	                   transferColorBorder, transferBorderThickness);

	al_draw_rectangle (transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth,
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferRowHeight,
	                   transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth + transferBorderThickness,
	                   transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferStatusTabHeight + transferRowHeight,
	                   transferColorBorder, transferBorderThickness);

	al_draw_rectangle (transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth,
	                   transferRows[transferRows.size () - 1].startY + (transferRowHeight),
	                   transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth + transferStatusTabWidth,
	                   transferRows[transferRows.size () - 1].startY + (transferRowHeight) + transferBorderThickness,
	                   transferColorBorder, transferBorderThickness);

	//
	// Draw left bar
	al_draw_filled_rectangle (transferSidebarGap, transferRows[0].startY - (transferRowHeight * 2),
	                          transferSidebarGap + transferSidebarWidth, transferRows[transferRows.size () - 1].startY + transferRowHeight,
	                          transferColorLeft);

	al_draw_rectangle (transferSidebarGap, transferRows[0].startY - (transferRowHeight * 2),
	                   transferSidebarGap + transferSidebarWidth, transferRows[transferRows.size () - 1].startY + transferRowHeight,
	                   transferColorBorder, transferBorderThickness);
	//
	// Draw right bar
	al_draw_filled_rectangle (transferBitmapWidth - (transferSidebarGap + transferSidebarWidth), transferRows[0].startY - (transferRowHeight * 2),
	                          transferBitmapWidth - transferSidebarGap, transferRows[transferRows.size () - 1].startY + transferRowHeight,
	                          transferColorRight);

	al_draw_rectangle (transferBitmapWidth - (transferSidebarGap + transferSidebarWidth), transferRows[0].startY - (transferRowHeight * 2),
	                   transferBitmapWidth - transferSidebarGap, transferRows[transferRows.size () - 1].startY + transferRowHeight,
	                   transferColorBorder, transferBorderThickness);
	//
	// Draw left side launchpad
	trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_LEFT);
	//
	// Draw right side launchpad
	trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_RIGHT);
	//
	// Draw the two sprite types
	float droidX, droidY;
	gapWidth = ((transferRows[0].startX - (transferSidebarGap + transferSidebarWidth)) / 2);

	if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
		{
			droidX = (transferSidebarGap + transferSidebarWidth) + gapWidth;
			droidY = transferRows[0].startY - (transferRowHeight * 2);

			for (int i = 0; i != dataBaseEntry[playerDroid.droidType].tokenCount; i++)
				{
					trn_renderToken (TOKEN_DIRECTION_RIGHT, transferSidebarGap - (BLOCK_WIDTH * 3), transferRows[i].startY, transferColorLeft);
				}

			io_renderSpriteFrame (gam_getSpriteName (playerDroid.droidType), 0, droidX, droidY);

			droidX = (transferBitmapWidth - (transferSidebarGap + transferSidebarWidth)) - gapWidth;
			//
			// TODO Remove
			playerDroid.transferTargetDroidType = 9;

			for (int i = 0; i != dataBaseEntry[playerDroid.transferTargetDroidType].tokenCount; i++)
				{
					trn_renderToken (TOKEN_DIRECTION_LEFT, transferBitmapWidth - (BLOCK_WIDTH * 3), transferRows[i].startY, transferColorRight);
				}

			io_renderTintedSpriteFrame (gam_getSpriteName (gam_getTransferTargetDroid ()), 0, droidX, droidY,
			                            0.0, 0.0, 0.0, 1.0);
		}
	else
		{
			droidX = (transferSidebarGap + transferSidebarWidth) + gapWidth;
			droidY = transferRows[0].startY - (transferRowHeight * 2);

			//
			// TODO Remove
			playerDroid.transferTargetDroidType = 9;

			for (int i = 0; i != dataBaseEntry[playerDroid.transferTargetDroidType].tokenCount; i++)
				{
					trn_renderToken (TOKEN_DIRECTION_RIGHT, transferSidebarGap - (BLOCK_WIDTH * 3), transferRows[i].startY, transferColorLeft);
				}

			io_renderTintedSpriteFrame (gam_getSpriteName (gam_getTransferTargetDroid ()), 0, droidX, droidY,
			                            0.0, 0.0, 0.0, 1.0);

			droidX = (transferBitmapWidth - (transferSidebarGap + transferSidebarWidth)) - gapWidth;

			io_renderSpriteFrame (gam_getSpriteName (playerDroid.droidType), 0, droidX, droidY);

			for (int i = 0; i != dataBaseEntry[playerDroid.droidType].tokenCount; i++)
				{
					trn_renderToken (TOKEN_DIRECTION_LEFT, transferBitmapWidth - (BLOCK_WIDTH * 3), transferRows[i].startY, transferColorRight);
				}
		}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Set a color for the transfer game
void trn_setTransferColor (int whichSide, float red, float green, float blue, float alpha)
//-------------------------------------------------------------------------------------------------------------------
{
	switch (whichSide)
		{
			case TRANSFER_COLOR_LEFT:
				transferColorLeft = al_map_rgba_f (red, green, blue, alpha);
			break;

			case TRANSFER_COLOR_RIGHT:
				transferColorRight = al_map_rgba_f (red, green, blue, alpha);
			break;

			case TRANSFER_COLOR_BACKGROUND:
				transferColorBackground = al_map_rgba_f (red, green, blue, alpha);
			break;

			case TRANSFER_COLOR_BORDER:
				transferColorBorder = al_map_rgba_f (red, green, blue, alpha);
			break;
		}
}
//-------------------------------------------------------------------------------------------------------------------
//
// Setup the default values for the transfer rows
void trn_initTransferValues ()
//-------------------------------------------------------------------------------------------------------------------
{
	__transferRow tempTransferRow;

	if (transferRows.empty ())
		{
			for (int i = 0; i != numberTransferRows; i++)
				{
					tempTransferRow.startX                 = transferBitmapWidth / 2;
					tempTransferRow.startY                 = transferRowStartY + (i * transferRowHeight);
					tempTransferRow.leftSideActiveCounter  = 0.0f;
					tempTransferRow.rightSideActiveCounter = 0.0f;
					tempTransferRow.leftSideType           = TRANSFER_ROW_REVERSE_QUARTER;
					tempTransferRow.rightSideType          = TRANSFER_ROW_REVERSE_QUARTER;

					if (i % 2 == 0)
						tempTransferRow.currentColor = TRANSFER_COLOR_LEFT;
					else
						tempTransferRow.currentColor = TRANSFER_COLOR_RIGHT;

					transferRows.push_back (tempTransferRow);
				}

			transferGameBitmap = al_create_bitmap (transferBitmapWidth, transferBitmapHeight);
			if (nullptr == transferGameBitmap)
				log_logMessage (LOG_LEVEL_EXIT, sys_getString ("Unable to create backing bitmap for transfer game."));

			al_set_target_bitmap (transferGameBitmap);

//					al_set_target_bitmap(al_get_backbuffer(display));
		}

	transferColorStatusCell = transferColorBorder;
	transferPlayerWhichSide = TRANSFER_COLOR_LEFT;

	setupTransferCellValues ();

//	transferRows[0].leftSideType = TRANSFER_ROW_HALF_LINE;
//	transferRows[1].leftSideType = TRANSFER_ROW_ONE_INTO_TWO_TOP;
//	transferRows[2].leftSideType = TRANSFER_ROW_ONE_INTO_TWO_MIDDLE;
//	transferRows[3].leftSideType = TRANSFER_ROW_ONE_INTO_TWO_BOTTOM;

	transferRows[0].rightSideType = TRANSFER_ROW_HALF_LINE;
	transferRows[1].rightSideType = TRANSFER_ROW_ONE_INTO_TWO_TOP;
	transferRows[2].rightSideType = TRANSFER_ROW_ONE_INTO_TWO_MIDDLE;
	transferRows[3].rightSideType = TRANSFER_ROW_ONE_INTO_TWO_BOTTOM;

}
