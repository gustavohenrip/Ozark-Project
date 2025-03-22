#include "radio_option.hpp"
#include "../util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "global/ui_vars.hpp"

void radioOption::render(int pos) {
	onUpdateCallback(this);

	menu::getRenderer()->drawText(name.get().c_str(), { global::uivars::menuX + 0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText);
	
	if (context->toggles[index]) {
		color col = menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText;

		std::string sprite = context->sprite.second;
		//if (strstr(sprite.c_str(), "shop_") && !strstr(sprite.c_str(), "shop_tick_icon")) {
		//	if (menu::getBase()->isOptionSelected(pos)) {
		//		sprite += "_b";
		//	}
		//	else {
		//		sprite += "_a";
		//	}

		//	col = { 255, 255, 255, 255 };
		//}

		menu::getRenderer()->drawSprite(context->sprite.first, sprite.c_str(), { global::uivars::menuX + 0.2195f, (global::uivars::menuY + 0.016f) + (pos * 0.0315f) }, { 0.0176f, 0.0312f }, 0.f, col);
	}


}

void radioOption::renderSelected(int pos, std::string submenuName) {
	onHoverCallback();

	if (requirementCallback() && menu::util::getInput()->isOptionPressed()) {
		onClickCallback();

		if (!context->toggles[index]) {
			context->toggles[index] = true;
			context->toggles[context->selected] = false;
			context->selected = ID;
		}
	}


	if (requirementCallback() && menu::util::getInput()->isOptionPressed()) {
		onClickCallback();
	}


	menu::getRenderer()->displayTooltip(tooltip.getOriginal());
}