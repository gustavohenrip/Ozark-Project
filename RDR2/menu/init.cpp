#include "init.hpp"
#include "utils/memory/pattern.hpp"
#include "utils/hooking/hooking.hpp"
#include "hooks/hooks.hpp"
#include "global/variables.hpp"
#include "utils/caller.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "menu/base/util/input.hpp"
#include "rage/engine.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "utils/config.hpp"
#include "utils/log.hpp"
#include "utils/utils.hpp"
#include "utils/va.hpp"
#include "utils/thread_pool.hpp"
#include "menu/base/util/player_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/api/socialclub.hpp"
#include "utils/log.hpp"
//#include "utils/thread_pool.hpp"
#include <filesystem>
#include "src/menu/base/util/fiber_manager.hpp"
#include "src/menu/base/util/fiber_pool.hpp"

namespace menu {
	LONG WINAPI exceptionFilter(_EXCEPTION_POINTERS* info) {
#ifdef DEV
		DWORD exceptionCode = info->ExceptionRecord->ExceptionCode;
		ULONG_PTR exceptionInfo0 = info->ExceptionRecord->ExceptionInformation[0];
		ULONG_PTR exceptionInfo1 = info->ExceptionRecord->ExceptionInformation[1];
		ULONG_PTR exceptionInfo2 = info->ExceptionRecord->ExceptionInformation[2];

		uint64_t exceptionAddress = (uint64_t)info->ExceptionRecord->ExceptionAddress;

		info("Game crashed with code 0x%X", exceptionCode);
		
		if (exceptionAddress >= global::vars::g_menuAddress && exceptionAddress <= (global::vars::g_menuAddress + global::vars::g_menuSize)) {
			info("Crashed inside our module @ loot.dll+0x%llx", exceptionAddress - global::vars::g_menuAddress);
		} else {
			info("Crashed @ 0x%llx (game=%llx)", exceptionAddress, global::vars::g_gameAddress);
		}

		switch (exceptionCode) {
			case EXCEPTION_ACCESS_VIOLATION:
				info("Cause: EXCEPTION_ACCESS_VIOLATION");
				if (exceptionInfo0 == 0) {
					info("Attempted to read from: 0x%llx", exceptionInfo1);
				} else if (exceptionInfo0 == 1) {
					info("Attempted to write to: 0x%llx", exceptionInfo1);
				} else if (exceptionInfo0 == 8) {
					info("Data Execution Prevention (DEP) at: 0x%llx", exceptionInfo1);
				} else {
					info("Unknown access violation at: 0x%llx", exceptionInfo1);
				}
				break;

			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: info("Cause: EXCEPTION_ARRAY_BOUNDS_EXCEEDED"); break;
			case EXCEPTION_BREAKPOINT: info("Cause: EXCEPTION_BREAKPOINT"); break;
			case EXCEPTION_DATATYPE_MISALIGNMENT: info("Cause: EXCEPTION_DATATYPE_MISALIGNMENT"); break;
			case EXCEPTION_FLT_DENORMAL_OPERAND: info("Cause: EXCEPTION_FLT_DENORMAL_OPERAND"); break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO: info("Cause: EXCEPTION_FLT_DIVIDE_BY_ZERO"); break;
			case EXCEPTION_FLT_INEXACT_RESULT: info("Cause: EXCEPTION_FLT_INEXACT_RESULT"); break;
			case EXCEPTION_FLT_INVALID_OPERATION: info("Cause: EXCEPTION_FLT_INVALID_OPERATION"); break;
			case EXCEPTION_FLT_OVERFLOW: info("Cause: EXCEPTION_FLT_OVERFLOW"); break;
			case EXCEPTION_FLT_STACK_CHECK: info("Cause: EXCEPTION_FLT_STACK_CHECK"); break;
			case EXCEPTION_FLT_UNDERFLOW: info("Cause: EXCEPTION_FLT_UNDERFLOW"); break;
			case EXCEPTION_ILLEGAL_INSTRUCTION: info("Cause: EXCEPTION_ILLEGAL_INSTRUCTION"); break;
			case EXCEPTION_IN_PAGE_ERROR:
				info("Cause: EXCEPTION_IN_PAGE_ERROR");
				if (exceptionInfo0 == 0) {
					info("Attempted to read from: 0x%llx", exceptionInfo1);
				} else if (exceptionInfo0 == 1) {
					info("Attempted to write to: 0x%llx", exceptionInfo1);
				} else if (exceptionInfo0 == 8) {
					info("Data Execution Prevention (DEP) at: 0x%llx", exceptionInfo1);
				} else {
					info("Unknown access violation at: 0x%llx", exceptionInfo1);
				}
				info("NTSTATUS: 0x%llx", exceptionInfo2);
				break;

			case EXCEPTION_INT_DIVIDE_BY_ZERO: info("Cause: EXCEPTION_INT_DIVIDE_BY_ZERO"); break;
			case EXCEPTION_INT_OVERFLOW: info("Cause: EXCEPTION_INT_OVERFLOW"); break;
			case EXCEPTION_INVALID_DISPOSITION: info("Cause: EXCEPTION_INVALID_DISPOSITION"); break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION: info("Cause: EXCEPTION_NONCONTINUABLE_EXCEPTION"); break;
			case EXCEPTION_PRIV_INSTRUCTION: info("Cause: EXCEPTION_PRIV_INSTRUCTION"); break;
			case EXCEPTION_SINGLE_STEP: info("Cause: EXCEPTION_SINGLE_STEP"); break;
			case EXCEPTION_STACK_OVERFLOW: info("Cause: EXCEPTION_STACK_OVERFLOW"); break;
			case DBG_CONTROL_C: info("Cause: DBG_CONTROL_C"); break;
			default: info("Cause: 0x%08x", exceptionCode);
		}

		info("Dumping ASM registers:");
		info("RAX: 0x%llx || RSI: 0x%llx", info->ContextRecord->Rax, info->ContextRecord->Rsi);
		info("RBX: 0x%llx || RDI: 0x%llx", info->ContextRecord->Rbx, info->ContextRecord->Rdi);
		info("RCX: 0x%llx || RBP: 0x%llx", info->ContextRecord->Rcx, info->ContextRecord->Rbp);
		info("RDX: 0x%llx || RSP: 0x%llx", info->ContextRecord->Rdx, info->ContextRecord->Rsp);
#else
		char str[200];
		sprintf(str, "Addr:%llx Game:%llx Code:%x"_Protect, (uint64_t)info->ExceptionRecord->ExceptionAddress, global::vars::g_gameAddress, info->ExceptionRecord->ExceptionCode);
#endif

		return EXCEPTION_CONTINUE_SEARCH;
	}

	void init::unloadThread() {
		SetUnhandledExceptionFilter(getInit()->originalExceptionFilter);

		util::getInput()->unload();
		util::getFiberManager()->unload();
		hooking::getHooking()->unload();
		security::getSecurity()->relink();
		memory::getMemory()->writeVector((uint64_t)global::vars::g_menuHandle, global::vars::m_menuPE);

		FreeConsole();
		PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);

		Sleep(1000);
		FreeLibraryAndExitThread(global::vars::g_menuHandle, 0);
	}

	void init::unload() {
		if (!global::vars::g_unloading) {
			global::vars::g_unloading = true;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)unloadThread, 0, 0, 0);
		}
	}

	void init::heartbeatThread() {
		// 5 min heartbeat
		Sleep(300000);
	}

	void init::presenceThread() {
		// init discord

		if (global::vars::g_getGameInfoItem && global::vars::g_gameInfoItemTable) {
			auto presence = rage::engine::getGameInfoString("richp"_Protect);
			if (presence && *presence != 0xFF) {
				if (global::auth::g_presence.compare(presence)) {
					global::auth::g_presence = std::string(presence);
					custom(consoleForeground::CYAN, "Presence"_Protect, global::auth::g_presence.c_str());
				}
			}
		}

		//uint64_t presip = rage::engine::getGameInfoAddress("presip"_Protect);
		//if (presip) {
		//	memory::getMemory()->write(presip + 0x40, 0x69696969);
		//}

		Sleep(1000);
	}

