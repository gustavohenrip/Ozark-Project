#include "all_particles.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "all_nasty.hpp"

using namespace networkAllParticleMenuVars;

namespace networkAllParticleMenuVars {
	vars_ vars;
}

void networkAllParticleMenu::init() {
	setName("Particles");
	setParentSubmenu<networkAllNastyMenu>();

	addOption(numberOption<float>(TOGGLE, "Override Predefined Scale")
		.addTranslation()
		.addToggle(vars.overrideScale)
		.addNumber(vars.scale, "%.1f", 0.1f).addMin(0.f).addMax(10.f));

	addOption(numberOption<int>(SCROLL, "Looped Particle Runtime")
		.addTranslation()
		.addNumber(vars.loopedTimeout, "%is", 1).addMin(0).addMax(60));

	addOption(breakOption("Particles")
		.addTranslation());

	for (int i = 0; i < NUMOF(global::arrays::g_particles); i++) {
		auto p = global::arrays::g_particles[i];
		addOption(buttonOption(p.name)
			.addOnClick([=] {
				menu::getPlayerManager()->foreach([=] (menu::playerVars& player) {
					native::requestNamedPtfxAsset(native::getHashKey(p.result.first));
					native::useParticleFXAsset(p.result.first);

					if (p.result.looped) {
						int handle = native::startNetworkedParticleFxLoopedOnEntity(p.result.second, player.ped, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, vars.overrideScale ? vars.scale : p.result.scale);

						menu::util::getFiberPool()->run([=] {
							menu::util::waitFiber(vars.loopedTimeout * 1000);
							native::stopParticleFxLooped(handle, FALSE);
						});
					} else {
						native::startNetworkedParticleFXNonLoopedAtCoord(p.result.second, player.coords, { 0.f, 0.f, 0.f }, vars.overrideScale ? vars.scale : p.result.scale, FALSE, FALSE, FALSE);
					}
				});
			}));
	}
}

/*Called while in submenu*/
void networkAllParticleMenu::update() {}

/*Called once on submenu open*/
void networkAllParticleMenu::updateOnce() {}

/*Called always*/
void networkAllParticleMenu::featureUpdate() {}

networkAllParticleMenu* _instance;
networkAllParticleMenu* networkAllParticleMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkAllParticleMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkAllParticleMenu::~networkAllParticleMenu() { delete _instance; }