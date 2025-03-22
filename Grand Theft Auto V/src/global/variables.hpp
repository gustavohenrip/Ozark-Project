#pragma once
#include "stdafx.hpp"
#include "rage/types/script_types.hpp"
#include "rage/types/invoker_types.hpp"
#include "rage/types/base_types.hpp"
#include "rage/types/network_types.hpp"
#include "rage/types/generic_types.hpp"
#include "utils/translation_string.hpp"
#include <queue>

struct MisalignmentFix {
	uint64_t m_Address;
	std::vector<uint8_t> m_Original;
	std::vector<uint8_t> m_Patch;
};

struct JoinTimeoutFix {
	Player m_ID;
	uint32_t m_ScriptHash;
	int m_RetryCount;
};

struct BitBufferMisalinmentThread {
	bool m_CameFromThread;
	bool m_Block;
	uint32_t m_ThreadID;
};

namespace Global::Vars {
	extern bool g_Unloading;
	extern bool g_UnloadingQueue;
	extern bool g_Windows7;
	extern bool g_StopRendering;
	extern bool g_FreemodeTerminated;
	extern bool g_Steam;
	extern HMODULE g_ModuleHandle;
	extern uint64_t g_MenuAddress;
	extern uint64_t g_MenuSize;
	extern HWND g_GameWindow;
	extern uint64_t g_GameAddress;
	extern uint64_t g_GameSize;
	extern std::unordered_map<uint64_t, uint32_t> g_SpoofedDataJoinAttempts;
	extern uint32_t g_PTFXProtectionHash;
	extern std::unordered_map<uint64_t, bool> g_OnlineRockstarIds;
	extern std::vector<uint64_t> g_OnlineRockstarIdQueue;
	extern std::size_t g_CurrentNewsContext;
	extern std::vector<std::pair<std::string, CustomNewsContext>> g_NewsContexts;
	extern std::queue<uint32_t> g_GeoQueue;
	extern std::unordered_map<uint32_t, GeoIP> g_Geo;
	extern std::string g_RealGsInfo;
	extern uint32_t g_GlobalChatIP;
	extern uint8_t g_ArxanRestoreBytes[3][2][25];
	extern uint64_t g_ArxanRestoreAddresses[3];
	extern std::vector<RelayUsage> g_RelayUsage;
	extern std::pair<float, std::string> m_SelectedMetricSystem;
	extern std::vector<SessionBrowser> g_Sessions;
	extern bool g_ReapplyWeapons;
	extern std::unordered_map<uint64_t, uint64_t> g_DesyncsHost;
	extern std::unordered_map<uint32_t, Desyncs> g_Desyncs;
	extern int g_SpoofedPeer;
	extern int g_NetworkMenuType;
	extern MisalignmentFix g_FunctionPatches[6];
	extern std::unordered_map<int, uint32_t> g_LastSyncedPlayerModels;
	extern std::vector<TranslationString*> g_TranslationTable;
	extern bool g_BypassProfanityChecks;
	extern uint64_t g_TextureBypass;
	extern std::vector<JoinTimeoutFix> g_JoinTimeoutFix;
	extern Rage::Network::NetGamePlayer* g_BrokenNodeSenders[3];
	extern BitBufferMisalinmentThread g_WriteArrayThreadInfo;
	extern BitBufferMisalinmentThread g_ReadNetworkEventThreadInfo;
	extern bool g_arxan_integ_calls[10];

