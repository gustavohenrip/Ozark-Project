#include "network_spoofing_crew_name.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_spoofing_crew.hpp"
#include "network_spoofing_crew_presets.hpp"
#include "menu/base/util/notify.hpp"

using namespace NetworkSpoofingCrewNameMenuVars;

namespace NetworkSpoofingCrewNameMenuVars {
	Vars_ m_Vars;

	void Spoof(NetworkSpoofingCrewNameMenu* This, bool Reset, const char* Name) {
		if (Reset) {
			m_Vars.m_SpoofedName.clear();
			m_Vars.m_TempName.clear();

			if (m_Vars.m_SaveToConfig) {
				Utils::GetConfig()->WriteString(This->m_ParentNameStack, "Name", "");
			}

			return;
		}

		m_Vars.m_SpoofedName = Name;

		if (m_Vars.m_SaveToConfig) {
			Utils::GetConfig()->WriteString(This->m_ParentNameStack, "Name", m_Vars.m_SpoofedName.c_str());
		}

		Menu::GetNotify()->NotifyStacked(NetworkSpoofingCrewNameMenu::GetInstance()->getString("Spoofing crew name to") + " " + m_Vars.m_SpoofedName);
	}
}

void NetworkSpoofingCrewNameMenu::Init() {
	SetName("Crew Name");
	SetParentSubmenu<NetworkSpoofingCrewMenu>();

	addString("Spoofing crew name to");

	addOption(SubmenuOption("Presets")
		.addTranslation()
		.addSubmenu<NetworkSpoofingCrewPresetsMenu>()
		.addOnClick([] {
			NetworkSpoofingCrewPresetsMenu::GetInstance()->SetParentSubmenu<NetworkSpoofingCrewNameMenu>();
			NetworkSpoofingCrewPresetsMenuVars::m_Vars.m_Editing = &m_Vars.m_TempName;
		}));

	addOption(ButtonOption("Input Name")
		.addTranslation()
		.addKeyboard("Enter name", 0x18, [] (const char* Name) { m_Vars.m_TempName = std::string(Name); }));

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
		.addOnClick([this] { Spoof(this, false, m_Vars.m_TempName.c_str()); }));

	if (m_Vars.m_SaveToConfig) {
		m_Vars.m_SpoofedName = Utils::GetConfig()->ReadString(m_ParentNameStack, "Name", m_Vars.m_SpoofedName.c_str());
		m_Vars.m_TempName = m_Vars.m_SpoofedName;
		Spoof(this, false, m_Vars.m_TempName.c_str());
	}
}

void NetworkSpoofingCrewNameMenu::Update() {
	snprintf(m_Vars.m_PreviewName, 50, "Apply: %s", m_Vars.m_TempName.c_str());
	m_Vars.m_PreviewButton->setName(m_Vars.m_PreviewName);
}

/*Called once on submenu open*/
void NetworkSpoofingCrewNameMenu::UpdateOnce() {}

/*Called always*/
void NetworkSpoofingCrewNameMenu::FeatureUpdate() {}

NetworkSpoofingCrewNameMenu* _instance;
NetworkSpoofingCrewNameMenu* NetworkSpoofingCrewNameMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkSpoofingCrewNameMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkSpoofingCrewNameMenu::~NetworkSpoofingCrewNameMenu() { delete _instance; }