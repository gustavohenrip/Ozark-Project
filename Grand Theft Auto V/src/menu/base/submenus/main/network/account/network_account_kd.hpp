#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountKDMenu : public Menu::Submenu {
public:
	static NetworkAccountKDMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountKDMenu()
		: Menu::Submenu() {}

	~NetworkAccountKDMenu();
};

namespace NetworkAccountKDMenuVars {
	struct Vars_ {
		int m_Kills;
		int m_Deaths;
		float m_Calculated;
		std::shared_ptr<Option> m_Button;
	};

	extern Vars_ m_Vars;
}