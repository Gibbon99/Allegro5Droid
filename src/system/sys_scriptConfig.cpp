#include <hdr/io/io_resources.h>
#include <hdr/io/io_resourceLevel.h>
#include <hdr/system/sys_init.h>
#include <hdr/gui/gui_text.h>
#include <hdr/io/io_resourceSprite.h>
#include <hdr/game/gam_droidAIPatrol.h>
#include <hdr/game/gam_player.h>
#include <hdr/game/gam_doors.h>
#include <hdr/gui/gui_sideView.h>
#include <hdr/game/gam_droids.h>
#include <hdr/game/gam_droidAI.h>
#include <hdr/game/gam_bullet.h>
#include "hdr/system/sys_scriptConfig.h"
#include <hdr/game/gam_healing.h>
#include <hdr/system/sys_gameFrameRender.h>
#include <hdr/gui/gui_scrollBox.h>
#include <hdr/gui/gui_checkBox.h>
#include <hdr/io/io_resourceImage.h>
#include <hdr/gui/gui_dialogBox.h>
#include <hdr/system/sys_timerFunctions.h>
#include <hdr/game/gam_terminal.h>
#include <hdr/gui/gui_database.h>
#include <hdr/game/gam_database.h>
#include <hdr/game/gam_particles.h>
#include <hdr/system/sys_audio.h>
#include <hdr/game/gam_game.h>
#include <hdr/game/gam_transferRender.h>
#include <hdr/game/gam_hud.h>
#include <hdr/system/sys_eventsEngine.h>
#include <hdr/game/gam_transfer.h>
#include <hdr/game/gam_transferDroidAI.h>
#include <hdr/system/sys_configFile.h>
#include <hdr/gui/gui_slider.h>
#include <hdr/game/gam_render.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the global variables to be shared between host and scripts
void sys_scriptInitVariables ()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_scriptAddHostVariable ("int quitProgram", &quitProgram);
	sys_scriptAddHostVariable ("bool initResourcesAllDone", &initResourcesAllDone);
	sys_scriptAddHostVariable ("int currentLanguage", &currentLanguage);
	sys_scriptAddHostVariable ("float wayPointDestinationSize", &wayPointDestinationSize);
	sys_scriptAddHostVariable ("float shipDamping", &shipDamping);
	sys_scriptAddHostVariable ("float playerRadius", &playerRadius);
	sys_scriptAddHostVariable ("float playerFriction", &playerFriction);
	sys_scriptAddHostVariable ("float playerElastic", &playerElastic);
	sys_scriptAddHostVariable ("float pixelsPerMeter", &pixelsPerMeter);
	sys_scriptAddHostVariable ("float playerAcceleration", &playerAcceleration);
	sys_scriptAddHostVariable ("float playerMaxSpeed", &playerMaxSpeed);
	sys_scriptAddHostVariable ("float gravity", &gravity);
	sys_scriptAddHostVariable ("float doorAnimSpeed", &doorAnimSpeed);
	sys_scriptAddHostVariable ("float sideviewDrawScaleScript", &sideviewDrawScaleScript);
	sys_scriptAddHostVariable ("float droidAnimateSpeed", &droidAnimateSpeed);
	sys_scriptAddHostVariable ("float badHealthFactor", &badHealthFactor);
	sys_scriptAddHostVariable ("int collisionExplosionDamage", &collisionExplosionDamage);
	sys_scriptAddHostVariable ("int collisionDamageInflicted", &collisionDamageInflicted);
	sys_scriptAddHostVariable ("float explodeAnimateSpeed", &explodeAnimateSpeed);
	sys_scriptAddHostVariable ("float bulletAnimSpeed", &bulletAnimSpeed);
	sys_scriptAddHostVariable ("float bulletMoveSpeed", &bulletMoveSpeed);
	sys_scriptAddHostVariable ("float bulletDensity", &bulletDensity);
	sys_scriptAddHostVariable ("float healingAnimSpeed", &healingAnimSpeed);
	sys_scriptAddHostVariable ("bool showDebugPhysics", &showDebugPhysics);
	sys_scriptAddHostVariable ("int logicalWidth", &screenWidth);
	sys_scriptAddHostVariable ("int logicalHeight", &screenHeight);
	sys_scriptAddHostVariable ("float droidBeenShotValue", &droidBeenShotValue);
	sys_scriptAddHostVariable ("int splashTimeout", &splashTimeout);

	sys_scriptAddHostVariable ("int currentGUIScreen", &currentGUIScreen);
	sys_scriptAddHostVariable ("int currentObjectSelected", &currentObjectSelected);
	sys_scriptAddHostVariable ("bool isGUIStarted", &isGUIStarted);
	sys_scriptAddHostVariable ("bool fullScreen", &fullScreen);
	sys_scriptAddHostVariable ("int screenType", &screenType);

	sys_scriptAddHostVariable ("float introScrollBoxStartX", &introScrollBoxStartX);
	sys_scriptAddHostVariable ("float introScrollBoxStartY", &introScrollBoxStartY);
	sys_scriptAddHostVariable ("float introScrollBoxWidth", &introScrollBoxWidth);
	sys_scriptAddHostVariable ("float introScrollBoxHeight", &introScrollBoxHeight);
	sys_scriptAddHostVariable ("float introScrollBoxSpeed", &introScrollBoxSpeed);
	sys_scriptAddHostVariable ("float introScrollBoxGapSize", &introScrollBoxGapSize);
	sys_scriptAddHostVariable ("float introScrollBoxBackRed", &introScrollBoxBackRed);
	sys_scriptAddHostVariable ("float introScrollBoxBackGreen", &introScrollBoxBackGreen);
	sys_scriptAddHostVariable ("float introScrollBoxBackBlue", &introScrollBoxBackBlue);
	sys_scriptAddHostVariable ("float introScrollBoxBackAlpha", &introScrollBoxBackAlpha);
	sys_scriptAddHostVariable ("float introScrollBoxRadius", &introScrollBoxRadius);
	sys_scriptAddHostVariable ("float introScrollBoxFontRed", &introScrollBoxFontRed);
	sys_scriptAddHostVariable ("float introScrollBoxFontGreen", &introScrollBoxFontGreen);
	sys_scriptAddHostVariable ("float introScrollBoxFontBlue", &introScrollBoxFontBlue);
	sys_scriptAddHostVariable ("float introScrollBoxFontAlpha", &introScrollBoxFontAlpha);

	sys_scriptAddHostVariable ("float databaseScrollBoxStartX", &databaseScrollBoxStartX);
	sys_scriptAddHostVariable ("float databaseScrollBoxStartY", &databaseScrollBoxStartY);
	sys_scriptAddHostVariable ("float databaseScrollBoxWidth", &databaseScrollBoxWidth);
	sys_scriptAddHostVariable ("float databaseScrollBoxHeight", &databaseScrollBoxHeight);
	sys_scriptAddHostVariable ("float databaseScrollBoxSpeed", &databaseScrollBoxSpeed);
	sys_scriptAddHostVariable ("float databaseScrollBoxGapSize", &databaseScrollBoxGapSize);
	sys_scriptAddHostVariable ("float databaseScrollBoxBackRed", &databaseScrollBoxBackRed);
	sys_scriptAddHostVariable ("float databaseScrollBoxBackGreen", &databaseScrollBoxBackGreen);
	sys_scriptAddHostVariable ("float databaseScrollBoxBackBlue", &databaseScrollBoxBackBlue);
	sys_scriptAddHostVariable ("float databaseScrollBoxBackAlpha", &databaseScrollBoxBackAlpha);
	sys_scriptAddHostVariable ("float databaseScrollBoxRadius", &databaseScrollBoxRadius);
	sys_scriptAddHostVariable ("float databaseScrollBoxFontRed", &databaseScrollBoxFontRed);
	sys_scriptAddHostVariable ("float databaseScrollBoxFontGreen", &databaseScrollBoxFontGreen);
	sys_scriptAddHostVariable ("float databaseScrollBoxFontBlue", &databaseScrollBoxFontBlue);
	sys_scriptAddHostVariable ("float databaseScrollBoxFontAlpha", &databaseScrollBoxFontAlpha);
	sys_scriptAddHostVariable ("float databaseAnimTime", &databaseAnimTime);

	sys_scriptAddHostVariable ("float droidBeenShotValue", &droidBeenShotValue);
	sys_scriptAddHostVariable ("float witnessShootValue", &witnessShootValue);
	sys_scriptAddHostVariable ("float witnessTransferValue", &witnessTransferValue);
	sys_scriptAddHostVariable ("float ai_beenShot", &ai_beenShot);
	sys_scriptAddHostVariable ("float ai_healthAmount", &ai_healthAmount);
	sys_scriptAddHostVariable ("float ai_playerVisible", &ai_playerVisible);
	sys_scriptAddHostVariable ("float ai_witnessShoot", &ai_witnessShoot);
	sys_scriptAddHostVariable ("float ai_witnessTransfer", &ai_witnessTransfer);
	sys_scriptAddHostVariable ("float ai_greenFactor", &ai_greenFactor);
	sys_scriptAddHostVariable ("float ai_yellowFactor", &ai_yellowFactor);
	sys_scriptAddHostVariable ("float ai_redFactor", &ai_redFactor);

	sys_scriptAddHostVariable ("float deckViewRatio", &deckViewRatio);
	sys_scriptAddHostVariable ("int numStartingBullets", &numStartingBullets);
	sys_scriptAddHostVariable ("bool useHoldBitmap", &useHoldBitmap);
	sys_scriptAddHostVariable ("bool profileParticles", &profileParticles);
	sys_scriptAddHostVariable ("bool profileParticles", &profileParticles);
	sys_scriptAddHostVariable ("bool profileParticles", &profileParticles);
	sys_scriptAddHostVariable ("bool profileParticles", &profileParticles);
	sys_scriptAddHostVariable ("int particleRenderType", &particleRenderType);
	sys_scriptAddHostVariable ("int particleVelocityExplosion", &particleVelocityExplosion);
	sys_scriptAddHostVariable ("int particleVelocityExplosionAdd", &particleVelocityExplosionAdd);
	sys_scriptAddHostVariable ("int particleVelocitySpark", &particleVelocitySpark);
	sys_scriptAddHostVariable ("int particleVelocitySparkAdd", &particleVelocitySparkAdd);
	sys_scriptAddHostVariable ("int particleNumberExplosion", &particleNumberExplosion);
	sys_scriptAddHostVariable ("int particleNumberExplosionAdd", &particleNumberExplosionAdd);
	sys_scriptAddHostVariable ("int particleNumberTrail", &particleNumberTrail);
	sys_scriptAddHostVariable ("int particleNumberTrailAdd", &particleNumberTrailAdd);
	sys_scriptAddHostVariable ("int particleTrailAlphaReset", &particleTrailAlphaReset);
	sys_scriptAddHostVariable ("int particleTrailAlphaResetAdd", &particleTrailAlphaResetAdd);
	sys_scriptAddHostVariable ("int particleNumberSpark", &particleNumberSpark);
	sys_scriptAddHostVariable ("int particleNumberSparkAdd", &particleNumberSparkAdd);
	sys_scriptAddHostVariable ("float particleExplosionReduce", &particleExplosionReduce);
	sys_scriptAddHostVariable ("float particleSparkReduce", &particleSparkReduce);
	sys_scriptAddHostVariable ("float particleTrailReduce", &particleTrailReduce);
	sys_scriptAddHostVariable ("int particleTrailLimit", &particleTrailLimit);
	sys_scriptAddHostVariable ("float particleMass", &particleMass);
	sys_scriptAddHostVariable ("float particleFriction", &particleFriction);
	sys_scriptAddHostVariable ("float particleElastic", &particleElastic);
	sys_scriptAddHostVariable ("float particleSize", &particleSize);

	sys_scriptAddHostVariable ("int particleBlendSrc", &particleBlendSrc);
	sys_scriptAddHostVariable ("int particleBlendDst", &particleBlendDst);

	sys_scriptAddHostVariable ("float scoreDelay", &scoreDelay);
	sys_scriptAddHostVariable ("int yellowAlertLevel", &yellowAlertLevel);
	sys_scriptAddHostVariable ("int redAlertLevel", &redAlertLevel);

	sys_scriptAddHostVariable ("int numberTransferRows", &numberTransferRows);
	sys_scriptAddHostVariable ("float transferRowHeight", &transferRowHeight);
	sys_scriptAddHostVariable ("float transferRowStartY", &transferRowStartY);
	sys_scriptAddHostVariable ("float transferRowCellWidth", &transferRowCellWidth);

	sys_scriptAddHostVariable ("float transferBackgroundStartX", &transferBackgroundStartX);
	sys_scriptAddHostVariable ("float transferBackgroundStartY", &transferBackgroundStartY);
	sys_scriptAddHostVariable ("float transferBackgroundWidth", &transferBackgroundWidth);
	sys_scriptAddHostVariable ("float transferBackgroundHeight", &transferBackgroundHeight);
	sys_scriptAddHostVariable ("int transferBorderThickness", &transferBorderThickness);
	sys_scriptAddHostVariable ("float transferStatusTabWidth", &transferStatusTabWidth);
	sys_scriptAddHostVariable ("float transferStatusTabHeight", &transferStatusTabHeight);
	sys_scriptAddHostVariable ("float transferSidebarGap", &transferSidebarGap);
	sys_scriptAddHostVariable ("float transferSidebarWidth", &transferSidebarWidth);
	sys_scriptAddHostVariable ("float transferLineThickness", &transferLineThickness);
	sys_scriptAddHostVariable ("int transferBitmapWidth", &transferBitmapWidth);
	sys_scriptAddHostVariable ("int transferBitmapHeight", &transferBitmapHeight);
	sys_scriptAddHostVariable ("int chooseSideTimeOut", &chooseSideTimeOut);
	sys_scriptAddHostVariable ("float chooseSideDelayTime", &chooseSideDelayTime);
	sys_scriptAddHostVariable ("float chooseRowDelayTime", &chooseRowDelayTime);
	sys_scriptAddHostVariable ("float transferImageFade", &transferImageFade);
	sys_scriptAddHostVariable ("bool renderBackdrop", &renderBackdrop);

	sys_scriptAddHostVariable ("bool enableSound", &enableSound);
	sys_scriptAddHostVariable ("int numDisrupterFrames", &numDisrupterFrames);
	sys_scriptAddHostVariable ("float disrupterFadeAmount", &disrupterFadeAmount);

	sys_scriptAddHostVariable ("string volumeLevelStr", &volumeLevelStr);
	sys_scriptAddHostVariable ("int volumeLevel", &volumeLevel);

	sys_scriptAddHostVariable ("string tileType", &tileType);
	sys_scriptAddHostVariable ("string tileColor", &tileColor);

	sys_scriptAddHostVariable ("int currentObjectSelectedDialog", &currentObjectSelectedDialog);
	sys_scriptAddHostVariable ("int currentDialogBox", &currentDialogBox);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the globals functions that the scripts can call for action in the host
