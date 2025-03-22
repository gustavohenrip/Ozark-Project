#include "network_account_outfits_edit_name.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_account_outfits_edit.hpp"
#include "menu/base/submenus/main/network/players/network_players_text.hpp"
#include "../network_account_outfits.hpp"
#include "utils/utils.hpp"

using namespace NetworkAccountOutfitsEditNameMenuVars;

namespace NetworkAccountOutfitsEditNameMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> TextSettings[] = {
		{ TranslationString("Normal", true), "" },
		{ TranslationString("Bold", true), "~h~" },
		{ TranslationString("Italic", true), "~italic~" },
	};

	ScrollStruct<const char*> SpecialIcons[] = {
		{ TranslationString("Normal", true), "" },
		{ TranslationString("Wanted Star", true), "~ws~" },
		{ TranslationString("Rockstar Verified", true), "\xC2\xA6" },
		{ TranslationString("Rockstar", true), "\xE2\x88\x91" },
	};

	void ChangeOutfitName() {
		char* Name = NetworkAccountOutfitsMenuVars::GetOutfitSlotNamePtr(NetworkAccountOutfitsEditMenuVars::m_Vars.m_Slot);
		snprintf(Name, 64, "%s%s%s%s", TextSettings[m_Vars.m_Style].m_Result, SpecialIcons[m_Vars.m_Icon].m_Result, NetworkPlayersTextMenuVars::TextColors[m_Vars.m_Color].m_Result, m_Vars.m_TempName.c_str());
	}
}

void NetworkAccountOutfitsEditNameMenu::Init() {
	SetName("Name");
	SetParentSubmenu<NetworkAccountOutfitsEditMenu>();

	addOption(ButtonOption("Input Name")
		.addTranslation()
		.addKeyboard("Enter name", 64, [] (const char* Name) { m_Vars.m_TempName = std::string(Name); }));

	addOption(BreakOption("Text Settings")
		.addTranslation());

	addOption(ScrollOption<const char*>(SCROLL, "Special Icons")
		.addTranslation()
		.addScroll(m_Vars.m_Icon, 0, NUMOF(SpecialIcons), SpecialIcons));

	addOption(ScrollOption<const char*>(SCROLL, "Style")
		.addTranslation()
		.addScroll(m_Vars.m_Style, 0, NUMOF(TextSettings), TextSettings));

	addOption(ScrollOption<const char*>(SCROLL, "Color")
		.addTranslation()
		.addScroll(m_Vars.m_Color, 0, NUMOF(NetworkPlayersTextMenuVars::TextColors), NetworkPlayersTextMenuVars::TextColors));

	m_Vars.m_PreviewButton = addOption(ButtonOption("")
		.addOnClick(ChangeOutfitName));
}

void NetworkAccountOutfitsEditNameMenu::Update() {
	// snprintf(m_Vars.m_PreviewName, 50, "Apply: %s%s%s%s", SpecialIcons[m_Vars.m_Icon].m_Result, TextSettings[m_Vars.m_Style].m_Result, NetworkPlayersTextMenuVars::TextColors[m_Vars.m_Color].m_Result, m_Vars.m_TempName.c_str());
	snprintf(m_Vars.m_PreviewName, 50, "Apply: %s%s%s%s", TextSettings[m_Vars.m_Style].m_Result, SpecialIcons[m_Vars.m_Icon].m_Result, NetworkPlayersTextMenuVars::TextColors[m_Vars.m_Color].m_Result, m_Vars.m_TempName.c_str());
	m_Vars.m_PreviewButton->setName(m_Vars.m_PreviewName);
}

/*Called once on submenu open*/
void NetworkAccountOutfitsEditNameMenu::UpdateOnce() {
	char* LocalName = NetworkAccountOutfitsMenuVars::GetOutfitSlotNamePtr(NetworkAccountOutfitsEditMenuVars::m_Vars.m_Slot);
	if (!LocalName || Native::IsStringNullOrEmpty(LocalName)) {
		return;
	}

	std::string Name = std::string(LocalName);

	for (int i = 1; i < NUMOF(TextSettings); i++) {
		if (strstr(Name.c_str(), TextSettings[i].m_Result)) {
			m_Vars.m_Style = i;
			Utils::ReplaceString(Name, TextSettings[i].m_Result, "");
			break;
		}
	}

	for (int i = 1; i < NUMOF(SpecialIcons); i++) {
		if (strstr(Name.c_str(), SpecialIcons[i].m_Result)) {
			m_Vars.m_Icon = i;
			Utils::ReplaceString(Name, SpecialIcons[i].m_Result, "");
			break;
		}
	}

	for (int i = 1; i < NUMOF(NetworkPlayersTextMenuVars::TextColors); i++) {
		if (strstr(Name.c_str(), NetworkPlayersTextMenuVars::TextColors[i].m_Result)) {
			m_Vars.m_Color = i;
			Utils::ReplaceString(Name, NetworkPlayersTextMenuVars::TextColors[i].m_Result, "");
		}
	}

	m_Vars.m_TempName = Name;
}

/*Called always*/
void NetworkAccountOutfitsEditNameMenu::FeatureUpdate() {}

NetworkAccountOutfitsEditNameMenu* _instance;
NetworkAccountOutfitsEditNameMenu* NetworkAccountOutfitsEditNameMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountOutfitsEditNameMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountOutfitsEditNameMenu::~NetworkAccountOutfitsEditNameMenu() { delete _instance; }