#pragma once
#include "menu/base/submenu.hpp"

class NetworkProtectionEntityLimitsAddMenu : public Menu::Submenu {
public:
	static NetworkProtectionEntityLimitsAddMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkProtectionEntityLimitsAddMenu()
		: Menu::Submenu() {}

	~NetworkProtectionEntityLimitsAddMenu();
};

namespace NetworkProtectionEntityLimitsAddMenuVars {
	struct Vars_ {
		uint32_t m_Model = 0;
		int m_Delay = 0;
		std::string m_Comment = "";
	};

	extern Vars_ m_Vars;
}