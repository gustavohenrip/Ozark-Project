#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountWinLossEditMenu : public Menu::Submenu {
public:
	static NetworkAccountWinLossEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountWinLossEditMenu()
		: Menu::Submenu() {}

	~NetworkAccountWinLossEditMenu();
};

namespace NetworkAccountWinLossEditMenuVars {
	struct Vars_ {
		bool m_Total = false;

		std::pair<std::string, int> m_Wins;
		std::pair<std::string, int> m_Losses;
	};

	extern Vars_ m_Vars;
}