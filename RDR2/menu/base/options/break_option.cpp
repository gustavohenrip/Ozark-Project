#include "break_option.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/submenu_handler.hpp"
#include "global/ui_vars.hpp"

void breakOption::render(int pos) {
	onUpdateCallback(this);

	menu::getRenderer()->drawText(("~italic~" + name.get()).c_str(), { global::uivars::menuX + .004f + ((0.23f - (0.004f * 2.f)) / 2.f), global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.310f, global::uivars::breakFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::breakText, JUSTIFY_CENTER);
}

void breakOption::renderSelected(int pos, std::string submenuName) {
	switch (menu::getBase()->breakScroll) {
		case 1: // up
			if (menu::getBase()->scrollOffset > 0 && menu::getBase()->currentOption - menu::getBase()->scrollOffset == 0) menu::getBase()->scrollOffset--;
			if (menu::getBase()->currentOption - 1 < 0) menu::util::getInput()->scrollBottom();
			else menu::getBase()->currentOption--;
			break;
		case 2: // down
			if (menu::getBase()->scrollOffset < menu::getSubmenuHandler()->getTotalOptions() - menu::getBase()->maxOptions && menu::getBase()->currentOption + 1 - menu::getBase()->scrollOffset == menu::getBase()->maxOptions) menu::getBase()->scrollOffset++;
			if (menu::getBase()->currentOption + 1 >= menu::getSubmenuHandler()->getTotalOptions()) menu::util::getInput()->scrollTop();
			else menu::getBase()->currentOption++;
			break;
		case 3: // bottom
			if (menu::getBase()->scrollOffset > 0 && menu::getBase()->currentOption - menu::getBase()->scrollOffset == 1) menu::getBase()->scrollOffset--;
			menu::getBase()->currentOption--;
			break;
		case 4: // top
			if (menu::getBase()->scrollOffset < menu::getSubmenuHandler()->getTotalOptions() - menu::getBase()->maxOptions && menu::getSubmenuHandler()->getTotalOptions() >= menu::getBase()->maxOptions && menu::getBase()->currentOption - menu::getBase()->scrollOffset == menu::getBase()->maxOptions) menu::getBase()->scrollOffset++;
			menu::getBase()->currentOption++;
			break;
	}
}