	extern Rage::Script::ScriptTable* g_ScriptTable;
	extern Rage::Invoker::NativeRegistration** g_NativeRegistration;
	extern Rage::Network::GameCameraAngles* g_GameCameraAngles;
	extern Rage::Types::PedFactory* g_PedFactory;
	extern Rage::Types::ExplosionData* g_ExplosionData;
	extern Rage::Types::ExplosionFX* g_ExplosionFX;
	extern Rage::Types::BlipList* g_BlipList;
	extern Rage::Types::WaterTune* g_WaterTune;
	extern Rage::Types::OceanQuads g_OceanQuads;
	extern Rage::Types::UIWeather* g_UIWeather;
	extern Rage::Types::UIPuddle* g_UIPuddle;
	extern Rage::Types::UIVisualSettings* g_UIVisualSettings;
	extern Rage::Network::RsInfo* g_RsInfo;
	extern Rage::Network::Friends* g_Friends;
	extern Rage::Types::VfxWheel* g_VfxWheel;
	extern Rage::Types::VfxLiquid* g_VfxLiquid;
	extern Rage::Types::TxdStore* g_TextureStore;
	extern Rage::Types::WaypointData* g_WaypointData;
	extern Rage::Network::NetShopQueue* g_NetShopQueue;
	extern Rage::Network::NetworkBaseConfig* g_NetworkBaseConfig;

	extern GameState* g_GameState;
	extern uint64_t** g_GlobalCache;
	extern Rage::Types::EntityPool* g_EntityPool;
	extern Rage::Types::VehicleEntityPool* g_VehiclePool;
	extern Rage::Types::GenericPool* g_PedPool;
	extern Rage::Types::GenericPool* g_ObjectPool;
	extern Rage::Types::GenericPool* g_PickupPool;
	extern Rage::Types::GenericPool* g_AttachmentPool;
	extern Rage::Types::GenericPool* g_NetworkEventQueuePool;
	extern uint64_t* g_MainEntityPool;
	extern uint8_t g_VfxWheelCount;

	extern uint64_t g_NetworkObjectManager;
	extern uint64_t g_NetworkPlayerManager;
	extern uint64_t g_GetThirdPersonCamera;
	extern uint64_t g_ThirdPersonCamera;
	extern uint64_t g_GetEntityAddress;
	extern uint64_t g_GetEntityHandleFromAddress;
	extern uint64_t g_GetNetGamePlayerFromIndex;
	extern uint64_t g_GetModelInfo;
	extern uint64_t g_SetVehicleGravity;
	extern uint64_t g_GetVehiclePaintArray;
	extern uint64_t g_DrawOriginIndex;
	extern uint64_t g_AddToClockTime;
	extern uint64_t g_DispatchServiceTable;
	extern uint64_t g_SyncClockTime;
	extern uint64_t g_SendClockTime;
	extern uint64_t g_SendWeatherUpdate;
	extern uint64_t g_GetHostNetGamePlayer;
	extern uint64_t g_TriggerScriptEvent;
	extern uint64_t g_SendIncrementStatEvent;
	extern uint64_t g_UpdateNetObjectOwner;
	extern uint64_t g_HandleRotationValuesFromOrder;
	extern uint64_t g_GetNetGamePlayerFromHandle;
	extern uint64_t g_GetSpriteTexture;
	extern uint64_t g_SendTextMessageNetMsg;
	extern uint64_t g_SocialClubGetGameInfoTable;
	extern uint64_t g_SocialClubGameInfoTable;
	extern uint64_t g_SetTrafficLights;
	extern uint64_t g_UpdateTrafficLights;
	extern uint64_t g_SetTrafficLightColors;
	extern uint64_t g_SetVehicleLights;
	extern uint64_t g_GetSyncTreeFromCloneType;
	extern uint64_t g_GetNetworkObjectFromNetworkID;
	extern uint64_t g_GetParticleInfo;
	extern uint64_t g_SendSessionInfoRequest;
	extern uint64_t g_GetSessionInfoFromGs;
	extern uint64_t g_JoinPlayerViaSessionInfo;
	extern uint64_t g_SendNetworkEventReply;
	extern uint64_t g_SendNetworkEventAck;
	extern uint64_t g_SetupGiveControlEvent;
	extern uint64_t g_SendFriendRequest;
	extern uint64_t g_SendGlobalChatMessage;
	extern uint64_t g_GlobalChatPtr;
	extern uint64_t g_GetClientPlayerKey;
	extern uint64_t g_RenderWaterOnMapIntensity;
	extern uint64_t g_MapFogIntensity;
	extern uint64_t g_SendPresenceEvent;
	extern uint64_t g_ScriptHandlerPoolCount;
	extern uint64_t g_ScriptHandlerPool;
	extern uint64_t g_MigrateScriptHost;
	extern uint64_t g_IsReportedForReason;
	extern uint64_t g_ChatInfo;
	extern uint64_t g_SendMatchmakingRequest;
	extern uint64_t g_ReadXmlString;
	extern uint64_t g_ReadXmlNodeString;
	extern uint64_t g_TaskLeaveGamer;
	extern uint64_t g_GetPeerFromNetGamePlayer;
	extern uint64_t g_GetNetworkConfigFromPeer;
	extern uint64_t g_NetworkBail;
	extern uint64_t g_UpdateUIValues1;
	extern uint64_t g_UpdateUIValues2;
	extern uint64_t g_SetVehicleNeonUI;
	extern uint64_t g_SetSeethrough;
	extern uint64_t g_AddBasketToQueue;
	extern uint64_t g_AddItemToBasket;
	extern uint64_t g_ProcessTransactionQueue;
	extern uint64_t g_PeerToken;
	extern uint64_t g_GetPeerToken;
	extern uint64_t g_ReadMsgNewHost;
	extern uint64_t g_CreateGameInvitePresence;
	extern uint64_t g_SendGameInvitePresence;
	extern uint64_t g_SendGameInvitePresenceTable;
	extern uint64_t g_ReadPickupFromBitBuffer;
	extern uint64_t g_RegisterTexture;
	extern uint64_t g_SendRagdollEvent;
	extern uint64_t g_GameStreamedScripts;
	extern uint64_t g_GetStreamedScript;
	extern uint64_t g_NetStubLogginInterface;

