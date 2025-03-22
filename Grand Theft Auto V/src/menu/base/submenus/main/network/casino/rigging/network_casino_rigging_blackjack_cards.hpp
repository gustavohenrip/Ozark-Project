#pragma once
#include "menu/base/submenu.hpp"
#include "network_casino_rigging_blackjack.hpp"

class NetworkCasinoRiggingBlackjackCardsMenu : public Menu::Submenu {
public:
	static NetworkCasinoRiggingBlackjackCardsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkCasinoRiggingBlackjackCardsMenu()
		: Menu::Submenu() {}

	~NetworkCasinoRiggingBlackjackCardsMenu();
};

namespace NetworkCasinoRiggingBlackjackCardsMenuVars {
	struct Vars_ {};

	extern ScrollStruct<NetworkCasinoRiggingBlackjackMenuVars::eBlackjackCards> Cards[52];
	extern Vars_ m_Vars;
}