#pragma once
#include "stdafx.hpp"

typedef int Void;
typedef int Any;
typedef int BOOL;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;

struct Transaction {
	int m_ID;
	int m_Unk1;
	int m_Price;
	int m_Multiplier;
	int m_Value;
};

struct NetShopTransactionBasket {
	char _0x0000[0x8];
	int m_TransactionID;
	char _0x000C[0xC];
	int m_Category;
	char _0x001C[0x8];
	int m_Action;
	int m_Target; // 4 = bank etc
	char _0x002C[0xB4];
	Transaction m_Transactions[71];
	int m_TransactionCount;
};

struct SessionBrowser {
	uint64_t m_RockstarID;
	uint64_t m_PeerToken;
	std::string m_Name;
	bool m_Requested;
};

struct CustomNewsContext {
	std::string m_Title;
	std::string m_Headline;
	std::string m_Subtitle;
	std::string m_Content;
	std::string m_ImageUri;
};

struct Desyncs {
	uint64_t m_Target;
	uint32_t m_Last;
	int m_Count;
	bool m_Possibility;
};

struct RelayUsage {
	char m_Name[0x20];
	uint64_t m_RockstarID;
	uint32_t m_ExternalIP;
};

struct GeoIP {
	std::string m_Country;
	std::string m_CountryCode;
	std::string m_RegionName;
	std::string m_City;
	std::string m_Zip;
	std::string m_Timezone;
	std::string m_ISP;
	std::string m_ORG;
};

enum eThreadState {
	STATE_IDLE,
	STATE_RUNNING,
	STATE_KILLED
};

enum eJoinResponses : uint8_t {
	JOIN_OK,
	FAILED_TO_JOIN_INTENDED_SESSION,
	FAILED_TO_JOIN_INTENDED_SESSION_2,
	FAILED_TO_JOIN_INTENDED_SESSION_3,
	FAILED_TO_JOIN_INTENDED_SESSION_4,
	YOU_HAVE_BEEN_VOTED_OUT,
	FAILED_TO_JOIN_INTENDED_SESSION_6,
	FAILED_TO_JOIN_DUE_TO_INCOMPATIBLE_ASSETS,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_CURRENTLY_FULL,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_CURRENTLY_FULL_OF_PLAYERS_DO_YOU_WANT_TO_JOIN_QUEUE,
	PLEASE_MAKE_SURE_ALL_PLAYERS_HAVE_THE_LATEST_TITLE_UPDATE,
	FAILED_TO_JOIN_INTENDED_SESSION_11,
	INVITES_ARE_CURRENTLY_DISABLED_IN_THIS_SESSION,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_USING_A_DIFFERENT_TARGETING_PREFERENCE,
	YOU_ARE_CLASSED_AS_A_CHEAT_AND_CAN_ONLY_PLAY_WITH_OTHER_CHEATS_UNTIL_YOU_ARE_FORGIVEN,
	FAILED_TO_JOIN_INTENDED_SESSION_15,
	INCOMPATILBLE_DOWNLOADABLE_CONTENT,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_HAS_AN_ACTIVE_CREW_LIMIT,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FOR_PEOPLE_WHO_ARE_NOT_BADSPORT_OR_CHEATERS_YOU_ARE_BADSPORT,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FOR_BADSPORTS_ONLY,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FOR_CHEATERS_ONLY,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_NO_LONGER_EXISTS,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_PRIVATE,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_A_DIFFERENT_BUILD_TYPE,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_NOT_USING_THE_SAME_CONTENT,
	THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_FRIENDS_ONLY,
	UNABLE_TO_JOIN_THIS_SESSION_YOUR_ACCOUNT_HAS_A_BAD_REPUTATION,
	UNABLE_TO_CONNECT_TO_SESSION,
	UNABLE_TO_JOIN_THE_SESSION_YOU_ARE_TRYING_TO_JOIN_IS_A_PREMIUM_RACE,
	FAILED_TO_JOIN_INTENDED_GTA_ONLINE_SESSION
};

enum eNetworkEvent {
	OBJECT_ID_FREED_EVENT,
	OBJECT_ID_REQUEST_EVENT,
	ARRAY_DATA_VERIFY_EVENT,
	SCRIPT_ARRAY_DATA_VERIFY_EVENT,
	REQUEST_CONTROL_EVENT,
	GIVE_CONTROL_EVENT,
	WEAPON_DAMAGE_EVENT,
	REQUEST_PICKUP_EVENT,
	REQUEST_MAP_PICKUP_EVENT,
	GAME_CLOCK_EVENT,
	GAME_WEATHER_EVENT,
	RESPAWN_PLAYER_PED_EVENT,	//
	GIVE_WEAPON_EVENT,
	REMOVE_WEAPON_EVENT,
	REMOVE_ALL_WEAPONS_EVENT,
	VEHICLE_COMPONENT_CONTROL_EVENT,
	FIRE_EVENT,
	EXPLOSION_EVENT,
	START_PROJECTILE_EVENT,
	UPDATE_PROJECTILE_TARGET_EVENT,
	REMOVE_PROJECTILE_ENTITY_EVENT,
	BREAK_PROJECTILE_TARGET_LOCK_EVENT,
	ALTER_WANTED_LEVEL_EVENT,
	CHANGE_RADIO_STATION_EVENT,
	RAGDOLL_REQUEST_EVENT,
	PLAYER_TAUNT_EVENT,
	PLAYER_CARD_STAT_EVENT,
	DOOR_BREAK_EVENT,
	SCRIPTED_GAME_EVENT,
	REMOTE_SCRIPT_INFO_EVENT,
	REMOTE_SCRIPT_LEAVE_EVENT,
	MARK_AS_NO_LONGER_NEEDED_EVENT,
	CONVERT_TO_SCRIPT_ENTITY_EVENT,
	SCRIPT_WORLD_STATE_EVENT,
	CLEAR_AREA_EVENT,
	CLEAR_RECTANGLE_AREA_EVENT,
	NETWORK_REQUEST_SYNCED_SCENE_EVENT,
	NETWORK_START_SYNCED_SCENE_EVENT,
	NETWORK_STOP_SYNCED_SCENE_EVENT,
	NETWORK_UPDATE_SYNCED_SCENE_EVENT,
	INCIDENT_ENTITY_EVENT,
	GIVE_PED_SCRIPTED_TASK_EVENT,
	GIVE_PED_SEQUENCE_TASK_EVENT,
	NETWORK_CLEAR_PED_TASKS_EVENT,
	NETWORK_START_PED_ARREST_EVENT,
	NETWORK_START_PED_UNCUFF_EVENT,
	NETWORK_SOUND_CAR_HORN_EVENT,
	NETWORK_ENTITY_AREA_STATUS_EVENT,
	NETWORK_GARAGE_OCCUPIED_STATUS_EVENT,
	PED_CONVERSATION_LINE_EVENT,
	SCRIPT_ENTITY_STATE_CHANGE_EVENT,
	NETWORK_PLAY_SOUND_EVENT,
	NETWORK_STOP_SOUND_EVENT,
	NETWORK_PLAY_AIRDEFENSE_FIRE_EVENT,
	NETWORK_BANK_REQUEST_EVENT,
	NETWORK_AUDIO_BARK_EVENT, // New 1.47
	REQUEST_DOOR_EVENT,
	NETWORK_TRAIN_REPORT_EVENT,
	NETWORK_TRAIN_REQUEST_EVENT,
	NETWORK_INCREMENT_STAT_EVENT,
	MODIFY_VEHICLE_LOCK_WORD_STATE_DATA,
	MODIFY_PTFX_WORD_STATE_DATA_SCRIPTED_EVOLVE_EVENT, // New 1.46
	REQUEST_PHONE_EXPLOSION_EVENT,
	REQUEST_DETACHMENT_EVENT,
	KICK_VOTES_EVENT,
	GIVE_PICKUP_REWARDS_EVENT,
	NETWORK_CRC_HASH_CHECK_EVENT,
	BLOW_UP_VEHICLE_EVENT,
	NETWORK_SPECIAL_FIRE_EQUIPPED_WEAPON,
	NETWORK_RESPONDED_TO_THREAT_EVENT,
	NETWORK_SHOUT_TARGET_POSITION,
	VOICE_DRIVEN_MOUTH_MOVEMENT_FINISHED_EVENT,
	PICKUP_DESTROYED_EVENT,
	UPDATE_PLAYER_SCARS_EVENT,
	NETWORK_CHECK_EXE_SIZE_EVENT,
	NETWORK_PTFX_EVENT,
	NETWORK_PED_SEEN_DEAD_PED_EVENT,
	REMOVE_STICKY_BOMB_EVENT,
	NETWORK_CHECK_CODE_CRCS_EVENT,
	INFORM_SILENCED_GUNSHOT_EVENT,
	PED_PLAY_PAIN_EVENT,
	CACHE_PLAYER_HEAD_BLEND_DATA_EVENT,
	REMOVE_PED_FROM_PEDGROUP_EVENT,
	REPORT_MYSELF_EVENT,
	REPORT_CASH_SPAWN_EVENT,
	ACTIVATE_VEHICLE_SPECIAL_ABILITY_EVENT,
	BLOCK_WEAPON_SELECTION,
	NETWORK_CHECK_CATALOG_CRC
};

