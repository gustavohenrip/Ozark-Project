#include "give_weapons.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../../weapon.hpp"
#include "give_weapon.hpp"
#include "give_weapon_attachment.hpp"
#include "give_weapon_tint.hpp"

using namespace GiveWeaponsMenuVars;

namespace GiveWeaponsMenuVars {
	Vars_ m_Vars;
}

void GiveWeaponsMenu::Init() {
	SetName("Give Weapons");
	SetParentSubmenu<WeaponMenu>();

	addOption(SubmenuOption("Give Weapon")
		.addTranslation().addHotkey()
		.addSubmenu<GiveWeaponMenu>());

	addOption(SubmenuOption("Give Weapon Attachment")
		.addTranslation().addHotkey()
		.addSubmenu<GiveWeaponAttachmentMenu>());

	addOption(SubmenuOption("Give Weapon Tint")
		.addTranslation().addHotkey()
		.addSubmenu<GiveWeaponTintMenu>());
}

void GiveWeaponsMenu::Update() {}

/*Called once on submenu open*/
void GiveWeaponsMenu::UpdateOnce() {}

/*Called always*/
void GiveWeaponsMenu::FeatureUpdate() {}

GiveWeaponsMenu* _instance;
GiveWeaponsMenu* GiveWeaponsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GiveWeaponsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

GiveWeaponsMenu::~GiveWeaponsMenu() { delete _instance; }