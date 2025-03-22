#pragma once
#include "stdafx.hpp"

typedef int Any;
typedef unsigned int uint;
typedef unsigned int Hash;
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

struct encryptedPool {
    uint64_t table1;
    uint64_t table2;
    uint8_t addition;
};

struct presenceInfo {
    uint64_t rockstarID;
    char _0x0004[2];
    BYTE type;
    char _0x0001[5];
};

struct presenceData {
    char _0x0000[0x40];
    char presence[0x200];
};

struct joinRequestData {
    char _0x0000[0x10];
    uint64_t rid;
    char _0x0020[0x8];
    char name[0x11];
    char _0x0031[0x7];
    uint32_t flag;
};

struct joinResponseData {
    char _0x0000[0x10];
    uint64_t rid;
    char _0x0018[0x8];
    char name[0x11];
    char _0x0031[3];
    int32_t kickFlag; 
};

struct entityPool {
    uint64_t vtable;
    uint64_t pool;
    uint64_t unk;
    int count;
    int size;
    uint64_t unk2;
    uint64_t unk3;
};

struct geoIP {
    std::string country;
    std::string countryCode;
    std::string regionName;
    std::string city;
    std::string zip;
    std::string timezone;
    std::string isp;
    std::string org;
};

struct watertune {
    uint32_t waterColor;
    float rippleSpeed;
    float fogPierceIntensity;
    float waterCycleDepth;
    float waterCycleFade;
    float waterLightningDepth;
    float waterLightningFade;
    float deepWaterModDepth;
    float deepWaterModFade;
    float godRaysLerpStart;
    float godRaysLerpEnd;
    float disturbFoamScale;
    float fogMin[2];
    float fogMax[2];
    float rainFadeMin;
    float rainFadeMax;
    float oceanFoamDepthFadeRange;
    float waterClipViewDistance;
    float waterClipFogColor;
    float waterClipFogBrightness;
    float waterClipCameraCausticStrength;
    float waterClipCausticStrength;
};

struct vehiclePopulationTuning {
    float multiplayerRandomVehicleDensityMultiplier;
    float multiplayerParkedVehicleDensityMultiplier;
    float maxVehicleSpacingAboveFurthestDefaultValue;
    float divisorForVehicleSpacingPastFurthestDefaultValue;
    float minRandomDensityVarianceMultiplier;
    float maxRandomDensityVarianceMultiplier;
    bool rejectAllVehiclesMovingAway;
    bool alsoRejectIfStationary;
    bool disableVehiclesInMP;
};

struct friendInfo {
    uint64_t rid;
    uint8_t status;
    uint8_t unk[7];
    char name[24];
};

struct networkHandle {
    int handle[13];
};

struct variants {
    int variant;
    Hash hash;
};

enum eExplosionTag
{
    EXP_TAG_DONTCARE = -1,
    EXP_TAG_GRENADE,
    EXP_TAG_STICKYBOMB,
    EXP_TAG_MOLOTOV,
    EXP_TAG_MOLOTOV_VOLATILE,
    EXP_TAG_HI_OCTANE,
    EXP_TAG_CAR,
    EXP_TAG_PLANE,
    EXP_TAG_PETROL_PUMP,
    EXP_TAG_DIR_STEAM,
    EXP_TAG_DIR_FLAME,
    EXP_TAG_DIR_WATER_HYDRANT,
    EXP_TAG_BOAT,
    EXP_TAG_BULLET,
    EXP_TAG_SMOKEGRENADE,
    EXP_TAG_BZGAS,
    EXP_TAG_GAS_CANISTER,
    EXP_TAG_EXTINGUISHER,
    EXP_TAG_TRAIN,
    EXP_TAG_DIR_FLAME_EXPLODE,
    EXP_TAG_VEHICLE_BULLET,
    EXP_TAG_BIRD_CRAP,
    EXP_TAG_FIREWORK,
    EXP_TAG_TORPEDO,
    EXP_TAG_TORPEDO_UNDERWATER,
    EXP_TAG_LANTERN,
    EXP_TAG_DYNAMITE,
    EXP_TAG_DYNAMITESTACK,
    EXP_TAG_DYNAMITE_VOLATILE,
    EXP_TAG_RIVER_BLAST,
    EXP_TAG_PLACED_DYNAMITE,
    EXP_TAG_FIRE_ARROW,
    EXP_TAG_DYNAMITE_ARROW,
    EXP_TAG_PHOSPHOROUS_BULLET,
    EXP_TAG_LIGHTNING_STRIKE,
    EXP_TAG_TRACKING_ARROW,
    EXP_TAG_POISON_BOTTLE
};