enum eVehicleModTypes {
	MOD_SPOILER,
	MOD_FRONTBUMPER,
	MOD_REARBUMPER,
	MOD_SIDESKIRT,
	MOD_EXHAUST,
	MOD_CHASSIS, //Or roll cage
	MOD_GRILLE,
	MOD_HOOD,
	MOD_FENDER,
	MOD_RIGHTFENDER,
	MOD_ROOF,
	MOD_ENGINE,
	MOD_BRAKES,
	MOD_TRANSMISSION,
	MOD_HORNS,
	MOD_SUSPENSION,
	MOD_ARMOR,
	MOD_UNK17 = 17,
	MOD_TURBO = 18,
	MOD_UNK19 = 19,
	MOD_TIRESMOKE = 20,
	MOD_UNK21 = 21,
	MOD_XENONLIGHTS = 22,
	MOD_FRONTWHEELS = 23,
	MOD_BACKWHEELS = 24, //Bikes only

	// Benny's
	MOD_PLATEHOLDER = 25,
	MOD_VANITY_PLATES = 26,
	MOD_TRIM_DESIGN = 27,
	MOD_ORNAMENTS = 28,
	MOD_DASHBOARD = 29,
	MOD_DIAL = 30,
	MOD_DOOR_SPEAKER = 31,
	MOD_SEATS = 32,
	MOD_STEERINGWHEEL = 33,
	MOD_SHIFTER_LEAVERS = 34,
	MOD_PLAQUES = 35,
	MOD_SPEAKERS = 36, // might be 36
	MOD_TRUNK = 37,
	MOD_HYDRULICS = 38,
	MOD_ENGINE_BLOCK = 39,
	MOD_AIR_FILTER = 40,
	MOD_STRUTS = 41,
	MOD_ARCH_COVER = 42,
	MOD_AERIALS = 43,
	MOD_TRIM = 44,
	MOD_TANK = 45,
	MOD_WINDOWS = 46,
	MOD_UNK47 = 47,
	MOD_LIVERY = 48,

	MOD_SPECIAL_FRONT_RIMS = 62,
	MOD_SPECIAL_REAR_RIMS = 63,

	// custom
	MOD_NEON,
	MOD_HORN_STANDARD,
	MOD_HORN_MUSICAL,
	MOD_HORN_LOOP,
	MOD_WINDOWS_CUSTOM,
	MOD_PLATES
};

enum eScaleformButtons {
	ARROW_UP,
	ARROW_DOWN,
	ARROW_LEFT,
	ARROW_RIGHT,
	BUTTON_DPAD_UP,
	BUTTON_DPAD_DOWN,
	BUTTON_DPAD_RIGHT,
	BUTTON_DPAD_LEFT,
	BUTTON_DPAD_BLANK,
	BUTTON_DPAD_ALL,
	BUTTON_DPAD_UP_DOWN,
	BUTTON_DPAD_LEFT_RIGHT,
	BUTTON_LSTICK_UP,
	BUTTON_LSTICK_DOWN,
	BUTTON_LSTICK_LEFT,
	BUTTON_LSTICK_RIGHT,
	BUTTON_LSTICK,
	BUTTON_LSTICK_ALL,
	BUTTON_LSTICK_UP_DOWN,
	BUTTON_LSTICK_LEFT_RIGHT,
	BUTTON_LSTICK_ROTATE,
	BUTTON_RSTICK_UP,
	BUTTON_RSTICK_DOWN,
	BUTTON_RSTICK_LEFT,
	BUTTON_RSTICK_RIGHT,
	BUTTON_RSTICK,
	BUTTON_RSTICK_ALL,
	BUTTON_RSTICK_UP_DOWN,
	BUTTON_RSTICK_LEFT_RIGHT,
	BUTTON_RSTICK_ROTATE,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_LB,
	BUTTON_LT,
	BUTTON_RB,
	BUTTON_RT,
	BUTTON_START,
	BUTTON_BACK,
	RED_BOX,
	RED_BOX_1,
	RED_BOX_2,
	RED_BOX_3,
	LOADING_HALF_CIRCLE_LEFT,
	ARROW_UP_DOWN,
	ARROW_LEFT_RIGHT,
	ARROW_ALL,
	LOADING_HALF_CIRCLE_LEFT_2,
	SAVE_HALF_CIRCLE_LEFT,
	LOADING_HALF_CIRCLE_RIGHT,
};

enum ExplosionTypes {
	EXPLOSION_GRENADE,
	EXPLOSION_GRENADELAUNCHER,
	EXPLOSION_STICKYBOMB,
	EXPLOSION_MOLOTOV,
	EXPLOSION_ROCKET,
	EXPLOSION_TANKSHELL,
	EXPLOSION_HI_OCTANE,
	EXPLOSION_CAR,
	EXPLOSION_PLANE,
	EXPLOSION_PETROL_PUMP,
	EXPLOSION_BIKE,
	EXPLOSION_DIR_STEAM,
	EXPLOSION_DIR_FLAME,
	EXPLOSION_DIR_WATER_HYDRANT,
	EXPLOSION_DIR_GAS_CANISTER,
	EXPLOSION_BOAT,
	EXPLOSION_SHIP_DESTROY,
	EXPLOSION_TRUCK,
	EXPLOSION_BULLET,
	EXPLOSION_SMOKEGRENADELAUNCHER,
	EXPLOSION_SMOKEGRENADE,
	EXPLOSION_BZGAS,
	EXPLOSION_FLARE,
	EXPLOSION_GAS_CANISTER,
	EXPLOSION_EXTINGUISHER,
	EXPLOSION_PROGRAMMABLEAR,
	EXPLOSION_TRAIN,
	EXPLOSION_BARREL,
	EXPLOSION_PROPANE,
	EXPLOSION_BLIMP,
	EXPLOSION_DIR_FLAME_EXPLODE,
	EXPLOSION_TANKER,
	EXPLOSION_PLANE_ROCKET,
	EXPLOSION_VEHICLE_BULLET,
	EXPLOSION_GAS_TANK,
	EXPLOSION_BIRD_CRAP
};

enum GameState {
    GameStatePlaying,
    GameStateIntro,
    GameStateLicense = 3,
    GameStateMainMenu = 5,
    GameStateLoadingSP_MP = 6
};

enum Font {
	FontChaletLondon = 0,
	FontHouseScript = 1,
	FontMonoSpace = 2,
	FontWindlings = 3,
	FontChaletComprimeCologne = 4,
	FontChaletComprimeCologne2 = 5,
	FontChaletComprimeCologne3 = 6,
	FontPricedown = 7
};

