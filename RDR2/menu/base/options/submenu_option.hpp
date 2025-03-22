#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "menu/base/submenu.hpp"

class submenuOption : public option {
public:
	submenuOption(std::string name)
		: option(name), onUpdateCallback([] (submenuOption*) {}), onClickCallback([] () {}), onHoverCallback([] () {})
	{}

	template<typename T>
	submenuOption& addSubmenu() {
		submenu = T::getInstance();
		return *this;
	}

	submenuOption& addOnClick(std::function<void()> func) {
		onClickCallback = func;
		return *this;
	}

	submenuOption& addRequirement(std::function<bool()> func) {
		requirementCallback = func;
		return *this;
	}

	submenuOption& addOnUpdate(std::function<void(submenuOption*)> func) {
		onUpdateCallback = func;
		return *this;
	}

	submenuOption& addOnHover(std::function<void()> func) {
		onHoverCallback = func;
		return *this;
	}

	submenuOption& addTooltip(std::string tip) {
		tooltip = tip;
		return *this;
	}

	submenuOption& addTranslation() {
		name.setHasTranslation(true);
		tooltip.setHasTranslation(true);
		return *this;
	}

	void render(int pos);
	void renderSelected(int pos, std::string submenuName);
private:
	std::function<void(submenuOption*)> onUpdateCallback;
	std::function<void()> onClickCallback;
	std::function<void()> onHoverCallback;

	menu::submenu* submenu;
};