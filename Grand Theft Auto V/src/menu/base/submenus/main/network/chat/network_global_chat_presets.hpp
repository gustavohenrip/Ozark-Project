#pragma once
#include "menu/base/submenu.hpp"

class NetworkGlobalChatPresetsMenu : public Menu::Submenu {
public:
	static NetworkGlobalChatPresetsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkGlobalChatPresetsMenu()
		: Menu::Submenu() {}

	~NetworkGlobalChatPresetsMenu();
};

namespace NetworkGlobalChatPresetsMenuVars {
	struct Vars_ {
		std::string* m_Editing;
	};

	extern Vars_ m_Vars;
}