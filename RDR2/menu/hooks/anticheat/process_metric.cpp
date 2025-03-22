#include "menu/hooks/hooks.hpp"
#include "utils/caller.hpp"
#include "utils/log.hpp"

const char* metricBlocks[] = {
	"CASH_CREATED",
	"CHEAT",
	"DIG",
	"WEATHER",
	// "FLOW_LOW",
	// "FLOW_MEDIUM",
	// "FLOW_HIGH",
	"NET_MEMORY",
	"NET_NO_MEMORY",
	"NET_MODULE",
	"STALL_DETECTED",
	"CUSTOM",
	"GAMESERVER_TXN_CLAIMLOOT",
	"LOOT",
	"ADD_ITEMS",
	"GAMESERVER_TXN_USE",
	"GAMESERVER_TXN",
	"GAMESERVER_PDC_FAILURE",
	"GAMESERVER_TXN_AWARDS",
	"NETWORK_BAIL",
	"SKINNED_ANIMAL",
	"SHOP_SELL",
	"FORBIDDEN_MISSION_INV",
	"GOLD_STORE"
};

uint64_t createEmptyJsonHandle(uint64_t rcx) {
	*(uint64_t*)rcx = 0;
	*(uint64_t*)(rcx + 8) = 0;
	*(uint64_t*)(rcx + 16) = 0;
	*(uint64_t*)(rcx + 24) = 0;
	*(BYTE*)(rcx + 32) &= 0xFC;
	*(BYTE*)(rcx + 32) |= 4;
	return rcx;
}

BYTE setupJsonHandle(uint64_t rcx, uint64_t rdx, uint32_t r8d, uint32_t r9d) {
	BYTE ret = rdx == 0 ? 4 : 0;

	*(uint64_t*)rcx = 0;
	*(uint64_t*)(rcx + 8) = rdx;
	*(uint32_t*)(rcx + 16) = 0;
	*(uint32_t*)(rcx + 20) = r8d;
	*(uint32_t*)(rcx + 24) = r9d;
	*(uint32_t*)(rcx + 28) = 0;
	*(BYTE*)(rcx + 32) &= 0xF8;
	*(BYTE*)(rcx + 32) |= ret;
	return ret;
}

void menu::hooks::processMetricHook(uint64_t unk, uint64_t metric) {
	if (metric) {
		uint64_t classDefinition = *(uint64_t*)metric;
		const char* name = caller::call<const char*>(*(uint64_t*)(classDefinition + 0x18));

#ifdef DEV1
		uint64_t ptr = (uint64_t)malloc(0x1000);
		uint64_t ptr2 = (uint64_t)malloc(0x1000);

		memset((void*)ptr, 0, 0x1000);
		memset((void*)ptr2, 0, 0x1000);

		createEmptyJsonHandle(ptr);
		setupJsonHandle(ptr, ptr2, 0x1000, 1);

		caller::call<int>(*(uint64_t*)(classDefinition + 0x20), metric, ptr);
		if (*(uint64_t*)(ptr + 8)) {
			customDev(name, "%s", (char*)(*(uint64_t*)(ptr + 8)));
			//if (strcmp(name, "USE_SCENARIO")
			//	&& strcmp(name, "SPAWN_PED")) {
			//}
		}

		free((void*)ptr);
		free((void*)ptr2);
#endif

		if (name) {
			if (!strcmp(name, "SPAWN_PED") && global::vars::g_blockPedMetric) {
				customDev("AC", "Blocked metric SPAWN_PED");
				global::vars::g_blockPedMetric = false;
				return;
			}

			for (int i = 0; i < NUMOF(metricBlocks); i++) {
				if (!strcmp(name, metricBlocks[i])) {
					customDev("AC", "Blocked metric %s", name);
					return;
				}
			}

			if (!strcmp(name, "FLOW_LOW") || !strcmp(name, "FLOW_MEDIUM") || !strcmp(name, "FLOW_HIGH")) {
				if (*(float*)(metric + 0x20) == 0.f) {
					return originalProcessMetric(unk, metric);
				}

				// smart detection here
				return;
			}
		}
	}

	return originalProcessMetric(unk, metric);
}

typedef USHORT(WINAPI* CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
void Stacktrace() {
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
			}
			else {
				char buf[512];
				sprintf_s(buf, "0x%016llx %s", callerAddress, i != count - 1 ? ", " : "");
				s += buf;
			}
		}

		s += "]";
		customDev("idkwhatimdoing", "stacktrace: %s\n", s.c_str());
	}

}

void menu::hooks::processMetric2Hook(uint64_t unk, uint64_t metric) {
	if (metric) {
		uint64_t classDefinition = *(uint64_t*)metric;
		const char* name = caller::call<const char*>(*(uint64_t*)(classDefinition + 0x18));

#ifdef DEV1
		uint64_t ptr = (uint64_t)malloc(0x1000);
		uint64_t ptr2 = (uint64_t)malloc(0x1000);

		memset((void*)ptr, 0, 0x1000);
		memset((void*)ptr2, 0, 0x1000);

		createEmptyJsonHandle(ptr);
		setupJsonHandle(ptr, ptr2, 0x1000, 1);

		caller::call<int>(*(uint64_t*)(classDefinition + 0x20), metric, ptr);
		if (*(uint64_t*)(ptr + 8)) {
			customDev(name, "%s", (char*)(*(uint64_t*)(ptr + 8)));
		}
		free((void*)ptr);
		free((void*)ptr2);
#endif
		//GAMESERVER_TXN_AWARDS

		/*
		process_metric | SES_LEFT

		process_metric | ALERT
		*/

		if (name) {

			if (!strcmp(name, "NETWORK_BAIL")) {
				// ozark log every game transaction
				//Stacktrace();
			}
			if (!strcmp(name, "SPAWN_PED") && global::vars::g_blockPedMetric) {
				// customDev("AC", "Blocked metric SPAWN_PED");
				global::vars::g_blockPedMetric = false;
				return;
			}

			for (int i = 0; i < NUMOF(metricBlocks); i++) {
				if (!strcmp(name, metricBlocks[i])) {
					customDev("AC", "Blocked metric %s", name);

					return;
				}
			}
		}
	}

	return originalProcessMetric2(unk, metric);
}