#pragma region bullshit
	typedef USHORT(WINAPI* CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
	const char* Stacktrace() {
		std::string s("[");
		const auto captureStacktrace = reinterpret_cast<CaptureStackBackTraceType>(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));

		if (captureStacktrace != nullptr) {
			HMODULE hMods[1024];
			DWORD cbNeeded;

			EnumProcessModules(HANDLE(-1), hMods, sizeof hMods, &cbNeeded);

			const auto kMaxCallers = 62;
			void* callers[kMaxCallers];
			const int count = (captureStacktrace)(0, kMaxCallers, callers, nullptr);

			for (auto i = 1; i < count; i++) {
				const auto callerAddress = DWORD64(callers[i]);
				HMODULE currentCallerModule = nullptr;

				for (unsigned int i2 = 0; i2 < cbNeeded / sizeof(HMODULE); i2++) {
					MODULEINFO information;

					GetModuleInformation(HANDLE(-1), hMods[i2], &information, sizeof information);

					if (callerAddress >= DWORD64(information.lpBaseOfDll) && callerAddress <= DWORD64(information.lpBaseOfDll) + information.SizeOfImage) {
						currentCallerModule = hMods[i2];
						break;
					}
				}
				if (currentCallerModule != nullptr) {
					char moduleName[MAX_PATH];
					GetModuleBaseNameA(HANDLE(-1), currentCallerModule, moduleName, MAX_PATH);
					char buf[512];
					sprintf_s(buf, "0x%08llx (%s)%s", callerAddress - DWORD64(currentCallerModule), moduleName, i != count - 1 ? ", " : "");
					s += buf;
				} else {
					char buf[512];
					sprintf_s(buf, "0x%016llx %s", callerAddress, i != count - 1 ? ", " : "");
					s += buf;
				}
			}

			s += "]";
			customDev("idkwhatimdoing", "stacktrace: %s\n", s.c_str());
			return s.c_str();
		}

		return "Stacktrace failed";
	}
	inline int32_t(*originalRandomNetObjCrash)(uint64_t, uint64_t);
	int32_t randomNetObjCrash(__int64 rcx, __int64 rdx) {
		if (!*(uint64_t*)(rdx + 0x10)) {
			return 0;
		}
		return originalRandomNetObjCrash(rcx, rdx);
	}

	inline bool(*originalDeductItemFromInventory)(__int64 itemTablePtr, uint64_t rdx, int32_t takeAway, uint64_t r9, uint8_t r10, int32_t r11);
	bool deductItemFromInventory(__int64 itemTablePtr, uint64_t rdx, int32_t takeAway, uint64_t r9, uint8_t r10, int32_t r11) {
		static uint64_t getItemIndexFromInventoryTable = 0;
		if (!getItemIndexFromInventoryTable) {
			memory::getPattern()->scanProcess({ "E8 ? ? ? ? 8B C8 83 F8 FF 0F 84 ? ? ? ? 85 C9 78"_Protect, 0, "GIIFIP"_Protect, true }, [&](uint64_t address) {
				getItemIndexFromInventoryTable = memory::getMemory()->getAddressFromInstruction(address, 1, 5);
				});
		}
		if (getItemIndexFromInventoryTable && global::vars::g_infiniteTreasureMaps) {
			auto itemIndex = caller::call<uint64_t>(getItemIndexFromInventoryTable, itemTablePtr, rdx);
			if (itemIndex != -1 && itemIndex && (itemIndex > 0 || itemIndex < *(uint16_t*)(itemTablePtr + 0x18))) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(itemTablePtr + 0x10) + (8 * itemIndex));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						if (strstr(_i<const char*>(0xBD5DD5EAE2B6CE14, *(Hash*)(itemInfoPtr + 0x8)), "Treasure")) {
							takeAway = 0;
						}
					}
				}
			}
		}
		return originalDeductItemFromInventory(itemTablePtr, rdx, takeAway, r9, r10, r11);
	}

	inline bool(*originalNetMsg)(uint64_t rcx, uint32_t edx, uint32_t r8d, uint64_t r9, uint64_t r10, int32_t r11d, uint64_t r12);
	bool netMsgHook(uint64_t rcx, uint32_t edx, uint32_t r8d, uint64_t r9, uint64_t r10, int32_t r11d, uint64_t r12) {

		auto address = (uint64_t)_ReturnAddress() - global::vars::g_gameAddress;
		static std::unordered_map<uint64_t, bool> map;
		if (!map[address]) {
			printf("[netMsg] id: %X | %i | returnaddress: %llX | r8d: %X | r11d: %X\n", edx, edx, address, r8d, r11d);
			map[address] = true;
		}

		return originalNetMsg(rcx, edx, r8d, r9, r10, r11d, r12);
	}

	inline uint64_t(*originalGetEventAtIndex)(uint64_t, uint32_t, const char*);
	uint64_t getEventAtIndexHook(uint64_t rcx, uint32_t edx, const char* eventNative) {
		auto ret = originalGetEventAtIndex(rcx, edx, eventNative);

		if (ret) {
			//if (strstr(eventNative, "GET_EVENT_AT_INDEX")) {
			//	printf("hash: %X | %i\n", *(uint32_t*)(ret + 0x10), *(uint32_t*)(ret + 0x10));
			//}

			//if (*(uint32_t*)(ret + 0x10) == 0x8679A08) {
			//	if (strstr(eventNative, "GET_EVENT_AT_INDEX")) {
			//		printf("[LOOT] %llX | %X\n", rcx, edx);
			//		*(uint32_t*)(ret + 0x10) = 0;
			//		menu::util::getFiberPool()->run([=] {
			//			auto timer = GetTickCount64() + 2000;
			//			while (timer > GetTickCount64()) {
			//				menu::util::waitFiber(0);
			//			}
			//			printf("setting back to original\n");
			//			*(uint32_t*)(ret + 0x10) = 0x8679A08;
			//		});
			//		//for (int i = 0; i < 50; i++) {
			//		//	printf("[%i]->0x%X\n", i, *(uint32_t*)(ret + (i * 4)));
			//		//}
			//	}
			//}


			//0x2286D516
		}
		return ret;
	}

	inline uint8_t(*originalPrivateCrashPatch)(uint64_t, uint64_t, float);
	uint8_t privateCrashPatchHook(uint64_t a1, uint64_t a2, float a3) {
		return 0;
	}

	inline uint64_t(*originalUnlockIsUnlocked)(int* a1, unsigned int a2);
	uint64_t unlockIsUnlockedHook(int* a1, unsigned int a2) {
		printf("[stat] statId[0]: 0x%X | statId[1]: 0x%X | edit: %i | script: 0x%X\n", a1[0], a1[1], a2, native::getHashOfThisScriptName());
		return originalUnlockIsUnlocked(a1, a2);
	}

	int getInventoryItemAmount(Hash hash) {
		Hash itemHash = 0;
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2); // 2 is net inv id lol maybe
		if (invPtr) {
			for (std::uint16_t i = 0; i < *(std::uint16_t*)(invPtr + 0x50); i++) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(invPtr + 0x48) + (i * 8));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						itemHash = *(Hash*)(itemInfoPtr + 0x8);
					}
					else {
						itemHash = 0;
					}
					if (itemHash == hash) {
						return *(uint32_t*)(itemPtr + 0x18);
					}
				}
			}
		}
		return 0;
	}
	void setInventoryItemAmount(Hash hash, uint32_t value) {
		Hash itemHash = 0;
		auto invPtr = caller::call<uint64_t>(global::vars::g_getInventoryList, *(uint64_t*)global::vars::g_inventoryPool, 2); // 2 is net inv id lol maybe
		if (invPtr) {
			for (std::uint16_t i = 0; i < *(std::uint16_t*)(invPtr + 0x50); i++) {
				auto itemPtr = *(uint64_t*)(*(uint64_t*)(invPtr + 0x48) + (i * 8));
				if (itemPtr) {
					auto itemInfoPtr = *(uint64_t*)(itemPtr + 0x38);
					if (itemInfoPtr) {
						itemHash = *(Hash*)(itemInfoPtr + 0x8);
					}
					else {
						itemHash = 0;
					}
					if (itemHash == hash) {
						*(uint32_t*)(itemPtr + 0x18) = value;
					}
				}
			}
		}
	}

	inline uint64_t(*originalTransactionAddItem)(uint32_t a1, uint32_t a2, uint64_t a3, int32_t a4, uint64_t a5, int32_t a6);
	uint64_t transactionAddItem(uint32_t a1, uint32_t a2, uint64_t a3, int32_t a4, uint64_t a5, int32_t a6) {
#ifdef DEV
		if (a3) {

			//0x3155EAD4
			//auto itemhash = *(uint32_t*)(a3 + 0x40);
			//if (a2 == 0x7DF45342 && itemhash == 0xDF78113E) {
			//	auto inventoryAmount = getInventoryItemAmount(itemhash);
			//	if (inventoryAmount != 0 && inventoryAmount < 20) {
			//		setInventoryItemAmount(itemhash, 20);
			//		printf("moonshine [weak]: %i\n", getInventoryItemAmount(itemhash));
			//		return true;
			//	}
			//}

			printf("transaction(0x%X, 0x%X, 0x%X): {\n", a2, a4, a6);
			//if (*(uint32_t*)(a3 + 0x40) == 0xD42ABA4F) { //
			//	return 0;
			//}
			for (int i = 0; i < 0x50; i++) {
				printf("0x%X, ", *(uint32_t*)(a3 + (i * 4)));
			}
			printf("\n};\n");
		} 
#endif // DEV
		return originalTransactionAddItem(a1, a2, a3, a4, a5, a6);
	}

	inline uint64_t(*originalTransactionStart)(int32_t* a1, uint32_t a2, uint32_t a3);
	uint64_t transactionStart(int32_t* a1, uint32_t a2, uint32_t a3) {
#ifdef DEV
		printf("[transaction_start] %X, %X | script: %X\n", a2, a3, native::getHashOfThisScriptName());
#endif // DEV
		return originalTransactionStart(a1, a2, a3);
	}

	inline uint64_t(*originalSetPedComponent)(__int64 a1, int a2, __int64 a3, __int64 a4);
	uint64_t setPedComponentHook(__int64 a1, int a2, __int64 a3, __int64 a4) {
#ifdef DEV
		printf("[set_ped_component] component: %X | script: %X\n", a2, native::getHashOfThisScriptName());
#endif // DEV
		return originalSetPedComponent(a1, a2, a3, a4);
	}
	
	inline bool(*originalSc)(const char* a1, const char* a2, char a3);
	bool ScHook(const char* a1, const char* a2, char a3) {
		bool result = originalSc(a1, a2, a3);
		if (result) {
			printf("socialclub: %s\n", (const char*)(*(uint64_t*)a1 + *(signed int*)(a1 + 0xC)));
		}

		return result;
	}


	//inline uint64_t(*originalTooltip)(uint64_t rcx, uint64_t rdx, bool r8b);
	//bool tooltipHook(uint64_t rcx, uint64_t rdx, bool r8b) {

	//	/*
	//		const char* yeet = native::createVarString(10, "LITERAL_STRING", toolTip.c_str());
	//		int32_t rcx[10] = { 5000, 0, 0, 0 };
	//		uint64_t rdx[10] = { 0x0, (uint64_t)yeet };
	//		_i<int>(0x049d5c615bd38bad, &rcx, &rdx, 1);
	//	
	//	*/

	//	if (rdx) {
	//		auto idkwhatimdoing = *(uint64_t*)(rdx + 0x8);
	//		if (idkwhatimdoing) {
	//			printf("%s\n", (const char*)(idkwhatimdoing));
	//		}
	//	}

	//	printf("script: %X\n", native::getHashOfThisScriptName());

	//	return originalTooltip(rcx, rdx, r8b);
	//}

	std::string setInt32ValueOfJsonValue(std::string str, std::string id, int32_t value) {
		id = "\"" + id + "\":";
		auto index = str.find(id) + id.length();
		std::string subStr = str.substr(index, str.length());
		auto endIndex = subStr.find(",") + index;
		if (index != std::string::npos) {
			str.erase(index, endIndex - index);
			str.insert(index, std::to_string(value));
		}
		return str;
	}

	std::string setStringValueOfJsonValue(std::string str, std::string id, int32_t value) {
		id = "\"" + id + "\":";
		auto index = str.find(id) + id.length();
		std::string subStr = str.substr(index, str.length());
		auto endIndex = subStr.find(",") + index;
		if (index != std::string::npos) {
			str.erase(index, endIndex - index);
			str.insert(index, std::to_string(value));
		}
		return str;
	}

	std::string getStringFromJson(std::string str, std::string id) {
		id = "\"" + id + "\":";
		auto index = str.find(id) + id.length();
		std::string subStr = str.substr(index, str.length());
		auto endIndex = subStr.find(",");
		if (endIndex != std::string::npos) {
			return str.substr(index, endIndex);
		}
		return "idkwhatimdoing";
	}

	bool (*originalTriggerPresence)(uint32_t rcx, presenceInfo* data, uint32_t r8d, const char* payload, uint32_t r10d, bool unk, bool unk2);
	bool triggerPresenceHook(uint32_t rcx, presenceInfo* target, uint32_t r8d, const char* payload, uint32_t r10d, bool unk, bool unk2) {
		std::string str = payload;

		//if (strstr(payload, "RsJoinRequest")) {
		//	str = setInt32ValueOfJsonValue(str, "u", 1);
		//	str = setInt32ValueOfJsonValue(str, "f", 1);

		//}

		//if (strstr(payload, "RsJoinResponse")) {
		//	str = setInt32ValueOfJsonValue(str, "f", 1);

		//}
		//	setInt32ValueOfJsonValue()

		////	printf("[RsSessionServerInfoResponse] %s\n", getStringFromJson(r8->presence, "uri").c_str());
		//	/*
		//		{"gm.evt":{"e":"RsInviteServerInfo","d":{"gh":"jvMyCgAAAAAAAAMAAAAAAA==","n":"RecklessOutlaws","i":"YSSqAgAAAAAAAAMAAAAAAA==","u":1,"f":8,"v":true,"uri":"wss://ewrsession3-rdr2-prod.ros.rockstargames.com:30213","ss":3596011127}}}
		//		{"gm.evt":{"e":"RsInviteRequestResultEvent","d":{"gh":"jvMyCgAAAAAAAAMAAAAAAA==","i":"YSSqAgAAAAAAAAMAAAAAAA==","r":1,"u":1,"f":8}}}

		//	*/
		//	//"wss://ewrsession3-rdr2-prod.ros.rockstargames.com:30213","ss":3596011127}}}
		//}

		//if (strstr(payload, "RsInvite")) {


		//}
#ifdef DEV
		custom(consoleForeground::MAGENTA, "[outgoing_presence]", "%lli %s", target->rockstarID, str.c_str());
#endif
		return originalTriggerPresence(rcx, target, r8d, str.c_str(), r10d, unk, unk2);
	}

	bool (*originalInvitePlayer)(presenceInfo*, int32_t, uint64_t);
	bool invitePlayerHook(presenceInfo* target, int32_t unk, uint64_t unk2) {
		// 0x51F6F30
		custom(consoleForeground::MAGENTA, "[invite_player]", "rid: %lli | unk: %i | ptr: 0x%llX\n", target->rockstarID, unk, unk2 - global::vars::g_gameAddress);

		auto ret = originalInvitePlayer(target, unk, unk2);
		return ret;
	}

	bool (*OriginalModelBypass)(uint64_t, uint32_t);
	bool modelBypassHook(uint64_t rcx, uint32_t rdx) {
		if (rdx == 0xE268A046 && global::vars::g_changeModel && !native::isEntityDead(menu::getLocalPlayer().ped) && native::networkIsSessionActive()) {
			return false;
		}
		return OriginalModelBypass(rcx, rdx);
	}

	int (*originalAccessTunableInt)(int a1, int a2, uint32_t a3);
	int accessTunableIntHook(int a1, int a2, unsigned int a3) {
		if (a2 == -1216955759 && global::vars::g_moonshinerBooster) { // moonshiner cooldown
			return 2000;
		}
		return originalAccessTunableInt(a1, a2, a3);
	}

	float(*originalAccessTunableFloat)(int a1, int a2, float a3);
	float accessTunableFloatHook(int a1, int a2, float a3) {
		if ((a2 == 178944019 || a2 == -41122219 || a2 == -925810086) && global::vars::g_moonshinerBooster) { // moonshiner cooldown
			return .01f;
		}
		return originalAccessTunableFloat(a1, a2, a3);
	}

