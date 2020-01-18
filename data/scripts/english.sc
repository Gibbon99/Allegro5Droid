//------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create English text strings
void as_setEnglishText ()
//------------------------------------------------------------
{
	gui_addKeyAndText ("scrollBoxIntroText", "  ^ ^       Welcome to Paradroid. ! ! Press [ LEFT CTRL KEY ] to play or wait for further instructions ! Use the [ ARROW KEYS ] to navigate the menus ! Press [ ENTER ] to make a choice ! ! ^ Clear the freighter of robots by destroying them with twin lasers or by transferring control to them. ! ! Control is by keyboard only, as follows: ! ! At all times the keyboard moves the robot and holding fire down will allow use of lifts and consoles. ! ! In addition, pressing fire with no keys pressed will prepare the current robot for transfer. Contact with another robot with the fire key down will initiate transfer. ! ! ! ^ ! ! ! ! A fleet of Robo-Freighters on its way to the Beta Ceti system reported entering an uncharted field of asteroids. Each ship carries a cargo of battle droids to reinforce the outworld defences. ! ! Two distress beacons have been recovered. Similar messages were stored on each. The ships had been bombarded by a powerful radionic beam from one of the asteroids. ! ! All of the robots on the ships, including those in storage, became hyper-active. The crews report an attack by droids, isolating them on the bridge. They cannot reach the shuttle and can hold out for only a couple more hours. ! ! Since these beacons were located two days ago, we can only fear the worst. ! ! Some of the fleet was last seen heading for enemy space. In enemy hands the droids can be used against our forces. ! ! Docking would be impossible, but we can beam aboard a prototype Influence Device. ! ^  ! ! ! The 001 Influence device consists of a helmet, which, when placed over a robot's control unit can halt the normal activities of that robot for a short time. The helmet has its own power supply and powers the robot itself, at an upgraded capability. The helmet also uses an energy cloak for protection of the host. ! ! The helmet is fitted with twin lasers mounted in a turrent. These are low powered and have a slow recycle rate. ! ! Most of the device's resources are channeled towards holding control of the host robot, as it attempts to resume 'normal' operation. It is therefore necessary to change the host robot often to prevent the device from burning out. Transfer to a new robot requires the device to drain its host of energy in order to take it over. Failure to achieve transfer results in the device being a free agent once more. ! ! Further information incoming... ! ^  ! ! ! An Influence device can transmit only certain data, namely its own location and the location of other robots in visual range. This data is merged with known ship layouts on your C64 remote terminal. ! ! Additional information about the ship and robots may be obtained by accessing the ship's computer at a console. A small-scale plan of the whole deck is available, as well as a side elevation of the ship. ! ! Robots are represented on-screen as a symbol showing a three-digit number. The first digit shown is the important one, the class of the robot. It denotes strength also. ! ! To find out more about any given robot, use the robot enquiry system at a console. Only data about units of a lower class than your current host is available, since it is the host's security clearance which is used to access the console. ! ! ! ! ! Further information incoming... ! ^  ! ! ! ! ! Paradroid programmed by Andrew Braybrook. ! ! ! Sound advice by Steve Turner. ! ! ! Allegro5 port by David Berry. ! 2019. ! ! ! ! ! Powered by the Allegro5 library. ! Scripting engine by AngelScript. ! Physics by Box2D. ! Filewatcher by James Wynn 2009. ! ! ! ! Press fire to play. ! ! ! ! ! ! ^ # ");

	gui_addKeyAndText ("gameLeft", "Move left");
	gui_addKeyAndText ("gameRight", "Move Right");
	gui_addKeyAndText ("gameDown", "Move down");
	gui_addKeyAndText ("gameUp", "Move up");
	gui_addKeyAndText ("gameAction", "Perform an action");
	gui_addKeyAndText ("gamePause", "Pause the game");
	gui_addKeyAndText ("gameEscape", "Escape");
	gui_addKeyAndText ("consoleAction", "Invoke the console");
	gui_addKeyAndText ("gameScreenShot", "Take a screenshot");

	gui_addKeyAndText ("startGameButton", "Start Game");
	gui_addKeyAndText ("optionsButton", "Options");
	gui_addKeyAndText ("tutorialButton", "Tutorial");
	gui_addKeyAndText ("briefingButton", "Briefing");
	gui_addKeyAndText ("exitGameButton", "Exit Game");
	gui_addKeyAndText ("mainScreenLabel", "Select your option using mouse or keyboard.");
	gui_addKeyAndText ("guiOptionsMainLabel", "OPTIONS");
	gui_addKeyAndText ("optionVideoLabel", "VIDEO OPTIONS");
	gui_addKeyAndText ("optionsVideoCheckFullScreen", "Fullscreen");
	gui_addKeyAndText ("optionsVideoCheckFullScreenWindowed", "Fullscreen windowed");
	gui_addKeyAndText ("optionsVideoCheckWindowed", "Windowed");
	gui_addKeyAndText ("optionVideoLabelWindowType", "Window Type");
	gui_addKeyAndText ("guiOptionsMainVideoButton", "Video");
	gui_addKeyAndText ("guiOptionsMainAudioButton", "Audio");
	gui_addKeyAndText ("guiOptionsMainGraphicsButton", "Graphics");
	gui_addKeyAndText ("guiOptionsMainNetworkButton", "Network");
	gui_addKeyAndText ("guiOptionsMainControlsButton", "Controls");

	gui_addKeyAndText ("tutMoveLabel", "Tutorial - Move");
	gui_addKeyAndText ("tutTransferLabel", "Tutorial - Transfer Mode");
	gui_addKeyAndText ("tutTransGameLabel", "Tutorial - Transfer Game");
	gui_addKeyAndText ("tutLiftLabel", "Tutorial - Lifts");
	gui_addKeyAndText ("tutTerminalLabel", "Tutorial - Terminals");
	gui_addKeyAndText ("tutHealingLabel", "Tutorial - Healing");
	gui_addKeyAndText ("tutTipsLabel", "Tutorial - Gameplay tips");

	gui_addKeyAndText ("tutMoveText", " Use the arrow keys to move. ! ! Pressing the Action Key will shoot in your current movement direction. ! ! Hold down the Action Key when no other keys are pressed to initiate transfer mode.");
	gui_addKeyAndText ("tutTransText", " To take over another droid you enter into Transfer mode. ! ! Press and hold the Action key with no other keys down to enter this mode. Colliding with another droid will initiate the transfer subroutine.");
	gui_addKeyAndText ("tutTransGameText", "To gain control of another droid, you must win the circuit transfer game. Select the side you will use. Press the UP / DOWN keys to move your token to a suitable circuit. Press the Action key to select that circuit. You must have more of your color before the time runs out in order to win.");
	gui_addKeyAndText ("tutLiftText", "To change between decks, you use a lift. A lift is activated by stopping over a lift tile and pressing the Action Key. Use the direction keys to select a new level and press the Action Key to select it.");
	gui_addKeyAndText ("tutTerminalText", "Stop at a terminal and press the Action Key. This will grant you access to the ships computer systems. Access to the droid database is controlled by the access level of your current droid.");
	gui_addKeyAndText ("tutHealingText", "All droids have a health indicator which spins on them. The slower this spins, the lower the health level of the droid. To repair damage stop over a repair tile. Your score is automatically subtracted as you repair.");
	gui_addKeyAndText ("tutTipsText", " - Colliding with another droid will damage both. ! - Some droids are immune to the disrupter weapons. ! - Your control over a droid weakens with time. ! - The first number on a droid indicates its ranking.");

	gui_addKeyAndText ("previousButton", "Previous");
	gui_addKeyAndText ("cancelButton", "Cancel");
	gui_addKeyAndText ("nextButton", "Next");

	gui_addKeyAndText ("pausedTitle", "Paused");
	gui_addKeyAndText ("pausedText", "Game Paused");

	gui_addKeyAndText ("terminalScreenLabel", "Terminal - Select your option");
	gui_addKeyAndText ("terminalLogoffButton", "Logoff Terminal");
	gui_addKeyAndText ("terminalDatabaseButton", "Droid Database");
	gui_addKeyAndText ("terminalDeckviewButton", "Deck View");
	gui_addKeyAndText ("terminalShipviewButton", "Ship View");

	gui_addKeyAndText ("databaseScrollBoxText", "Droid database text");
	gui_addKeyAndText ("imageLoading", "Image is loading");

	gui_addKeyAndText ("db_001", "! ! Description : 001 - Influence Device ! ClassName : Influence ! Drive  : None ! Brain : None ! Weapon : Lasers ! Sensor 1 : ~ ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Robot activity influence device. This helmet is self-powered and will control any robot for a short time. Lasers are turret mounted. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_123", "! ! Description : 123 - Disposal Droid ! ClassName : Disposal ! Drive  : Tracks ! Brain : None ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : Infra-red ! Sensor 3 : ~ ! Notes : Simple rubbish disposal robot. Common device in most space craft to maintain a clean ship. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_139", "! ! Description : 139 - Disposal Droid ! ClassName : Disposal ! Drive  : Anti-grav ! Brain : None ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Created by Dr. Masternak to clean up large heaps of rubbish. Its scoop is used to collect rubbish. It is then crushed internally. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_247", "! ! Description : 247 - Servant Droid ! ClassName : Servant ! Drive  : Anti-grav ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Light duty servant robot. One of the first to use the anti-grav system. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_249", "! ! Description : 249 - Servant Droid ! ClassName : Servant ! Drive  : Tripedal ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Cheaper version of the anti-grav servant robot. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_296", "! ! Description : 296 - Servant Droid ! ClassName : Servant ! Drive  : Tracks ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : This robot is mainly used for serving drinks. A tray is mounted on the head. Built by Orchard and Marsden Enterprises. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_302", "! ! Description : 302 - Messenger Droid ! ClassName : Messenger ! Drive  : Anti-grav ! Brain : None ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Common device for moving small packages. Clamp is mounted on the lower body. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_329", "! ! Description : 329 - Messenger Droid ! ClassName : Messenger ! Drive  : Wheels ! Brain : None ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Early type messenger robot. Large wheels impede motion on small craft. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_420", "! ! Description : 420 - Maintenance Droid ! ClassName : Maintenance ! Drive  : Tracks ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Slow maintenance robot. Confined to drive maintenance during flight. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_476", "! ! Description : 476 - Maintenance Droid ! ClassName : Maintenance ! Drive  : Anti-grav ! Brain : Neutronic ! Weapon : Lasers ! Sensor 1 : Spectral ! Sensor 2 : Infra-red ! Sensor 3 : ~ ! Notes : Ship maintenance robot. Fitted with multiple arms to carry out repairs to the ship efficiently. All craft built after the Jupiter Incident are supplied with a team of these. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_493", "! ! Description : 493 - Maintenance Droid ! ClassName : Maintenance ! Drive  : Anti-grav ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Slave maintenance droid. Standard version will carry its own toolbox. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_516", "! ! Description : 516 - Crew Droid ! ClassName : Crew ! Drive  : Bipedal ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Early crew droid. Able to carry out simple flight checks only. No longer supplied. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_571", "! ! Description : 571 - Crew Droid ! ClassName : Crew ! Drive  : Bipedal ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Standard crew droid. Supplied with the ship. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_598", "! ! Description : 598 - Crew Droid ! ClassName : Crew ! Drive  : Bipedal ! Brain : Neutronic ! Weapon : None ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : A highly sophisticated device. Able to control the Robo-Freighter on its own. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_614", "! ! Description : 614 - Sentinel Droid ! ClassName : Sentinel ! Drive  : Bipedal ! Brain : Neutronic ! Weapon : Laser Rifle ! Sensor 1 : Spectral ! Sensor 2 : Subsonic ! Sensor 3 : ~ ! Notes : Low security sentinel droid. Used to protect areas of the ship from intruders. A slow but sure device. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_615", "! ! Description : 615 - Sentinel Droid ! ClassName : Sentinel ! Drive  : Anti-grav ! Brain : Neutronic ! Weapon : Lasers ! Sensor 1 : Spectral ! Sensor 2 : Infra-red ! Sensor 3 : ~ ! Notes : Sophisticated sentinel droid. Only 2000 built by the Nicholson corporation. These are now very rare. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_629", "! ! Description : 629 - Sentinel Droid ! ClassName : Sentinel ! Drive  : Tracks ! Brain : Neutronic ! Weapon : Lasers ! Sensor 1 : Spectral ! Sensor 2 : Subsonic ! Sensor 3 : ~ ! Notes : Slow sentinel droid. Lasers are built into the turret. These are mounted on a small tank body. May be fitted with an auto-cannon on the Gillen version. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_711", "! ! Description : 711 - Battle Droid ! ClassName : Battle ! Drive  : Bipdeal ! Brain : Neutronic ! Weapon : Disrupter ! Sensor 1 : Ultra-sonic ! Sensor 2 : Radar ! Sensor 3 : ~ ! Notes : Heavy duty battle droid. Disruptor is built into the head. One of the first in service with the Military. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_742", "! ! Description : 742 - Battle Droid ! ClassName : Battle ! Drive  : Bipedeal ! Brain : Neutronic ! Weapon : Disrupter ! Sensor 1 : Spectral ! Sensor 2 : Radar ! Sensor 3 : ~ ! Notes : This version is the one mainly used by the Military. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_751", "! ! Description : 751 - Battle Droid ! ClassName : Battle ! Drive  : Bipedal ! Brain : Neutronic ! Weapon : Lasers ! Sensor 1 : Spectral ! Sensor 2 : ~ ! Sensor 3 : ~ ! Notes : Very heavy duty battle droid. Only a few have so far entered service. These are the most powerful battle units ever built. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_821", "! ! Description : 821 - Security Droid ! ClassName : Security ! Drive  : Anti-grav ! Brain : Neutronic ! Weapon : Lasers ! Sensor 1 : Spectral ! Sensor 2 : Radar ! Sensor 3 : Infra-red ! Notes : A very reliable anti-grav unit is fitted onto this droid. It will patrol the ship and eliminate intruders as soon as detected by powerful sensors. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_834", "! ! Description : 834 - Security Droid ! ClassName : Security ! Drive  : Anti-grav ! Brain : Neutronic ! Weapon : Lasers ! Sensor 1 : Spectral ! Sensor 2 : Radar ! Sensor 3 : ~ ! Notes : Early type anti-grav security droid. Fitted with an over-driven anti-grav unit. This droid is very fast but is not reliable. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_883", "! ! Description : 883 - Security Droid ! ClassName : Security ! Drive  : Wheels ! Brain : Neutronic ! Weapon : Exterminator ! Sensor 1 : Spectral ! Sensor 2 : Radar ! Sensor 3 : ~ ! Notes : This droid was designed from archive data. For some unknown reason it instills great fear in Human adversaries. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");
	gui_addKeyAndText ("db_999", "! ! Description : 999 - Command cyborg ! ClassName : Command ! Drive  : Anti-grav ! Brain : Primode ! Weapon : Lasers ! Sensor 1 : Infra-red ! Sensor 2 : Radar ! Sensor 3 : Subsonic ! Notes : Experimental command cyborg. Fitted with a new type of brain. Mounted on a Security Droid anti-grav unit for convenience. Warning: the influence device may not control a primode brain for long. ! ! ! ! ! ! Information Repeats ! ! ! ! ! ");

	gui_addKeyAndText ("labelTransferOne", "Current Droid");
	gui_addKeyAndText ("textBoxTransferOne", "This is the unit you currently control..");

	gui_addKeyAndText ("labelTransferTwo", "Target Droid");
	gui_addKeyAndText ("textBoxTransferTwo", "This is the unit you wish to control. Prepare to transfer.");

	gui_addKeyAndText ("hudTransfer", "Transfer");
	gui_addKeyAndText ("hudMoving", "Moving");
	gui_addKeyAndText ("hudCaptured", "Captured");
	gui_addKeyAndText ("hudHealing", "Healing");
	gui_addKeyAndText ("hudRecharging", "Recharge");
	gui_addKeyAndText ("hudChooseSide", "Choose side");
	gui_addKeyAndText ("hudTerminal", "Terminal");
	gui_addKeyAndText ("hudLift", "Lift");
	gui_addKeyAndText ("hudDatabase", "Database");
	gui_addKeyAndText ("hudDeckView", "Deck View");
	gui_addKeyAndText ("hudShipView", "Ship View");
	gui_addKeyAndText ("hudBriefing", "Briefing");
	gui_addKeyAndText ("hudOptions", "Options");
	gui_addKeyAndText ("hudGameOn", "Game On");
	gui_addKeyAndText ("optionsVideoCheckBackdrop", "Show backdrop");

	gui_addKeyAndText ("optionAudioLabel", "Audio");
	gui_addKeyAndText ("optionsAudioUseSound", "Enable sound");
	gui_addKeyAndText ("optionsAudioVolume", "Audio Volume");
}

