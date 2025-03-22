#include "clipsets.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../animation.hpp"
#include "utils/fibers.hpp"
#include "utils/fiber_pool.hpp"

using namespace ClipsetMenuVars;

namespace ClipsetMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> Motion[] = {
		{ TranslationString("Businessman", true), "move_m@business@a" },
		{ TranslationString("Gangster", true), "move_m@gangster@generic" },
		{ TranslationString("Hero", true), "move_m@brave@a" },
		{ TranslationString("Hiker", true), "move_m@hiking" },
		{ TranslationString("Hobo", true), "move_m@hobo@a" },
		{ TranslationString("Injured", true), "move_m@injured" },
		{ TranslationString("Quick", true), "move_m@quick" },
		{ TranslationString("Wasted", true), "move_m@drunk@slightlydrunk" },
		{ TranslationString("Very Wasted", true), "move_m@drunk@verydrunk" }
	};

	ScrollStruct<const char*> Weapon[] = {
		{ TranslationString("Bag", true), "missfbi4prepp1_garbageman" },
		{ TranslationString("Bucket", true), "move_ped_wpn_bucket" },
		{ TranslationString("Cane", true), "move_lester_CaneUp" },
		{ TranslationString("Jerry Can", true), "move_ped_wpn_jerrycan_generic" },
		{ TranslationString("Mop", true), "move_ped_wpn_mop" },
		{ TranslationString("Rifle", true), "Wpn_AssaultRifle_WeaponHoldingCrouched" },
		{ TranslationString("Sack", true), "MOVE_P_M_ZERO_RUCKSACK" }
	};

	void StartClipset(bool Weapon, const char* Clipset) {
		Utils::GetFiberPool()->Push([=] {
			while (!Native::HasClipSetLoaded(Clipset)) {
				Native::RequestClipSet(Clipset);
				Utils::WaitFiber(100);
			}

			if (Weapon) {
				Native::ResetPedWeaponMovementClipset(Menu::GetLocalPlayer().m_Ped);
				Native::SetPedWeaponMovementClipset(Menu::GetLocalPlayer().m_Ped, Clipset);
			} else {
				Native::ResetPedMovementClipset(Menu::GetLocalPlayer().m_Ped, 0.f);
				Native::SetPedMovementClipset(Menu::GetLocalPlayer().m_Ped, Clipset, 1.f);
			}
		});
	}
}

void ClipsetMenu::Init() {
	SetName("Clipset");
	SetParentSubmenu<AnimationMenu>();

	addOption(ButtonOption("Default Motion Clipset")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::ResetPedMovementClipset(Menu::GetLocalPlayer().m_Ped, 0.f); }));

	addOption(ScrollOption<const char*>(SCROLLSELECT, "Motion")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Motion, 0, NUMOF(Motion), Motion)
		.addOnClick([] { StartClipset(false, Motion[m_Vars.m_Motion].m_Result); }));

	addOption(ButtonOption("Default Weapon Clipset")
		.addTranslation().addHotkey()
		.addOnClick([] { Native::ResetPedWeaponMovementClipset(Menu::GetLocalPlayer().m_Ped); }));

	addOption(ScrollOption<const char*>(SCROLLSELECT, "Weapon")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_Weapon, 0, NUMOF(ClipsetMenuVars::Weapon), ClipsetMenuVars::Weapon)
		.addOnClick([] { StartClipset(true, ClipsetMenuVars::Weapon[m_Vars.m_Weapon].m_Result); }));
}

void ClipsetMenu::Update() {}

/*Called once on submenu open*/
void ClipsetMenu::UpdateOnce() {}

/*Called always*/
void ClipsetMenu::FeatureUpdate() {}

ClipsetMenu* _instance;
ClipsetMenu* ClipsetMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ClipsetMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ClipsetMenu::~ClipsetMenu() { delete _instance; }