#pragma endregion

	void init::geoIPThread() {
		Sleep(1000);

		if (global::vars::g_geoQueue.size()) {
			for (std::size_t i = 0; i < global::vars::g_geoQueue.size(); i++) {
				uint32_t item = global::vars::g_geoQueue[i];
				if (global::vars::g_geo.find(item) == global::vars::g_geo.end()) {
					char ipStr[16];
					sprintf(ipStr, "%i.%i.%i.%i"_Protect, ((item & 0xFF000000) >> 24), ((item & 0xFF0000) >> 16), ((item & 0xFF00) >> 8), (item & 0xFF));

					httpClient client(std::wstring(utils::va::vaw("https://ozark.gg/geo.php?ip=%s"_Protect, ipStr)));

					if (client.SendHttpRequest(std::wstring(utils::va::vaw("GET"_Protect)))) {
						try {
							nlohmann::json j = nlohmann::json::parse(client.GetResponseContent());
							if (!j.empty()) {
								if (!j["status"].is_null()) {
									if (!strcmp(j["status"].get<std::string>().c_str(), "success")) {
										geoIP geo;
										if (!j["country"].is_null()) geo.country = j["country"].get<std::string>();
										if (!j["countryCode"].is_null()) geo.countryCode = j["countryCode"].get<std::string>();
										if (!j["regionName"].is_null()) geo.regionName = j["regionName"].get<std::string>();
										if (!j["city"].is_null()) geo.city = j["city"].get<std::string>();
										if (!j["zip"].is_null()) geo.zip = j["zip"].get<std::string>();
										if (!j["timezone"].is_null()) geo.timezone = j["timezone"].get<std::string>();
										if (!j["isp"].is_null()) geo.isp = j["isp"].get<std::string>();
										if (!j["org"].is_null()) geo.org = j["org"].get<std::string>();

										global::vars::g_geo[item] = geo;
									}
								}
							}
						} catch (...) {
							warn("Failed to parse GeoIP request for %s"_Protect, ipStr);
						}
					} else {
						warn("Failed to send GeoIP request for %s"_Protect, ipStr);
					}
				}

				global::vars::g_geoQueue.erase(global::vars::g_geoQueue.begin() + i);
				Sleep(1000);
			}
		}
	}

	void init::ntVirtualQueryMemoryRdrBypass() {
		*(uint64_t*)(global::vars::ntQueryVirtualMemoryLoc) = (uint64_t)hooks::ntQueryVirtualMemoryHook;
		Sleep(0);
	}

	void init::rockstarAccountThread() {
		Sleep(1000);

		if (global::vars::g_rockstarAccountsQueue.size()) {
			for (std::size_t i = 0; i < global::vars::g_rockstarAccountsQueue.size(); i++) {
				uint64_t item = global::vars::g_rockstarAccountsQueue[i];

				if (global::vars::g_rockstarAccounts.find(item) == global::vars::g_rockstarAccounts.end()) {
					rage::api::getSocialclub()->getProfileFromRID(item, [] (bool successful, rage::network::rockstarAccount account) {
						if (successful) {
							global::vars::g_rockstarAccounts[account.rockstarId] = account;
						}
					});
				}

				global::vars::g_rockstarAccountsQueue.erase(global::vars::g_rockstarAccountsQueue.begin() + i);
				Sleep(1000);
			}
		}
	}

	void init::scriptHookThread() {
		Sleep(5000);

		// bg script hash = 0xDBE6C60
		hooking::getHooking()->createScriptAll((uint64_t)rage::invoker::getNativeHandler(0x4EDE34FBADD967A6), &hooks::waitHook);
		hooking::getHooking()->createScriptAll((uint64_t)rage::invoker::getNativeHandler(0x46ED607DDD40D7FE), &hooks::requestScriptHook);
	}

	void init::modelBypassThread() {
		if (global::vars::g_changeModel) {
			if (native::networkIsInSession() && native::isPlayerPlaying(menu::getLocalPlayer().id)) {

				/*
				[thread break 1] 4 - 0
				[thread break] 3 - 1 | 3 - 3
				[thread break 1] 4 - 0
				[thread break] 3 - 1 | 3 - 3
				*/


				static auto global_1572864_1 = menu::globals(1572864).at(1).get<int>();
				static auto global_1572864_3 = menu::globals(1572864).at(3).get<int>();

				if (*global_1572864_1 == 1) {
					*global_1572864_1 = 3;
				}
				if (*global_1572864_3 == 4) {
					*global_1572864_3 = 0;
				}
			}
		}
		Sleep(0);
	}

	void init::entry(HMODULE hModule) {
		srand((unsigned int)time(0));

		utils::getConfig()->initialize();
		logging::getLog()->initialize();

		MODULEINFO gameModuleInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(0), &gameModuleInfo, sizeof(MODULEINFO));
		global::vars::g_gameAddress = (uint64_t)gameModuleInfo.lpBaseOfDll;
		global::vars::g_gameSize = (uint64_t)gameModuleInfo.SizeOfImage;

		global::vars::g_ntdll = GetModuleHandleA("ntdll.dll"_Protect);
		global::vars::g_socialclub = GetModuleHandleA("socialclub.dll"_Protect);

