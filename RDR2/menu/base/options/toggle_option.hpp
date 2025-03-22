#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "utils/config.hpp"

class toggleOption : public option {
public:
	toggleOption(std::string name)
		: option(name), onUpdateCallback([] (toggleOption*) {}), onClickCallback([] () {}), onHoverCallback([] () {})
	{}

	std::function<void()>& getOnClick() {
		return onClickCallback;
	}

	toggleOption& addToggle(bool& tog) {
		toggle = &tog;
		return *this;
	}

	toggleOption& addOnClick(std::function<void()> func) {
		onClickCallback = func;
		return *this;
	}

	toggleOption& addRequirement(std::function<bool()> func) {
		requirementCallback = func;
		return *this;
	}

	toggleOption& addOnUpdate(std::function<void(toggleOption*)> func) {
		onUpdateCallback = func;
		return *this;
	}

	toggleOption& addTooltip(std::string tip) {
		tooltip = tip;
		return *this;
	}

	toggleOption& canBeSaved(std::string submenuName) {
		canBeSaved_ = true;
		if (toggle && requirementCallback()) {
			*toggle = utils::getConfig()->readBool(submenuName.c_str(), name.getOriginal().c_str(), *toggle);
		}

		return *this;
	}

	toggleOption& addTranslation() {
		name.setHasTranslation(true);
		tooltip.setHasTranslation(true);
		return *this;
	}

	void render(int pos);
	void renderSelected(int pos, std::string submenuName);
private:
	std::function<void(toggleOption*)> onUpdateCallback;
	std::function<void()> onClickCallback;
	std::function<void()> onHoverCallback;

	bool* toggle;
};