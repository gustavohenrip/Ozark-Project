#pragma once
#include "menu/base/submenu.hpp"

class ObjectSpawnerSelectMenu : public Menu::Submenu {
public:
	static ObjectSpawnerSelectMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectSpawnerSelectMenu()
		: Menu::Submenu() {}

	~ObjectSpawnerSelectMenu();
};

namespace ObjectSpawnerSelectMenuVars {
	struct Vars_ {
		ScrollStruct<uint32_t>* m_ListPtr;
		int m_ListSize;
	};

	extern Vars_ m_Vars;
}