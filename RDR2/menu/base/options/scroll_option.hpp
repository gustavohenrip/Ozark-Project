#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "../util/input.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/base.hpp"
#include "global/ui_vars.hpp"
#include "utils/config.hpp"

template<typename Type>
struct scrollStruct {
	std::string name;
	Type result;
};

template<typename Type>
class scrollOption : public option {
private:
	std::function<void(scrollOption*)> onUpdateCallback;
	std::function<void()> onClickCallback;
	std::function<void()> onHoverCallback;

	ScrollOptionType type;
	int* number;
	bool* toggle;
	int min;
	int max;
	int leftTimer = 0;
	int rightTimer = 0;
	DWORD scrollSpeed;

	bool hasMin;
	bool hasMax;
	bool loop;
	bool leftDisabled = false;
	bool rightDisabled = false;

	scrollStruct<Type>* scroll;
public:
	scrollOption(ScrollOptionType typ, std::string name)
		: option(name), type(typ), number(nullptr), toggle(nullptr), hasMin(false), hasMax(false), loop(false), scrollSpeed(100), onUpdateCallback([] (scrollOption*) {}), onClickCallback([] () {}), onHoverCallback([] () {})
	{}

	std::function<void()>& getOnClick() {
		return onClickCallback;
	}

	scrollOption& addScroll(int& num, int min_, int max_, scrollStruct<Type>* structure) {
		hasMin = true;
		hasMax = true;
		min = min_;
		max = max_ - 1;
		number = &num;
		scroll = structure;
		return *this;
	}

	scrollOption& addToggle(bool& t) {
		toggle = &t;
		return *this;
	}

	scrollOption& addOnClick(std::function<void()> func) {
		onClickCallback = func;
		return *this;
	}

	scrollOption& addRequirement(std::function<bool()> func) {
		requirementCallback = func;
		return *this;
	}

	scrollOption& addOnUpdate(std::function<void(scrollOption*)> func) {
		onUpdateCallback = func;
		return *this;
	}

	scrollOption& addTooltip(std::string tip) {
		tooltip = tip;
		return *this;
	}

	scrollOption& setScrollSpeed(DWORD speed) {
		scrollSpeed = speed;
		return *this;
	}

	scrollOption& canLoop() {
		loop = true;
		return *this;
	}

	scrollOption& addTranslation() {
		name.setHasTranslation(true);
		tooltip.setHasTranslation(true);
		return *this;
	}

	scrollOption& canBeSaved(std::string submenuName) {
		canBeSaved_ = true;
		if (number && requirementCallback()) {
			*number = utils::getConfig()->readInt((submenuName + " - Values").c_str(), name.getOriginal().c_str(), (int)*number);
			if (type == TOGGLE) *toggle = utils::getConfig()->readBool(submenuName.c_str(), name.getOriginal().c_str(), *toggle);
		}

		return *this;
	}

	void render(int pos) {
		onUpdateCallback(this);

		char nameBuffer[128];
		sprintf(nameBuffer, "%s ~m~[%i/%i]", scroll[*number].name.c_str(), (*number) + 1, max + 1);

		switch (type) {
			case TOGGLE:
				menu::getRenderer()->drawSprite("generic_textures", "counter_bg_1b", { global::uivars::menuX + 0.221f, (global::uivars::menuY + 0.016f) + (pos * 0.0315f) }, { 0.007f, 0.011f }, 0.f, *toggle ? global::uivars::toggleOn : global::uivars::toggleOff);
				menu::getRenderer()->drawText(nameBuffer, { global::uivars::menuX + 0.002f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (global::uivars::menuX + (0.315f / 2.f)) - .057f)) });
				break;
			case SCROLL:
			case SCROLLSELECT:
				menu::getRenderer()->drawText(nameBuffer, { global::uivars::menuX + 0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText, JUSTIFY_RIGHT, false, { 0.f, (1.0f - (1.0f - (global::uivars::menuX + (0.315f / 2.f)) - .068f)) });
				break;
		}

		menu::getRenderer()->drawText(name.get().c_str(), { global::uivars::menuX + 0.004f, global::uivars::menuY + (pos * 0.0315f) + 0.006f }, 0.315f, global::uivars::optionFont, menu::getBase()->isOptionSelected(pos) ? global::uivars::selectedText : global::uivars::mainText);
	}

	void renderSelected(int pos, std::string submenuName) {
		onHoverCallback();
		menu::getRenderer()->displayTooltip(tooltip.getOriginal());
		if (requirementCallback() && menu::util::getInput()->isOptionPressed()) {
			switch (type) {
				case TOGGLE: *toggle = !*toggle; break;
				case SCROLL: /*Open custom input*/ break;
			}

			onClickCallback();
		}

		if (canBeSaved_) {
			utils::getConfig()->writeInt((submenuName + " - Values").c_str(), name.get().c_str(), (int)*number);
			if (type == TOGGLE) utils::getConfig()->writeBool(submenuName.c_str(), name.getOriginal().c_str(), *toggle);
		}

		if (menu::util::getInput()->isLeftJustPressed()) {
			if (!leftDisabled) {
				*number -= 1;
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
				*number += 1;
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
					*number -= 1;
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
					*number += 1;
					if (loop) {
						if (*number > max) *number = min;
					} else if (*number > max&& hasMax) *number = max;

					if (type == SCROLL || type == TOGGLE) onClickCallback();
					timer = timeGetTime();
				}
			}
		}
	}
};