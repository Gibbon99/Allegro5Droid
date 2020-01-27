
enum RESOURCE_TYPES
{
	RESOURCE_BITMAP = 0,
	RESOURCE_AUDIO,
	RESOURCE_FONT,
	RESOURCE_LEVEL,
	RESOURCE_SHIP_VIEW,
	RESOURCE_HIGH_SCORE,
	RESOURCE_SPRITE,
	RESOURCE_TILES
};

enum languageTypes
{
	LANG_ENGLISH = 1,
	LANG_ITALIAN,
	LANG_GERMAN
};

enum sideViewColorsIndex
{
	SIDEVIEW_SHIP_COLOR = 0,
	SIDEVIEW_ACTIVE_DECK_COLOR,
	SIDEVIEW_ENGINE_COLOR,
	SIDEVIEW_LIFT_COLOR,
	SIDEVIEW_ACTIVE_LIFT_COLOR,
	SIDEVIEW_NUM_COLORS,
};

enum guiObjectTypes
{
	GUI_OBJECT_BUTTON = 0,
	GUI_OBJECT_CHECKBOX,
	GUI_OBJECT_SLIDER,
	GUI_OBJECT_KEYCODE,
	GUI_OBJECT_SCREEN,
	GUI_OBJECT_TEXTBOX,
	GUI_OBJECT_LABEL,
	GUI_OBJECT_IMAGE,
	GUI_COORD_TYPE_PERCENT,
	GUI_COORD_TYPE_ABSOLUTE,
	GUI_LABEL_CENTER,
	GUI_LABEL_RIGHT,
	GUI_LABEL_LEFT,

	GUI_ACTIVE_COL,
	GUI_INACTIVE_COL,
	GUI_ACTIVE_CORNER_COL,
	GUI_INACTIVE_CORNER_COL,
	GUI_ACTIVE_LABEL_COL,
	GUI_INACTIVE_LABEL_COL,

	MESSAGE_BOX_PAUSED,

	SCROLLBOX_INTRO,
	SCROLLBOX_DB,
};

enum MODES
{
	MODE_GUI_OPTIONS = 0,
	MODE_GUI_OPTIONS_VIDEO,
	MODE_GUI_OPTIONS_AUDIO,
	MODE_GUI_OPTIONS_GRAPHICS,
	MODE_GUI_OPTIONS_CONTROLS,
	MODE_SPLASH,
	MODE_GUI_TUT_MOVE,
	MODE_GUI_TUT_TRANSFER_GAME,
	MODE_GUI_TUT_TRANSFER_START,
	MODE_GUI_TUT_LIFTS,
	MODE_GUI_TUT_TERMINALS,
	MODE_GUI_TUT_HEALING,
	MODE_GUI_TUT_TIPS,
	MODE_PRE_GAME,
	MODE_GUI_INTRO,
	MODE_GUI,
	MODE_CONSOLE,
	MODE_GAME,
	MODE_GAME_RETURN,
	MODE_GAME_PAUSED,
	MODE_LIFT_VIEW,
	MODE_GUI_TERMINAL,
	MODE_GUI_TERMINAL_SHIPVIEW,
	MODE_GUI_TERMINAL_DECKVIEW,
	MODE_GUI_DATABASE,
	MODE_GUI_TRANSFER_SCREEN_ONE,
	MODE_GUI_TRANSFER_SCREEN_TWO,
	MODE_GUI_TRANSFER_CHOOSE_SIDE,
	MODE_GUI_TRANSFER_SCREEN_ONE_LOAD,
	MODE_GUI_TRANSFER_SCREEN_TWO_LOAD,
	MODE_GUI_TRANSFER_INIT_GAME,
	MODE_GUI_TRANSFER_RENDER_GAME,
	MODE_PLAY_TRANSFER_GAME
};

//
// Defines for what type of particle to draw
// Primitive is using al_draw_circle
//
enum PARTICLE_RENDER_TYPE
{
	PARTICLE_RENDER_OFF = 0,
	PARTICLE_RENDER_BITMAP,
	PARTICLE_RENDER_PRIMITIVE
};

enum PARTICLE_TYPES
{
	PARTICLE_TYPE_EXPLOSION = 0,
	PARTICLE_TYPE_TRAIL,
	PARTICLE_TYPE_SPARK
};

enum transferColors
{
	TRANSFER_COLOR_LEFT = 0,
	TRANSFER_COLOR_RIGHT,
	TRANSFER_COLOR_BACKGROUND,
	TRANSFER_COLOR_BORDER
};

enum tokenDirections
{
	TOKEN_DIRECTION_RIGHT = 0,
	TOKEN_DIRECTION_LEFT
};

enum PARA_EVENT_TYPES
{
	PARA_EVENT_LOGFILE = 0,
	PARA_EVENT_CONSOLE,
	PARA_EVENT_GAME,
	PARA_EVENT_MAIN,
	PARA_EVENT_AUDIO
};

enum eventActions
{
	LOGFILE_EVENT_START = 0,
	LOGFILE_EVENT_STOP,
	LOGFILE_EVENT_LOG,

	CONSOLE_EVENT_START,
	CONSOLE_EVENT_STOP,
	CONSOLE_EVENT_LOG,

	MAIN_LOOP_EVENT,
	MAIN_LOOP_EVENT_ADD_BULLET,
	MAIN_LOOP_EVENT_REMOVE_DROID_PHYSICS,

	GAME_EVENT_CHANGE_MODE,
	GAME_EVENT_DOOR,
	GAME_EVENT_DOOR_ANIMATE,
	GAME_DOOR_STATE_ENTER,
	GAME_DOOR_STATE_EXIT,

	GAME_EVENT_PLAY_AUDIO,
	GAME_EVENT_STOP_AUDIO,

	GAME_EVENT_HEALING_START,
	GAME_EVENT_HEALING_STOP
};

enum tokenMoveDirections
{
	MOVE_UP = 0,
	MOVE_DOWN
};

enum sliderTypes
{
	SLIDER_TYPE_INT = 0,
	SLIDER_TYPE_FLOAT,
	SLIDER_TYPE_STRING
};