enum eControlsGay {
	INPUT_NEXT_CAMERA = 0,
	INPUT_LOOK_LR = 1,
	INPUT_LOOK_UD = 2,
	INPUT_LOOK_UP_ONLY = 3,
	INPUT_LOOK_DOWN_ONLY = 4,
	INPUT_LOOK_LEFT_ONLY = 5,
	INPUT_LOOK_RIGHT_ONLY = 6,
	INPUT_CINEMATIC_SLOWMO = 7,
	INPUT_SCRIPTED_FLY_UD = 8,
	INPUT_SCRIPTED_FLY_LR = 9,
	INPUT_SCRIPTED_FLY_ZUP = 10,
	INPUT_SCRIPTED_FLY_ZDOWN = 11,
	INPUT_WEAPON_WHEEL_UD = 12,
	INPUT_WEAPON_WHEEL_LR = 13,
	INPUT_WEAPON_WHEEL_NEXT = 14,
	INPUT_WEAPON_WHEEL_PREV = 15,
	INPUT_SELECT_NEXT_WEAPON = 16,
	INPUT_SELECT_PREV_WEAPON = 17,
	INPUT_SKIP_CUTSCENE = 18,
	INPUT_CHARACTER_WHEEL = 19,
	INPUT_MULTIPLAYER_INFO = 20,
	INPUT_SPRINT = 21,
	INPUT_JUMP = 22,
	INPUT_ENTER = 23,
	INPUT_ATTACK = 24,
	INPUT_AIM = 25,
	INPUT_LOOK_BEHIND = 26,
	INPUT_PHONE = 27,
	INPUT_SPECIAL_ABILITY = 28,
	INPUT_SPECIAL_ABILITY_SECONDARY = 29,
	INPUT_MOVE_LR = 30,
	INPUT_MOVE_UD = 31,
	INPUT_MOVE_UP_ONLY = 32,
	INPUT_MOVE_DOWN_ONLY = 33,
	INPUT_MOVE_LEFT_ONLY = 34,
	INPUT_MOVE_RIGHT_ONLY = 35,
	INPUT_DUCK = 36,
	INPUT_SELECT_WEAPON = 37,
	INPUT_PICKUP = 38,
	INPUT_SNIPER_ZOOM = 39,
	INPUT_SNIPER_ZOOM_IN_ONLY = 40,
	INPUT_SNIPER_ZOOM_OUT_ONLY = 41,
	INPUT_SNIPER_ZOOM_IN_SECONDARY = 42,
	INPUT_SNIPER_ZOOM_OUT_SECONDARY = 43,
	INPUT_COVER = 44,
	INPUT_RELOAD = 45,
	INPUT_TALK = 46,
	INPUT_DETONATE = 47,
	INPUT_HUD_SPECIAL = 48,
	INPUT_ARREST = 49,
	INPUT_ACCURATE_AIM = 50,
	INPUT_CONTEXT = 51,
	INPUT_CONTEXT_SECONDARY = 52,
	INPUT_WEAPON_SPECIAL = 53,
	INPUT_WEAPON_SPECIAL_TWO = 54,
	INPUT_DIVE = 55,
	INPUT_DROP_WEAPON = 56,
	INPUT_DROP_AMMO = 57,
	INPUT_THROW_GRENADE = 58,
	INPUT_VEH_MOVE_LR = 59,
	INPUT_VEH_MOVE_UD = 60,
	INPUT_VEH_MOVE_UP_ONLY = 61,
	INPUT_VEH_MOVE_DOWN_ONLY = 62,
	INPUT_VEH_MOVE_LEFT_ONLY = 63,
	INPUT_VEH_MOVE_RIGHT_ONLY = 64,
	INPUT_VEH_SPECIAL = 65,
	INPUT_VEH_GUN_LR = 66,
	INPUT_VEH_GUN_UD = 67,
	INPUT_VEH_AIM = 68,
	INPUT_VEH_ATTACK = 69,
	INPUT_VEH_ATTACK2 = 70,
	INPUT_VEH_ACCELERATE = 71,
	INPUT_VEH_BRAKE = 72,
	INPUT_VEH_DUCK = 73,
	INPUT_VEH_HEADLIGHT = 74,
	INPUT_VEH_EXIT = 75,
	INPUT_VEH_HANDBRAKE = 76,
	INPUT_VEH_HOTWIRE_LEFT = 77,
	INPUT_VEH_HOTWIRE_RIGHT = 78,
	INPUT_VEH_LOOK_BEHIND = 79,
	INPUT_VEH_CIN_CAM = 80,
	INPUT_VEH_NEXT_RADIO = 81,
	INPUT_VEH_PREV_RADIO = 82,
	INPUT_VEH_NEXT_RADIO_TRACK = 83,
	INPUT_VEH_PREV_RADIO_TRACK = 84,
	INPUT_VEH_RADIO_WHEEL = 85,
	INPUT_VEH_HORN = 86,
	INPUT_VEH_FLY_THROTTLE_UP = 87,
	INPUT_VEH_FLY_THROTTLE_DOWN = 88,
	INPUT_VEH_FLY_YAW_LEFT = 89,
	INPUT_VEH_FLY_YAW_RIGHT = 90,
	INPUT_VEH_PASSENGER_AIM = 91,
	INPUT_VEH_PASSENGER_ATTACK = 92,
	INPUT_VEH_SPECIAL_ABILITY_FRANKLIN = 93,
	INPUT_VEH_STUNT_UD = 94,
	INPUT_VEH_CINEMATIC_UD = 95,
	INPUT_VEH_CINEMATIC_UP_ONLY = 96,
	INPUT_VEH_CINEMATIC_DOWN_ONLY = 97,
	INPUT_VEH_CINEMATIC_LR = 98,
	INPUT_VEH_SELECT_NEXT_WEAPON = 99,
	INPUT_VEH_SELECT_PREV_WEAPON = 100,
	INPUT_VEH_ROOF = 101,
	INPUT_VEH_JUMP = 102,
	INPUT_VEH_GRAPPLING_HOOK = 103,
	INPUT_VEH_SHUFFLE = 104,
	INPUT_VEH_DROP_PROJECTILE = 105,
	INPUT_VEH_MOUSE_CONTROL_OVERRIDE = 106,
	INPUT_VEH_FLY_ROLL_LR = 107,
	INPUT_VEH_FLY_ROLL_LEFT_ONLY = 108,
	INPUT_VEH_FLY_ROLL_RIGHT_ONLY = 109,
	INPUT_VEH_FLY_PITCH_UD = 110,
	INPUT_VEH_FLY_PITCH_UP_ONLY = 111,
	INPUT_VEH_FLY_PITCH_DOWN_ONLY = 112,
	INPUT_VEH_FLY_UNDERCARRIAGE = 113,
	INPUT_VEH_FLY_ATTACK = 114,
	INPUT_VEH_FLY_SELECT_NEXT_WEAPON = 115,
	INPUT_VEH_FLY_SELECT_PREV_WEAPON = 116,
	INPUT_VEH_FLY_SELECT_TARGET_LEFT = 117,
	INPUT_VEH_FLY_SELECT_TARGET_RIGHT = 118,
	INPUT_VEH_FLY_VERTICAL_FLIGHT_MODE = 119,
	INPUT_VEH_FLY_DUCK = 120,
	INPUT_VEH_FLY_ATTACK_CAMERA = 121,
	INPUT_VEH_FLY_MOUSE_CONTROL_OVERRIDE = 122,
	INPUT_VEH_SUB_TURN_LR = 123,
	INPUT_VEH_SUB_TURN_LEFT_ONLY = 124,
	INPUT_VEH_SUB_TURN_RIGHT_ONLY = 125,
	INPUT_VEH_SUB_PITCH_UD = 126,
	INPUT_VEH_SUB_PITCH_UP_ONLY = 127,
	INPUT_VEH_SUB_PITCH_DOWN_ONLY = 128,
	INPUT_VEH_SUB_THROTTLE_UP = 129,
	INPUT_VEH_SUB_THROTTLE_DOWN = 130,
	INPUT_VEH_SUB_ASCEND = 131,
	INPUT_VEH_SUB_DESCEND = 132,
	INPUT_VEH_SUB_TURN_HARD_LEFT = 133,
	INPUT_VEH_SUB_TURN_HARD_RIGHT = 134,
	INPUT_VEH_SUB_MOUSE_CONTROL_OVERRIDE = 135,
	INPUT_VEH_PUSHBIKE_PEDAL = 136,
	INPUT_VEH_PUSHBIKE_SPRINT = 137,
	INPUT_VEH_PUSHBIKE_FRONT_BRAKE = 138,
	INPUT_VEH_PUSHBIKE_REAR_BRAKE = 139,
	INPUT_MELEE_ATTACK_LIGHT = 140,
	INPUT_MELEE_ATTACK_HEAVY = 141,
	INPUT_MELEE_ATTACK_ALTERNATE = 142,
	INPUT_MELEE_BLOCK = 143,
	INPUT_PARACHUTE_DEPLOY = 144,
	INPUT_PARACHUTE_DETACH = 145,
	INPUT_PARACHUTE_TURN_LR = 146,
	INPUT_PARACHUTE_TURN_LEFT_ONLY = 147,
	INPUT_PARACHUTE_TURN_RIGHT_ONLY = 148,
	INPUT_PARACHUTE_PITCH_UD = 149,
	INPUT_PARACHUTE_PITCH_UP_ONLY = 150,
	INPUT_PARACHUTE_PITCH_DOWN_ONLY = 151,
	INPUT_PARACHUTE_BRAKE_LEFT = 152,
	INPUT_PARACHUTE_BRAKE_RIGHT = 153,
	INPUT_PARACHUTE_SMOKE = 154,
	INPUT_PARACHUTE_PRECISION_LANDING = 155,
	INPUT_MAP = 156,
	INPUT_SELECT_WEAPON_UNARMED = 157,
	INPUT_SELECT_WEAPON_MELEE = 158,
	INPUT_SELECT_WEAPON_HANDGUN = 159,
	INPUT_SELECT_WEAPON_SHOTGUN = 160,
	INPUT_SELECT_WEAPON_SMG = 161,
	INPUT_SELECT_WEAPON_AUTO_RIFLE = 162,
	INPUT_SELECT_WEAPON_SNIPER = 163,
	INPUT_SELECT_WEAPON_HEAVY = 164,
	INPUT_SELECT_WEAPON_SPECIAL = 165,
	INPUT_SELECT_CHARACTER_MICHAEL = 166,
	INPUT_SELECT_CHARACTER_FRANKLIN = 167,
	INPUT_SELECT_CHARACTER_TREVOR = 168,
	INPUT_SELECT_CHARACTER_MULTIPLAYER = 169,
	INPUT_SAVE_REPLAY_CLIP = 170,
	INPUT_SPECIAL_ABILITY_PC = 171,
	INPUT_CELLPHONE_UP = 172,
	INPUT_CELLPHONE_DOWN = 173,
	INPUT_CELLPHONE_LEFT = 174,
	INPUT_CELLPHONE_RIGHT = 175,
	INPUT_CELLPHONE_SELECT = 176,
	INPUT_CELLPHONE_CANCEL = 177,
	INPUT_CELLPHONE_OPTION = 178,
	INPUT_CELLPHONE_EXTRA_OPTION = 179,
	INPUT_CELLPHONE_SCROLL_FORWARD = 180,
	INPUT_CELLPHONE_SCROLL_BACKWARD = 181,
	INPUT_CELLPHONE_CAMERA_FOCUS_LOCK = 182,
	INPUT_CELLPHONE_CAMERA_GRID = 183,
	INPUT_CELLPHONE_CAMERA_SELFIE = 184,
	INPUT_CELLPHONE_CAMERA_DOF = 185,
	INPUT_CELLPHONE_CAMERA_EXPRESSION = 186,
	INPUT_FRONTEND_DOWN = 187,
	INPUT_FRONTEND_UP = 188,
	INPUT_FRONTEND_LEFT = 189,
	INPUT_FRONTEND_RIGHT = 190,
	INPUT_FRONTEND_RDOWN = 191,
	INPUT_FRONTEND_RUP = 192,
	INPUT_FRONTEND_RLEFT = 193,
	INPUT_FRONTEND_RRIGHT = 194,
	INPUT_FRONTEND_AXIS_X = 195,
	INPUT_FRONTEND_AXIS_Y = 196,
	INPUT_FRONTEND_RIGHT_AXIS_X = 197,
	INPUT_FRONTEND_RIGHT_AXIS_Y = 198,
	INPUT_FRONTEND_PAUSE = 199,
	INPUT_FRONTEND_PAUSE_ALTERNATE = 200,
	INPUT_FRONTEND_ACCEPT = 201,
	INPUT_FRONTEND_CANCEL = 202,
	INPUT_FRONTEND_X = 203,
	INPUT_FRONTEND_Y = 204,
	INPUT_FRONTEND_LB = 205,
	INPUT_FRONTEND_RB = 206,
	INPUT_FRONTEND_LT = 207,
	INPUT_FRONTEND_RT = 208,
	INPUT_FRONTEND_LS = 209,
	INPUT_FRONTEND_RS = 210,
	INPUT_FRONTEND_LEADERBOARD = 211,
	INPUT_FRONTEND_SOCIAL_CLUB = 212,
	INPUT_FRONTEND_SOCIAL_CLUB_SECONDARY = 213,
	INPUT_FRONTEND_DELETE = 214,
	INPUT_FRONTEND_ENDSCREEN_ACCEPT = 215,
	INPUT_FRONTEND_ENDSCREEN_EXPAND = 216,
	INPUT_FRONTEND_SELECT = 217,
	INPUT_SCRIPT_LEFT_AXIS_X = 218,
	INPUT_SCRIPT_LEFT_AXIS_Y = 219,
	INPUT_SCRIPT_RIGHT_AXIS_X = 220,
	INPUT_SCRIPT_RIGHT_AXIS_Y = 221,
	INPUT_SCRIPT_RUP = 222,
	INPUT_SCRIPT_RDOWN = 223,
	INPUT_SCRIPT_RLEFT = 224,
	INPUT_SCRIPT_RRIGHT = 225,
	INPUT_SCRIPT_LB = 226,
	INPUT_SCRIPT_RB = 227,
	INPUT_SCRIPT_LT = 228,
	INPUT_SCRIPT_RT = 229,
	INPUT_SCRIPT_LS = 230,
	INPUT_SCRIPT_RS = 231,
	INPUT_SCRIPT_PAD_UP = 232,
	INPUT_SCRIPT_PAD_DOWN = 233,
	INPUT_SCRIPT_PAD_LEFT = 234,
	INPUT_SCRIPT_PAD_RIGHT = 235,
	INPUT_SCRIPT_SELECT = 236,
	INPUT_CURSOR_ACCEPT = 237,
	INPUT_CURSOR_CANCEL = 238,
	INPUT_CURSOR_X = 239,
	INPUT_CURSOR_Y = 240,
	INPUT_CURSOR_SCROLL_UP = 241,
	INPUT_CURSOR_SCROLL_DOWN = 242,
	INPUT_ENTER_CHEAT_CODE = 243,
	INPUT_INTERACTION_MENU = 244,
	INPUT_MP_TEXT_CHAT_ALL = 245,
	INPUT_MP_TEXT_CHAT_TEAM = 246,
	INPUT_MP_TEXT_CHAT_FRIENDS = 247,
	INPUT_MP_TEXT_CHAT_CREW = 248,
	INPUT_PUSH_TO_TALK = 249,
	INPUT_CREATOR_LS = 250,
	INPUT_CREATOR_RS = 251,
	INPUT_CREATOR_LT = 252,
	INPUT_CREATOR_RT = 253,
	INPUT_CREATOR_MENU_TOGGLE = 254,
	INPUT_CREATOR_ACCEPT = 255,
	INPUT_CREATOR_DELETE = 256,
	INPUT_ATTACK2 = 257,
	INPUT_RAPPEL_JUMP = 258,
	INPUT_RAPPEL_LONG_JUMP = 259,
	INPUT_RAPPEL_SMASH_WINDOW = 260,
	INPUT_PREV_WEAPON = 261,
	INPUT_NEXT_WEAPON = 262,
	INPUT_MELEE_ATTACK1 = 263,
	INPUT_MELEE_ATTACK2 = 264,
	INPUT_WHISTLE = 265,
	INPUT_MOVE_LEFT = 266,
	INPUT_MOVE_RIGHT = 267,
	INPUT_MOVE_UP = 268,
	INPUT_MOVE_DOWN = 269,
	INPUT_LOOK_LEFT = 270,
	INPUT_LOOK_RIGHT = 271,
	INPUT_LOOK_UP = 272,
	INPUT_LOOK_DOWN = 273,
	INPUT_SNIPER_ZOOM_IN = 274,
	INPUT_SNIPER_ZOOM_OUT = 275,
	INPUT_SNIPER_ZOOM_IN_ALTERNATE = 276,
	INPUT_SNIPER_ZOOM_OUT_ALTERNATE = 277,
	INPUT_VEH_MOVE_LEFT = 278,
	INPUT_VEH_MOVE_RIGHT = 279,
	INPUT_VEH_MOVE_UP = 280,
	INPUT_VEH_MOVE_DOWN = 281,
	INPUT_VEH_GUN_LEFT = 282,
	INPUT_VEH_GUN_RIGHT = 283,
	INPUT_VEH_GUN_UP = 284,
	INPUT_VEH_GUN_DOWN = 285,
	INPUT_VEH_LOOK_LEFT = 286,
	INPUT_VEH_LOOK_RIGHT = 287,
	INPUT_REPLAY_START_STOP_RECORDING = 288,
	INPUT_REPLAY_START_STOP_RECORDING_SECONDARY = 289,
	INPUT_SCALED_LOOK_LR = 290,
	INPUT_SCALED_LOOK_UD = 291,
	INPUT_SCALED_LOOK_UP_ONLY = 292,
	INPUT_SCALED_LOOK_DOWN_ONLY = 293,
	INPUT_SCALED_LOOK_LEFT_ONLY = 294,
	INPUT_SCALED_LOOK_RIGHT_ONLY = 295,
	INPUT_REPLAY_MARKER_DELETE = 296,
	INPUT_REPLAY_CLIP_DELETE = 297,
	INPUT_REPLAY_PAUSE = 298,
	INPUT_REPLAY_REWIND = 299,
	INPUT_REPLAY_FFWD = 300,
	INPUT_REPLAY_NEWMARKER = 301,
	INPUT_REPLAY_RECORD = 302,
	INPUT_REPLAY_SCREENSHOT = 303,
	INPUT_REPLAY_HIDEHUD = 304,
	INPUT_REPLAY_STARTPOINT = 305,
	INPUT_REPLAY_ENDPOINT = 306,
	INPUT_REPLAY_ADVANCE = 307,
	INPUT_REPLAY_BACK = 308,
	INPUT_REPLAY_TOOLS = 309,
	INPUT_REPLAY_RESTART = 310,
	INPUT_REPLAY_SHOWHOTKEY = 311,
	INPUT_REPLAY_CYCLEMARKERLEFT = 312,
	INPUT_REPLAY_CYCLEMARKERRIGHT = 313,
	INPUT_REPLAY_FOVINCREASE = 314,
	INPUT_REPLAY_FOVDECREASE = 315,
	INPUT_REPLAY_CAMERAUP = 316,
	INPUT_REPLAY_CAMERADOWN = 317,
	INPUT_REPLAY_SAVE = 318,
	INPUT_REPLAY_TOGGLETIME = 319,
	INPUT_REPLAY_TOGGLETIPS = 320,
	INPUT_REPLAY_PREVIEW = 321,
	INPUT_REPLAY_TOGGLE_TIMELINE = 322,
	INPUT_REPLAY_TIMELINE_PICKUP_CLIP = 323,
	INPUT_REPLAY_TIMELINE_DUPLICATE_CLIP = 324,
	INPUT_REPLAY_TIMELINE_PLACE_CLIP = 325,
	INPUT_REPLAY_CTRL = 326,
	INPUT_REPLAY_TIMELINE_SAVE = 327,
	INPUT_REPLAY_PREVIEW_AUDIO = 328,
	INPUT_VEH_DRIVE_LOOK = 329,
	INPUT_VEH_DRIVE_LOOK2 = 330,
	INPUT_VEH_FLY_ATTACK2 = 331,
	INPUT_RADIO_WHEEL_UD = 332,
	INPUT_RADIO_WHEEL_LR = 333,
	INPUT_VEH_SLOWMO_UD = 334,
	INPUT_VEH_SLOWMO_UP_ONLY = 335,
	INPUT_VEH_SLOWMO_DOWN_ONLY = 336,
	INPUT_MAP_POI = 337
};

