#pragma once
#include "menu/base/submenu.hpp"

class ObjectSpawnerMenu : public Menu::Submenu {
public:
	static ObjectSpawnerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectSpawnerMenu()
		: Menu::Submenu() {}

	~ObjectSpawnerMenu();
};

namespace ObjectSpawnerMenuVars {
	struct Vars_ {
	};

	void SpawnObject(uint32_t Model, const char* Name = nullptr);
	extern Vars_ m_Vars;
}