#ifndef DEV
		if (!security::getSecurity()->initialize()) {
			getInit()->unload();
			return;
		}

		uint64_t peb = (uint64_t)__readgsqword(0x60);
		if (peb) {
			if (*(uint32_t*)(peb + 288) != 7599) {
				error("Bypasses failed to initialize!"_Protect);
				getInit()->unload();
				return;
			}
		}
#else
		dev("Base: %llx", global::vars::g_menuAddress);
		uint64_t peb = (uint64_t)__readgsqword(0x60);
		if (peb) {
			*(uint32_t*)(peb + 280) = 6;
			*(uint8_t*)(peb + 284) = 1;
			*(uint16_t*)(peb + 288) = 7599;
		}

#endif

		int windowTimeout = 0;
		while (!global::vars::g_gameWindow && !global::vars::g_unloading) {
			if (windowTimeout >= 20) {
				error("Window not found within timeout, exiting"_Protect);
				getInit()->unload();
				return;
			}

			global::vars::g_gameWindow = FindWindowA(nullptr, "Red Dead Redemption 2"_Protect);
			Sleep(1000);
			windowTimeout++;
		}

		getInit()->anticheat();

		getInit()->originalExceptionFilter = SetUnhandledExceptionFilter(exceptionFilter);
		getInit()->globals();

		//if (!memory::getPattern()->scanProcess({ "45 33 C9 48 89 44 24 ? 83 C8 FF 66 89 44 24 ? C7"_Protect, 0, "RC"_Protect, true }, [&](uint64_t address) {
		//	hooking::getHooking()->create("RC"_Protect, address - 0x57, &randomNetObjCrash, &originalRandomNetObjCrash);
		//})) return;

		if (!memory::getPattern()->scanProcess({ "E8 ? ? ? ? B9 ? ? ? ? 4D 8D 76 08 03 F1 8A"_Protect, 0, "DI"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("DI"_Protect, memory::getMemory()->getAddressFromInstruction(address, 1, 5), &deductItemFromInventory, &originalDeductItemFromInventory);
		})) return;

		if (!memory::getPattern()->scanProcess({ "45 33 ED 89 03 44 88 6B 4C 4C 89 6B 3C"_Protect, 0, "PCH"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("PCH"_Protect, address - 0x35, &privateCrashPatchHook, &originalPrivateCrashPatch);
		})) return;

		if (!memory::getPattern()->scanProcess({ "FF 50 38 48 85 C0 74 32 48 8B 03 48 8B"_Protect, 0, "DKB"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("DKB"_Protect, address - 0x55, &menu::hooks::deletePedHook, &menu::hooks::originalDeletePed);
		})) return;

		//if (!memory::getPattern()->scanProcess({ "E9 ? ? ? ? CC 40 53 48 83 EC 20 48 8B 41 10 4C 8D 44 24 ? 48 8B D9"_Protect, 0, "DKB"_Protect, true }, [&](uint64_t address) {
		//	hooking::getHooking()->create("DKB"_Protect, memory::getMemory()->getAddressFromInstruction(address, 1, 5), &unlockIsUnlockedHook, &originalUnlockIsUnlocked);
		//})) return;

