#pragma once
#include "rage/types/invoker_types.hpp"
#include "rage/types/base_types.hpp"
#include "rage/types/script_types.hpp"
#include "rage/types/network_types.hpp"
#include "utils/json.hpp"

namespace global::vars {
	// ozark
	extern uint64_t g_menuAddress;
	extern uint64_t g_menuSize;
	extern HMODULE g_menuHandle;
	extern std::vector<BYTE> m_menuPE;
	extern uint64_t g_gameAddress;
	extern uint64_t g_gameSize;
	extern HWND g_gameWindow;
	extern HMODULE g_ntdll;
	extern HMODULE g_socialclub;
	extern bool g_windows7;
	extern bool g_unloading;
	extern std::vector<uint32_t> g_geoQueue;
	extern std::unordered_map<uint32_t, geoIP> g_geo;
	extern std::vector<uint64_t> g_rockstarAccountsQueue;
	extern std::unordered_map<uint64_t, rage::network::rockstarAccount> g_rockstarAccounts;
	extern std::unordered_map<uint64_t, uint32_t> g_relayUsage; // rid, ip
	extern uint64_t g_gamesProcessHandle;
	extern HANDLE g_explorerHandle;
	extern bool g_blockPedMetric;
	extern Ped g_spoofLocationPed;
	extern bool g_showingNotifyMessage;
	extern std::string g_notifyMessage;
	extern std::unordered_map<uint64_t, bool> g_onlineRockstarIds;
	extern std::vector<uint64_t> g_onlineRockstarIdQueue;

	// invoker
	extern rage::invoker::nativeRegistration** g_nativeRegistration;

	// addresses
	extern uint64_t g_getHostNetGamePlayer;								// 0x14224311C
	extern uint64_t g_changeNetObjOwner;								// 0x142B46034
	extern uint64_t g_netObjMgr;										// 0x145977420
	extern uint64_t g_getEntityAddress;									// 0x142A56E10
	extern uint64_t g_getEntityHandleFromAddress;						// 0x142A55380
	extern uint64_t g_getNetGamePlayerFromIndex;						// 0x142243E70
	extern uint64_t g_getListPoolElementFromIndex;						// 0X14035681C
	extern uint64_t g_migrateScriptHost;								// 0x140ECC52C
	extern uint64_t g_getModelInfo;										// 0x142B2BAEC
	extern uint64_t g_currentFrameCount;								// 0x14581DAB4
	extern uint64_t g_getIndexFromHashList;
	extern uint64_t** g_globalPointer;
	extern uint64_t g_drawPoints;
    extern uint64_t g_triggerPresence;
	extern uint64_t g_triggerClockEvent;
	extern uint64_t g_triggerWeatherEvent;
	extern uint64_t g_weatherTable;
	extern uint64_t g_networkPlayerMgr;
	extern uint64_t g_networkClockHourPatch;
	extern uint64_t g_gameInfoItemTable;
	extern uint64_t g_getGameInfoItem;
	extern uint64_t g_getWeaponInfo;
	extern uint64_t g_worldToScreen;
	extern uint64_t g_rockstarTicketInfo;
	extern uint64_t g_showStackedNotifyMessage;
	extern uint64_t g_joinPlayer;
	extern uint64_t g_joinPlayerTable;
	extern uint64_t g_joinPlayerU;
	extern uint64_t g_sendSessionInfoRequest;
	extern uint64_t g_friendData;
	extern uint64_t g_scriptThreadListSize;
	extern uint64_t g_scriptThreadList;
	extern uint64_t g_joinOrHostSession;
	extern rage::script::scriptTable* g_scriptTable;
	extern watertune* wtune;
	extern vehiclePopulationTuning* vehiclePopTuning;

	// ui
	extern uint64_t g_textWrapMin;										// 0x14396F474
	extern uint64_t g_textWrapMax;										// 0x14396F478
	extern uint64_t g_textFont;											// 0x14396F47C
	extern uint64_t g_textAlign;										// 0x14396F484
	extern uint64_t g_textOutline;										// 0x14396F487

	// misc
	extern uint64_t g_visionHashList;
	extern uint64_t g_timecycleIndex;
	extern uint64_t g_weaponComponentHashListCount;
	extern uint64_t g_weaponComponentHashList;
	extern uint64_t g_weaponHashListCount;
	extern uint64_t g_weaponHashList;
	extern uint64_t g_nativeTableEncryption;
	extern std::vector<std::pair<DWORD, int>> g_visionHashes;
	extern std::vector<DWORD> g_weaponComponentHashes;
	extern std::vector<DWORD> g_weaponHashes;
	extern encryptedPool g_encryptedPedPool;
	extern encryptedPool g_encryptedVehiclePool;
	extern encryptedPool g_encryptedPickupPool;
	extern encryptedPool g_encryptedObjectPool;
	extern encryptedPool g_encryptedEntityPool;
	extern encryptedPool g_encryptedAttachmentPool;
	extern encryptedPool g_encryptedNetworkEventQueuePool;
	extern encryptedPool g_encryptedPedFactory;
	extern uint64_t g_offTheRadarFlag;
	extern uint64_t g_inventoryPool;
	extern uint64_t g_getInventoryList;
	extern uint64_t g_infiniteItems;
	extern bool g_infiniteTreasureMaps;
	extern bool g_changeModel;
	extern bool g_moonshinerBooster;

	// anticheat
	extern uint64_t g_deleteNetworkEventReportCashSpawn;
	extern uint64_t g_deleteNetworkEventChestDataChange;
	extern uint64_t g_deleteNetworkEventChestRelease;
	extern uint64_t ntQueryVirtualMemoryLoc;

	// crash vars
	extern std::string g_syncCrashPlayerName;
	extern Entity g_syncCrashEntity;
	extern rage::network::netGamePlayer* g_pickupRewardsNetGamePlayer;

	extern uint64_t g_rewardPickupsEvent;
}