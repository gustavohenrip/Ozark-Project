#include "network_spoofing_player_rotation.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing.hpp"

using namespace NetworkSpoofingPlayerRotationMenuVars;

namespace NetworkSpoofingPlayerRotationMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> FaceDirection[] = {
		{ TranslationString("Left", true), 0 },
		{ TranslationString("Right", true), 1 },
		{ TranslationString("Back", true), 2 }
	};

	ScrollStruct<int> StaticType[] = {
		{ TranslationString("Relative", true), 0 },
		{ TranslationString("Static", true), 1 }
	};
}

void NetworkSpoofingPlayerRotationMenu::Init() {
	SetName("Rotation");
	SetParentSubmenu<NetworkSpoofingMenu>();

	addOption(NumberOption<float>(TOGGLE, "Spinbot")
		.addTranslation()
		.addToggle(m_Vars.m_Spinbot)
		.addNumber(m_Vars.m_SpinbotSpeed, "%.0f", 1.f).addMin(0.f).addMax(14.f).setScrollSpeed(10)
		.addOnClick([] { m_Vars.m_FaceDirection = false; m_Vars.m_Jitter = false; }).canBeSaved(m_ParentNameStack));

	addOption(ScrollOption<int>(TOGGLE, "Face Direction")
		.addTranslation()
		.addToggle(m_Vars.m_FaceDirection)
		.addScroll(m_Vars.m_FaceDirectionVal, 0, NUMOF(FaceDirection), FaceDirection)
		.addOnClick([] { m_Vars.m_Spinbot = false; m_Vars.m_Jitter = false; }).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Jitter")
		.addTranslation()
		.addToggle(m_Vars.m_Jitter)
		.addOnClick([] { m_Vars.m_Spinbot = false; m_Vars.m_FaceDirection = false; }).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Relative")
		.addTranslation()
		.addToggle(m_Vars.m_Relative).canBeSaved(m_ParentNameStack)
		.addTooltip("Current rotation will be added to new rotation"));
}

void NetworkSpoofingPlayerRotationMenu::Update() {}

/*Called once on submenu open*/
void NetworkSpoofingPlayerRotationMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingPlayerRotationMenu::FeatureUpdate() {
	if (m_Vars.m_Spinbot) {
		static float Value = 0.f;
		Value += m_Vars.m_SpinbotSpeed;

		if (Value > 180.f) Value = -180.f + (Value - 180.f);
		m_Vars.m_HookValue = { Value * 0.0174532924f, Value * 0.0174532924f };
		return;
	}

	if (m_Vars.m_Jitter) {
		static float Value = 0.f;
		Value += 40.f;
		if (Value > 180.f) Value = -180.f + (Value - 180.f);

		m_Vars.m_HookValue = { Value * 0.0174532924f, Value * 0.0174532924f };
		return;
	}
}

NetworkSpoofingPlayerRotationMenu* _instance;
NetworkSpoofingPlayerRotationMenu* NetworkSpoofingPlayerRotationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingPlayerRotationMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingPlayerRotationMenu::~NetworkSpoofingPlayerRotationMenu() { delete _instance; }