#ifdef DEV
		if (!memory::getPattern()->scanProcess({ "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 41 8B E9 49 8B D8 8B F2 8B F9 83 38 00 0F"_Protect, 0, "TAI"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("TAI"_Protect, address, &transactionAddItem, &originalTransactionAddItem);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 33 DB C7 01 ? ? ? ? 38 1D ? ? ? ? 41 8B F0 48"_Protect, 0, "TS"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("TS"_Protect, address, &transactionStart, &originalTransactionStart);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 85 C9 74 5C 48 89 5C 24 ? 57 48 83 EC 40 41 8A C1 89 54 24 50 44 8A 4C 24 ? 48"_Protect, 0, "SPC"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("SPC"_Protect, address, &setPedComponentHook, &originalSetPedComponent);
		})) return;

		if (!memory::getPattern()->serverScanProcess("HPE"_Protect, [](uint64_t address) {
			hooking::getHooking()->create("HPE"_Protect, address, &hooks::handlePresenceEventHook, &hooks::originalHandlePresenceEvent);
		})) return;

		memory::getPattern()->scanModule("socialclub"_Protect,{ "48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 83 39 00 41 8A F0 48 8B FA 48 8B D9 74 42 83"_Protect, 0, "SOCIALCLUB_P"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("SOCIALCLUB_P"_Protect, address, &ScHook, &originalSc);
		});
		//if (!memory::getPattern()->scanProcess({ "48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 70 20 55 48 8D 68 A1 48 81 EC ? ? ? ? 45 33 F6 41 8A F0 48 8B DA 41 8B"_Protect, 0, "pog"_Protect, true }, [&](uint64_t address) {
		//	hooking::getHooking()->create("pog"_Protect, address, &tooltipHook, &originalTooltip);
		//})) return;
		//48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 4C 89 70 20 55 48 8D 68 A1 48 81 EC ? ? ? ? 45 33 F6 41 8A F0 48 8B DA 41 8B