	extern uint64_t g_WeaponComponentHashListCount;
	extern uint64_t g_WeaponComponentHashList;
	extern std::vector<uint32_t> g_WeaponComponentHashes;
	extern uint64_t g_WeaponInfoListCount;
	extern uint64_t g_WeaponInfoList;

	extern uint64_t g_ReadBoolFromBitBuffer;
	extern uint64_t g_ReadUnsignedIntFromBitBuffer;
	extern uint64_t g_ReadUnsignedLongLongBuffer;
	extern uint64_t g_ReadIntFromBitBuffer;
	extern uint64_t g_ReadNetMsgHeaderFromBitBuffer;
	extern uint64_t g_ReadShortFromBitBuffer;
	extern uint64_t g_ReadStringFromBitBuffer;
	extern uint64_t g_ReadVector3FromBitBuffer;
	extern uint64_t g_ReadUintArrayFromBitBuffer;
	extern uint64_t g_WriteUintToBitBuffer;
	extern uint64_t g_SyncDataReaderVtable;
	extern uint64_t g_NodeFixCDynamicEntityGameStateDataNode;
	extern uint64_t g_NodeFixCVehicleGadgetDataNode;
	extern uint64_t g_NodeFixCVehicleHealthDataNode;
	extern uint64_t g_NodeFixCVehicleComponentReservationDataNode;
	extern uint64_t g_NodeFixCPedGameStateDataNode;
	extern uint64_t g_NodeFixCPedComponentReservationDataNode;
	extern uint64_t g_NodeFixCPedScriptGameStateDataNode;
	extern uint64_t g_NodeFixCPedTaskTreeDataNode;
	extern uint64_t g_NodeFixCPedInventoryDataNode;
	extern uint64_t g_NodeFixCPedTaskSequenceDataNode;
	extern uint64_t g_NodeFixCPlayerGameStateDataNode;
	extern uint64_t g_NodeFixCPickupCreationDataNode;
	extern uint64_t g_NodeFixCVehicleControlDataNode;
	extern uint64_t g_NodeFixCVehicleProximityMigrationDataNode;

	extern uint64_t g_NetworkEventRegistration;
}