#pragma once
#include "menu/base/submenu.hpp"

class NetworkGlobalChatTargetsMenu : public Menu::Submenu {
public:
	static NetworkGlobalChatTargetsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkGlobalChatTargetsMenu()
		: Menu::Submenu() {}

	~NetworkGlobalChatTargetsMenu();
};

namespace NetworkGlobalChatTargetsMenuVars {
	struct Vars_ {
		bool m_Toggle;
	};

	extern Vars_ m_Vars;
}