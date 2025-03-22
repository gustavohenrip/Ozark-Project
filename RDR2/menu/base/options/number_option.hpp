#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "utils/config.hpp"

template<typename Type>
class numberOption : public option {
public:
	numberOption(ScrollOptionType typ, std::string name)
		: option(name), type(typ), format(""), number(nullptr), toggle(nullptr), hasMin(false), hasMax(false), loop(false), scrollSpeed(100), onUpdateCallback([] (numberOption*) {}), onClickCallback([] () {}), onHoverCallback([] () {})
	{}

	numberOption& addToggle(bool& t) {
		toggle = &t;
		return *this;
	}

	numberOption& addNumber(Type& num, std::string fmt, Type step_) {
		number = &num;
		format = fmt;
		step = step_;
		return *this;
	}

	numberOption& addMin(Type m) {
		min = m;
		hasMin = true;
		return *this;
	}

	numberOption& addMax(Type m) {
		max = m;
		hasMax = true;
		return *this;
	}

	numberOption& addOnClick(std::function<void()> func) {
		onClickCallback = func;
		return *this;
	}

	numberOption& addOnHover(std::function<void()> func) {
		onHoverCallback = func;
		return *this;
	}

	numberOption& addRequirement(std::function<bool()> func) {
		requirementCallback = func;
		return *this;
	}

	numberOption& addOnUpdate(std::function<void(numberOption*)> func) {
		onUpdateCallback = func;
		return *this;
	}

	numberOption& addTooltip(std::string tip) {
		tooltip = tip;
		return *this;
	}

	numberOption& setScrollSpeed(DWORD speed) {
		scrollSpeed = speed;
		return *this;
	}

	numberOption& canLoop() {
		loop = true;
		return *this;
	}

	numberOption& addTranslation() {
		name.setHasTranslation(true);
		tooltip.setHasTranslation(true);
		return *this;
	}

	numberOption& canBeSaved(std::string submenuName) {
		canBeSaved_ = true;
		if (number && requirementCallback()) {
			if (std::is_same<Type, int>::value) *number = (Type)utils::getConfig()->readInt((submenuName + " - Values").c_str(), name.getOriginal().c_str(), (int)*number);
			else if (std::is_same<Type, float>::value) *number = (Type)utils::getConfig()->readFloat((submenuName + " - Values").c_str(), name.getOriginal().c_str(), (float)*number);
			if (type == TOGGLE) *toggle = utils::getConfig()->readBool(submenuName.c_str(), name.getOriginal().c_str(), *toggle);
		}

		if (hasMin && *number < min) *number = min;
		if (hasMax && *number > max) *number = max;
		return *this;
	}

	void render(int pos);
	void renderSelected(int pos, std::string submenuName);
private:
	std::function<void(numberOption*)> onUpdateCallback;
	std::function<void()> onClickCallback;
	std::function<void()> onHoverCallback;

	ScrollOptionType type;
	std::string format;
	Type* number;
	Type step;
	Type min;
	Type max;
	
	bool* toggle;
	bool hasMin;
	bool hasMax;
	bool loop;
	DWORD scrollSpeed;
};