enum eNetObjType
{
    NET_OBJ_TYPE_ANIMAL,
    NET_OBJ_TYPE_AUTOMOBILE,
    NET_OBJ_TYPE_BIKE,
    NET_OBJ_TYPE_BOAT,
    NET_OBJ_TYPE_DOOR,
    NET_OBJ_TYPE_HELI,
    NET_OBJ_TYPE_OBJECT,
    NET_OBJ_TYPE_PED,
    NET_OBJ_TYPE_PICKUP,
    NET_OBJ_TYPE_PICKUP_PLACEMENT,
    NET_OBJ_TYPE_PLANE,
    NET_OBJ_TYPE_SUBMARINE,
    NET_OBJ_TYPE_PLAYER,
    NET_OBJ_TYPE_TRAILER,
    NET_OBJ_TYPE_TRAIN,
    NET_OBJ_TYPE_DRAFT_VEH,
    NET_OBJ_TYPE_STATS_TRACKER,
    NET_OBJ_TYPE_PROP_SET,
    NET_OBJ_TYPE_ANIM_SCENE,
    NET_OBJ_TYPE_GROUP_SCENARIO,
    NET_OBJ_TYPE_HERD,
    NET_OBJ_TYPE_HORSE,
    NET_OBJ_TYPE_WORLD_STATE,
    NET_OBJ_TYPE_PROJECTILE,
    NET_OBJ_TYPE_INCIDENT,
    NET_OBJ_TYPE_GUARDZONE,
    NET_OBJ_TYPE_PEDGROUP,
    NET_OBJ_TYPE_COMBATDIRECTOR,
    NET_OBJ_TYPE_PEDSHAREDTARGETING,
    NET_OBJ_TYPE_PERSISTENT,
};

enum ePresenceEvents {
    PRESENCE_STAT_UPDATE = 0xedf54ce2,
    PRESENCE_FRIEND_CREW_JOINED = 0x12859620,
    PRESENCE_FRIEND_CREATED_CREW = 0x3f1009b2,
    PRESENCE_MISSION_VERIFIED = 0x8c571656,
    PRESENCE_ROCKSTAR_MESSAGE = 0x70e4d0e1,
    PRESENCE_CREW_MESSAGE = 0xcbb12972,
    PRESENCE_GAME_AWARD = 0xd36120ce,
    PRESENCE_VOICE_INVITE = 0xb7aecb0c,
    PRESENCE_VOICE_RESPONSE = 0x7ec656f7,
    PRESENCE_SESSION_SERVER_INFO_REQUEST = 0x472fdbcd,
    PRESENCE_SESSION_SERVER_INFO_RESPONSE = 0x6357fabb,
    PRESENCE_INVITE = 0xfbcc029b,
    PRESENCE_INVITE_CANCEL = 0x193994f5,
    PRESENCE_INVITE_REQUEST_RESULT_EVENT = 0xe89a4022,
    PRESENCE_INVITE_RESPONSE = 0x391da84f,
    PRESENCE_INVITE_UPDATE_EVENT = 0x4d9b285a,
    PRESENCE_INVITE_SERVER_INFO = 0xf3b22733,
    PRESENCE_JOIN_REQUEST = 0x5e61ebb6,
    PRESENCE_JOIN_RESPONSE = 0x606d1ddb,
    PRESENCE_ADMIN_JOIN_EVENT = 0xc167b73,
    PRESENCE_NEWS = 0x6eaf6b7d,
    PRESENCE_FORCE_SESSION_UPDATE = 0xecaa6fa0,
    PRESENCE_GAME_TRIGGER_EVENT = 0x28c617d9,
    PRESENCE_TEXT_MESSAGE = 0x1a8a9221,
    PRESENCE_GAME_SERVER_AWARD = 0x76849aa6,
    PRESENCE_ASSET_DATA_REQUEST = 0xb179d727,
    PRESENCE_ASSET_DATA_RESPONSE = 0xbd14ef85
};