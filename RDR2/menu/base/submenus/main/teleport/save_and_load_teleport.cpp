#include "save_and_load_teleport.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../teleport.hpp"

using namespace saveLoadTeleportMenuVars;

namespace saveLoadTeleportMenuVars {
	vars_ vars;

	void load(saveLoadTeleportMenu* pthis) {
		std::ifstream file(utils::getConfig()->getTeleportSavePath());
		if (file.good()) {
			nlohmann::json json;
			file >> json;
			file.close();

			nlohmann::json& data = json["teleports"];
			if (data.size()) {
				vars.locations.clear();

				for (auto it = data.begin(); it != data.end(); ++it) {
					nlohmann::json& coords = *it;
					vars.locations.push_back(std::make_pair(it.key(), math::vector3<float>(coords[0].get<float>(), coords[1].get<float>(), coords[2].get<float>())));
				}
			}
		} else {
			std::ofstream output(utils::getConfig()->getTeleportSavePath());
			if (output.good()) {
				nlohmann::json json;
				json["teleports"]["Thieves Landing"] = { -1423.340210f, -2292.360596f, 43.028f };

				output << json.dump(4);
				output.close();
			}
		}

		pthis->clearOptionsOffset(2);

		if (vars.locations.size() <= 0) {
			pthis->addOption(buttonOption("No teleports saved :(")
				.addTranslation());
		} else {
			for (auto& tp : vars.locations) {
				pthis->addOption(buttonOption(tp.first)
					.addOnClick([=] {
						teleportMenuVars::teleportToLocation(tp.second);
					}));
			}
		}
	}

	void save(std::string name, math::vector3<float> pos) {
		std::ifstream file(utils::getConfig()->getTeleportSavePath());
		if (file.good()) {
			nlohmann::json json;
			file >> json;
			file.close();

			json["teleports"][name] = { pos.x, pos.y, pos.z };
			std::ofstream output(utils::getConfig()->getTeleportSavePath());
			if (output.good()) {
				output << json.dump(4);
				output.close();
			}
		}
	}
}

void saveLoadTeleportMenu::init() {
	setName("Save and Load");
	setParentSubmenu<teleportMenu>();

	addOption(buttonOption("Save Current Location")
		.addTranslation()
		.addKeyboard("Enter name of location", 100, [this] (const char* name) {
			save(name, menu::getLocalPlayer().coords);
			load(this);
		}));

	addOption(breakOption("Locations")
		.addTranslation());

	load(this);
}

/*Called while in submenu*/
void saveLoadTeleportMenu::update() {}

/*Called once on submenu open*/
void saveLoadTeleportMenu::updateOnce() {
	load(this);
}

/*Called always*/
void saveLoadTeleportMenu::featureUpdate() {}

saveLoadTeleportMenu* _instance;
saveLoadTeleportMenu* saveLoadTeleportMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new saveLoadTeleportMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

saveLoadTeleportMenu::~saveLoadTeleportMenu() { delete _instance; }