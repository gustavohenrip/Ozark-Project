#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class ObjectSpawnerManageMenu : public Menu::Submenu {
public:
	static ObjectSpawnerManageMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectSpawnerManageMenu()
		: Menu::Submenu() {}

	~ObjectSpawnerManageMenu();
};

namespace ObjectSpawnerManageMenuVars {
	struct SpawnedObjectContext {
		Object m_Handle;
		uint32_t m_ModelHash = 0;
		std::string m_Name;
		int m_CreationTime = 0;
		ESPMenuVars::ESP m_ESP;
	};

	struct Vars_ {
		std::vector<SpawnedObjectContext> m_SpawnedObjects;
	};

	extern Vars_ m_Vars;
}