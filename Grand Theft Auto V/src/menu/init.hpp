#pragma once
#include "stdafx.hpp"

namespace Menu {
	class Init {
	public:
		static void UnloadThread();
		static void Entry(HMODULE hModule);
		static void ExternalUnload();
		static void HeartbeatThread();
		static void ScriptHookThread();
		static void GeoIPThread();

		void AntiCheat();
		void Globals();
		void GlobalsInGame();
		void Unload();
	};

	Init* GetInit();
}