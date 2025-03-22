#include "particle_fx.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../player.hpp"
#include "menu/base/util/control_manager.hpp"
#include "ptfx/hand_trails.hpp"
#include "ptfx/particle_manager.hpp"

using namespace ParticleFXMenuVars;

namespace ParticleFXMenuVars {
	Vars_ m_Vars;
}

void ParticleFXMenu::Init() {
	SetName("Particle FX");
	SetParentSubmenu<PlayerMenu>();

	addOption(SubmenuOption("Particle Manager")
		.addTranslation().addHotkey()
		.addSubmenu<ParticleManagerMenu>());

	addOption(SubmenuOption("Hand Trails")
		.addTranslation().addHotkey()
		.addSubmenu<ParticleHandTrailsMenu>());
}

void ParticleFXMenu::Update() {}

/*Called once on submenu open*/
void ParticleFXMenu::UpdateOnce() {}

/*Called always*/
void ParticleFXMenu::FeatureUpdate() {}

ParticleFXMenu* _instance;
ParticleFXMenu* ParticleFXMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ParticleFXMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ParticleFXMenu::~ParticleFXMenu() { delete _instance; }