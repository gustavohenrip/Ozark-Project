#include "give_weapon_attachment_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "give_weapon_attachment.hpp"
#include "../ammunation.hpp"
#include "rage/engine.hpp"

using namespace GiveWeaponAttachmentMenuSelectedVars;

namespace GiveWeaponAttachmentMenuSelectedVars {
	Vars_ m_Vars;
}

void GiveWeaponAttachmentMenuSelected::Init() {
	SetName("Give Weapon Attachment");
	SetParentSubmenu<GiveWeaponAttachmentMenu>();
}

void GiveWeaponAttachmentMenuSelected::Update() {}

/*Called once on submenu open*/
void GiveWeaponAttachmentMenuSelected::UpdateOnce() {
	ClearOptionsOffset(0);

	auto Hashes = Rage::Engine::GetHashListFromHashTable(*(uint32_t*)(Global::Vars::g_WeaponComponentHashListCount) - 1, Global::Vars::g_WeaponComponentHashList);
	for (std::size_t i = 0; i < Hashes.size(); i++) {
		uint32_t Component = Hashes[i];
		if (Native::DoesWeaponTakeWeaponComponent(m_Vars.m_SelectedWeapon, Component)) {
			const char* Label = AmmunationMenuVars::Scripts::func_181(Component, m_Vars.m_SelectedWeapon);
			if (Label) {
				if (Native::HasPedGotWeaponComponent(Menu::GetLocalPlayer().m_Ped, m_Vars.m_SelectedWeapon, Component)) {
					addOption(ButtonOption(Native::_GetLabelText(Label))
						.addOnClick([=, this] {
							Native::RemoveWeaponComponentFromPed(Menu::GetLocalPlayer().m_Ped, m_Vars.m_SelectedWeapon, Component);
							UpdateOnce();
						})
						.addSprite("commonmenu", "shop_gunclub_icon")
						.addSpriteScale(0.02112f, 0.03744f));
				} else {
					addOption(ButtonOption(Native::_GetLabelText(Label))
						.addOnClick([=, this] {
							Native::GiveWeaponComponentToPed(Menu::GetLocalPlayer().m_Ped, m_Vars.m_SelectedWeapon, Component);
							UpdateOnce();
						}));
				}
			}
		}
	}
}

/*Called always*/
void GiveWeaponAttachmentMenuSelected::FeatureUpdate() {}

GiveWeaponAttachmentMenuSelected* _instance;
GiveWeaponAttachmentMenuSelected* GiveWeaponAttachmentMenuSelected::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GiveWeaponAttachmentMenuSelected();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

GiveWeaponAttachmentMenuSelected::~GiveWeaponAttachmentMenuSelected() { delete _instance; }