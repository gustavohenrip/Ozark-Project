#pragma once
#include "stdafx.hpp"

namespace security {
	namespace metrics {
		enum eMetrics {
			METRIC_INFO,
			METRIC_DEBUGGER,
			METRIC_THREAD_DEBUGGER,
			METRIC_INTEGRITY_CHECK_FAILED,
			METRIC_BLACKLISTED_PROCESS,
			METRIC_MODULE_DIGEST_MISMATCH,
			METRIC_CLIENT_CRASH,
		};

		class metric {
		public:
			virtual eMetrics getMetric();
			virtual void onEvent();
		};

		class metricInfo : public metric {
		public:
			eMetrics getMetric();
		};

		class metricDebugger : public metric {
		public:
			eMetrics getMetric();
			void onEvent();
		};

		class metricThreadDebugger : public metric {
		public:
			eMetrics getMetric();
			void onEvent();
		};

		class metricIntegrityCheckFailed : public metric {
		public:
			eMetrics getMetric();
			void onEvent();
		};

		class metricBlacklistedProcess : public metric {
		public:
			eMetrics getMetric();
			void onEvent();
		};

		class metricModuleDigestMismatch : public metric {
		public:
			eMetrics getMetric();
			void onEvent();
		};

		class metricClientCrash : public metric {
		public:
			eMetrics getMetric();
		};
	}

	struct integrityItem {
		const char* name;
		uint64_t address;
		BYTE originalBytes[5];
	};

	struct imageSection {
		std::string name;
		uint64_t pointer;
		uint64_t size;
	};

	typedef struct _UNICODE_STRING {
		USHORT length;
		USHORT maximumLength;
		PWSTR buffer;
	} UNICODE_STRING, *PUNICODE_STRING;

	typedef struct _LDR_DATA_TABLE_ENTRY {
		LIST_ENTRY inLoadOrderLinks;
		LIST_ENTRY inMemoryOrderLinks;
		LIST_ENTRY inInitializationOrderLinks;
		void* dllBase;
		void* entryPoint;
		ULONG sizeOfImage;
		UNICODE_STRING fullDllName;
		UNICODE_STRING baseDllName;
		ULONG flags;
	} LDR_DATA_TABLE_ENTRY;

	class securityHelpers {
	public:
		BYTE moduleDigest[64];

		bool initialize();
		bool getSyscall(int ordinal, int* syscall);
		uint64_t generateSyscallCaller(int syscall);
		void closeProcess();
		void bluescreen();
		void changeWallpaper();
		bool registerIntegrity(const char* name, uint64_t address, BYTE expectedFirstByte = 0);
		bool getModuleMemoryIntegrityHash(HMODULE hModule, std::vector<BYTE> originalPE = std::vector<BYTE>());
		std::vector<imageSection> enumMemorySections(HMODULE hModule, std::vector<BYTE> originalPE);
		void unlink();
		void relink();
		void fakePE(uint64_t moduleBase);
		void printModules();

		std::vector<integrityItem>& getIntegrityItems() { return integrityItems; }
	private:
		std::vector<integrityItem> integrityItems;
	}; securityHelpers* getSecurity();
}