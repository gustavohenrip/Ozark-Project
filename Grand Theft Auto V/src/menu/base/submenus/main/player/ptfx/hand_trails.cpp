#include "hand_trails.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../particle_fx.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace ParticleHandTrailsMenuVars;

namespace ParticleHandTrailsMenuVars {
	Vars_ m_Vars;

	void UpdateColor() {
		if (m_Vars.m_TrailBoneLeft && m_Vars.m_TrailBoneRight) {
			Native::SetParticleFxLoopedColour(m_Vars.m_TrailBoneLeft, (float)m_Vars.m_TrailColor.R, (float)m_Vars.m_TrailColor.G, (float)m_Vars.m_TrailColor.B, true);
			Native::SetParticleFxLoopedColour(m_Vars.m_TrailBoneRight, (float)m_Vars.m_TrailColor.R, (float)m_Vars.m_TrailColor.G, (float)m_Vars.m_TrailColor.B, true);
		}
	}

	void StartTrails() {
		Menu::GetControlManager()->RequestParticleFX({ "scr_minigametennis", "scr_tennis_ball_trail" }, [](std::pair<const char*, const char*> Anim) {
			Native::_UseParticleFxAssetNextCall(Anim.first);
			m_Vars.m_TrailBoneLeft = Native::_StartParticleFxLoopedOnEntityBone2(Anim.second, Menu::GetLocalPlayer().m_Ped, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, Native::GetPedBoneIndex(Menu::GetLocalPlayer().m_Ped, 0x8cbd), 0.5f, false, false, false, 0, 0, 0);

			Native::_UseParticleFxAssetNextCall(Anim.first);
			m_Vars.m_TrailBoneRight = Native::_StartParticleFxLoopedOnEntityBone2(Anim.second, Menu::GetLocalPlayer().m_Ped, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, Native::GetPedBoneIndex(Menu::GetLocalPlayer().m_Ped, 0x188e), 0.5f, false, false, false, 0, 0, 0);

			UpdateColor();
		});
	}
}

void ParticleHandTrailsMenu::Init() {
	SetName("Hand Trails");
	SetParentSubmenu<ParticleFXMenu>();

	addOption(ToggleOption("Toggle Hand Trails")
		.addTranslation()
		.addToggle(m_Vars.m_Trails)
		.addOnClick([] {
			if (!m_Vars.m_Trails) {
				Native::RemoveParticleFx(m_Vars.m_TrailBoneLeft, true);
				Native::RemoveParticleFx(m_Vars.m_TrailBoneRight, true);
			} else {
				StartTrails();
			}
		}));

	addOption(NumberOption<int>(SCROLL, "[R]ed")
		.addTranslation()
		.addNumber(m_Vars.m_TrailColor.R, "%i", 1).addMin(0).addMax(255).canLoop().showMax()
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_TrailColor); })
		.addOnClick([] { UpdateColor(); }));

	addOption(NumberOption<int>(SCROLL, "[G]reen")
		.addTranslation()
		.addNumber(m_Vars.m_TrailColor.G, "%i", 1).addMin(0).addMax(255).canLoop().showMax()
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_TrailColor); })
		.addOnClick([] { UpdateColor(); }));

	addOption(NumberOption<int>(SCROLL, "[B]lue")
		.addTranslation()
		.addNumber(m_Vars.m_TrailColor.B, "%i", 1).addMin(0).addMax(255).canLoop().showMax()
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_TrailColor); })
		.addOnClick([] { UpdateColor(); }));
}

void ParticleHandTrailsMenu::Update() {}

/*Called once on submenu open*/
void ParticleHandTrailsMenu::UpdateOnce() {}

/*Called always*/
void ParticleHandTrailsMenu::FeatureUpdate() {}

ParticleHandTrailsMenu* _instance;
ParticleHandTrailsMenu* ParticleHandTrailsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ParticleHandTrailsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ParticleHandTrailsMenu::~ParticleHandTrailsMenu() { delete _instance; }