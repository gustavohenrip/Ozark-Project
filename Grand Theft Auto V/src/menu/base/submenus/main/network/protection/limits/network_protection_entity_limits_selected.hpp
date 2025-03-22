#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEntityLimitsSelectedMenu : public Menu::Submenu {
public:
	static NetworkProtectionEntityLimitsSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEntityLimitsSelectedMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEntityLimitsSelectedMenu();
};

namespace NetworkProtectionEntityLimitsSelectedMenuVars {
	struct Vars_ {
		std::size_t m_Selected;
	};

	extern Vars_ m_Vars;
}