#include "give_weapon_tint_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "give_weapon_tint.hpp"
#include "utils/utils.hpp"

using namespace GiveWeaponTintSelectedMenuVars;

namespace GiveWeaponTintSelectedMenuVars {
	Vars_ m_Vars;
}

void GiveWeaponTintSelectedMenu::Init() {
	SetName("Give Weapon Tint");
	SetParentSubmenu<GiveWeaponTintMenu>();
}

void GiveWeaponTintSelectedMenu::Update() {}

/*Called once on submenu open*/
void GiveWeaponTintSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	auto Camos = GiveWeaponTintMenuVars::GetWeaponTints(m_Vars.m_Selected);

	m_Vars.m_Context.Reset();
	m_Vars.m_Context.m_Selected = 0;

	std::vector<std::shared_ptr<RadioOption>> Options;
	for (auto& Camo : Camos) {
		std::string LabelText = Native::_GetLabelText(Camo.first.c_str());
		Utils::ReplaceString(LabelText, "tint", "Tint");

		Options.push_back(addOption(RadioOption(Native::_GetLabelText(Camo.first.c_str()))
			.addRadio(m_Vars.m_Context)
			.addOnClick([=] { Native::SetPedWeaponTintIndex(Menu::GetLocalPlayer().m_Ped, m_Vars.m_Selected, Camo.second); })));
	}

	if (Native::GetPedWeaponTintIndex(Menu::GetLocalPlayer().m_Ped, m_Vars.m_Selected) != 0) {
		Options[Native::GetPedWeaponTintIndex(Menu::GetLocalPlayer().m_Ped, m_Vars.m_Selected)]->UpdateSelected();
	}
}

/*Called always*/
void GiveWeaponTintSelectedMenu::FeatureUpdate() {}

GiveWeaponTintSelectedMenu* _instance;
GiveWeaponTintSelectedMenu* GiveWeaponTintSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new GiveWeaponTintSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

GiveWeaponTintSelectedMenu::~GiveWeaponTintSelectedMenu() { delete _instance; }