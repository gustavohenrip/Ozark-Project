#pragma once
#include "menu/base/submenu.hpp"

class MiscScriptHookMenu : public Menu::Submenu {
public:
	static MiscScriptHookMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscScriptHookMenu()
		: Menu::Submenu() {}

	~MiscScriptHookMenu();
};

namespace MiscScriptHookMenuVars {
	struct ScriptHookScript {
		bool m_Initialized;
		bool m_Loaded;
		HMODULE m_Module;

		ScriptHookScript()
			: m_Initialized(false), m_Loaded(false), m_Module(0) 
		{}
	};

	struct Vars_ {
		Rage::Invoker::NativeSetup m_Context;
		uint64_t m_NativeHash;
		std::unordered_map<HMODULE, std::pair<uint64_t, uint64_t>> m_Modules;
		std::vector<void(*)(uint32_t, int16_t, uint8_t, int, int, int, int)> m_Keyboards;
		std::unordered_map<std::string, ScriptHookScript> m_ScriptState;
	};

	namespace Proxy {
		void ScriptRegister(HMODULE hModule, void(*callback)());
		void ScriptRegisterAdditionalThread(HMODULE hModule, void(*callback)());
		void ScriptUnregister(HMODULE hModule);
		void NativeInit(uint64_t hash);
		void NativePush64(uint64_t val);
		uint64_t* NativeCall();
		uint64_t* GetGlobalPtr(int index);
		void ScriptWait(uint32_t waitTime);
		void KeyboardHandlerRegister(void(*callback)(uint32_t, int16_t, uint8_t, int, int, int, int));
		void KeyboardHandlerUnregister(void(*callback)(uint32_t, int16_t, uint8_t, int, int, int, int));
		uint8_t* GetScriptHandleBaseAddress(int handle);
		void PresentCallbackRegister(void(*callback)());
		void PresentCallbackUnregister(void(*callback)());
		int WorldGetAllVehicles(int* arr, int arrSize);
		int WorldGetAllPeds(int* arr, int arrSize);
		int WorldGetAllObjects(int* arr, int arrSize);
		int WorldGetAllPickups(int* arr, int arrSize);
	}

	extern Vars_ m_Vars;
}