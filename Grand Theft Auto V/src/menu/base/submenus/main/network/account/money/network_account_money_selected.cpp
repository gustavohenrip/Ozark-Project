#include "network_account_money_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account_money.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "rage/engine.hpp"
#include "utils/caller.hpp"
#include "menu/base/util/stats.hpp"
#include "menu/base/util/notify.hpp"
#include "global/arrays.hpp"
#include "utils/va.hpp"
#include "rage/engine.hpp"

using namespace NetworkAccountMoneySelectedMenuVars;

namespace NetworkAccountMoneySelectedMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> ChipCount[] = {
		{ TranslationString("100k", true), 100000 },
		{ TranslationString("250k", true), 250000 },
		{ TranslationString("500k", true), 500000 },
		{ TranslationString("750k", true), 750000 },
		{ TranslationString("1m", true), 1000000 },
		{ TranslationString("2m", true), 2000000 },
		{ TranslationString("2.5m", true), 2500000 },
		{ TranslationString("All", true), -1 }
	};

	bool CanBuyChips() {
		return !((m_Vars.m_CurrentChips + ChipCount[m_Vars.m_BuyChipCount].m_Result > std::numeric_limits<int>::max() 
			|| (m_Vars.m_CurrentChips + ChipCount[m_Vars.m_BuyChipCount].m_Result) < 0));
	}

	bool CanSellChips() {
		return m_Vars.m_CurrentChips >= ChipCount[m_Vars.m_SellChipCount].m_Result;
	}

	void BuyChips() {
		if (m_Vars.m_ChipLoopBuy || m_Vars.m_ChipLoopSell || m_Vars.m_ChipLoop) return;

		if (CanBuyChips()) {
			Utils::GetFiberPool()->Push([=] {
				Rage::Engine::ProcessBasketTransaction(1950528552, -126744038, 4, { { 657241867, -1, 0, 0, ChipCount[m_Vars.m_BuyChipCount].m_Result }, { 1196301501, -1, 0, 0, 1 } });
			});
		} else {
			Menu::GetNotify()->NotifyStacked(NetworkAccountMoneySelectedMenu::GetInstance()->getString("You can't buy any more chips, sell some first"), Global::UIVars::g_NotificationError);
		}
	}

	void SellChips() {
		if (m_Vars.m_ChipLoopBuy || m_Vars.m_ChipLoopSell || m_Vars.m_ChipLoop) return;

		int Count = ChipCount[m_Vars.m_SellChipCount].m_Result;
		if (!CanSellChips() || Count == -1) {
			if (m_Vars.m_CurrentChips <= 0) return;
			Count = m_Vars.m_CurrentChips;
		}

		Utils::GetFiberPool()->Push([=] {
			if (Rage::Engine::ProcessBasketTransaction(1950528552, -22148635, 4, { { 657241867, -1, 0, 0, 25 }, { -1304782539, -1, 0, 0, 1 } })) {
				while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
				Rage::Engine::ProcessBasketTransaction(1950528552, -22148635, 4, { { 657241867, -1, 1, 0, Count }, { -1612659516, -1, 0, 0, 1 } });
				while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
			}
		});
	}

	void HandleLoopBuyChips() {
		if (m_Vars.m_ChipLoopBuy) {
			if (m_Vars.m_Fibers[0] == nullptr) {
				m_Vars.m_Fibers[0] = Utils::GetFiberManager()->Add("B_CHIPS", [] {
					if (CanBuyChips()) {
						Rage::Engine::ProcessBasketTransaction(1950528552, -126744038, 4, { { 657241867, -1, 0, 0, ChipCount[m_Vars.m_BuyChipCount].m_Result }, { 1196301501, -1, 0, 0, 1 } });
						// while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
					} else {
						Menu::GetNotify()->NotifyStacked(NetworkAccountMoneySelectedMenu::GetInstance()->getString("Cap reached for casino chip purchasing, disabling loop"), Global::UIVars::g_NotificationError);
						m_Vars.m_ChipLoopBuy = false;

						for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
							if (Fiber.m_FiberHandle == m_Vars.m_Fibers[0]) {
								Fiber.m_Running = false;
								break;
							}
						}
					}
				});
			} else {
				for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
					if (Fiber.m_FiberHandle == m_Vars.m_Fibers[0]) {
						Fiber.m_Running = true;
						break;
					}
				}
			}
		} else {
			for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
				if (Fiber.m_FiberHandle == m_Vars.m_Fibers[0]) {
					Fiber.m_Running = false;
					break;
				}
			}
		}
	}

	void HandleLoopSellChips() {
		if (m_Vars.m_ChipLoopSell) {
			if (m_Vars.m_Fibers[1] == nullptr) {
				m_Vars.m_Fibers[1] = Utils::GetFiberManager()->Add("S_CHIPS", [] {
					int Count = ChipCount[m_Vars.m_SellChipCount].m_Result;

					if (!CanSellChips()) {
						if (m_Vars.m_CurrentChips <= 0) {
							m_Vars.m_ChipLoopSell = false;

							for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
								if (Fiber.m_FiberHandle == m_Vars.m_Fibers[1]) {
									Fiber.m_Running = false;
									break;
								}
							}

							return;
						}

						Count = m_Vars.m_CurrentChips;
					}

					if (Rage::Engine::ProcessBasketTransaction(1950528552, -22148635, 4, { { 657241867, -1, 0, 0, 25 }, { -1304782539, -1, 0, 0, 1 } })) {
						while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
						Rage::Engine::ProcessBasketTransaction(1950528552, -22148635, 4, { { 657241867, -1, 1, 0, Count }, { -1612659516, -1, 0, 0, 1 } });
						while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
					}
				});
			} else {
				for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
					if (Fiber.m_FiberHandle == m_Vars.m_Fibers[1]) {
						Fiber.m_Running = true;
						break;
					}
				}
			}
		} else {
			for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
				if (Fiber.m_FiberHandle == m_Vars.m_Fibers[1]) {
					Fiber.m_Running = false;
					break;
				}
			}
		}
	}

	void HandleLoopChips() {
		if (m_Vars.m_ChipLoop) {
			if (m_Vars.m_Fibers[2] == nullptr) {
				m_Vars.m_Fibers[2] = Utils::GetFiberManager()->Add("L_CHIPS", [] {
					if (!CanSellChips()) {
						if (Rage::Engine::ProcessBasketTransaction(1950528552, -22148635, 4, { { 657241867, -1, 0, 0, 75 }, { -1304782539, -1, 0, 0, 1 } })) {
							while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
							Rage::Engine::ProcessBasketTransaction(1950528552, -126744038, 4, { { 657241867, -1, 0, 0, ChipCount[m_Vars.m_ChipLoopCount].m_Result }, { 1196301501, -1, 0, 0, 1 } });
							while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
						}
					}

					// Sell
					if (Rage::Engine::ProcessBasketTransaction(1950528552, -22148635, 4, { { 657241867, -1, 0, 0, 25 }, { -1304782539, -1, 0, 0, 1 } })) {
						while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
						Rage::Engine::ProcessBasketTransaction(1950528552, -22148635, 4, { { 657241867, -1, 1, 0, ChipCount[m_Vars.m_ChipLoopCount].m_Result }, { -1612659516, -1, 0, 0, 1 } });
						while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
					}
				});
			} else {
				for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
					if (Fiber.m_FiberHandle == m_Vars.m_Fibers[2]) {
						Fiber.m_Running = true;
						break;
					}
				}
			}
		} else {
			for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
				if (Fiber.m_FiberHandle == m_Vars.m_Fibers[2]) {
					Fiber.m_Running = false;
					break;
				}
			}
		}
	}
}

