#include "number_option.hpp"
#include "../util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "utils/va.hpp"
#include "global/ui_vars.hpp"

static bool leftDisabled = false;
static bool rightDisabled = false;
static int leftTimer = 0;
static int rightTimer = 0;

template<class Type>
void numberOption<Type>::render(int pos) {
	onUpdateCallback(this);

	switch (type) {
		case TOGGLE:
			menu::getRenderer()->drawSprite("generic_textures", "counter_bg_1b", { global::uivars::menuX + 0.221f, (global::uivars::menuY  + 0.016f) + (pos * 0.0315f) }, { 0.007f, 0.011f }, 0.f, *toggle ? global::uivars::toggleOn : global::uivars::toggleOff);
			menu::getRenderer()->drawText(utils::va::va(format.c_str(), *number), { global::uivars::menuX + 0.002f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (global::uivars::menuX + (0.315f / 2.f)) - .057f)) });
			break;
		case SCROLL:
		case SCROLLSELECT:
			menu::getRenderer()->drawText(utils::va::va(format.c_str(), *number), { global::uivars::menuX + 0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (global::uivars::menuX + (0.315f / 2.f)) - .068f)) });
			break;
	}

	menu::getRenderer()->drawText(name.get().c_str(), { global::uivars::menuX + 0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText);
}

template<class Type>
void numberOption<Type>::renderSelected(int pos, std::string submenuName) {
	onHoverCallback();

	if (requirementCallback() && menu::util::getInput()->isOptionPressed()) {
		switch (type) {
			case TOGGLE: *toggle = !*toggle; break;
			case SCROLL: /*Open custom input*/ break;
		}

		onClickCallback();
	}

	if (canBeSaved_) {
		if (std::is_same<Type, int>::value) utils::getConfig()->writeInt((submenuName + " - Values").c_str(), name.getOriginal().c_str(), (int)*number);
		else if (std::is_same<Type, float>::value) utils::getConfig()->writeFloat((submenuName + " - Values").c_str(), name.getOriginal().c_str(), (float)*number);
		if (type == TOGGLE) utils::getConfig()->writeBool(submenuName.c_str(), name.getOriginal().c_str(), *toggle);
	}

	if (menu::util::getInput()->isLeftJustPressed()) {
		if (!leftDisabled) {
			*number -= step;
			if (loop) {
				if (*number < min) *number = max;
			} else if (*number < min && hasMin) *number = min;

			if (type == SCROLL || type == TOGGLE) onClickCallback();
		}

		leftDisabled = false;
		leftTimer = 0;
	}

	if (menu::util::getInput()->isRightJustPressed()) {
		if (!rightDisabled) {
			*number += step;
			if (loop) {
				if (*number > max) *number = min;
			} else if (*number > max&& hasMax) *number = max;

			if (type == SCROLL || type == TOGGLE) onClickCallback();
		}

		rightDisabled = false;
		rightTimer = 0;
	}

	static DWORD timer = 0;

	if (menu::util::getInput()->isLeftPressed()) {
		if (++leftTimer > 20) {
			leftDisabled = true;
			if ((timeGetTime() - timer) > scrollSpeed) {
				*number -= step;
				if (loop) {
					if (*number < min) *number = max;
				} else if (*number < min && hasMin) *number = min;

				if (type == SCROLL || type == TOGGLE) onClickCallback();
				timer = timeGetTime();
			}
		}
	}

	if (menu::util::getInput()->isRightPressed()) {
		if (++rightTimer > 20) {
			rightDisabled = true;
			if ((timeGetTime() - timer) > scrollSpeed) {
				*number += step;
				if (loop) {
					if (*number > max) *number = min;
				} else if (*number > max&& hasMax) *number = max;

				if (type == SCROLL || type == TOGGLE) onClickCallback();
				timer = timeGetTime();
			}
		}
	}
	menu::getRenderer()->displayTooltip(tooltip.getOriginal());
}

template class numberOption<int>;
template class numberOption<float>;