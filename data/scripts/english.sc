//------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create English text strings
void as_setEnglishText()
//------------------------------------------------------------
{
	gui_addKeyAndText("scrollBoxIntroText",		"  ^ ^       Welcome to Paradroid. ! ! Press [ LEFT CTRL KEY ] to play or wait for further instructions ! Use the [ ARROW KEYS ] to navigate the menus ! Press [ ENTER ] to make a choice ! ! ^ Clear the freighter of robots by destroying them with twin lasers or by transferring control to them. ! ! Control is by keyboard only, as follows: ! ! At all times the keyboard moves the robot and holding fire down will allow use of lifts and consoles. ! ! In addition, pressing fire with no keys pressed will prepare the current robot for transfer. Contact with another robot with the fire key down will initiate transfer. ! ! ! ^ ! ! ! ! A fleet of Robo-Freighters on its way to the Beta Ceti system reported entering an uncharted field of asteroids. Each ship carries a cargo of battle droids to reinforce the outworld defences. ! ! Two distress beacons have been recovered. Similar messages were stored on each. The ships had been bombarded by a powerful radionic beam from one of the asteroids. ! ! All of the robots on the ships, including those in storage, became hyper-active. The crews report an attack by droids, isolating them on the bridge. They cannot reach the shuttle and can hold out for only a couple more hours. ! ! Since these beacons were located two days ago, we can only fear the worst. ! ! Some of the fleet was last seen heading for enemy space. In enemy hands the droids can be used against our forces. ! ! Docking would be impossible, but we can beam aboard a prototype Influence Device. ! ^  ! ! ! The 001 Influence device consists of a helmet, which, when placed over a robot's control unit can halt the normal activities of that robot for a short time. The helmet has its own power supply and powers the robot itself, at an upgraded capability. The helmet also uses an energy cloak for protection of the host. ! ! The helmet is fitted with twin lasers mounted in a turrent. These are low powered and have a slow recycle rate. ! ! Most of the device's resources are channeled towards holding control of the host robot, as it attempts to resume 'normal' operation. It is therefore necessary to change the host robot often to prevent the device from burning out. Transfer to a new robot requires the device to drain its host of energy in order to take it over. Failure to achieve transfer results in the device being a free agent once more. ! ! Further information incoming... ! ^  ! ! ! An Influence device can transmit only certain data, namely its own location and the location of other robots in visual range. This data is merged with known ship layouts on your C64 remote terminal. ! ! Additional information about the ship and robots may be obtained by accessing the ship's computer at a console. A small-scale plan of the whole deck is available, as well as a side elevation of the ship. ! ! Robots are represented on-screen as a symbol showing a three-digit number. The first digit shown is the important one, the class of the robot. It denotes strength also. ! ! To find out more about any given robot, use the robot enquiry system at a console. Only data about units of a lower class than your current host is available, since it is the host's security clearance which is used to access the console. ! ! ! ! ! Further information incoming... ! ^  ! ! ! ! ! Paradroid programmed by Andrew Braybrook. ! ! ! Sound advice by Steve Turner. ! ! ! SDL port by David Berry. ! 2019. ! ! ! ! ! Powered by the SDL2.0 library. ! Scripting engine by AngelScript. ! Physics by Chipmunk Physics. ! Network library by RakNet. ! SimpleINI by Mark E. Davis, 1994. ! Filewatcher by James Wynn 2009. ! ! ! ! Press fire to play. ! ! ! ! ! ! ^ # ");

	gui_addKeyAndText("gameLeft", "Move left");
	gui_addKeyAndText("gameRight", "Move Right");
	gui_addKeyAndText("gameDown", "Move down");
	gui_addKeyAndText("gameUp", "Move up");
	gui_addKeyAndText("gameAction", "Perform an action");
	gui_addKeyAndText("gamePause", "Pause the game");
	gui_addKeyAndText("gameEscape", "Escape");
	gui_addKeyAndText("consoleAction", "Invoke the console");
	gui_addKeyAndText("gameScreenShot", "Take a screenshot");

    gui_addKeyAndText("startGameButton", "Start Game");
    gui_addKeyAndText("optionsButton", "Options");
	gui_addKeyAndText("tutorialButton", "Tutorial");
	gui_addKeyAndText("briefingButton", "Briefing");
	gui_addKeyAndText("exitGameButton", "Exit Game");
	gui_addKeyAndText("mainScreenLabel", "Select your option using mouse or keyboard.");
	gui_addKeyAndText("guiOptionsMainLabel", "OPTIONS");
	gui_addKeyAndText("optionVideoLabel", "VIDEO OPTIONS");
	gui_addKeyAndText("optionsVideoCheckFullScreen", "Fullscreen");
	gui_addKeyAndText("guiOptionsMainVideoButton", "Video");
	gui_addKeyAndText("guiOptionsMainAudioButton", "Audio");
	gui_addKeyAndText("guiOptionsMainGraphicsButton", "Graphics");
	gui_addKeyAndText("guiOptionsMainNetworkButton", "Network");
	gui_addKeyAndText("guiOptionsMainControlsButton", "Controls");

	gui_addKeyAndText("tutMoveLabel", 		"Tutorial - Move");
	gui_addKeyAndText("tutTransferLabel",	"Tutorial - Transfer Mode");
	gui_addKeyAndText("tutTransGameLabel",	"Tutorial - Transfer Game");
	gui_addKeyAndText("tutLiftLabel",		"Tutorial - Lifts");
	gui_addKeyAndText("tutTerminalLabel",	"Tutorial - Terminals");
	gui_addKeyAndText("tutHealingLabel",	"Tutorial - Healing");
	gui_addKeyAndText("tutTipsLabel",		"Tutorial - Gameplay tips");

	gui_addKeyAndText("tutMoveText",		" Use the arrow keys to move. ! ! Pressing the Action Key will shoot in your current movement direction. ! ! Hold down the Action Key when no other keys are pressed to initiate transfer mode.");
	gui_addKeyAndText("tutTransText",		" To take over another droid you enter into Transfer mode. ! ! Press and hold the Action key with no other keys down to enter this mode. Colliding with another droid will initiate the transfer subroutine.");
	gui_addKeyAndText("tutTransGameText",	"To gain control of another droid, you must win the circuit transfer game. Select the side you will use. Press the UP / DOWN keys to move your token to a suitable circuit. Press the Action key to select that circuit. You must have more of your color before the time runs out in order to win.");
	gui_addKeyAndText("tutLiftText",		"To change between decks, you use a lift. A lift is activated by stopping over a lift tile and pressing the Action Key. Use the direction keys to select a new level and press the Action Key to select it.");
	gui_addKeyAndText("tutTerminalText",	"Stop at a terminal and press the Action Key. This will grant you access to the ships computer systems. Access to the droid database is controlled by the access level of your current droid.");
	gui_addKeyAndText("tutHealingText",		"All droids have a health indicator which spins on them. The slower this spins, the lower the health level of the droid. To repair damage stop over a repair tile. Your score is automatically subtracted as you repair.");
	gui_addKeyAndText("tutTipsText",		" - Colliding with another droid will damage both. ! - Some droids are immune to the disrupter weapons. ! - Your control over a droid weakens with time. ! - The first number on a droid indicates its ranking.");

	gui_addKeyAndText("cancelButton", "Cancel");
	gui_addKeyAndText("nextButton", "Next");

	gui_addKeyAndText("networkLost", "Network lost");
	gui_addKeyAndText("serverLost", "The connection to the server is down.");
	gui_addKeyAndText("pausedTitle", "Paused");
	gui_addKeyAndText("pausedText", "Game Paused");
}



