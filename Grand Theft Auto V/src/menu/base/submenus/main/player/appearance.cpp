#include "appearance.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../player.hpp"
#include "model.hpp"
#include "wardrobe.hpp"

using namespace AppearanceMenuVars;

namespace AppearanceMenuVars {
	Vars_ m_Vars;
}

void AppearanceMenu::Init() {
	SetName("Appearance");
	SetParentSubmenu<PlayerMenu>();

	addOption(SubmenuOption("Model")
		.addTranslation().addHotkey()
		.addSubmenu<ModelMenu>());

	addOption(SubmenuOption("Wardrobe")
		.addTranslation().addHotkey()
		.addSubmenu<WardrobeMenu>()
		.addOnClick([] { WardrobeMenu::GetInstance()->SetParentSubmenu<AppearanceMenu>(); }));

	addOption(BreakOption("Transform")
		.addTranslation());

	addOption(NumberOption<float>(TOGGLE, "Width")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Width)
		.addNumber(m_Vars.m_WidthVal, "%.1f", 0.1f).addMin(-1.2f).addMax(10.f)
		.addTooltip("~y~Notice: ~w~Local only"));

	addOption(NumberOption<float>(TOGGLE, "Height")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Height)
		.addNumber(m_Vars.m_HeightVal, "%.1f", 0.1f).addMin(-1.f).addMax(10.f)
		.addTooltip("~y~Notice: ~w~Local only"));

	addOption(NumberOption<float>(TOGGLE, "Lean (F/B)")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_LeanFB)
		.addNumber(m_Vars.m_LeanFBVal, "%.1f", 0.1f).addMin(-10.f).addMax(10.f)
		.addTooltip("~y~Notice: ~w~Local only"));

	addOption(NumberOption<float>(TOGGLE, "Lean (L/R)")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_LeanLR)
		.addNumber(m_Vars.m_LeanLRVal, "%.1f", 0.1f).addMin(-10.f).addMax(10.f)
		.addTooltip("~y~Notice: ~w~Local only"));
}

void AppearanceMenu::Update() {
	if (IsValidPtr(Global::Vars::g_PedFactory)) {
		auto Ped = Global::Vars::g_PedFactory->m_LocalPed;
		if (IsValidPtr(Ped)) {
			if (m_Vars.m_Width) Ped->m_Transform.m_Left.m_X = m_Vars.m_WidthVal;
			else m_Vars.m_WidthVal = Ped->m_Transform.m_Left.m_X;

			if (m_Vars.m_Height) Ped->m_Transform.m_Forward.m_Z = m_Vars.m_HeightVal;
			else m_Vars.m_HeightVal = Ped->m_Transform.m_Forward.m_Z;

			if (m_Vars.m_LeanFB) Ped->m_Transform.m_Forward.m_X = m_Vars.m_LeanFBVal;
			else m_Vars.m_LeanFBVal = Ped->m_Transform.m_Forward.m_X;

			if (m_Vars.m_LeanLR) Ped->m_Transform.m_Forward.m_Y = m_Vars.m_LeanLRVal;
			else m_Vars.m_LeanLRVal = Ped->m_Transform.m_Forward.m_Y;
		}
	}
}

/*Called once on submenu open*/
void AppearanceMenu::UpdateOnce() {
	if (WardrobeMenuVars::m_Vars.m_PreviewCam) {
		WardrobeMenuVars::RemoveCamera();
	}
}

/*Called always*/
void AppearanceMenu::FeatureUpdate() {}

AppearanceMenu* _instance;
AppearanceMenu* AppearanceMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AppearanceMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AppearanceMenu::~AppearanceMenu() { delete _instance; }