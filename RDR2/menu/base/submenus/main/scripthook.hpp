#pragma once
#include "menu/base/submenu.hpp"
#include "rage/invoker/native_invoker.hpp"
#include <set>

class scriptHookMenu : public menu::submenu {
public:
	static scriptHookMenu* getInstance();

	void init() override;
	void updateOnce() override;
	void update() override;
	void featureUpdate() override;

	scriptHookMenu()
		: menu::submenu()
	{}

	~scriptHookMenu();
};

namespace scriptHookMenuVars {
	struct scriptHookScript {
		bool initialized;
		bool loaded;
		HMODULE hModule;

		scriptHookScript()
			: initialized(false), loaded(false), hModule(0)
		{}
	};

	struct vars_ {
		rage::invoker::nativeSetup cxt;
		uint64_t nativeHash;
		std::unordered_map<HMODULE, std::pair<uint64_t, uint64_t>> modules;
		std::vector<void(*)(uint32_t, int16_t, uint8_t, int32_t, int32_t, int32_t, int32_t)> keyboards;
		std::unordered_map<std::string, scriptHookScript> scriptState;
	};

	extern vars_ vars;

	void scriptRegister(HMODULE hModule, void(*callback)());
	void scriptRegisterAdditionalThread(HMODULE hModule, void(*callback)());
	void scriptUnregister(HMODULE hModule);
	void nativeInit(uint64_t hash);
	void nativePush64(uint64_t val);
	uint64_t* nativeCall();
	uint64_t* getGlobalPtr(int32_t index);
	void scriptWait(uint32_t waitTime);
	void keyboardHandlerRegister(void(*callback)(uint32_t, int16_t, uint8_t, int32_t, int32_t, int32_t, int32_t));
	void keyboardHandlerUnregister(void(*callback)(uint32_t, int16_t, uint8_t, int32_t, int32_t, int32_t, int32_t));
	uint8_t* getScriptHandleBaseAddress(int handle);
	void presentCallbackRegister(void(*callback)());
	void presentCallbackUnregister(void(*callback)());
	int worldGetAllVehicles(int32_t* arr, int32_t arrSize);
	int worldGetAllPeds(int32_t* arr, int32_t arrSize);
	int worldGetAllObjects(int32_t* arr, int32_t arrSize);
	int worldGetAllPickups(int32_t* arr, int32_t arrSize);
}