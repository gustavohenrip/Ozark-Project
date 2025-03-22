#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountMoneyMenu : public Menu::Submenu {
public:
	static NetworkAccountMoneyMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountMoneyMenu()
		: Menu::Submenu() {}

	~NetworkAccountMoneyMenu();
};

namespace NetworkAccountMoneyMenuVars {
	struct Vars_ {
		bool m_Cargo;
		int m_CargoAmount = 10;

		void* m_CargoFiber;
	};

	extern Vars_ m_Vars;
}