#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"

namespace menu {
	class init {
	public:
		static void unloadThread();
		static void presenceThread();
		static void scriptHookThread();
		static void modelBypassThread();
		static void heartbeatThread();
		static void geoIPThread();
		static void rockstarAccountThread();
		static void ntVirtualQueryMemoryRdrBypass();
		static void entry(HMODULE hModule);
		void anticheat();
		void globals();
		void unload();

		LPTOP_LEVEL_EXCEPTION_FILTER originalExceptionFilter;
	}; init* getInit();
}