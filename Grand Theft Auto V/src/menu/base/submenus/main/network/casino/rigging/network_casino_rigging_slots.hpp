#pragma once
#include "menu/base/submenu.hpp"

class NetworkCasinoRiggingSlotsMenu : public Menu::Submenu {
public:
	static NetworkCasinoRiggingSlotsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkCasinoRiggingSlotsMenu()
		: Menu::Submenu() {}

	~NetworkCasinoRiggingSlotsMenu();
};

namespace NetworkCasinoRiggingSlotsMenuVars {
	enum eCasinoSlots {
		WHEEL_SYMBOL_NONE,
		WHEEL_SYMBOL_CHERRY,
		WHEEL_SYMBOL_PLUM,
		WHEEL_SYMBOL_MELON,
		WHEEL_SYMBOL_BELL,
		WHEEL_SYMBOL_SEVEN,
		WHEEL_SYMBOL_JACKPOT,
		WHEEL_SYMBOL_ANKH,
	};

	struct Vars_ {
		bool m_First;
		bool m_Second;
		bool m_Third;

		int m_FirstSlot;
		int m_SecondSlot;
		int m_ThirdSlot;
	};

	extern Vars_ m_Vars;
}