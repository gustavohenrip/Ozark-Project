#include "network_players_text.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_players_selected.hpp"
#include "menu/base/util/helpers.hpp"
#include "menu/base/util/notify.hpp"
#include "text/network_text_save_load.hpp"
#include "text/network_text_preset.hpp"
#include "../network_session.hpp"

using namespace NetworkPlayersTextMenuVars;

namespace NetworkPlayersTextMenuVars {
	Vars_ m_Vars;

	ScrollStruct<const char*> TextFonts[] = {
		{ TranslationString("Default", true), "" },
		{ TranslationString("Chalet Comprime 60", true), "$Font2_cond" },
		{ TranslationString("Sign Painter", true), "$Font5" },
		{ TranslationString("Rockstar Tag", true), "$RockstarTAG" },
		{ TranslationString("Pricedown", true), "$gtaCash" },
		{ TranslationString("Blocky", true), "$WebFont3_Times" },
	};

	ScrollStruct<const char*> TextColors[] = {
		{ TranslationString("Default", true), "" },
		{ TranslationString("~v~Black", true), "~v~" },
		{ TranslationString("~b~Blue", true), "~b~" },
		{ TranslationString("~y~Gold", true), "~y~" },
		{ TranslationString("~g~Green", true), "~g~" },
		{ TranslationString("~c~Grey", true), "~c~" },
		{ TranslationString("~f~Light Blue", true), "~f~" },
		{ TranslationString("~o~Orange", true), "~o~" },
		{ TranslationString("~p~Purple", true), "~p~" },
		{ TranslationString("~r~Red", true), "~r~" }
	};

	ScrollStruct<const char*> TextSizes[] = {
		{ TranslationString("Default", true), "" },
		{ TranslationString("Small", true), "10" },
		{ TranslationString("Large", true), "30" },
		{ TranslationString("Larger", true), "40" },
		{ TranslationString("Big", true), "50" },
		{ TranslationString("Massive", true), "70" }
	};

	void SendTextMessage(Menu::PlayerVars Player, Menu::PlayerVars Sender, const char* Message) {
		Menu::Helpers::SendTextMessage(Player.m_ID, Sender.m_ID, Message);
	}
}

void NetworkPlayersTextMenu::Init() {
	SetName("Text Message");
	SetParentSubmenu<NetworkPlayersSelectedMenu>();

	addOption(SubmenuOption("Save and Load")
		.addTranslation()
		.addSubmenu<NetworkTextSaveLoadMenu>());

	addOption(SubmenuOption("Presets")
		.addTranslation()
		.addSubmenu<NetworkTextPresetMenu>());

	addOption(ButtonOption("Input Message")
		.addTranslation()
		.addKeyboard("Enter the message to send", 0x100, [](const char* Msg) {
			Menu::GetSelectedPlayer().m_TextMessageMsg = Msg;
		}));

	addOption(ScrollOption<const char*>(SCROLL, "Color")
		.addTranslation()
		.addScroll(Menu::GetSelectedPlayer().m_TextMessageColor, 0, NUMOF(TextColors), TextColors)
		.addOnUpdate([](ScrollOption<const char*>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_TextMessageColor, 0, NUMOF(TextColors), TextColors); }));

	addOption(ScrollOption<const char*>(SCROLL, "Font")
		.addTranslation()
		.addScroll(Menu::GetSelectedPlayer().m_TextMessageFont, 0, NUMOF(TextFonts), TextFonts)
		.addOnUpdate([](ScrollOption<const char*>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_TextMessageFont, 0, NUMOF(TextFonts), TextFonts); }));

	addOption(ScrollOption<const char*>(SCROLL, "Size")
		.addTranslation()
		.addScroll(Menu::GetSelectedPlayer().m_TextMessageSize, 0, NUMOF(TextSizes), TextSizes)
		.addOnUpdate([](ScrollOption<const char*>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_TextMessageSize, 0, NUMOF(TextSizes), TextSizes); }));

	addOption(ScrollOption<int>(TOGGLE, "Spoof Sender")
		.addTranslation()
		.addToggle(m_Vars.m_SenderToggle)
		.addOnClick([] {
			m_Vars.m_RandomizeSender = false;
			m_Vars.m_SenderID = m_Vars.m_PlayerList[m_Vars.m_Sender].m_Result;
		})
		.addScroll(m_Vars.m_Sender, 0, NUMOF(m_Vars.m_PlayerList), m_Vars.m_PlayerList)
		.addOnUpdate([](ScrollOption<int>* Option) { Option->addScroll(m_Vars.m_Sender, 0, m_Vars.m_ListSize, m_Vars.m_PlayerList); })
		.addRequirement([] { return m_Vars.m_ListSize >= 1; }));

	addOption(ToggleOption("Randomize Sender")
		.addTranslation()
		.addToggle(m_Vars.m_RandomizeSender)
		.addOnClick([] { m_Vars.m_SenderToggle = false; })
		.addRequirement([] { return m_Vars.m_ListSize >= 1; }));

	addOption(NumberOption<int>(SCROLLSELECT, "Send Message")
		.addTranslation()
		.addNumber(Menu::GetSelectedPlayer().m_TextMessageCount, "%i", 1).addMin(1).addMax(100)
		.addOnUpdate([](NumberOption<int>* Option) { Option->addNumber(Menu::GetSelectedPlayer().m_TextMessageCount, "%i", 1); })
		.addOnClick([] {
			for (int i = 0; i < Menu::GetSelectedPlayer().m_TextMessageCount; i++) {
				Menu::PlayerVars Sender = m_Vars.m_SenderToggle ? Menu::GetPlayer(m_Vars.m_PlayerList[m_Vars.m_Sender].m_Result) : Menu::GetLocalPlayer();
				if (m_Vars.m_RandomizeSender) {
					int MaxRange = 0;
					Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
						MaxRange = Player.m_ID;
					});

					Sender = Menu::GetPlayer(Native::GetRandomIntInRange(0, MaxRange));
				}

				if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
					SendTextMessage(Menu::GetSelectedPlayer(), Sender, Menu::GetSelectedPlayer().m_TextMessageFinal);
				} else {
					Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
						SendTextMessage(Player, Sender, Menu::GetSelectedPlayer().m_TextMessageFinal);
					});
				}
			}
		}));

	addString("Network -> Text Message -> Spoof sender disabled, player no longer valid");
}

