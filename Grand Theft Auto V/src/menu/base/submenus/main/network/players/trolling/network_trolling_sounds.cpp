#include "network_trolling_sounds.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players_trolling.hpp"
#include "global/arrays.hpp"

using namespace NetworkTrollingSoundsMenuVars;

namespace NetworkTrollingSoundsMenuVars {
	Vars_ m_Vars;

	void StartSoundPlayer(Menu::PlayerVars Player, std::pair<const char*, const char*> Sound, bool Add, int OptionID) {
		/*if (!Native::HasSoundFinished(m_Vars.m_Sounds[OptionID])) {
			Native::StopSound(m_Vars.m_Sounds[OptionID]);
			return;
		}*/

		int SoundID = Native::GetSoundId();
		Native::PlaySoundFromCoord(SoundID, Sound.first, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Sound.second, true, 0, true);

		if (Add) {
			m_Vars.m_Sounds[OptionID] = SoundID;
		}
	}
}

void NetworkTrollingSoundsMenu::Init() {
	SetName("Sounds");
	SetParentSubmenu<NetworkPlayersTrollingMenu>();

	addOption(ButtonOption("Stop All")
		.addTranslation()
		.addOnClick([] {
			for (auto& Sound : m_Vars.m_Sounds) {
				Native::StopSound(Sound.second);
			}
		}));

	addOption(BreakOption("Sounds")
		.addTranslation());

	for (int i = 0; i < NUMOF(Global::Arrays::g_Sounds); i++) {
		auto Arr = Global::Arrays::g_Sounds[i];

		m_Vars.m_OptionCache.push_back(addOption(ButtonOption(Arr.m_Name.Get())
			.addTranslation()
			.addOnClick([=] {
				if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
					StartSoundPlayer(Menu::GetSelectedPlayer(), Arr.m_Result, true, i);
				} else {
					Menu::GetPlayerManager()->Foreach([=](Menu::PlayerVars& Player) {
						StartSoundPlayer(Player, Arr.m_Result, true, i);
					});
				}
			})));
	}
}

void NetworkTrollingSoundsMenu::Update() {
	for (auto& Sound : m_Vars.m_Sounds) {
		if (Native::HasSoundFinished(Sound.second)) {
			m_Vars.m_OptionCache[Sound.first]->removeSprite();
		} else {
			m_Vars.m_OptionCache[Sound.first]->addSprite("commonmenu", "shop_tick_icon").addSpriteScale(0.022f, 0.039f);
		}
	}
}

/*Called once on submenu open*/
void NetworkTrollingSoundsMenu::UpdateOnce() {}

/*Called always*/
void NetworkTrollingSoundsMenu::FeatureUpdate() {}

NetworkTrollingSoundsMenu* _instance;
NetworkTrollingSoundsMenu* NetworkTrollingSoundsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkTrollingSoundsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkTrollingSoundsMenu::~NetworkTrollingSoundsMenu() { delete _instance; }