#include "network_spoofing_crew_icon.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing_crew.hpp"
#include "network_spoofing_crew_presets.hpp"
#include "menu/base/util/notify.hpp"

using namespace NetworkSpoofingCrewIconMenuVars;

namespace NetworkSpoofingCrewIconMenuVars {
	Vars_ m_Vars;

	void Spoof(NetworkSpoofingCrewIconMenu* This, bool Reset, int Icon) {
		if (Reset) {
			m_Vars.m_SpoofedIcon = -1;
			m_Vars.m_TempName.clear();

			if (m_Vars.m_SaveToConfig) {
				Utils::GetConfig()->WriteInt(This->m_ParentNameStack, "Name", -1);
			}

			return;
		}

		m_Vars.m_SpoofedIcon = Icon;

		if (m_Vars.m_SaveToConfig) {
			Utils::GetConfig()->WriteInt(This->m_ParentNameStack, "Name", m_Vars.m_SpoofedIcon);
		}

		Menu::GetNotify()->NotifyStacked(NetworkSpoofingCrewIconMenu::GetInstance()->getString("Spoofing crew icon to") + " " + std::to_string(m_Vars.m_SpoofedIcon));
	}
}

void NetworkSpoofingCrewIconMenu::Init() {
	SetName("Crew Icon");
	SetParentSubmenu<NetworkSpoofingCrewMenu>();

	addString("Spoofing crew icon to");

	addOption(SubmenuOption("Presets")
		.addTranslation()
		.addSubmenu<NetworkSpoofingCrewPresetsMenu>()
		.addOnClick([] {
			NetworkSpoofingCrewPresetsMenu::GetInstance()->SetParentSubmenu<NetworkSpoofingCrewIconMenu>();
			NetworkSpoofingCrewPresetsMenuVars::m_Vars.m_Editing = &m_Vars.m_TempName;
		}));

	addOption(ButtonOption("Input ID")
		.addTranslation()
		.addKeyboard("Enter crew ID", 20, [] (const char* Name) { m_Vars.m_TempName = std::string(Name); }));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([this] { Spoof(this, true); }));

	addOption(ToggleOption("Save to Config")
		.addTranslation()
		.addToggle(m_Vars.m_SaveToConfig).canBeSaved(m_ParentNameStack)
		.addTooltip("Saves to the config when applied"));

	m_Vars.m_PreviewButton = addOption(ButtonOption("")
		.addOnClick([this] { Spoof(this, false, std::stoi(m_Vars.m_TempName.c_str())); }));

	if (m_Vars.m_SaveToConfig) {
		m_Vars.m_SpoofedIcon = Utils::GetConfig()->ReadInt(m_ParentNameStack, "Name", m_Vars.m_SpoofedIcon);
		m_Vars.m_TempName = std::to_string(m_Vars.m_SpoofedIcon);
		Spoof(this, false, std::stoi(m_Vars.m_TempName.c_str()));
	}
}

void NetworkSpoofingCrewIconMenu::Update() {
	snprintf(m_Vars.m_PreviewName, 50, "Apply: %s", m_Vars.m_TempName.c_str());
	m_Vars.m_PreviewButton->setName(m_Vars.m_PreviewName);
}

/*Called once on submenu open*/
void NetworkSpoofingCrewIconMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingCrewIconMenu::FeatureUpdate() {}

NetworkSpoofingCrewIconMenu* _instance;
NetworkSpoofingCrewIconMenu* NetworkSpoofingCrewIconMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingCrewIconMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingCrewIconMenu::~NetworkSpoofingCrewIconMenu() { delete _instance; }