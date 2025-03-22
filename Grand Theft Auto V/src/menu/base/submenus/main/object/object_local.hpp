#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/esp.hpp"

class ObjectLocalMenu : public Menu::Submenu {
public:
	static ObjectLocalMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ObjectLocalMenu()
		: Menu::Submenu() {}

	~ObjectLocalMenu();
};

namespace ObjectLocalMenuVars {
	struct Vars_ {
		ESPMenuVars::ESP m_ESP;
	};

	extern Vars_ m_Vars;
}