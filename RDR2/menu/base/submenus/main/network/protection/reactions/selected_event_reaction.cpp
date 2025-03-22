#include "selected_event_reaction.hpp"
#include "menu/base/submenu_handler.hpp"
#include "utils/va.hpp"

using namespace networkSelectedEventReactionMenuVars;

namespace networkSelectedEventReactionMenuVars {
	vars_ vars;

	void handleSave(int type) {
		const char* tag = NULL;
		bool value = false;

		switch (type) {
			case 1: tag = "Explode"; value = vars.reaction->explode; break;
			case 2: tag = "Crash"; value = vars.reaction->crash; break;
			case 3: tag = "Add to Overseer"; value = vars.reaction->explode; break;
		}

		switch (vars.previous) {
			case 1: // on report
				utils::getConfig()->writeBool("Event Reactions: On Report", tag, value);
				break;

			case 2: // on aim
				utils::getConfig()->writeBool("Event Reactions: On Aim", tag, value);
				break;

			case 3: // on death
				utils::getConfig()->writeBool("Event Reactions: On Death", tag, value);
				break;

			case 4: // on cash spawn
				utils::getConfig()->writeBool("Event Reactions: On Cash Spawn", tag, value);
				break;
		}
	}

	std::vector<std::pair<networkEventReactionsMenuVars::eventReactions*, std::string>> list = {
		{ &networkEventReactionsMenuVars::vars.onReport, "On Report" },
		{ &networkEventReactionsMenuVars::vars.onAim, "On Aim" },
		{ &networkEventReactionsMenuVars::vars.onDeath, "On Death" },
		{ &networkEventReactionsMenuVars::vars.onCashSpawn, "On Cash Spawn" }
	};
}

void networkSelectedEventReactionMenu::init() {
	setName("Event Reactions", true, false);
	setParentSubmenu<networkEventReactionsMenu>();

	addOption(toggleOption("Explode")
		.addTranslation()
		.addOnClick([] { handleSave(1); })
		.addToggle(vars.tmp.explode)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(vars.reaction->explode); }));

	addOption(toggleOption("Crash")
		.addTranslation()
		.addOnClick([] { handleSave(2); })
		.addToggle(vars.tmp.crash)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(vars.reaction->crash); }));

	addOption(toggleOption("Add to Overseer")
		.addTranslation()
		.addOnClick([] { handleSave(3); })
		.addToggle(vars.tmp.addToOverseer)
		.addOnUpdate([] (toggleOption* option) { option->addToggle(vars.reaction->addToOverseer); }));

	// gay but meh
	for (auto& e : list) {
		e.first->addToOverseer = utils::getConfig()->readBool(utils::va::va("Event Reactions: %s", e.second.c_str()), "Add to Overseer", false);
		e.first->crash = utils::getConfig()->readBool(utils::va::va("Event Reactions: %s", e.second.c_str()), "Crash", false);
		e.first->explode = utils::getConfig()->readBool(utils::va::va("Event Reactions: %s", e.second.c_str()), "Explode", false);
	}
}

/*Called while in submenu*/
void networkSelectedEventReactionMenu::update() {}

/*Called once on submenu open*/
void networkSelectedEventReactionMenu::updateOnce() {}

/*Called always*/
void networkSelectedEventReactionMenu::featureUpdate() {}

networkSelectedEventReactionMenu* _instance;
networkSelectedEventReactionMenu* networkSelectedEventReactionMenu::getInstance() {
	if (_instance == nullptr) {
		_instance = new networkSelectedEventReactionMenu();
		_instance->init();
		menu::getSubmenuHandler()->addSubmenu(_instance);
	}

	return _instance;
}

networkSelectedEventReactionMenu::~networkSelectedEventReactionMenu() { delete _instance; }