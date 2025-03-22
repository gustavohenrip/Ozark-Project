#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEventReactionsSelectedMenu : public Menu::Submenu {
public:
	static NetworkProtectionEventReactionsSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEventReactionsSelectedMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEventReactionsSelectedMenu();
};

namespace NetworkProtectionEventReactionsSelectedMenuVars {
	struct Vars_ {
		std::size_t m_Selected;
	};

	extern Vars_ m_Vars;
}