enum eControls {
	ControlNextCamera = 0,
	ControlLookLeftRight = 1,
	ControlLookUpDown = 2,
	ControlLookUpOnly = 3,
	ControlLookDownOnly = 4,
	ControlLookLeftOnly = 5,
	ControlLookRightOnly = 6,
	ControlCinematicSlowMo = 7,
	ControlFlyUpDown = 8,
	ControlFlyLeftRight = 9,
	ControlScriptedFlyZUp = 10,
	ControlScriptedFlyZDown = 11,
	ControlWeaponWheelUpDown = 12,
	ControlWeaponWheelLeftRight = 13,
	ControlWeaponWheelNext = 14,
	ControlWeaponWheelPrev = 15,
	ControlSelectNextWeapon = 16,
	ControlSelectPrevWeapon = 17,
	ControlSkipCutscene = 18,
	ControlCharacterWheel = 19,
	ControlMultiplayerInfo = 20,
	ControlSprint = 21,
	ControlJump = 22,
	ControlEnter = 23,
	ControlAttack = 24,
	ControlAim = 25,
	ControlLookBehind = 26,
	ControlPhone = 27,
	ControlSpecialAbility = 28,
	ControlSpecialAbilitySecondary = 29,
	ControlMoveLeftRight = 30,
	ControlMoveUpDown = 31,
	ControlMoveUpOnly = 32,
	ControlMoveDownOnly = 33,
	ControlMoveLeftOnly = 34,
	ControlMoveRightOnly = 35,
	ControlDuck = 36,
	ControlSelectWeapon = 37,
	ControlPickup = 38,
	ControlSniperZoom = 39,
	ControlSniperZoomInOnly = 40,
	ControlSniperZoomOutOnly = 41,
	ControlSniperZoomInSecondary = 42,
	ControlSniperZoomOutSecondary = 43,
	ControlCover = 44,
	ControlReload = 45,
	ControlTalk = 46,
	ControlDetonate = 47,
	ControlHUDSpecial = 48,
	ControlArrest = 49,
	ControlAccurateAim = 50,
	ControlContext = 51,
	ControlContextSecondary = 52,
	ControlWeaponSpecial = 53,
	ControlWeaponSpecial2 = 54,
	ControlDive = 55,
	ControlDropWeapon = 56,
	ControlDropAmmo = 57,
	ControlThrowGrenade = 58,
	ControlVehicleMoveLeftRight = 59,
	ControlVehicleMoveUpDown = 60,
	ControlVehicleMoveUpOnly = 61,
	ControlVehicleMoveDownOnly = 62,
	ControlVehicleMoveLeftOnly = 63,
	ControlVehicleMoveRightOnly = 64,
	ControlVehicleSpecial = 65,
	ControlVehicleGunLeftRight = 66,
	ControlVehicleGunUpDown = 67,
	ControlVehicleAim = 68,
	ControlVehicleAttack = 69,
	ControlVehicleAttack2 = 70,
	ControlVehicleAccelerate = 71,
	ControlVehicleBrake = 72,
	ControlVehicleDuck = 73,
	ControlVehicleHeadlight = 74,
	ControlVehicleExit = 75,
	ControlVehicleHandbrake = 76,
	ControlVehicleHotwireLeft = 77,
	ControlVehicleHotwireRight = 78,
	ControlVehicleLookBehind = 79,
	ControlVehicleCinCam = 80,
	ControlVehicleNextRadio = 81,
	ControlVehiclePrevRadio = 82,
	ControlVehicleNextRadioTrack = 83,
	ControlVehiclePrevRadioTrack = 84,
	ControlVehicleRadioWheel = 85,
	ControlVehicleHorn = 86,
	ControlVehicleFlyThrottleUp = 87,
	ControlVehicleFlyThrottleDown = 88,
	ControlVehicleFlyYawLeft = 89,
	ControlVehicleFlyYawRight = 90,
	ControlVehiclePassengerAim = 91,
	ControlVehiclePassengerAttack = 92,
	ControlVehicleSpecialAbilityFranklin = 93,
	ControlVehicleStuntUpDown = 94,
	ControlVehicleCinematicUpDown = 95,
	ControlVehicleCinematicUpOnly = 96,
	ControlVehicleCinematicDownOnly = 97,
	ControlVehicleCinematicLeftRight = 98,
	ControlVehicleSelectNextWeapon = 99,
	ControlVehicleSelectPrevWeapon = 100,
	ControlVehicleRoof = 101,
	ControlVehicleJump = 102,
	ControlVehicleGrapplingHook = 103,
	ControlVehicleShuffle = 104,
	ControlVehicleDropProjectile = 105,
	ControlVehicleMouseControlOverride = 106,
	ControlVehicleFlyRollLeftRight = 107,
	ControlVehicleFlyRollLeftOnly = 108,
	ControlVehicleFlyRollRightOnly = 109,
	ControlVehicleFlyPitchUpDown = 110,
	ControlVehicleFlyPitchUpOnly = 111,
	ControlVehicleFlyPitchDownOnly = 112,
	ControlVehicleFlyUnderCarriage = 113,
	ControlVehicleFlyAttack = 114,
	ControlVehicleFlySelectNextWeapon = 115,
	ControlVehicleFlySelectPrevWeapon = 116,
	ControlVehicleFlySelectTargetLeft = 117,
	ControlVehicleFlySelectTargetRight = 118,
	ControlVehicleFlyVerticalFlightMode = 119,
	ControlVehicleFlyDuck = 120,
	ControlVehicleFlyAttackCamera = 121,
	ControlVehicleFlyMouseControlOverride = 122,
	ControlVehicleSubTurnLeftRight = 123,
	ControlVehicleSubTurnLeftOnly = 124,
	ControlVehicleSubTurnRightOnly = 125,
	ControlVehicleSubPitchUpDown = 126,
	ControlVehicleSubPitchUpOnly = 127,
	ControlVehicleSubPitchDownOnly = 128,
	ControlVehicleSubThrottleUp = 129,
	ControlVehicleSubThrottleDown = 130,
	ControlVehicleSubAscend = 131,
	ControlVehicleSubDescend = 132,
	ControlVehicleSubTurnHardLeft = 133,
	ControlVehicleSubTurnHardRight = 134,
	ControlVehicleSubMouseControlOverride = 135,
	ControlVehiclePushbikePedal = 136,
	ControlVehiclePushbikeSprint = 137,
	ControlVehiclePushbikeFrontBrake = 138,
	ControlVehiclePushbikeRearBrake = 139,
	ControlMeleeAttackLight = 140,
	ControlMeleeAttackHeavy = 141,
	ControlMeleeAttackAlternate = 142,
	ControlMeleeBlock = 143,
	ControlParachuteDeploy = 144,
	ControlParachuteDetach = 145,
	ControlParachuteTurnLeftRight = 146,
	ControlParachuteTurnLeftOnly = 147,
	ControlParachuteTurnRightOnly = 148,
	ControlParachutePitchUpDown = 149,
	ControlParachutePitchUpOnly = 150,
	ControlParachutePitchDownOnly = 151,
	ControlParachuteBrakeLeft = 152,
	ControlParachuteBrakeRight = 153,
	ControlParachuteSmoke = 154,
	ControlParachutePrecisionLanding = 155,
	ControlMap = 156,
	ControlSelectWeaponUnarmed = 157,
	ControlSelectWeaponMelee = 158,
	ControlSelectWeaponHandgun = 159,
	ControlSelectWeaponShotgun = 160,
	ControlSelectWeaponSmg = 161,
	ControlSelectWeaponAutoRifle = 162,
	ControlSelectWeaponSniper = 163,
	ControlSelectWeaponHeavy = 164,
	ControlSelectWeaponSpecial = 165,
	ControlSelectCharacterMichael = 166,
	ControlSelectCharacterFranklin = 167,
	ControlSelectCharacterTrevor = 168,
	ControlSelectCharacterMultiplayer = 169,
	ControlSaveReplayClip = 170,
	ControlSpecialAbilityPC = 171,
	ControlPhoneUp = 172,
	ControlPhoneDown = 173,
	ControlPhoneLeft = 174,
	ControlPhoneRight = 175,
	ControlPhoneSelect = 176,
	ControlPhoneCancel = 177,
	ControlPhoneOption = 178,
	ControlPhoneExtraOption = 179,
	ControlPhoneScrollForward = 180,
	ControlPhoneScrollBackward = 181,
	ControlPhoneCameraFocusLock = 182,
	ControlPhoneCameraGrid = 183,
	ControlPhoneCameraSelfie = 184,
	ControlPhoneCameraDOF = 185,
	ControlPhoneCameraExpression = 186,
	ControlFrontendDown = 187,
	ControlFrontendUp = 188,
	ControlFrontendLeft = 189,
	ControlFrontendRight = 190,
	ControlFrontendRdown = 191,
	ControlFrontendRup = 192,
	ControlFrontendRleft = 193,
	ControlFrontendRright = 194,
	ControlFrontendAxisX = 195,
	ControlFrontendAxisY = 196,
	ControlFrontendRightAxisX = 197,
	ControlFrontendRightAxisY = 198,
	ControlFrontendPause = 199,
	ControlFrontendPauseAlternate = 200,
	ControlFrontendAccept = 201,
	ControlFrontendCancel = 202,
	ControlFrontendX = 203,
	ControlFrontendY = 204,
	ControlFrontendLb = 205,
	ControlFrontendRb = 206,
	ControlFrontendLt = 207,
	ControlFrontendRt = 208,
	ControlFrontendLs = 209,
	ControlFrontendRs = 210,
	ControlFrontendLeaderboard = 211,
	ControlFrontendSocialClub = 212,
	ControlFrontendSocialClubSecondary = 213,
	ControlFrontendDelete = 214,
	ControlFrontendEndscreenAccept = 215,
	ControlFrontendEndscreenExpand = 216,
	ControlFrontendSelect = 217,
	ControlScriptLeftAxisX = 218,
	ControlScriptLeftAxisY = 219,
	ControlScriptRightAxisX = 220,
	ControlScriptRightAxisY = 221,
	ControlScriptRUp = 222,
	ControlScriptRDown = 223,
	ControlScriptRLeft = 224,
	ControlScriptRRight = 225,
	ControlScriptLB = 226,
	ControlScriptRB = 227,
	ControlScriptLT = 228,
	ControlScriptRT = 229,
	ControlScriptLS = 230,
	ControlScriptRS = 231,
	ControlScriptPadUp = 232,
	ControlScriptPadDown = 233,
	ControlScriptPadLeft = 234,
	ControlScriptPadRight = 235,
	ControlScriptSelect = 236,
	ControlCursorAccept = 237,
	ControlCursorCancel = 238,
	ControlCursorX = 239,
	ControlCursorY = 240,
	ControlCursorScrollUp = 241,
	ControlCursorScrollDown = 242,
	ControlEnterCheatCode = 243,
	ControlInteractionMenu = 244,
	ControlMpTextChatAll = 245,
	ControlMpTextChatTeam = 246,
	ControlMpTextChatFriends = 247,
	ControlMpTextChatCrew = 248,
	ControlPushToTalk = 249,
	ControlCreatorLS = 250,
	ControlCreatorRS = 251,
	ControlCreatorLT = 252,
	ControlCreatorRT = 253,
	ControlCreatorMenuToggle = 254,
	ControlCreatorAccept = 255,
	ControlCreatorDelete = 256,
	ControlAttack2 = 257,
	ControlRappelJump = 258,
	ControlRappelLongJump = 259,
	ControlRappelSmashWindow = 260,
	ControlPrevWeapon = 261,
	ControlNextWeapon = 262,
	ControlMeleeAttack1 = 263,
	ControlMeleeAttack2 = 264,
	ControlWhistle = 265,
	ControlMoveLeft = 266,
	ControlMoveRight = 267,
	ControlMoveUp = 268,
	ControlMoveDown = 269,
	ControlLookLeft = 270,
	ControlLookRight = 271,
	ControlLookUp = 272,
	ControlLookDown = 273,
	ControlSniperZoomIn = 274,
	ControlSniperZoomOut = 275,
	ControlSniperZoomInAlternate = 276,
	ControlSniperZoomOutAlternate = 277,
	ControlVehicleMoveLeft = 278,
	ControlVehicleMoveRight = 279,
	ControlVehicleMoveUp = 280,
	ControlVehicleMoveDown = 281,
	ControlVehicleGunLeft = 282,
	ControlVehicleGunRight = 283,
	ControlVehicleGunUp = 284,
	ControlVehicleGunDown = 285,
	ControlVehicleLookLeft = 286,
	ControlVehicleLookRight = 287,
	ControlReplayStartStopRecording = 288,
	ControlReplayStartStopRecordingSecondary = 289,
	ControlScaledLookLeftRight = 290,
	ControlScaledLookUpDown = 291,
	ControlScaledLookUpOnly = 292,
	ControlScaledLookDownOnly = 293,
	ControlScaledLookLeftOnly = 294,
	ControlScaledLookRightOnly = 295,
	ControlReplayMarkerDelete = 296,
	ControlReplayClipDelete = 297,
	ControlReplayPause = 298,
	ControlReplayRewind = 299,
	ControlReplayFfwd = 300,
	ControlReplayNewmarker = 301,
	ControlReplayRecord = 302,
	ControlReplayScreenshot = 303,
	ControlReplayHidehud = 304,
	ControlReplayStartpoint = 305,
	ControlReplayEndpoint = 306,
	ControlReplayAdvance = 307,
	ControlReplayBack = 308,
	ControlReplayTools = 309,
	ControlReplayRestart = 310,
	ControlReplayShowhotkey = 311,
	ControlReplayCycleMarkerLeft = 312,
	ControlReplayCycleMarkerRight = 313,
	ControlReplayFOVIncrease = 314,
	ControlReplayFOVDecrease = 315,
	ControlReplayCameraUp = 316,
	ControlReplayCameraDown = 317,
	ControlReplaySave = 318,
	ControlReplayToggletime = 319,
	ControlReplayToggletips = 320,
	ControlReplayPreview = 321,
	ControlReplayToggleTimeline = 322,
	ControlReplayTimelinePickupClip = 323,
	ControlReplayTimelineDuplicateClip = 324,
	ControlReplayTimelinePlaceClip = 325,
	ControlReplayCtrl = 326,
	ControlReplayTimelineSave = 327,
	ControlReplayPreviewAudio = 328,
	ControlVehicleDriveLook = 329,
	ControlVehicleDriveLook2 = 330,
	ControlVehicleFlyAttack2 = 331,
	ControlRadioWheelUpDown = 332,
	ControlRadioWheelLeftRight = 333,
	ControlVehicleSlowMoUpDown = 334,
	ControlVehicleSlowMoUpOnly = 335,
	ControlVehicleSlowMoDownOnly = 336,
	ControlMapPointOfInterest = 337,
};

