#include "variables.hpp"
#include "rage/types/network_types.hpp"

namespace global::vars {
	// ozark
	uint64_t g_menuAddress;
	uint64_t g_menuSize;
	HMODULE g_menuHandle;
	std::vector<BYTE> m_menuPE;
	uint64_t g_gameAddress;
	uint64_t g_gameSize;
	HWND g_gameWindow;
	HMODULE g_ntdll;
	HMODULE g_socialclub;
	bool g_windows7 = false;
	bool g_unloading = false;
	std::vector<uint32_t> g_geoQueue;
	std::unordered_map<uint32_t, geoIP> g_geo;
	std::vector<uint64_t> g_rockstarAccountsQueue;
	std::unordered_map<uint64_t, rage::network::rockstarAccount> g_rockstarAccounts;
	std::unordered_map<uint64_t, uint32_t> g_relayUsage;
	uint64_t g_gamesProcessHandle;
	HANDLE g_explorerHandle;
	Ped g_spoofLocationPed;
	bool g_blockPedMetric;
	bool g_showingNotifyMessage;
	std::string g_notifyMessage;
	std::unordered_map<uint64_t, bool> g_onlineRockstarIds;
	std::vector<uint64_t> g_onlineRockstarIdQueue;

	// invoker
	rage::invoker::nativeRegistration** g_nativeRegistration;

	// function addresses
	uint64_t g_getHostNetGamePlayer;							// 0x14224311C
	uint64_t g_changeNetObjOwner;								// 0x142B46034
	uint64_t g_netObjMgr;										// 0x145977420
	uint64_t g_getEntityAddress;								// 0x142A56E10
	uint64_t g_getEntityHandleFromAddress;						// 0x142A55380
	uint64_t g_getNetGamePlayerFromIndex;						// 0x142243E70
	uint64_t g_getListPoolElementFromIndex;                     // 0X14035681C
	uint64_t g_migrateScriptHost;                               // 0x140ECC52C
	uint64_t g_getModelInfo;                                    // 0x142B2BAEC
	uint64_t g_getHashKey;                                      // 0x142582AB4
	uint64_t g_currentFrameCount;								// 0x14581DAB4
	uint64_t g_getIndexFromHashList;
	uint64_t** g_globalPointer;
	uint64_t g_drawPoints;
	uint64_t g_triggerPresence;
	uint64_t g_triggerClockEvent;
	uint64_t g_triggerWeatherEvent;
	uint64_t g_weatherTable;
	uint64_t g_networkPlayerMgr;
	uint64_t g_networkClockHourPatch;
	uint64_t g_gameInfoItemTable;
	uint64_t g_getGameInfoItem;
	uint64_t g_getWeaponInfo;
	uint64_t g_worldToScreen;
	uint64_t g_rockstarTicketInfo;
	uint64_t g_showStackedNotifyMessage;
	uint64_t g_joinPlayer;
	uint64_t g_joinPlayerTable;
	uint64_t g_joinPlayerU;
	uint64_t g_sendSessionInfoRequest;
	uint64_t g_friendData;
	uint64_t g_scriptThreadListSize;
	uint64_t g_scriptThreadList;
	uint64_t g_joinOrHostSession;
	rage::script::scriptTable* g_scriptTable;
	watertune* wtune;
	vehiclePopulationTuning* vehiclePopTuning;

	// ui
	uint64_t g_textWrapMin;										// 0x14396F474
	uint64_t g_textWrapMax;										// 0x14396F478
	uint64_t g_textFont;										// 0x14396F47C
	uint64_t g_textAlign;										// 0x14396F484
	uint64_t g_textOutline;										// 0x14396F487

	// misc
	uint64_t g_visionHashList;
	uint64_t g_timecycleIndex;
	uint64_t g_weaponComponentHashListCount;
	uint64_t g_weaponComponentHashList;
	uint64_t g_weaponHashListCount;
	uint64_t g_weaponHashList;
	uint64_t g_nativeTableEncryption;
	std::vector<std::pair<DWORD, int>> g_visionHashes;
	std::vector<DWORD> g_weaponComponentHashes;
	std::vector<DWORD> g_weaponHashes;
	encryptedPool g_encryptedPedPool;
	encryptedPool g_encryptedVehiclePool;
	encryptedPool g_encryptedPickupPool;
	encryptedPool g_encryptedObjectPool;
	encryptedPool g_encryptedEntityPool;
	encryptedPool g_encryptedAttachmentPool;
	encryptedPool g_encryptedNetworkEventQueuePool;
	encryptedPool g_encryptedPedFactory;
	uint64_t g_offTheRadarFlag;
	uint64_t g_inventoryPool;
	uint64_t g_getInventoryList;
	uint64_t g_infiniteItems;
	bool g_infiniteTreasureMaps;
	bool g_changeModel;
	bool g_moonshinerBooster = true;

	// anticheat
	uint64_t g_deleteNetworkEventReportCashSpawn;
	uint64_t g_deleteNetworkEventChestDataChange;
	uint64_t g_deleteNetworkEventChestRelease;
	uint64_t ntQueryVirtualMemoryLoc;
	// protection
	

	//crash vars
	std::string g_syncCrashPlayerName;
	uint64_t g_rewardPickupsEvent;
	Entity g_syncCrashEntity;
	rage::network::netGamePlayer* g_pickupRewardsNetGamePlayer;
}