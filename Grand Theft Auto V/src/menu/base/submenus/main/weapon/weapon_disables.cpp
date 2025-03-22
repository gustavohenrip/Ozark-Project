#include "weapon_disables.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "menu/base/util/timers.hpp"
#include "menu/base/util/stats.hpp"

using namespace WeaponDisablesMenuVars;

namespace WeaponDisablesMenuVars {
	Vars_ m_Vars;

	void DisableWeaponLimits() {
		if (m_Vars.m_DisableVehicleWeaponLimits) WeaponModsMenuVars::m_Vars.m_WeaponGroups.clear();

		uint64_t Table = *(uint64_t*)Global::Vars::g_WeaponInfoList;
		if (Table) {
			for (int16_t i = 0; i < (*(int16_t*)Global::Vars::g_WeaponInfoListCount) - 1; i++) {
				Rage::Types::Weapon* Ptr = (Rage::Types::Weapon*) * (uint64_t*)(Table + (i * 8));
				if (Ptr) {
					if (!m_Vars.m_DisableVehicleWeaponLimits) {
						auto Vit = std::find_if(WeaponModsMenuVars::m_Vars.m_WeaponGroups.begin(), WeaponModsMenuVars::m_Vars.m_WeaponGroups.end(), [=] (std::pair<uint32_t, uint32_t>& Element) {
							return Element.first == Ptr->m_NameHash;
						});

						if (Vit != WeaponModsMenuVars::m_Vars.m_WeaponGroups.end()) {
							Ptr->m_GroupHash = Vit->second;
						}
					} else {
						uint32_t WeaponGroups[] = {
							0x18D5FA97,
							0xC6E9A5C5,
							0x39D5C192,
							0x451B04BC,
							0x33431399,
							0xB7BBD827,
							0xA27A4F9F,
							0x29268262,
							0xFDBF656C
						};

						for (auto& Hash : WeaponGroups) {
							if (Ptr->m_GroupHash == Hash) {
								WeaponModsMenuVars::m_Vars.m_WeaponGroups.push_back({ Ptr->m_NameHash, Ptr->m_GroupHash });
								Ptr->m_GroupHash = 0x18D5FA97; // Set to a Pistol
								break;
							}
						}
					}
				}
			}
		}
	}
}

void WeaponDisablesMenu::Init() {
	SetName("Disables");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ToggleOption("Disable Vehicle Restrictions")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableVehicleWeaponLimits)
		.addOnClick(DisableWeaponLimits).canBeSaved(m_ParentNameStack)
		.addTooltip("Disables not being able to use any weapon in vehicles"));

	addOption(ToggleOption("Disable Passive Restrictions")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_PassiveWeapons).canBeSaved(m_ParentNameStack)
		.addTooltip("Allows you to use weapons while in passive"));

	addOption(ToggleOption("Disable Interior Restrictions")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_InteriorWeapons).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Disable Orbital Cooldown")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_DisableOrbitalCooldown).canBeSaved(m_ParentNameStack));
}

void WeaponDisablesMenu::Update() {}

/*Called once on submenu open*/
void WeaponDisablesMenu::UpdateOnce() {}

/*Called always*/
void WeaponDisablesMenu::FeatureUpdate() {
	if (m_Vars.m_DisableOrbitalCooldown) {
		static int Timer = 0;
		Menu::Timers::RunTimedFunction(&Timer, 1000, [] {
			Stats::Interface<int>::Set("ORBITAL_CANNON_COOLDOWN", 0);
		});
	}
}

WeaponDisablesMenu* _instance;
WeaponDisablesMenu* WeaponDisablesMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WeaponDisablesMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WeaponDisablesMenu::~WeaponDisablesMenu() { delete _instance; }