enum eAnimationFlags {
	AnimationNormal = 0,
	AnimationRepeat = 1,
	AnimationStopLastFrame = 2,
	AnimationUpperBody = 16,
	AnimationEnablePlayerControl = 32,
	AnimationCancelable = 120,
	AnimationCreature = 256
};

enum eDispatchType {
	DT_PoliceAutomobile = 1,
	DT_PoliceHelicopter = 2,
	DT_FireDepartment = 3,
	DT_SwatAutomobile = 4,
	DT_AmbulanceDepartment = 5,
	DT_PoliceRiders = 6,
	DT_PoliceVehicleRequest = 7,
	DT_PoliceRoadBlock = 8,
	DT_PoliceAutomobileWaitPulledOver = 9,
	DT_PoliceAutomobileWaitCruising = 10,
	DT_Gangs = 11,
	DT_SwatHelicopter = 12,
	DT_PoliceBoat = 13,
	DT_ArmyVehicle = 14,
	DT_BikerBackup = 15
};

enum eHudColors {
	HUD_COLOUR_PURE_WHITE,
	HUD_COLOUR_WHITE,
	HUD_COLOUR_BLACK,
	HUD_COLOUR_GREY,
	HUD_COLOUR_GREYLIGHT,
	HUD_COLOUR_GREYDARK,
	HUD_COLOUR_RED,
	HUD_COLOUR_REDLIGHT,
	HUD_COLOUR_REDDARK,
	HUD_COLOUR_BLUE,
	HUD_COLOUR_BLUELIGHT,
	HUD_COLOUR_BLUEDARK,
	HUD_COLOUR_YELLOW,
	HUD_COLOUR_YELLOWLIGHT,
	HUD_COLOUR_YELLOWDARK,
	HUD_COLOUR_ORANGE,
	HUD_COLOUR_ORANGELIGHT,
	HUD_COLOUR_ORANGEDARK,
	HUD_COLOUR_GREEN,
	HUD_COLOUR_GREENLIGHT,
	HUD_COLOUR_GREENDARK,
	HUD_COLOUR_PURPLE,
	HUD_COLOUR_PURPLELIGHT,
	HUD_COLOUR_PURPLEDARK,
	HUD_COLOUR_PINK,
	HUD_COLOUR_RADAR_HEALTH,
	HUD_COLOUR_RADAR_ARMOUR,
	HUD_COLOUR_RADAR_DAMAGE,
	HUD_COLOUR_NET_PLAYER1,
	HUD_COLOUR_NET_PLAYER2,
	HUD_COLOUR_NET_PLAYER3,
	HUD_COLOUR_NET_PLAYER4,
	HUD_COLOUR_NET_PLAYER5,
	HUD_COLOUR_NET_PLAYER6,
	HUD_COLOUR_NET_PLAYER7,
	HUD_COLOUR_NET_PLAYER8,
	HUD_COLOUR_NET_PLAYER9,
	HUD_COLOUR_NET_PLAYER10,
	HUD_COLOUR_NET_PLAYER11,
	HUD_COLOUR_NET_PLAYER12,
	HUD_COLOUR_NET_PLAYER13,
	HUD_COLOUR_NET_PLAYER14,
	HUD_COLOUR_NET_PLAYER15,
	HUD_COLOUR_NET_PLAYER16,
	HUD_COLOUR_NET_PLAYER17,
	HUD_COLOUR_NET_PLAYER18,
	HUD_COLOUR_NET_PLAYER19,
	HUD_COLOUR_NET_PLAYER20,
	HUD_COLOUR_NET_PLAYER21,
	HUD_COLOUR_NET_PLAYER22,
	HUD_COLOUR_NET_PLAYER23,
	HUD_COLOUR_NET_PLAYER24,
	HUD_COLOUR_NET_PLAYER25,
	HUD_COLOUR_NET_PLAYER26,
	HUD_COLOUR_NET_PLAYER27,
	HUD_COLOUR_NET_PLAYER28,
	HUD_COLOUR_NET_PLAYER29,
	HUD_COLOUR_NET_PLAYER30,
	HUD_COLOUR_NET_PLAYER31,
	HUD_COLOUR_NET_PLAYER32,
	HUD_COLOUR_SIMPLEBLIP_DEFAULT,
	HUD_COLOUR_MENU_BLUE,
	HUD_COLOUR_MENU_GREY_LIGHT,
	HUD_COLOUR_MENU_BLUE_EXTRA_DARK,
	HUD_COLOUR_MENU_YELLOW,
	HUD_COLOUR_MENU_YELLOW_DARK,
	HUD_COLOUR_MENU_GREEN,
	HUD_COLOUR_MENU_GREY,
	HUD_COLOUR_MENU_GREY_DARK,
	HUD_COLOUR_MENU_HIGHLIGHT,
	HUD_COLOUR_MENU_STANDARD,
	HUD_COLOUR_MENU_DIMMED,
	HUD_COLOUR_MENU_EXTRA_DIMMED,
	HUD_COLOUR_BRIEF_TITLE,
	HUD_COLOUR_MID_GREY_MP,
	HUD_COLOUR_NET_PLAYER1_DARK,
	HUD_COLOUR_NET_PLAYER2_DARK,
	HUD_COLOUR_NET_PLAYER3_DARK,
	HUD_COLOUR_NET_PLAYER4_DARK,
	HUD_COLOUR_NET_PLAYER5_DARK,
	HUD_COLOUR_NET_PLAYER6_DARK,
	HUD_COLOUR_NET_PLAYER7_DARK,
	HUD_COLOUR_NET_PLAYER8_DARK,
	HUD_COLOUR_NET_PLAYER9_DARK,
	HUD_COLOUR_NET_PLAYER10_DARK,
	HUD_COLOUR_NET_PLAYER11_DARK,
	HUD_COLOUR_NET_PLAYER12_DARK,
	HUD_COLOUR_NET_PLAYER13_DARK,
	HUD_COLOUR_NET_PLAYER14_DARK,
	HUD_COLOUR_NET_PLAYER15_DARK,
	HUD_COLOUR_NET_PLAYER16_DARK,
	HUD_COLOUR_NET_PLAYER17_DARK,
	HUD_COLOUR_NET_PLAYER18_DARK,
	HUD_COLOUR_NET_PLAYER19_DARK,
	HUD_COLOUR_NET_PLAYER20_DARK,
	HUD_COLOUR_NET_PLAYER21_DARK,
	HUD_COLOUR_NET_PLAYER22_DARK,
	HUD_COLOUR_NET_PLAYER23_DARK,
	HUD_COLOUR_NET_PLAYER24_DARK,
	HUD_COLOUR_NET_PLAYER25_DARK,
	HUD_COLOUR_NET_PLAYER26_DARK,
	HUD_COLOUR_NET_PLAYER27_DARK,
	HUD_COLOUR_NET_PLAYER28_DARK,
	HUD_COLOUR_NET_PLAYER29_DARK,
	HUD_COLOUR_NET_PLAYER30_DARK,
	HUD_COLOUR_NET_PLAYER31_DARK,
	HUD_COLOUR_NET_PLAYER32_DARK,
	HUD_COLOUR_BRONZE,
	HUD_COLOUR_SILVER,
	HUD_COLOUR_GOLD,
	HUD_COLOUR_PLATINUM,
	HUD_COLOUR_GANG1,
	HUD_COLOUR_GANG2,
	HUD_COLOUR_GANG3,
	HUD_COLOUR_GANG4,
	HUD_COLOUR_SAME_CREW,
	HUD_COLOUR_FREEMODE,
	HUD_COLOUR_PAUSE_BG,
	HUD_COLOUR_FRIENDLY,
	HUD_COLOUR_ENEMY,
	HUD_COLOUR_LOCATION,
	HUD_COLOUR_PICKUP,
	HUD_COLOUR_PAUSE_SINGLEPLAYER,
	HUD_COLOUR_FREEMODE_DARK,
	HUD_COLOUR_INACTIVE_MISSION,
	HUD_COLOUR_DAMAGE,
	HUD_COLOUR_PINKLIGHT,
	HUD_COLOUR_PM_MITEM_HIGHLIGHT,
	HUD_COLOUR_SCRIPT_VARIABLE,
	HUD_COLOUR_YOGA,
	HUD_COLOUR_TENNIS,
	HUD_COLOUR_GOLF,
	HUD_COLOUR_SHOOTING_RANGE,
	HUD_COLOUR_FLIGHT_SCHOOL,
	HUD_COLOUR_NORTH_BLUE,
	HUD_COLOUR_SOCIAL_CLUB,
	HUD_COLOUR_PLATFORM_BLUE,
	HUD_COLOUR_PLATFORM_GREEN,
	HUD_COLOUR_PLATFORM_GREY,
	HUD_COLOUR_FACEBOOK_BLUE,
	HUD_COLOUR_INGAME_BG,
	HUD_COLOUR_DARTS,
	HUD_COLOUR_WAYPOINT,
	HUD_COLOUR_MICHAEL,
	HUD_COLOUR_FRANKLIN,
	HUD_COLOUR_TREVOR,
	HUD_COLOUR_GOLF_P1,
	HUD_COLOUR_GOLF_P2,
	HUD_COLOUR_GOLF_P3,
	HUD_COLOUR_GOLF_P4,
	HUD_COLOUR_WAYPOINTLIGHT,
	HUD_COLOUR_WAYPOINTDARK,
	HUD_COLOUR_PANEL_LIGHT,
	HUD_COLOUR_MICHAEL_DARK,
	HUD_COLOUR_FRANKLIN_DARK,
	HUD_COLOUR_TREVOR_DARK,
	HUD_COLOUR_OBJECTIVE_ROUTE,
	HUD_COLOUR_PAUSEMAP_TINT,
	HUD_COLOUR_PAUSE_DESELECT,
	HUD_COLOUR_PM_WEAPONS_PURCHASABLE,
	HUD_COLOUR_PM_WEAPONS_LOCKED,
	HUD_COLOUR_END_SCREEN_BG,
	HUD_COLOUR_CHOP,
	HUD_COLOUR_PAUSEMAP_TINT_HALF,
	HUD_COLOUR_NORTH_BLUE_OFFICIAL,
	HUD_COLOUR_SCRIPT_VARIABLE_2,
	HUD_COLOUR_H,
	HUD_COLOUR_HDARK,
	HUD_COLOUR_T,
	HUD_COLOUR_TDARK,
	HUD_COLOUR_HSHARD,
	HUD_COLOUR_CONTROLLER_MICHAEL,
	HUD_COLOUR_CONTROLLER_FRANKLIN,
	HUD_COLOUR_CONTROLLER_TREVOR,
	HUD_COLOUR_CONTROLLER_CHOP,
	HUD_COLOUR_VIDEO_EDITOR_VIDEO,
	HUD_COLOUR_VIDEO_EDITOR_AUDIO,
	HUD_COLOUR_VIDEO_EDITOR_TEXT,
	HUD_COLOUR_HB_BLUE,
	HUD_COLOUR_HB_YELLOW,
	HUD_COLOUR_VIDEO_EDITOR_SCORE,
	HUD_COLOUR_VIDEO_EDITOR_AUDIO_FADEOUT,
	HUD_COLOUR_VIDEO_EDITOR_TEXT_FADEOUT,
	HUD_COLOUR_VIDEO_EDITOR_SCORE_FADEOUT,
	HUD_COLOUR_HEIST_BACKGROUND,
	HUD_COLOUR_VIDEO_EDITOR_AMBIENT,
	HUD_COLOUR_VIDEO_EDITOR_AMBIENT_FADEOUT,
	HUD_COLOUR_GB,
	HUD_COLOUR_G,
	HUD_COLOUR_B,
	HUD_COLOUR_LOW_FLOW,
	HUD_COLOUR_LOW_FLOW_DARK,
	HUD_COLOUR_G1,
	HUD_COLOUR_G2,
	HUD_COLOUR_G3,
	HUD_COLOUR_G4,
	HUD_COLOUR_G5,
	HUD_COLOUR_G6,
	HUD_COLOUR_G7,
	HUD_COLOUR_G8,
	HUD_COLOUR_G9,
	HUD_COLOUR_G10,
	HUD_COLOUR_G11,
	HUD_COLOUR_G12,
	HUD_COLOUR_G13,
	HUD_COLOUR_G14,
	HUD_COLOUR_G15,
	HUD_COLOUR_ADVERSARY,
	HUD_COLOUR_DEGEN_RED,
	HUD_COLOUR_DEGEN_YELLOW,
	HUD_COLOUR_DEGEN_GREEN,
	HUD_COLOUR_DEGEN_CYAN,
	HUD_COLOUR_DEGEN_BLUE,
	HUD_COLOUR_DEGEN_MAGENTA,
	HUD_COLOUR_STUNT_1,
	HUD_COLOUR_STUNT_2
};

