#include "toggle_option.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/ui_vars.hpp"

void toggleOption::render(int pos) {
	onUpdateCallback(this);

	menu::getRenderer()->drawText(name.get().c_str(), { global::uivars::menuX + 0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText);
	menu::getRenderer()->drawSprite("generic_textures", "counter_bg_1b", { global::uivars::menuX + 0.221f, (global::uivars::menuY + 0.016f) + (pos * 0.0315f) }, { 0.007f, 0.011f }, 0.f, *toggle ? global::uivars::toggleOn : global::uivars::toggleOff);
}

void toggleOption::renderSelected(int pos, std::string submenuName) {
	onHoverCallback();

	if (toggle && requirementCallback() && menu::util::getInput()->isOptionPressed()) {
		*toggle = !*toggle;
		onClickCallback();

		if (canBeSaved_) {
			utils::getConfig()->writeBool(submenuName.c_str(), name.getOriginal().c_str(), *toggle);
		}
	}
	menu::getRenderer()->displayTooltip(tooltip.getOriginal());
}