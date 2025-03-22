#include "teleport_popular.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"
#include "global/arrays.hpp"

using namespace TeleportPopularMenuVars;

namespace TeleportPopularMenuVars {
	Vars_ m_Vars;
}

void TeleportPopularMenu::Init() {
	SetName("Popular");
	SetParentSubmenu<TeleportMenu>();

	for (auto& Loc : Global::Arrays::g_PopularLocations) {
		addOption(ButtonOption(Loc.m_Name.Get())
			.addTranslation()
			.addOnClick([=] { TeleportMenuVars::Teleport(Math::Vector3_<float>::ToPadded(Loc.m_Result)); }));
	}
}

void TeleportPopularMenu::Update() {}

/*Called once on submenu open*/
void TeleportPopularMenu::UpdateOnce() {}

/*Called always*/
void TeleportPopularMenu::FeatureUpdate() {}

TeleportPopularMenu* _instance;
TeleportPopularMenu* TeleportPopularMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TeleportPopularMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TeleportPopularMenu::~TeleportPopularMenu() { delete _instance; }