void NetworkAccountMoneySelectedMenu::Init() {
	SetName("Money");
	SetParentSubmenu<NetworkAccountMoneyMenu>();

	addString("Buy Chips");
	addString("Sell Chips");
	addString("Loop Buy Chips");
	addString("Loop Sell Chips");
	addString("Loop Both");
	addString("Buy then sell chips to deposit $ into your account");
	addString("You can't buy any more chips, sell some first");
	addString("Cap reached for casino chip purchasing, disabling loop");
}

void NetworkAccountMoneySelectedMenu::Update() {
	m_Vars.m_CurrentChips = Stats::Interface<int>::Get("CASINO_CHIPS");
}

/*Called once on submenu open*/
void NetworkAccountMoneySelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	/* Casino Chips */
	if (m_Vars.m_Selected == 1) {
		addOption(ScrollOption<int>(SCROLLSELECT, getString("Buy Chips"))
			.addScroll(m_Vars.m_BuyChipCount, 0, NUMOF(ChipCount) - 1, ChipCount)
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addTooltip(Utils::VA::VA("Current Chips: %i", m_Vars.m_CurrentChips)); })
			.addOnClick(BuyChips));

		addOption(ScrollOption<int>(SCROLLSELECT, getString("Sell Chips"))
			.addScroll(m_Vars.m_SellChipCount, 0, NUMOF(ChipCount), ChipCount)
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addTooltip(Utils::VA::VA("Current Chips: %i", m_Vars.m_CurrentChips)); })
			.addOnClick(SellChips));

		addOption(ToggleOption(getString("Loop Buy Chips"))
			.addToggle(m_Vars.m_ChipLoopBuy)
			.addOnUpdate([](ToggleOption* Option) { Option->addTooltip(Utils::VA::VA("Current Chips: %i", m_Vars.m_CurrentChips)); })
			.addOnClick([] {
				m_Vars.m_ChipLoopSell = false;
				m_Vars.m_ChipLoop = false;
				HandleLoopChips(); // Disables the loop
				HandleLoopSellChips(); // Disables the sell loop
				HandleLoopBuyChips();
			}));

		addOption(ToggleOption(getString("Loop Sell Chips"))
			.addToggle(m_Vars.m_ChipLoopSell)
			.addOnUpdate([](ToggleOption* Option) { Option->addTooltip(Utils::VA::VA("Current Chips: %i", m_Vars.m_CurrentChips)); })
			.addOnClick([] {
				m_Vars.m_ChipLoopBuy = false;
				m_Vars.m_ChipLoop = false;
				HandleLoopChips(); // Disables the loop
				HandleLoopBuyChips(); // Disables the buy loop
				HandleLoopSellChips();
			}));

		addOption(ScrollOption<int>(TOGGLE, getString("Loop Both"))
			.addScroll(m_Vars.m_ChipLoopCount, 0, NUMOF(ChipCount) - 1, ChipCount)
			.addToggle(m_Vars.m_ChipLoop)
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addTooltip(Utils::VA::VA("Current Chips: %i", m_Vars.m_CurrentChips)); })
			.addOnClick([] {
				m_Vars.m_ChipLoopBuy = false;
				m_Vars.m_ChipLoopSell = false;
				HandleLoopBuyChips(); // Disables the buy loop
				HandleLoopSellChips(); // Disables the sell loop
				HandleLoopChips();
			}));

		return;
	}
}

/*Called always*/
void NetworkAccountMoneySelectedMenu::FeatureUpdate() {
	if (m_Vars.m_ChipLoopBuy || m_Vars.m_ChipLoopSell || m_Vars.m_ChipLoop) {
		m_Vars.m_CurrentChips = Stats::Interface<int>::Get("CASINO_CHIPS");
	}
}

NetworkAccountMoneySelectedMenu* _instance;
NetworkAccountMoneySelectedMenu* NetworkAccountMoneySelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountMoneySelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountMoneySelectedMenu::~NetworkAccountMoneySelectedMenu() { delete _instance; }