#include "vehicle_particles.hpp"
#include "menu/base/submenu_handler.hpp"
#include "vehicle_mods.hpp"
#include "global/arrays.hpp"

using namespace vehicleParticlesMenuVars;

namespace vehicleParticlesMenuVars {
	vars_ vars;

	std::vector<std::pair<const char*, const char*>> bones = {
		{ "Front Right Wheel", "wheel_rf" },
		{ "Front Left Wheel", "wheel_lf" },
		{ "Rear Right Wheel", "wheel_rr" },
		{ "Rear Left Wheel", "wheel_lr" }
	};
}

void vehicleParticlesMenu::init() {
	setName("Particles");
	setParentSubmenu<vehicleModsMenu>();

	addString("Enable Particles");
	addString("Wheels");
	addString("Particle");
	addString("Scale");
	addString("Incompatible vehicle :(");
	addString("You need to be in a vehicle :(");

	vars.values.reserve(bones.size());
	for (auto& bone : bones) {
		addString(bone.first);
		vars.values.push_back({ bone.first, bone.second, native::getEntityBoneIndexByName(menu::getLocalPlayer().vehicle, bone.second), false });
	}
}

/*Called while in submenu*/
void vehicleParticlesMenu::update() {}

/*Called once on submenu open*/
void vehicleParticlesMenu::updateOnce() {
	clearOptionsOffset(0);

	if (menu::getLocalPlayer().inVehicle) {
		bool validBone = false;
		for (auto& v : vars.values) {
			v.boneIndex = native::getEntityBoneIndexByName(menu::getLocalPlayer().vehicle, v.boneName);
			if (v.boneIndex != -1) validBone = true;
		}

		if (validBone) {
			addOption(toggleOption(getString("Enable Particles"))
				.addToggle(vars.enabled));

			addOption(scrollOption<global::arrays::particle>(SCROLL, getString("Particle"))
				.addScroll(vars.particle, 0, NUMOF(global::arrays::g_particles), global::arrays::g_particles)
				.addRequirement([] { return vars.enabled; }));

			addOption(numberOption<float>(SCROLL, getString("Scale"))
				.addNumber(vars.scale, "%.2f", 0.01f).addMin(0.1f).addMax(10.f).setScrollSpeed(10)
				.addRequirement([] { return vars.enabled; }));

			addOption(breakOption(getString("Wheels"))
				.addRequirement([] { return vars.enabled; }));

			for (auto& wheel : vars.values) {
				if (wheel.boneIndex != -1) {
					addOption(toggleOption(getString(wheel.displayName))
						.addToggle(wheel.enabled)
						.addRequirement([] { return vars.enabled; }));
				}
			}
		} else {
			addOption(buttonOption(getString("Incompatible vehicle :(")));
		}
	} else {
		addOption(buttonOption(getString("You need to be in a vehicle :(")));
	}
}

/*Called always*/
void vehicleParticlesMenu::featureUpdate() {
	if (menu::getLocalPlayer().inVehicle) {
		if (vars.enabled) {
			for (auto& wheel : vars.values) {
				if (wheel.enabled) {
					if (wheel.boneIndex != -1) {
						math::vector3 bonePosition = native::getWorldPositionOfEntityBone(menu::getLocalPlayer().vehicle, wheel.boneIndex);

						if (!bonePosition.isZero()) {
							native::requestNamedPtfxAsset(native::getHashKey(global::arrays::g_particles[vars.particle].result.first));
							native::useParticleFXAsset(global::arrays::g_particles[vars.particle].result.first);
							native::startNetworkedParticleFXNonLoopedAtCoord(global::arrays::g_particles[vars.particle].result.second, bonePosition, { 0.f, 0.f, 0.f }, vars.scale);
						}
					}
				}
			}
		}
	}
}

vehicleParticlesMenu* _instance;
vehicleParticlesMenu* vehicleParticlesMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new vehicleParticlesMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

vehicleParticlesMenu::~vehicleParticlesMenu() { delete _instance; }