#endif // DEV
		//if (!memory::getPattern()->scanProcess({ "48 8B C4 48 89 58 08 48 89 70 10 48 89 78 18 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 4C 8B F9 4D 8B F0 48 8D 4A 18 48 8B F2 E8 ? ? ? ? 84 C0 0F 84"_Protect, 0, "PRU"_Protect, true }, [&](uint64_t address) {
		//	hooking::getHooking()->create("PRU"_Protect, address, &menu::hooks::processRelayUsageHook, &menu::hooks::originalProcessRelayUsage);
		//})) return;

		if (!memory::getPattern()->scanProcess({ "48 8D 54 24 ? 48 8B 01 FF 90 ? ? ? ? 84 C0 74 16 8B"_Protect, 0, "SPIM"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("SPIM"_Protect, address - 0xB0, &menu::hooks::syncPlayerInfoMsgHook, &menu::hooks::originalSyncPlayerInfoMsg);
			printf("%llX\n", address - 0xB0);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 54 41 55 41 56 41 57 48 83 EC 20 4C 8D A1 ? ? ? ? 48 8B F1 49 8B CC 44 8B FA E8 ? ? ? ? 44"_Protect, 0, "MB"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("MB"_Protect, address, &modelBypassHook, &OriginalModelBypass);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 57 48 83 EC 20 48 8B 1D ? ? ? ? 41 8B F8 48 85 DB 75 04 33 C0 EB"_Protect, 0, "ATI"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("ATI"_Protect, address, &accessTunableIntHook, &originalAccessTunableInt);
		})) return;

		if (!memory::getPattern()->scanProcess({ "40 53 48 83 EC 30 48 8B 1D ? ? ? ? 0F 29 74 24 ? 0F 28 F2 48 85 DB 75 05 0F 57 C0 EB 12 E8 ? ? ? ? 8B D0"_Protect, 0, "ATF"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("ATF"_Protect, address, &accessTunableFloatHook, &originalAccessTunableFloat);
		})) return;

		if (!memory::getPattern()->serverScanProcess("SNO"_Protect, [](uint64_t address) {
			hooking::getHooking()->create("SNO"_Protect, address, &menu::hooks::syncNetworkObject, &menu::hooks::originalSyncNetworkObject);
		})) return;

		if (!memory::getPattern()->scanProcess({ "22 D8 74 1D 4C 8D 45 38 41 B9 ? ? ? ? 48 8D 15 ? ? ? ? 48 8B CE E8 ? ? ? ? 84"_Protect, 0, "IRF"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("IRF"_Protect, address - 0x11E, &menu::hooks::inviteResponseHook, &menu::hooks::originalInviteResponse);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 8D 0D ? ? ? ? 41 0F B7 FF 48 8B F0 4C 8D 3D ? ? ? ? 0F B7 DB 4C 0F 44 F9 E8 ? ? ? ? 48 8B"_Protect, 0, "RCC"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("RCC"_Protect, address - 0x13F, &menu::hooks::recieveCloneCreateHook, &menu::hooks::originalRecieveCloneCreate); // find
		})) return;

		if (!memory::getPattern()->serverScanProcess("RSIR"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("RSIR"_Protect, address, &menu::hooks::readSessionInfoResponseHook, &menu::hooks::originalReadSessionInfoResponse);
		})) return;

		// removed
		//if (!memory::getPattern()->serverScanProcess("ASWRT"_Protect, [] (uint64_t address) {
		//	hooking::getHooking()->create("ASWRT"_Protect, address, &hooks::appendStringWithResolvedTagHook, &hooks::originalAppendStringWithResolvedTag);
		//})) return;

		if (!memory::getPattern()->serverScanProcess("GPI"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("GPI"_Protect, address, &hooks::getPoolIndexHook, &hooks::originalGetPoolIndex);
		})) return;

		//if (!memory::getPattern()->serverScanProcess("WCH"_Protect, [] (uint64_t address) {
		//	hooking::getHooking()->create("WCH"_Protect, address, &hooks::weatherCrashHook, &hooks::originalWeatherCrash);
		//})) return;

		memory::getPattern()->scanProcess({ "0F 10 80 ? ? ? ? 0F 11 83 ? ? ? ? 8B 80"_Protect, 0, "GAC"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("GAC"_Protect, address - 0x7B, &menu::hooks::gatlingAttachCrashHook, &menu::hooks::originalGatlingAttachCrash);
		});

		memory::getPattern()->scanProcess({ "48 85 FF 74 73 41 8B EC 66 44 3B A7 ? ? ? ? 73"_Protect, 0, "GDC"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("GDC"_Protect, address - 0x18B, &menu::hooks::gatlingDetachmentCrashHook, &menu::hooks::originalGatlingDetachmentCrash);
		});

		memory::getPattern()->scanProcess({ "48 89 5C 24 ? 57 48 83 EC 20 48 8B D9 49 8B C1 48 8B"_Protect, 0, "UAE"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("UAE"_Protect, address, &menu::hooks::unbindAttachmentExtensionHook, &menu::hooks::originalUnbindAttachmentExtension);
		});

		if (!memory::getPattern()->serverScanProcess("SCMB"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("SCMB"_Protect, address, &hooks::spawnCrashMessageBoxHook, &hooks::originalSpawnCrashMessageBox);
		})) return;

		if (!memory::getPattern()->serverScanProcess("SPI"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("SPI"_Protect, address, &hooks::syncPlayerInfoHook, &hooks::originalSyncPlayerInfo);
		})) return;

		if (!memory::getPattern()->serverScanProcess("UPDNOBJ"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("UPDNOBJ"_Protect, address, &hooks::updateNetObjHook, &hooks::originalUpdateNetObj);
		})) return;

		if (!memory::getPattern()->serverScanProcess("PJL"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("PJL"_Protect, address, &hooks::processJoinLeaveHook, &hooks::originalProcessJoinLeave);
		})) return;

		if (!memory::getPattern()->serverScanProcess("GGL"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("GGL"_Protect, address, &hooks::getGxtTextHook, &hooks::orginalGetGxtText);
		})) return;

		if (!memory::getPattern()->serverScanProcess("PFB"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("PFB"_Protect, address, &hooks::scProfanityCheckStringHook, &hooks::originalScProfanityCheckString);
		})) return;

		if (!memory::getPattern()->serverScanProcess("SRP"_Protect, [] (uint64_t address) {
			hooking::getHooking()->create("SRP"_Protect, address, &hooks::scriptImRenderPhaseHook, &hooks::originalScriptImRenderPhase);
		})) return;

		hooking::getHooking()->create("GPA"_Protect, (uint64_t)&GetProcAddress, &hooks::getProcAddressHook, &hooks::originalGetProcAddress);
		hooking::getHooking()->create("GMFNA"_Protect, (uint64_t)&GetModuleFileNameA, &hooks::getModuleFileNameAHook, &hooks::originalGetModuleFileNameA);
		hooking::getHooking()->create("GMFNW"_Protect, (uint64_t)&GetModuleFileNameW, &hooks::getModuleFileNameWHook, &hooks::originalGetModuleFileNameW);

		if (!memory::getPattern()->serverScanProcess("NASA"_Protect, [](uint64_t callerfunc) {
			uint64_t address = (uint64_t)GetProcAddress(global::vars::g_ntdll, "NtQueryVirtualMemory"_Protect);
			if (!hooking::getHooking()->create("NASA"_Protect, address, &hooks::ntQueryVirtualMemoryHook, &hooks::originalNtQueryVirtualMemory)) {
				if (!memory::getPattern()->scanProcess({ "48 8B 05 ? ? ? ? 48 8B 08 48 39 0D ? ? ? ? 75 5F 48 8B 05 ? ? ? ? 48 8B 08 48 39 0D ? ? ? ? 75"_Protect, 0, "NASA"_Protect, true }, [&](uint64_t address) {
					if (hooking::getHooking()->createVirtual("NASA"_Protect, memory::getMemory()->getAddressFromInstruction(address), 0, &hooks::ntQueryVirtualMemoryHook, &hooks::originalNtQueryVirtualMemory)) {
						global::vars::ntQueryVirtualMemoryLoc = memory::getMemory()->getAddressFromInstruction(address);
						utils::getThreadPool()->add("RNTQ"_Protect, ntVirtualQueryMemoryRdrBypass);
					} else {
						getInit()->unload();
					}
				})) {
					getInit()->unload();
				}
				caller::call<int>(callerfunc);
			}
		})) return;

		if (!memory::getPattern()->serverScanProcess("NE_RCE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_RCE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::requestControlEventHook, &hooks::originalRequestControlEvent);
		})) return;

		//if (!memory::getPattern()->serverScanProcess("NE_GCE"_Protect, [] (uint64_t address) {
		//	hooking::getHooking()->createVirtual("NE_GCE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::gameClockEventHook, &hooks::originalGameClockEvent);
		//})) return;

		//if (!memory::getPattern()->serverScanProcess("NE_GWE"_Protect, [] (uint64_t address) {
		//	hooking::getHooking()->createVirtual("NE_GWE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::gameWeatherEventHook, &hooks::originalGameWeatherEvent);
		//})) return;

		if (!memory::getPattern()->serverScanProcess("NE_FE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_FE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::fireEventHook, &hooks::originalFireEvent);
		})) return;

		if (!memory::getPattern()->serverScanProcess("NE_EE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_EE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::explosionEventHook, &hooks::originalExplosionEvent);
		})) return;

		if (!memory::getPattern()->serverScanProcess("NE_CPTE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_CPTE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::networkClearPedTasksEventHook, &hooks::originalNetworkClearPedTasksEvent);
		})) return;

		if (!memory::getPattern()->serverScanProcess("NE_ISE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_ISE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::networkIncrementStatEventHook, &hooks::originalNetworkIncrementStatEvent);
		})) return;

		if (!memory::getPattern()->serverScanProcess("NE_PE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_PE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::networkPTFXEventHook, &hooks::originalNetworkPTFXEvent);
		})) return;

		if (!memory::getPattern()->serverScanProcess("NE_RCSE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_RCSE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::reportCashSpawnEventHook, &hooks::originalReportCashSpawnEvent);
		})) return;
		
		if (!memory::getPattern()->serverScanProcess("NE_GPRE"_Protect, [] (uint64_t address) {
			hooking::getHooking()->createVirtual("NE_GPRE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 6, &hooks::givePickupRewardsEventHook, &hooks::originalGivePickupRewardsEvent);
			//hooking::getHooking()->createVirtual("NE_TGPRE"_Protect, memory::getMemory()->getAddressFromInstruction(address), 1, &hooks::targetPickuprewardsHook, &hooks::originalTargetPickupRewardsEvent);
		})) return;

		utils::getThreadPool()->add("GIPT"_Protect, geoIPThread);
		utils::getThreadPool()->add("RAQT"_Protect, rockstarAccountThread);
		utils::getThreadPool()->add("SCRT"_Protect, [] { rage::api::getSocialclub()->requestThread(); });
		utils::getThreadPool()->add("HBT"_Protect, heartbeatThread);
		utils::getThreadPool()->add("PT"_Protect, presenceThread);
		utils::getThreadPool()->add("SHT"_Protect, scriptHookThread);
		//utils::getThreadPool()->add("MBT"_Protect, modelBypassThread);
	}

	void init::anticheat() {
		if (!memory::getPattern()->scanProcess({ "84 C0 74 3E 8D 4B 50 E8 ? ? ? ? 48 85 C0 74 22 8A 4C 24 68 44 8B 4C 24 ? 44 8B 44 24 ? 48 8B 54 24 ? 88 4C 24 20 48 8B C8 E8 ? ? ? ? 48 8B D8 48 8B 0D ? ? ? ? 48 8B D3 E8 ? ? ? ? 48 8B CF E8 ? ? ? ? 48 83 C4 38 5F 5B C3"_Protect, 0, "RCSRA"_Protect, true }, [] (uint64_t address) {
			global::vars::g_deleteNetworkEventReportCashSpawn = address;
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 20 4C 8B F1 40 8A FA 48 83 C1 08 E8 ? ? ? ? 49 8B 06 49 8B CE FF 50 18 33 DB 85 C0 0F 85 ? ? ? ? 40 84 FF 0F 84"_Protect, 0, "DNE"_Protect, true }, [] (uint64_t address) {
			hooking::getHooking()->create("DNE"_Protect, address, &hooks::deleteNetworkEventHook, &hooks::originalDeleteNetworkEvent);
		})) return;

		if (!memory::getPattern()->scanProcess({ "4C 8B DC 49 89 5B 08 49 89 6B 10 49 89 73 18 57 48 81 EC ? ? ? ? 48 8B 01 48 8B FA 41 8B E8 49 8D 93 ? ? ? ? 41 B8 ? ? ? ? 48 8B D9 FF 50 68 48 8B F0 48 85 C0 0F 84"_Protect, 0, "BRU"_Protect, true }, [] (uint64_t address) {
			hooking::getHooking()->create("BRU"_Protect, address, &hooks::constructRequestUrlHook, &hooks::originalConstructRequestUrl);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 20 65 4C 8B 04 25 ? ? ? ? 8B E9 8B 05 ? ? ? ? BE ? ? ? ? B9 ? ? ? ? 4C 8B"_Protect, 0, "PM2"_Protect, true }, [] (uint64_t address) {
			hooking::getHooking()->create("PM2"_Protect, address, &hooks::processMetric2Hook, &hooks::originalProcessMetric2);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 54 41 56 41 57 48 83 EC 20 33 DB 48 8B FA 48 8B F1 48 85 D2"_Protect, 0, "PM"_Protect, true }, [] (uint64_t address) {
			hooking::getHooking()->create("PM"_Protect, address, &hooks::processMetricHook, &hooks::originalProcessMetric);
		})) return;

		if (!memory::getPattern()->scanProcess({ "57 48 83 EC 30 83 64 24 ? ? 48 8B F2 48 8B E9 40 B7 01"_Protect, 0, "PJR"_Protect, true }, [&](uint64_t address) {
			hooking::getHooking()->create("PJR"_Protect, address - 0x7, &menu::hooks::presJoinRequestHook, &menu::hooks::originalPresJoinRequest);
		})) return;

		if (!memory::getPattern()->scanProcess({ "48 8D 05 ? ? ? ? 4C 03 C0 41 83 B8 ? ? ? ? ? 0F 86 ? ? ? ? 83 CF FF 48 8D 15 ? ? ? ? 89 7C 24 28 41 B1 01 83 64 24 ? ? 48 8B CB E8"_Protect, 0, "RIS"_Protect, true }, [] (uint64_t address) {
			global::vars::g_rockstarTicketInfo = memory::getMemory()->getAddressFromInstruction(address);
			if (global::vars::g_rockstarTicketInfo) {
				global::auth::g_socialClubName = utils::rot13(std::string((char*)(global::vars::g_rockstarTicketInfo + 0x3af)));
				global::auth::g_socialClubEmail = utils::rot13(std::string((char*)(global::vars::g_rockstarTicketInfo + 0x33f)));
				global::auth::g_rockstarId = *(uint64_t*)(global::vars::g_rockstarTicketInfo + 0x3f8);
			}
		})) return;

		if (!memory::getPattern()->scanProcess({ "4C 8D 05 ? ? ? ? 48 8B CB 48 8D 15 ? ? ? ? E8 ? ? ? ? 48 8D 4C 24 ? E8 ? ? ? ? 48 8D"_Protect, 0, "RIS2"_Protect, true }, [] (uint64_t address) {
			uint64_t build = memory::getMemory()->getAddressFromInstruction(address);
			if (build) {
				global::auth::g_build = utils::rot13(std::string((char*)build));
			}
		})) return;
	}

	void init::globals() {
		memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8B EC 48 83 EC 60 49 83 20 00 8B F2 49 83 60 ? ? 48"_Protect, 0, "JOH"_Protect, true }, [](uint64_t address) {
			global::vars::g_joinOrHostSession = address;
		});

		memory::getPattern()->scanProcess({ "48 8B 15 ? ? ? ? 4C 8B C9 49 F7 D1 48 C1 CA 05 48 C1 C2 20 49 C1 C1 20 8B CA 83 E1"_Protect, 0, "NTE"_Protect, true }, [] (uint64_t address) {
			global::vars::g_nativeTableEncryption = memory::getMemory()->getAddressFromInstruction(address);
		});

		memory::getPattern()->scanProcess({ "49 2B D4 4D 8B C6 48 8B CF FF 90 ? ? ? ? B8"_Protect, 0, "II"_Protect, true }, [](uint64_t address) {
			global::vars::g_infiniteItems = address;
		});

		memory::getPattern()->scanProcess({ "48 8B 0D ? ? ? ? 45 33 C0 E8 ? ? ? ? 48 8B F8 48 85 C0 0F 84 ? ? ? ? 48"_Protect, 0, "IP"_Protect, true }, [](uint64_t address) {
			global::vars::g_inventoryPool = memory::getMemory()->getAddressFromInstruction(address, 3, 7);
		});

		
		memory::getPattern()->scanProcess({ "E8 ? ? ? ? 48 8B F8 48 85 C0 0F 84 ? ? ? ? 48 8B C8 84 DB 75 11 8B D5 E8"_Protect, 0, "GIL"_Protect, true }, [](uint64_t address) {
			global::vars::g_getInventoryList = memory::getMemory()->getAddressFromInstruction(address, 1, 5);
		});

		memory::getPattern()->serverScanProcess("NRT"_Protect, [] (uint64_t address) {
			global::vars::g_nativeRegistration = (rage::invoker::nativeRegistration**)memory::getMemory()->getAddressFromInstruction(address);
		});

		//printf("Native Table Hash Test: %llx\n", rage::invoker::getNativeHandler(0xED40380076A31506));

		memory::getPattern()->serverScanProcess("CFC"_Protect, [] (uint64_t address) {
			global::vars::g_currentFrameCount = memory::getMemory()->getAddressFromInstruction(address, 2, 6);
		});

		memory::getPattern()->serverScanProcess("GHNGP"_Protect, [] (uint64_t address) {
			global::vars::g_getHostNetGamePlayer = address;
		});

		memory::getPattern()->serverScanProcess("CNOO"_Protect, [] (uint64_t address) {
			global::vars::g_changeNetObjOwner = address;
			//hooking::getHooking()->create("idkwhatimdoing"_Protect, address, &changeNetObjOwnerHook, &originalChangeNetObjOwner);
		});

		memory::getPattern()->serverScanProcess("NOM"_Protect, [] (uint64_t address) {
			global::vars::g_netObjMgr = memory::getMemory()->getAddressFromInstruction(address);
		});

		memory::getPattern()->serverScanProcess("GEA"_Protect, [] (uint64_t address) {
			global::vars::g_getEntityAddress = address;
		});

		memory::getPattern()->serverScanProcess("GEHFA"_Protect, [] (uint64_t address) {
			global::vars::g_getEntityHandleFromAddress = address;
		});

		memory::getPattern()->serverScanProcess("GNGPFI"_Protect, [] (uint64_t address) {
			global::vars::g_getNetGamePlayerFromIndex = memory::getMemory()->getAddressFromInstruction(address, 1, 5);
		});

		memory::getPattern()->serverScanProcess("MSH"_Protect, [] (uint64_t address) {
			global::vars::g_migrateScriptHost = address;
		});

		memory::getPattern()->serverScanProcess("GMI"_Protect, [] (uint64_t address) {
			global::vars::g_getModelInfo = address;
		});

		memory::getPattern()->serverScanProcess("UIE"_Protect, [] (uint64_t address) {
			global::vars::g_textWrapMin = memory::getMemory()->getAddressFromInstruction(address, 4, 8);
			global::vars::g_textWrapMax = global::vars::g_textWrapMin + 0x4;
			global::vars::g_textFont = global::vars::g_textWrapMin + 0x8;
			global::vars::g_textAlign = global::vars::g_textWrapMin + 0x10;
			global::vars::g_textOutline = global::vars::g_textWrapMin + 0x12;
		});

		memory::getPattern()->serverScanProcess("VHL"_Protect, [] (uint64_t address) {
			global::vars::g_visionHashList = memory::getMemory()->getAddressFromInstruction(address);
			global::vars::g_timecycleIndex = memory::getMemory()->getAddressFromInstruction(address + 0x19, 2, 6);
		});

		memory::getPattern()->serverScanProcess("GIFHL"_Protect, [] (uint64_t address) {
			global::vars::g_getIndexFromHashList = address;
		});

		memory::getPattern()->serverScanProcess("GP"_Protect, [] (uint64_t address) {
			global::vars::g_globalPointer = (uint64_t**)memory::getMemory()->getAddressFromInstruction(address);
		});

		memory::getPattern()->serverScanProcess("PWCH"_Protect, [] (uint64_t address) {
			global::vars::g_weaponComponentHashListCount = memory::getMemory()->getAddressFromInstruction(address);
			global::vars::g_weaponComponentHashList = memory::getMemory()->getAddressFromInstruction(address + 0xF);
		});

		memory::getPattern()->serverScanProcess("PWH"_Protect, [] (uint64_t address) {
			global::vars::g_weaponHashListCount = memory::getMemory()->getAddressFromInstruction(address);
			global::vars::g_weaponHashList = memory::getMemory()->getAddressFromInstruction(address + 0x17);
		});

		memory::getPattern()->serverScanProcess("SRPDP"_Protect, [] (uint64_t address) {
			global::vars::g_drawPoints = address;
		});

		//memory::getPattern()->serverScanProcess("SCE"_Protect, [] (uint64_t address) {
		//	global::vars::g_triggerClockEvent = address;
		//});
		
		//memory::getPattern()->serverScanProcess("SWE"_Protect, [] (uint64_t address) {
		//	global::vars::g_triggerWeatherEvent = address;
		//});

		memory::getPattern()->serverScanProcess("WT"_Protect, [] (uint64_t address) {
			global::vars::g_weatherTable = memory::getMemory()->getAddressFromInstruction(address, 3, 7);
		});

		memory::getPattern()->serverScanProcess("NPM"_Protect, [] (uint64_t address) {
			global::vars::g_networkPlayerMgr = memory::getMemory()->getAddressFromInstruction(address, 3, 7);
		});
		//4C 8D 2D ? ? ? ? 45 8A E1 49 8B E8 4C 8B FA 44 8B F3 74 43 85 C9 75 3F 48 69 FE ? ? ? ? 49 8B CF 49 03 FD 44 8B 47
		//memory::getPattern()->serverScanProcess("GIIT"_Protect, [] (uint64_t tableAddress) {
		//	global::vars::g_gameInfoItemTable = memory::getMemory()->getAddressFromInstruction(tableAddress);
		//});

		memory::getPattern()->scanProcess({ "48 8D 05 ? ? ? ? 48 03 D8 E8 ? ? ? ? 48 8B F0 48 B8"_Protect, 0, "GIIT"_Protect, true }, [](uint64_t address) {
			global::vars::g_gameInfoItemTable = memory::getMemory()->getAddressFromInstruction(address); // FIND
		});

		//0F B7 CA 83 F9

		//4C 8D 2D ? ? ? ? 45 8A E1 49 8B E8 4C 8B FA 44 8B F3 74 51 85 C9 75 4D 48 69 FE ? ? ? ? 48
		
		//memory::getPattern()->serverScanProcess("GGII"_Protect, [] (uint64_t funcAddress) {
		//	global::vars::g_getGameInfoItem = funcAddress;
		//});

		memory::getPattern()->scanProcess({ "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 33 DB 41 8B F8 48 8B F2 48 8B E9 45 85 C0 7E 1F 48 63 C3 48 8B CD 48 69 D0 ? ? ? ? 48 03 D6 E8 ? ? ? ? 85 C0 74 1E FF C3 3B DF 7C E1 83 C8 FF 48 8B 5C"_Protect, 0, "GGII"_Protect, true }, [](uint64_t address) {
			global::vars::g_getGameInfoItem = address;
		});
		
		memory::getPattern()->serverScanProcess("GWI"_Protect, [] (uint64_t address) {
			global::vars::g_getWeaponInfo = memory::getMemory()->getAddressFromInstruction(address, 1, 5);
		});

		/*memory::getPattern()->scanProcess({ "48 8B 05 ? ? ? ? 48 8D 55 48 8B CB 48 8B 0C C8 48 8B 39 49 8B CF 48 83 C7 10", 0, "script thread list" }, [](uint64_t address) {
			global::vars::g_scriptThreadList = memory::getMemory()->getAddressFromInstruction(address, 3, 7);
		});

		memory::getPattern()->scanProcess({ "66 44 3B 25 ? ? ? ? 45 8B F4 4C 89 65 EC 41 8B F4 44 89 65 E8 41 8B DC 4C 8B B8 ? ? ? ? 4C", 0, "script thread size" }, [](uint64_t address) {
			global::vars::g_scriptThreadListSize = memory::getMemory()->getAddressFromInstruction(address, 4, 8);
		});*/

		if (!memory::getPattern()->serverScanProcess("ST"_Protect, [] (uint64_t address) {
			global::vars::g_scriptTable = (rage::script::scriptTable*)memory::getMemory()->getAddressFromInstruction(address);
		})) return;

		if (!memory::getPattern()->serverScanProcess("PEDF"_Protect, [] (uint64_t address) {
			global::vars::g_encryptedPedFactory.table1 = memory::getMemory()->getAddressFromInstruction(address + 0x5E);
			global::vars::g_encryptedPedFactory.table2 = memory::getMemory()->getAddressFromInstruction(address + 0x65);
			global::vars::g_encryptedPedFactory.addition = 1;
		})) return;

		if (!memory::getPattern()->serverScanProcess("WTUNE"_Protect, [] (uint64_t address) {
			global::vars::wtune = (watertune*)memory::getMemory()->getAddressFromInstruction(address);
		})) return;

		if (!memory::getPattern()->serverScanProcess("VPT"_Protect, [] (uint64_t address) {
			global::vars::vehiclePopTuning = (vehiclePopulationTuning*)memory::getMemory()->getAddressFromInstruction(address, 4, 8);
		})) return;

		if (!memory::getPattern()->serverScanProcess("PRE"_Protect, [](uint64_t address) {
			global::vars::g_rewardPickupsEvent = address;
		})) return;

		if (!memory::getPattern()->serverScanProcess("W2S"_Protect, [] (uint64_t address) {
			global::vars::g_worldToScreen = address;
		})) return;

		if (!memory::getPattern()->serverScanProcess("TPE"_Protect, [] (uint64_t address) {
			global::vars::g_triggerPresence = address;
#ifdef DEV
			hooking::getHooking()->create("TRIGGER_PRES"_Protect, address, &triggerPresenceHook, &originalTriggerPresence);
#endif
		})) return;

		if (!memory::getPattern()->serverScanProcess("JOINP"_Protect, [] (uint64_t address) {
			global::vars::g_joinPlayer = address;
		})) return;

		if (!memory::getPattern()->serverScanProcess("JOINPT"_Protect, [] (uint64_t address) {
			global::vars::g_joinPlayerTable = address;
		})) return;

		if (!memory::getPattern()->serverScanProcess("JOINPU"_Protect, [] (uint64_t address) {
			global::vars::g_joinPlayerU = address;
		})) return;

		/*if (!memory::getPattern()->serverScanProcess("SSNM"_Protect, [&] (uint64_t address) {
			global::vars::g_showStackedNotifyMessage = address;
		})) return;*/

		if (!memory::getPattern()->serverScanProcess("SSIR"_Protect, [&] (uint64_t address) {
			global::vars::g_sendSessionInfoRequest = address;
		})) return;

		if (!memory::getPattern()->serverScanProcess("FRIENDD"_Protect, [&] (uint64_t address) {
			global::vars::g_friendData = memory::getMemory()->getAddressFromInstruction(address);
		})) return;

		if (!memory::getPattern()->scanProcess({ "66 89 3D ? ? ? ? 66 89 3D ? ? ? ? 66 89 3D ? ? ? ? C6 05"_Protect, 0, "OF"_Protect, true }, [&](uint64_t address) {
			global::vars::g_offTheRadarFlag = memory::getMemory()->getAddressFromInstruction(address);
		})) return;

		pools::getPoolManager()->initialize();
	}

	init* getInit() {
		static init instance;
		return &instance;
	}
}