enum eNetObjectTypes : uint16_t {
	NetObjectAutomobile,
	NetObjectBike,
	NetObjectBoat,
	NetObjectDoor,
	NetObjectHeli,
	NetObjectObject,
	NetObjectPed,
	NetObjectPickup,
	NetObjectPickupPlacement,
	NetObjectPlane,
	NetObjectSubmarine,
	NetObjectPlayer,
	NetObjectTrailer,
	NetObjectTrain,
	INVALID
};

enum PresenceEvent {
	PRESENCE_GAME_SERVER = 0x76849AA6,
	PRESENCE_UGC_STAT_UPDATE = 0x2190F9E1,
	PRESENCE_BOUNTY = 0xECB54731,
	PRESENCE_STAT_UPDATE = 0xEDF54CE2,
	PRESENCE_FRIEND_CREW_JOINED = 0x12859620,
	PRESENCE_FRIEND_CREW_CREATED = 0x3F1009B2,
	PRESENCE_MISSION_VERIFIED = 0x8C571656,
	PRESENCE_ROCKSTAR_MESSAGE = 0x70E4D0E1,
	PRESENCE_CREW_MESSAGE = 0xCBB12972,
	PRESENCE_GAME_AWARD = 0xD36120CE,
	PRESENCE_VOICE_SESSION_INVITE = 0xB7AECB0C,
	PRESENCE_VOICE_SESSION_RESPONSE = 0x7EC656F7,
	PRESENCE_GAME_INVITE = 0x04ED840B,
	PRESENCE_GAME_INVITED_CANCEL = 0xFCEFA1A2,
	PRESENCE_GAME_INVITE_REPLY = 0x227E6048,
	PRESENCE_TOURNAMENT_INVITE = 0xCFDF8DF4,
	PRESENCE_FOLLOW_INVITE = 0x13F209B2,
	PRESENCE_ADMIN_INVITE = 0x255FF988,
	PRESENCE_JOIN_QUEUE_REQUEST = 0x0FD33101,
	PRESENCE_JOIN_QUEUE_UPDATE = 0x33A53ACB,
	PRESENCE_FINGER_OF_GOD = 0xB07F4711,
	PRESENCE_NEWS_ITEM = 0x6EAF6B7D,
	PRESENCE_FORCE_SESSION_UPDATE = 0xECAA6FA0,
	PRESENCE_GAME_TRIGGER_EVENT = 0x28C617D9,
	PRESENCE_TEXT_MESSAGE = 0x1A8A9221
};

enum PlayerMenuType {
	SELECTED_PLAYER,
	ALL_PLAYERS,
};