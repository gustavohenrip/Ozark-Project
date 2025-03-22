#include "network_account_money.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account.hpp"
#include "money/network_account_money_selected.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "rage/engine.hpp"

using namespace NetworkAccountMoneyMenuVars;

namespace NetworkAccountMoneyMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> CargoCount[] = {
		{ TranslationString("100k", true), 100000 },
		{ TranslationString("250k", true), 250000 },
		{ TranslationString("500k", true), 500000 },
		{ TranslationString("750k", true), 750000 },
		{ TranslationString("1m", true), 1000000 },
		{ TranslationString("2m", true), 2000000 },
		{ TranslationString("2.5m", true), 2500000 },
		{ TranslationString("3m", true), 3000000 },
		{ TranslationString("4m", true), 4000000 },
		{ TranslationString("5m", true), 5000000 },
		{ TranslationString("6m", true), 6000000 },
	};

	void HandleLoopCargo() {
		if (m_Vars.m_Cargo) {
			if (m_Vars.m_CargoFiber == nullptr) {
				m_Vars.m_CargoFiber = Utils::GetFiberManager()->Add("B_CARGO", [] {
					if (Rage::Engine::ProcessBasketTransaction(-1799524201, -1018905335, 4, { { -937301143, -1453621952, 18000, 0, 1 } })) {
						while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);

						if (Rage::Engine::ProcessBasketTransaction(-1799524201, 2108234284, 4, { { -937301143, -1453621952, 0, 1, 1 }, { 1295461001, -1, 0, 0, 3 }, { 980795479, 1295461001, 0, 3, 1 } })) {
							while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);

							if (Rage::Engine::ProcessBasketTransaction(-1799524201, -1018905335, 4, { { -937301143, 1679637634, 0, 0, 1 } })) {
								while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);

								if (Rage::Engine::ProcessBasketTransaction(-1799524201, -788753717, 4, { { -937301143, 1679637634, 0, 1, 1 }, { 1295461001, -1, 0, 1, 2 }, { 1783677420, 1295461001, CargoCount[m_Vars.m_CargoAmount].m_Result, 0, 1 } })) {
									while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
								}
							}
						}
					}
				});
			} else {
				for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
					if (Fiber.m_FiberHandle == m_Vars.m_CargoFiber) {
						Fiber.m_Running = true;
						break;
					}
				}
			}
		} else {
			for (auto& Fiber : Utils::GetFiberManager()->GetFibers()) {
				if (Fiber.m_FiberHandle == m_Vars.m_CargoFiber) {
					Fiber.m_Running = false;
					break;
				}
			}
		}
	}
}

void NetworkAccountMoneyMenu::Init() {
	SetName("Money");
	SetParentSubmenu<NetworkAccountMenu>();

	addOption(SubmenuOption("Casino Chips (2.5m/t)")
		.addTranslation()
		.addSubmenu<NetworkAccountMoneySelectedMenu>()
		.addOnClick([] { NetworkAccountMoneySelectedMenuVars::m_Vars.m_Selected = 1; }));

	addOption(ScrollOption<int>(TOGGLE, "Cargo (6m/t)")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Cargo)
		.addScroll(m_Vars.m_CargoAmount, 0, NUMOF(CargoCount), CargoCount)
		.addOnClick(HandleLoopCargo)
		.addTooltip("Securoserv cargo warehouse is needed"));
}

void NetworkAccountMoneyMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountMoneyMenu::UpdateOnce() {}

/*Called always*/
void NetworkAccountMoneyMenu::FeatureUpdate() {}

NetworkAccountMoneyMenu* _instance;
NetworkAccountMoneyMenu* NetworkAccountMoneyMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountMoneyMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountMoneyMenu::~NetworkAccountMoneyMenu() { delete _instance; }