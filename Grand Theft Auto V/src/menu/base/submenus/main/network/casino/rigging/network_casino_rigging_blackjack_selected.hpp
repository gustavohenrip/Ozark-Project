#pragma once
#include "menu/base/submenu.hpp"

class NetworkCasinoRiggingBlackjackSelectedMenu : public Menu::Submenu {
public:
	static NetworkCasinoRiggingBlackjackSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkCasinoRiggingBlackjackSelectedMenu()
		: Menu::Submenu() {}

	~NetworkCasinoRiggingBlackjackSelectedMenu();
};

namespace NetworkCasinoRiggingBlackjackSelectedMenuVars {
	struct Rig {
		bool m_First;
		int m_FirstCard;
		bool m_Second;
		int m_SecondCard;
	};
	
	struct BlackjackCardRigging {
		Rig m_PlayerCards[5]; // 4 index is reserved for dealer
	};

	struct Vars_ {
		int m_Selected;
		int m_SelectedSeat = 0;
		BlackjackCardRigging m_Tables[4];
	};

	extern Vars_ m_Vars;
}