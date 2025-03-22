#include "submenu_option.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/ui_vars.hpp"

void submenuOption::render(int pos) {
	onUpdateCallback(this);

	menu::getRenderer()->drawText(name.get().c_str(), { global::uivars::menuX +  0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText);
	menu::getRenderer()->drawRect({ global::uivars::menuX + 0.23f - 0.005f, global::uivars::menuY + (pos * 0.0315f) }, { 0.005f, 0.0315f }, global::uivars::submenuIndicator);
}

void submenuOption::renderSelected(int pos, std::string submenuName) {
	onHoverCallback();

	if (requirementCallback() && menu::util::getInput()->isOptionPressed()) {
		onClickCallback();

		menu::getSubmenuHandler()->setSubmenu(submenu);
	}
	menu::getRenderer()->displayTooltip(tooltip.getOriginal());
}