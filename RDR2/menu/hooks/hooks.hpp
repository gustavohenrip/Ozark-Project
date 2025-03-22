#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"
#include "rage/invoker/native_invoker.hpp"
#include "rage/types/network_types.hpp"
#include "utils/memory/memory.hpp"
#include <SubAuth.h>

namespace menu::hooks {
	inline void(*originalWait)(rage::invoker::nativeContext*);
	void waitHook(rage::invoker::nativeContext* context);

	inline uint8_t(*originalGetPeerId)(uint64_t*);
	uint8_t getPeerIdHook(uint64_t*);
	
	inline void(*originalUpdateNetObj)(uint64_t, rage::network::netGamePlayer*, rage::network::netObject*, uint64_t, uint32_t*, uint16_t*, uint64_t, uint8_t);
	void updateNetObjHook(uint64_t, rage::network::netGamePlayer*, rage::network::netObject*, uint64_t, uint32_t*, uint16_t*, uint64_t, uint8_t);

	inline uint64_t(*originalSyncUserMadeObjects)(uint64_t, uint64_t, rage::network::netGamePlayer*, uint64_t);
	uint64_t syncUserMadeObjectsHook(uint64_t, uint64_t, rage::network::netGamePlayer*, uint64_t);

	inline uint64_t(*originalProcessJoinLeave)(uint8_t*, rage::network::netGamePlayer*, uint32_t, uint8_t);
	uint64_t processJoinLeaveHook(uint8_t*, rage::network::netGamePlayer*, uint32_t, uint8_t);

	inline void(*originalIsDlcPresent)(rage::invoker::nativeContext*);
	void isDlcPresentHook(rage::invoker::nativeContext*);

	inline const char*(*orginalGetGxtText)(uint64_t, const char*);
	const char* getGxtTextHook(uint64_t ptr, const char* label);

	inline bool(*originalScProfanityCheckString)(const char*, int*);
	bool scProfanityCheckStringHook(const char* str, int* token);

	inline void(*originalScriptImRenderPhase)();
	void scriptImRenderPhaseHook();

	inline bool(*originalSyncDataString)(uint64_t, const char*, int);
	bool syncDataStringHook(uint64_t stream, const char* data, int length);

	inline bool(*originalSyncPlayerInfo)(rage::network::netPlayerInfo*, uint64_t, uint32_t, uint64_t);
	bool syncPlayerInfoHook(rage::network::netPlayerInfo* rcx, uint64_t rdx, uint32_t r8d, uint64_t r9);

	inline bool (*originalSyncPlayerInfoMsg)(uint64_t, rage::network::netMsgPlayer*);
	bool syncPlayerInfoMsgHook(uint64_t rcx, rage::network::netMsgPlayer* rdx);

	inline bool(*originalSyncDataQword)(uint64_t, uint64_t);
	bool syncDataQwordHook(uint64_t stream, uint64_t data);

