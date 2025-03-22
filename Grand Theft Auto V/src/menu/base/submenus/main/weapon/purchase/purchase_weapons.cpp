#include "purchase_weapons.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../../weapon.hpp"
#include "purchase_weapon.hpp"
#include "purchase_weapon_attachment.hpp"

using namespace PurchaseWeaponsMenuVars;

namespace MenuVars {
	Vars_ m_Vars;
}

void PurchaseWeaponsMenu::Init() {
	SetName("Purchase Weapons");
	SetParentSubmenu<WeaponMenu>();

	addOption(SubmenuOption("Purchase Weapon")
		.addTranslation().addHotkey()
		.addSubmenu<PurchaseWeaponMenu>());

	addOption(SubmenuOption("Purchase Weapon Attachment")
		.addTranslation().addHotkey()
		.addSubmenu<PurchaseWeaponAttachmentMenu>());
}

void PurchaseWeaponsMenu::Update() {}

/*Called once on submenu open*/
void PurchaseWeaponsMenu::UpdateOnce() {}

/*Called always*/
void PurchaseWeaponsMenu::FeatureUpdate() {}

PurchaseWeaponsMenu* _instance;
PurchaseWeaponsMenu* PurchaseWeaponsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PurchaseWeaponsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PurchaseWeaponsMenu::~PurchaseWeaponsMenu() { delete _instance; }