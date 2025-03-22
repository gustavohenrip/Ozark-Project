#include "button_option.hpp"
#include "../util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "global/ui_vars.hpp"

void buttonOption::render(int pos) {
	onUpdateCallback(this);

	menu::getRenderer()->drawText(name.get().c_str(), { global::uivars::menuX + 0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText);
}

void buttonOption::renderSelected(int pos, std::string submenuName) {
	onHoverCallback();

	if (requirementCallback() && menu::util::getInput()->isOptionPressed()) {
		onClickCallback();

		if (hasKeyboard) {
			isKeyboardActive = true;
			menu::getBase()->keyboardTitle = keyboardTitle;
			native::displayOnscreenKeyboard(0, "OzarkKeyboardTitle", "", "", "", "", "", keyboardMaxChars);
		}
	}

	if (isKeyboardActive) {
		menu::getBase()->disableInputThisFrame = true;
		if (native::updateOnscreenKeyboard() > 1) {
			isKeyboardActive = false;
		} else {
			if (native::updateOnscreenKeyboard() == 1) {
				isKeyboardActive = false;

				if (onKeyboardCallbackThis) {
					onKeyboardCallbackThis(this, native::getOnscreenKeyboardResult());
				}

				if (onKeyboardCallback) {
					onKeyboardCallback(native::getOnscreenKeyboardResult());
				}
			}
		}
	}
	menu::getRenderer()->displayTooltip(tooltip.getOriginal());
}