	inline bool(*originalRequestControlEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool requestControlEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalGameClockEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool gameClockEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalGameWeatherEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool gameWeatherEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalFireEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool fireEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalExplosionEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool explosionEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalNetworkClearPedTasksEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool networkClearPedTasksEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalNetworkIncrementStatEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool networkIncrementStatEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalNetworkPTFXEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool networkPTFXEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline uint64_t(*originalSendWeatherEvent)(uint64_t, uint64_t);
	uint64_t sendWeatherEventHook(uint64_t, uint64_t);

	inline uint8_t(*originalWriteRespawnPlayerPedEvent)(uint64_t, uint8_t*);
	uint8_t writeRespawnPlayerPedEvent(uint64_t, uint8_t*);

	//unsigned int a1, __int64 a2, unsigned int a3, __int64 a4, int a5
	inline uint8_t(*originalCashInventoryAdditem)(uint32_t, uint64_t, uint32_t, uint64_t, int32_t);
	uint8_t cashInventoryAddItem(uint32_t, uint64_t, uint32_t, uint64_t, int32_t);


	inline uint8_t(*originalCheckPlayerData)(uint64_t, rage::network::netGamePlayer*);
	uint8_t checkPlayerDataHook(uint64_t, rage::network::netGamePlayer*);

	inline uint64_t(*originalCommandEventSend)(uint64_t, uint64_t, uint64_t);
	uint64_t commandEventSend(uint64_t, uint64_t, uint64_t);

	inline bool(*originalReportCashSpawnEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool reportCashSpawnEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalGivePickupRewardsEvent)(uint64_t, rage::network::netGamePlayer*, uint64_t);
	bool givePickupRewardsEventHook(uint64_t netEvent, rage::network::netGamePlayer* sender, uint64_t r8);

	inline bool(*originalTargetPickupRewardsEvent)(uint64_t, rage::network::netGamePlayer*);
	bool targetPickuprewardsHook(uint64_t rcx, rage::network::netGamePlayer* reciever);

	inline bool(*originalSyncNetworkObject)(uint64_t, rage::network::netObject*);
	bool syncNetworkObject(uint64_t tree, rage::network::netObject* netObj);

	inline bool(*originalRecieveCloneCreate)(uint64_t, rage::network::netGamePlayer*, uint16_t, uint16_t, uint16_t, uint64_t, uint64_t, unsigned int, std::uint32_t);
	bool recieveCloneCreateHook(uint64_t this_, rage::network::netGamePlayer* sender, uint16_t objectType, uint16_t objectID, uint16_t objectFlags, uint64_t unk, uint64_t bitBuffer, unsigned int shit, std::uint32_t timestamp);

	inline uint64_t(*originalDeletePed)(Ped* ped);
	uint64_t deletePedHook(Ped* ped);

	inline uint64_t*(*originalDoesEntityExist)(int32_t ped);
	uint64_t* doesEntityExist(int32_t ped);

	inline void(*originalSpawnCrashMessageBox)(int, int, uint32_t, bool);
	void spawnCrashMessageBoxHook(int error, int fileHash, uint32_t fileLine, bool terminate);

	inline uint64_t(*originalGetSpriteDataFromHash)(uint32_t, uint32_t);
	uint64_t getSpriteDataFromHashHook(uint32_t first, uint32_t second);

	inline uint64_t(*originalGetSpriteDataFromName)(const char*, const char*);
	uint64_t getSpriteDataFromNameHook(const char* first, const char* second);

	inline void(*originalRequestScript)(rage::invoker::nativeContext*);
	void requestScriptHook(rage::invoker::nativeContext* context);

	inline bool(*originalHandlePresenceEvent)(uint64_t, uint64_t, presenceData*, uint64_t);
	bool handlePresenceEventHook(uint64_t rcx, uint64_t rdx, presenceData* r8, uint64_t r9);

	inline bool(*originalPresJoinRequest)(joinRequestData*, uint64_t);
	bool presJoinRequestHook(joinRequestData* rcx, uint64_t rdx);

	inline bool (*originalInviteResponse)(joinResponseData*, uint64_t);
	bool inviteResponseHook(joinResponseData* rcx, uint64_t rdx);

	inline const char* (*originalAppendStringWithResolvedTag)(uint32_t, int, const char**);
	const char* appendStringWithResolvedTagHook(uint32_t tag, int unk, const char** string);

	inline bool(*originalReadSessionInfoResponse)(uint64_t, int*);
	bool readSessionInfoResponseHook(uint64_t rcx, int* edx);

	inline bool(*originalProcessRelayUsage)(uint64_t, rage::network::relayUser*, rage::network::relayNetwork*);
	bool processRelayUsageHook(uint64_t rcx, rage::network::relayUser* relay, rage::network::relayNetwork* network);

	// crash hooks
	inline void(*originalWeatherCrash)(uint64_t, uint32_t, uint32_t, float);
	void weatherCrashHook(uint64_t rcx, uint32_t edx, uint32_t r8d, float xmm0);

	inline uint64_t(*originalGetPoolIndex)(entityPool*, int);
	uint64_t getPoolIndexHook(entityPool* decryptedPool, int index);

	inline bool (*originalGatlingAttachCrash)(uint64_t);
	bool gatlingAttachCrashHook(uint64_t rcx);

	inline void (*originalGatlingDetachmentCrash)(uint64_t, uint16_t, uint64_t);
	void gatlingDetachmentCrashHook(uint64_t trailer, uint16_t flag, uint64_t r8);

	inline void (*originalUnbindAttachmentExtension)(uint64_t, uint8_t, uint8_t, uint64_t);
	void unbindAttachmentExtensionHook(uint64_t target, uint8_t dl, uint8_t r8b, uint64_t Override);

	// anticheat
	inline bool(*originalDeleteNetworkEvent)(uint64_t, bool);
	bool deleteNetworkEventHook(uint64_t eventPool, bool deleteNow);

	inline NTSTATUS(*originalNtQueryVirtualMemory)(HANDLE, void*, int, void*, SIZE_T, SIZE_T*);
	NTSTATUS ntQueryVirtualMemoryHook(HANDLE processHandle, void* baseAddress, int memoryInformationClass, void* memoryInformation, SIZE_T memoryInformationLength, SIZE_T* returnLength);

	inline void(*originalProcessMetric)(uint64_t, uint64_t);
	void processMetricHook(uint64_t unk, uint64_t metric);

	inline void(*originalProcessMetric2)(uint64_t, uint64_t);
	void processMetric2Hook(uint64_t unk, uint64_t metric);

	inline const char*(*originalConstructRequestUrl)(uint64_t, char*, int);
	const char* constructRequestUrlHook(uint64_t thisPtr, char* buffer, int size);

	inline DWORD(*originalGetModuleFileNameA)(HMODULE, const char*, DWORD);
	DWORD getModuleFileNameAHook(HMODULE hModule, const char* filename, DWORD size);

	inline DWORD(*originalGetModuleFileNameW)(HMODULE, const wchar_t*, DWORD);
	DWORD getModuleFileNameWHook(HMODULE hModule, const wchar_t* filename, DWORD size);

	// scripthook
	inline FARPROC(*originalGetProcAddress)(HMODULE, LPCSTR);
	FARPROC getProcAddressHook(HMODULE moduleHandle, LPCSTR functionName);
}