#pragma once
#include "menu/base/submenu.hpp"
#include "menu/base/submenus/main/helpers/structs.hpp"

class NetworkProtectionEventHandlerMenu : public Menu::Submenu {
public:
	static NetworkProtectionEventHandlerMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEventHandlerMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEventHandlerMenu();
};

namespace NetworkProtectionEventHandlerMenuVars {
	struct Vars_ {
		Menu::Structs::ProtectionEventContext* m_Context;
	};

	extern Vars_ m_Vars;
}