void sys_scriptInitFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_scriptAddHostFunction ("void sys_printConInt(string &in, uint param)", (functionPtr) &sys_scriptPrintInt);
	sys_scriptAddHostFunction ("void sys_loadResource(string &in, string &in, int type, int numFrames, int size)", (functionPtr) &sys_loadResource);
	sys_scriptAddHostFunction ("bool lvl_loadShipLevel (string &in)", (functionPtr) &lvl_loadShipLevel);
	sys_scriptAddHostFunction ("void gui_addKeyAndText(string &in, string &in)", (functionPtr) &gui_addKeyAndText);
	sys_scriptAddHostFunction ("string gui_getString(string &in)", (functionPtr) &gui_getString);
	sys_scriptAddHostFunction ("void io_spriteSetAlphaColor(string &in, int red, int green, int blue)", (functionPtr) &io_spriteSetAlphaColor);
	sys_scriptAddHostFunction ("void as_createSideViewColor  (int index, int red, int green, int blue, int alpha)", (functionPtr) &gui_createSideViewColor);

	sys_scriptAddHostFunction ("void as_guiCreateObject	    (int guiObjectType, string &in)", (functionPtr) &gui_hostCreateObject);
	sys_scriptAddHostFunction ("void as_guiSetObjectPosition (int guiObjectType, string &in, int coordType, int startX, int startY, int width, int height)", (functionPtr) &gui_hostSetObjectPosition);
	sys_scriptAddHostFunction ("void as_guiSetObjectLabel    (int guiObjectType, string &in, int labelPos, string &in)", (functionPtr) &gui_hostSetObjectLabel);
	sys_scriptAddHostFunction ("void as_guiAddObjectToScreen (int guiObjectType, string &in, string &in)", (functionPtr) &gui_hostAddObjectToScreen);
	sys_scriptAddHostFunction ("void as_guiSetObjectFunction (int guiObjectType, string &in, string &in)", (functionPtr) &gui_hostSetObjectFunctions);
	sys_scriptAddHostFunction ("void as_guiSetObjectFontName (int guiObjectType, string &in, string &in)", (functionPtr) &gui_hostSetObjectFontName);

	sys_scriptAddHostFunction ("void as_guiSetObjectColor    (int guiObjectType, string &in, int whichColor, float red, float green, float blue, float alpha)", (functionPtr) &gui_hostSetObjectColor);
	sys_scriptAddHostFunction ("void as_guiSetReadyState     (int guiObjectType, string &in, bool newState)", (functionPtr) &gui_hostSetReadyState);
	sys_scriptAddHostFunction ("int as_guiFindIndex		    (int guiObjectType, string &in)", (functionPtr) &gui_findIndex);
	sys_scriptAddHostFunction ("void as_guiCreateNewScreen   (string &in)", (functionPtr) &gui_hostCreateNewScreen);
	sys_scriptAddHostFunction ("void as_guiSetObjectFocus    (string &in)", (functionPtr) &gui_hostSetObjectFocus);
	sys_scriptAddHostFunction ("void gui_addKeyAndText(string &in, string &in)", (functionPtr) &gui_addKeyAndText);
	sys_scriptAddHostFunction ("void as_guiChangeCurrentScreen(int newScreen)", (functionPtr) &gui_changeToGUIScreen);

	sys_scriptAddHostFunction ("void as_guiSetupScrollBox(int whichScrollBox, string &in)", (functionPtr) &gui_setupScrollBox);

	sys_scriptAddHostFunction ("int gam_getTextureWidth(string &in)", (functionPtr) &sys_getImageWidth);
	sys_scriptAddHostFunction ("int gam_getTextureHeight(string &in)", (functionPtr) &sys_getImageHeight);

	sys_scriptAddHostFunction ("void as_guiSetCheckboxGroup  (string &in, int whichGroup)", (functionPtr) &gui_hostSetCheckboxGroup);
	sys_scriptAddHostFunction ("void as_guiSetCheckboxTick   (string &in, int whichGroup, bool ticked)", (functionPtr) &gui_hostSetCheckboxTick);
	sys_scriptAddHostFunction ("void as_guiSetCheckboxValue (string &in, int newValue)", (functionPtr) &gui_hostSetCheckboxValue);
	sys_scriptAddHostFunction ("int as_guiGetCheckboxValue (string &in)", (functionPtr) &gui_hostGetCheckboxValue);

	sys_scriptAddHostFunction ("void as_guiSetSliderValue(string &in, string &in)", (functionPtr) &gui_hostSetSliderValue);
	sys_scriptAddHostFunction ("string as_guiGetSliderValue(string &in)", (functionPtr) &gui_hostGetSliderValue);

	sys_scriptAddHostFunction ("void as_guiAddNewElement (string &in, string &in, string &in, int type)", (functionPtr) &gui_addNewElement);
	sys_scriptAddHostFunction ("void as_guiSetImageKeyName(string &in, string &in)", (functionPtr) &gui_setImageKeyName);
	sys_scriptAddHostFunction ("void sys_changeCurrentMode(int newMode, bool fade)", (functionPtr) &sys_changeMode);

	sys_scriptAddHostFunction ("void gam_enterDatabaseMode()", (functionPtr) &gam_enterDatabaseMode);
	sys_scriptAddHostFunction ("void gam_previousDatabaseDroid()", (functionPtr) &gam_previousDatabaseDroid);
	sys_scriptAddHostFunction ("void gam_nextDatabaseDroid()", (functionPtr) &gam_nextDatabaseDroid);
	sys_scriptAddHostFunction ("void gam_setLocalDroidType()", (functionPtr) &gam_setLocalDroidType);

	sys_scriptAddHostFunction ("void as_setParticleColor(int whichParticle, float red, float green, float blue, float alpha)", (functionPtr) &par_hostSetParticleColor);
	sys_scriptAddHostFunction ("int sys_getCappedRandomNum(int cap)", (functionPtr) &sys_getCappedRandomNum);
	sys_scriptAddHostFunction ("float sys_getLogicalWidth()", (functionPtr) &sys_getLogicalWidth);
	sys_scriptAddHostFunction ("float sys_getLogicalHeight()", (functionPtr) &sys_getLogicalHeight);
	sys_scriptAddHostFunction ("void trn_setTransferColor(int whichSide, float red, float green, float blue, float alpha)", (functionPtr) &trn_setTransferColor);

	sys_scriptAddHostFunction ("void hud_setText(bool useDirectValue, string &in)", (functionPtr) &hud_setText);

	sys_scriptAddHostFunction ("void evt_pushEvent (int delayCount, int eventType, int eventAction, int data1, int data2, string &in)", (functionPtr) &evt_pushEvent);
	sys_scriptAddHostFunction ("void cfg_setConfigValue(string &in, string &in)", (functionPtr) &cfg_setConfigValue);
	sys_scriptAddHostFunction ("string sys_boolToString(bool boolValue)", (functionPtr) &sys_boolToString);
	sys_scriptAddHostFunction ("string sys_intToString(int intValue)", (functionPtr) &sys_intToString);
	sys_scriptAddHostFunction ("int sys_stringToInt(string &in)", (functionPtr) &sys_stringToInt);
	sys_scriptAddHostFunction ("void sys_loadTileBitmap(string &in, string &in)", (functionPtr) &sys_loadTileBitmap);

	sys_scriptAddHostFunction ("void as_guiAddDialogBox (string &in, string &in, string &in, int posX, int posY, bool modal)", (functionPtr) &gui_addDialogBox);
	sys_scriptAddHostFunction ("void as_guiAddObjectToDialog (int guiObjectType, string &in, string &in)", (functionPtr) &gui_hostAddObjectToDialog);
}

//----------------------------------------------------------------------------------------------------------------------
//
// This is how we call a script from the Host program : Name in Script : Name to call from host
void sys_scriptInitScriptFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_scriptAddScriptFunction ("void script_loadAllResources()", "script_loadAllResources");
	sys_scriptAddScriptFunction ("void script_initGUI()", "script_initGUI");
	sys_scriptAddScriptFunction ("void as_guiHandleElementAction(string &in objectID)", "as_guiHandleElementAction");
	sys_scriptAddScriptFunction ("void as_guiHandleTerminalAction(string &in objectID)", "as_guiHandleTerminalAction");
	sys_scriptAddScriptFunction ("void as_guiHandleDatabaseAction(string &in objectID)", "as_guiHandleDatabaseAction");
	sys_scriptAddScriptFunction ("void as_guiHandleTransferAction(string &in objectID)", "as_guiHandleTransferAction");
}
