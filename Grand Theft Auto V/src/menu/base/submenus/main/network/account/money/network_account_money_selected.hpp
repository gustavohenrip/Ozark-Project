#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountMoneySelectedMenu : public Menu::Submenu {
public:
	static NetworkAccountMoneySelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountMoneySelectedMenu()
		: Menu::Submenu() {}

	~NetworkAccountMoneySelectedMenu();
};

namespace NetworkAccountMoneySelectedMenuVars {
	struct Vars_ {
		int m_Selected;
		int m_BuyChipCount = 6;
		int m_SellChipCount = 6;
		int m_ChipLoopCount = 6;
		int m_CurrentChips = 0;

		bool m_ChipLoopBuy;
		bool m_ChipLoopSell;
		bool m_ChipLoop;

		void* m_Fibers[4];
	};

	void HandleVehicleLoop();
	extern Vars_ m_Vars;
}