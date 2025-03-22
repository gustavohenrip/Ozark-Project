#include "network_particles.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/fiber_pool.hpp"
#include "menu/base/util/fiber_manager.hpp"
#include "nasty.hpp"
#include "menu/base/util/panel_manager.hpp"

using namespace networkParticleMenuVars;

namespace networkParticleMenuVars {
	vars_ vars;
}

void networkParticleMenu::init() {
	setName("Particles");
	setParentSubmenu<networkNastyMenu>();

	addOption(numberOption<float>(TOGGLE, "Override Predefined Scale")
		.addTranslation()
		.addToggle(menu::getSelectedPlayer().particleOverrideScale)
		.addNumber(menu::getSelectedPlayer().particleScale, "%.1f", 0.1f).addMin(0.f).addMax(10.f)
		.addOnUpdate([] (numberOption<float>* op) { op->addToggle(menu::getSelectedPlayer().particleOverrideScale); op->addNumber(menu::getSelectedPlayer().particleScale, "%.1f", 0.1f); }));

	addOption(numberOption<int>(SCROLL, "Looped Particle Runtime")
		.addTranslation()
		.addNumber(menu::getSelectedPlayer().particleLoopedTimeout, "%is", 1).addMin(0).addMax(60)
		.addOnUpdate([] (numberOption<int>* op) { op->addNumber(menu::getSelectedPlayer().particleLoopedTimeout, "%is", 1); }));

	addOption(breakOption("Particles")
		.addTranslation());

	for (int i = 0; i < NUMOF(global::arrays::g_particles); i++) {
		auto p = global::arrays::g_particles[i];
		addOption(buttonOption(p.name)
			.addOnClick([=] {
			native::requestNamedPtfxAsset(native::getHashKey(p.result.first));
			native::useParticleFXAsset(p.result.first);

			if (p.result.looped) {
				int handle = native::startNetworkedParticleFxLoopedOnEntity(p.result.second, menu::getSelectedPlayer().ped, { 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }, menu::getSelectedPlayer().particleOverrideScale ? menu::getSelectedPlayer().particleScale : p.result.scale);

				menu::util::getFiberPool()->run([=] {
					menu::util::waitFiber(menu::getSelectedPlayer().particleLoopedTimeout * 1000);
					native::stopParticleFxLooped(handle, FALSE);
				});
			} else {
				native::startNetworkedParticleFXNonLoopedAtCoord(p.result.second, menu::getSelectedPlayer().coords, { 0.f, 0.f, 0.f }, menu::getSelectedPlayer().particleOverrideScale ? menu::getSelectedPlayer().particleScale : p.result.scale, FALSE, FALSE, FALSE);
			}
		}));
	}
}

/*Called while in submenu*/
void networkParticleMenu::update() {
	menu::getPanelManager()->panelPlayer(menu::getSelectedPlayer().id);
}

/*Called once on submenu open*/
void networkParticleMenu::updateOnce() {}

/*Called always*/
void networkParticleMenu::featureUpdate() {}

networkParticleMenu* _instance;
networkParticleMenu* networkParticleMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkParticleMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkParticleMenu::~networkParticleMenu() { delete _instance; }