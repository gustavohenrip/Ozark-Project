#include "particles.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../player.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"

using namespace particleMenuVars;

namespace particleMenuVars {
	vars_ vars;
}

void particleMenu::init() {
	setName("Particles");
	setParentSubmenu<playerMenu>();

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
				native::requestNamedPtfxAsset(native::getHashKey(p.result.first));
				native::useParticleFXAsset(p.result.first);

				if (p.result.looped) {
					int handle = native::startNetworkedParticleFxLoopedOnEntity(p.result.second, menu::getLocalPlayer().ped, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, vars.overrideScale ? vars.scale : p.result.scale);
					
					menu::util::getFiberPool()->run([=] {
						static auto tick = GetTickCount() + vars.loopedTimeout * 1000;
						while(GetTickCount() <= tick){
							native::setParticleFxLoopedEvolution(handle, p.result.evolution, 1.0f, FALSE);
							menu::util::waitFiber(0);
						}
					});

					menu::util::getFiberPool()->run([=] {
						menu::util::waitFiber(vars.loopedTimeout * 1000);
						native::stopParticleFxLooped(handle, FALSE);
					});
				} else {
					native::startNetworkedParticleFXNonLoopedAtCoord(p.result.second, menu::getLocalPlayer().coords, { 0.f, 0.f, 0.f }, vars.overrideScale ? vars.scale : p.result.scale, FALSE, FALSE, FALSE);
				}
			}));
	}
}

/*Called while in submenu*/
void particleMenu::update() {}

/*Called once on submenu open*/
void particleMenu::updateOnce() {}

/*Called always*/
void particleMenu::featureUpdate() {
////native::setParticleFxLoopedEvolution(handle, "strength", 1.0f, false);

}

particleMenu* _instance;
particleMenu* particleMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new particleMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

particleMenu::~particleMenu() { delete _instance; }