void NetworkPlayersTextMenu::Update() {
	const char* FontFace = Menu::GetSelectedPlayer().m_TextMessageFont > 0 ? TextFonts[Menu::GetSelectedPlayer().m_TextMessageFont].m_Result : "$Font2";
	const char* FontSize = Menu::GetSelectedPlayer().m_TextMessageSize > 0 ? TextSizes[Menu::GetSelectedPlayer().m_TextMessageSize].m_Result : "20";

	if (Menu::GetSelectedPlayer().m_TextMessageFont == 0 && Menu::GetSelectedPlayer().m_TextMessageSize == 0) {
		snprintf(Menu::GetSelectedPlayer().m_TextMessageFinal, 100, "%s%s",
			TextColors[Menu::GetSelectedPlayer().m_TextMessageColor].m_Result,
			Menu::GetSelectedPlayer().m_TextMessageMsg.c_str());
	} else {
		snprintf(Menu::GetSelectedPlayer().m_TextMessageFinal, 100, "<font face=\"%s\" size=\"%s\">%s%s",
			FontFace,
			FontSize,
			TextColors[Menu::GetSelectedPlayer().m_TextMessageColor].m_Result,
			Menu::GetSelectedPlayer().m_TextMessageMsg.c_str());
	}

	if (Menu::GetSelectedPlayer().m_TextMessageMsg.length()) {
		Menu::GetNotify()->Subtitle(Menu::GetSelectedPlayer().m_TextMessageFinal);
	}

	m_Vars.m_ListSize = 0;
	Menu::GetPlayerManager()->Foreach([&] (Menu::PlayerVars& Player) {
		m_Vars.m_PlayerList[m_Vars.m_ListSize].m_Name.Set(Player.m_Name);
		m_Vars.m_PlayerList[m_Vars.m_ListSize].m_Result = Player.m_ID;

		if (m_Vars.m_SenderToggle) {
			if (m_Vars.m_SenderID == Player.m_ID) {
				m_Vars.m_Sender = m_Vars.m_ListSize;
			}
		}

		m_Vars.m_ListSize++;
	});

	if (m_Vars.m_SenderToggle) {
		if (!Menu::GetPlayer(m_Vars.m_SenderID).m_Connected) {
			m_Vars.m_SenderToggle = false;
			m_Vars.m_Sender = 0;
			Menu::GetNotify()->NotifyStacked(getString("Network -> Text Message -> Spoof sender disabled, player no longer valid"), Global::UIVars::g_NotificationError);
		}
	}
}

/*Called once on submenu open*/
void NetworkPlayersTextMenu::UpdateOnce() {
	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersSelectedMenu>();
	} else {
		SetParentSubmenu<NetworkSessionMenu>();
	}
}

/*Called always*/
void NetworkPlayersTextMenu::FeatureUpdate() {}

NetworkPlayersTextMenu* _instance;
NetworkPlayersTextMenu* NetworkPlayersTextMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkPlayersTextMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkPlayersTextMenu::~NetworkPlayersTextMenu() { delete _instance; }