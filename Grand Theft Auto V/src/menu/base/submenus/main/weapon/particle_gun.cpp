#include "particle_gun.hpp"
#include "menu/base/submenu_handler.hpp"
#include "weapon_mods.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/raycast.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace ParticleGunMenuVars;

namespace ParticleGunMenuVars {
	Vars_ m_Vars;
}

void ParticleGunMenu::Init() {
	SetName("Particle Gun");
	SetParentSubmenu<WeaponModsMenu>();

	addOption(ToggleOption("Toggle Particle Gun")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Toggle).canBeSaved(m_ParentNameStack));

	addOption(NumberOption<float>(SCROLL, "Scale")
		.addTranslation()
		.addNumber(m_Vars.m_Scale, "%.2f", 0.01f).addMin(0.f).addMax(10.f).setScrollSpeed(10).showMax().canBeSaved(m_ParentNameStack));

	addOption(BreakOption("Particles")
		.addTranslation());

	for (int i = 0; i < 20; i++) {
		addOption(RadioOption(Global::Arrays::g_Particles[i].m_Name.Get())
			.addTranslation()
			.addRadio(m_Vars.m_Radio));
	}
}

void ParticleGunMenu::Update() {}

/*Called once on submenu open*/
void ParticleGunMenu::UpdateOnce() {}

/*Called always*/
void ParticleGunMenu::FeatureUpdate() {
	if (m_Vars.m_Toggle) {
		if (Menu::GetLocalPlayer().m_IsShooting) {
			Menu::Raycast Cast = Menu::RaycastInfront(9999.f);
			if (!Cast.GetHitCoords().IsZero()) {
				if (Cast.GetHitCoords().GetMathmaticalDistance(Menu::GetLocalPlayer().m_Coords) > 3.5f) {
					auto Particle = Global::Arrays::g_Particles[m_Vars.m_Radio.m_Selected];
					auto Position = Cast.GetHitCoords();
					Menu::GetControlManager()->RequestParticleFX({ Particle.m_Result.m_Dict, Particle.m_Result.m_Texture }, [=] (std::pair<const char*, const char*> Anim) {
						Native::_UseParticleFxAssetNextCall(Anim.first);
						Native::_StartParticleFxNonLoopedAtCoord2(Anim.second, Position.m_X, Position.m_Y, Position.m_Z, 0.f, 0.f, 0.f, m_Vars.m_Scale, false, false, false);
					});
				}
			}
		}
	}
}

ParticleGunMenu* _instance;
ParticleGunMenu* ParticleGunMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ParticleGunMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ParticleGunMenu::~ParticleGunMenu() { delete _instance; }