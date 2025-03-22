#include "purchase_weapon_attachment.hpp"
#include "menu/base/submenu_handler.hpp"
#include "purchase_weapons.hpp"
#include "rage/engine.hpp"
#include "purchase_weapon.hpp"
#include "utils/fiber_pool.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "utils/fibers.hpp"

using namespace PurchaseWeaponAttachmentMenuVars;

namespace PurchaseWeaponAttachmentMenuVars {
	Vars_ m_Vars;

	std::vector<Transaction> CreateComponentTransactionList(uint32_t Weapon) {
		std::vector<Transaction> List;

		auto Hashes = Rage::Engine::GetHashListFromHashTable(*(uint32_t*)(Global::Vars::g_WeaponComponentHashListCount) - 1, Global::Vars::g_WeaponComponentHashList);
		for (uint32_t Component : Hashes) {
			if (Native::DoesWeaponTakeWeaponComponent(Weapon, Component)) {
				if (!Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, Weapon, Component)) {
					char Output[0x100];
					PurchaseWeaponMenuVars::Scripts::func_116(Output, PurchaseWeaponMenuVars::Scripts::func_181(Component, (int)Weapon), Native::GetEntityModel(Native::PlayerPedId()), 3, 1, PurchaseWeaponMenuVars::Scripts::func_57((int)Weapon), PurchaseWeaponMenuVars::Scripts::func_117((int)Weapon), -1, 0, 0);

					int TransactionHash = (int)Native::GetHashKey(Output);

					if (Native::_NetworkShopGetPrice(TransactionHash, 1671640801, true) != -1) {
						List.push_back({ TransactionHash, -1, Native::_NetworkShopGetPrice(TransactionHash, 1671640801, true), 1, 1 });
					}
				}
			}
		}

		return List;
	}

	int CalculateTotalPrice() {
		int Total = 0;

		for (auto Hash : Global::Arrays::g_Weapons) {
			auto List = CreateComponentTransactionList(Hash.m_Result);
			if (!List.empty()) {
				for (auto& L : List) {
					Total += L.m_Price;
				}
			}
		}

		return Total;
	}

	void PurchaseAllAttachments() {
		Utils::GetFiberPool()->Push([] {
			for (auto Hash : Global::Arrays::g_Weapons) {
				auto List = CreateComponentTransactionList(Hash.m_Result);
				if (!List.empty()) {
					int Total = 0;
					for (auto& L : List) {
						Total += L.m_Price;
					}

					Rage::Engine::ProcessBasketTransaction(1671640801, 537254313, 4, List);
					while (Rage::Engine::IsBasketTransactionQueueBusy()) Utils::WaitFiber(1);
				}
			}

			Menu::GetNotify()->NotifyStacked(PurchaseWeaponAttachmentMenu::GetInstance()->getString("Purchase complete! Change sessions to take affect"));
		});
	}
}

void PurchaseWeaponAttachmentMenu::Init() {
	SetName("Purchase Weapon Attachment");
	SetParentSubmenu<PurchaseWeaponsMenu>();

	addString("Total price: $");
	addString("Purchase complete! Change sessions to take affect");

	m_Vars.m_Button = addOption(ButtonOption("Purchase All Weapon Attachments")
		.addTranslation().addHotkey()
		.addOnClick(PurchaseAllAttachments));
}

void PurchaseWeaponAttachmentMenu::Update() {}

/*Called once on submenu open*/
void PurchaseWeaponAttachmentMenu::UpdateOnce() {
	m_Vars.m_Button->setTooltipInternal(getString("Total price: $") + std::to_string(CalculateTotalPrice()));
}

/*Called always*/
void PurchaseWeaponAttachmentMenu::FeatureUpdate() {}

PurchaseWeaponAttachmentMenu* _instance;
PurchaseWeaponAttachmentMenu* PurchaseWeaponAttachmentMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PurchaseWeaponAttachmentMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PurchaseWeaponAttachmentMenu::~PurchaseWeaponAttachmentMenu() { delete _instance; }