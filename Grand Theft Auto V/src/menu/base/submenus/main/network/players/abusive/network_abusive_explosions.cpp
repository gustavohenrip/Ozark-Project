#include "network_abusive_explosions.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players_abusive.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/network/network_session.hpp"

using namespace NetworkAbusiveExplosionsMenuVars;

namespace NetworkAbusiveExplosionsMenuVars {
	Vars_ m_Vars;

	ScrollStruct<Global::Arrays::Particle> Particles[22] = {
		{ TranslationString("Default", true), { "", "", false } },
		{ TranslationString("Invisible", true), { "", "", false } }
	};

	void CreateExplosion(Menu::PlayerVars Player) {
		Native::AddExplosion(Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, Global::Arrays::g_Explosions[Player.m_ExplosionType].m_Result, Player.m_ExplosionRadius, Player.m_ExplosionSound, Player.m_ExplosionVisual != 0, Player.m_ExplosionCameraShake, false);

		if (Player.m_ExplosionVisual >= 2) {
			auto Particle = Particles[Player.m_ExplosionVisual];
			Menu::GetControlManager()->RequestParticleFX({ Particle.m_Result.m_Dict, Particle.m_Result.m_Texture }, [=] (std::pair<const char*, const char*> Anim) {
				Native::_UseParticleFxAssetNextCall(Anim.first);
				Native::_StartParticleFxNonLoopedAtCoord2(Anim.second, Player.m_Coords.m_X, Player.m_Coords.m_Y, Player.m_Coords.m_Z, 0.f, 0.f, 0.f, 1.f, false, false, false);
			});
		}
	}
}

void NetworkAbusiveExplosionsMenu::Init() {
	SetName("Custom Explosion");
	SetParentSubmenu<NetworkPlayersAbusiveMenu>();

	for (int i = 2; i < 22; i++) {
		Particles[i] = Global::Arrays::g_Particles[i - 2];
	}

	addString("Type");
	addString("Visual");
	addString("Radius");
	addString("Camera Shake");
	addString("Sound");
	addString("Create Explosion");
	addString("Create Loop");
}

void NetworkAbusiveExplosionsMenu::Update() {}

/*Called once on submenu open*/
void NetworkAbusiveExplosionsMenu::UpdateOnce() {
	ClearOptionsOffset(0);
	
	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		addOption(ScrollOption<ExplosionTypes>(SCROLL, getString("Type"))
			.addScroll(Menu::GetSelectedPlayer().m_ExplosionType, 0, NUMOF(Global::Arrays::g_Explosions), Global::Arrays::g_Explosions)
			.addOnUpdate([] (ScrollOption<ExplosionTypes>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_ExplosionType, 0, NUMOF(Global::Arrays::g_Explosions), Global::Arrays::g_Explosions); }));

		addOption(ScrollOption<Global::Arrays::Particle>(SCROLL, getString("Visual"))
			.addScroll(Menu::GetSelectedPlayer().m_ExplosionVisual, 0, NUMOF(Particles), Particles)
			.addOnUpdate([] (ScrollOption<Global::Arrays::Particle>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_ExplosionVisual, 0, NUMOF(Particles), Particles); }));

		addOption(NumberOption<float>(SCROLL, getString("Radius"))
			.addNumber(Menu::GetSelectedPlayer().m_ExplosionRadius, "%.0f", 1.f).addMin(0.f).addMax(15.f)
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(Menu::GetSelectedPlayer().m_ExplosionRadius, "%.0f", 1.f); }));

		addOption(NumberOption<float>(SCROLL, getString("Camera Shake"))
			.addNumber(Menu::GetSelectedPlayer().m_ExplosionCameraShake, "%.0f", 1.f).addMin(0.f).addMax(100.f).setScrollSpeed(10)
			.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(Menu::GetSelectedPlayer().m_ExplosionCameraShake, "%.0f", 1.f); }));

		addOption(ToggleOption(getString("Sound"))
			.addToggle(Menu::GetSelectedPlayer().m_ExplosionSound)
			.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_ExplosionSound); }));

		addOption(ButtonOption(getString("Create Explosion"))
			.addOnClick([] { CreateExplosion(Menu::GetSelectedPlayer()); }));

		addOption(ToggleOption(getString("Create Loop"))
			.addToggle(Menu::GetSelectedPlayer().m_ExplosionLoop)
			.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_ExplosionLoop); }));
	} else {
		addOption(ScrollOption<ExplosionTypes>(SCROLL, getString("Type"))
			.addScroll(m_Vars.m_ExplosionType, 0, NUMOF(Global::Arrays::g_Explosions), Global::Arrays::g_Explosions));

		addOption(ScrollOption<Global::Arrays::Particle>(SCROLL, getString("Visual"))
			.addScroll(m_Vars.m_ExplosionVisual, 0, NUMOF(Particles), Particles));

		addOption(NumberOption<float>(SCROLL, getString("Radius"))
			.addNumber(m_Vars.m_ExplosionRadius, "%.0f", 1.f).addMin(0.f).addMax(15.f));

		addOption(NumberOption<float>(SCROLL, getString("Camera Shake"))
			.addNumber(m_Vars.m_ExplosionCameraShake, "%.0f", 1.f).addMin(0.f).addMax(100.f).setScrollSpeed(10));

		addOption(ToggleOption(getString("Sound"))
			.addToggle(m_Vars.m_ExplosionSound));

		addOption(ButtonOption(getString("Create Explosion"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
					CreateExplosion(Player);
				});
			}));

		addOption(ToggleOption(getString("Create Loop"))
			.addTranslation()
			.addToggle(m_Vars.m_ExplosionLoop));
	}
}

/*Called always*/
void NetworkAbusiveExplosionsMenu::FeatureUpdate() {
	Menu::GetPlayerManager()->Foreach([] (Menu::PlayerVars& Player) {
		if (Player.m_ExplosionLoop || m_Vars.m_ExplosionLoop) {
			CreateExplosion(Player);
		}
	});
}

NetworkAbusiveExplosionsMenu* _instance;
NetworkAbusiveExplosionsMenu* NetworkAbusiveExplosionsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAbusiveExplosionsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAbusiveExplosionsMenu::~NetworkAbusiveExplosionsMenu() { delete _instance; }