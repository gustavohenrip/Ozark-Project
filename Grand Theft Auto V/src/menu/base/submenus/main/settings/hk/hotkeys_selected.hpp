#pragma once
#include "menu/base/submenu.hpp"

class HotkeysSelectedMenu : public Menu::Submenu {
public:
	static HotkeysSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	HotkeysSelectedMenu()
		: Menu::Submenu() {}

	~HotkeysSelectedMenu();
};

namespace HotkeysSelectedMenuVars {
	struct Vars_ {
		void* m_Hotkey;
	};

	extern Vars_ m_Vars;
}