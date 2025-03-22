#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"

class breakOption : public option {
public:
	breakOption(std::string name)
		: option(name), onUpdateCallback([](breakOption*) {})
	{}

	breakOption& addRequirement(std::function<bool()> func) {
		requirementCallback = func;
		return *this;
	}

	breakOption& addOnUpdate(std::function<void(breakOption*)> func) {
		onUpdateCallback = func;
		return *this;
	}

	breakOption& addTranslation() {
		name.setHasTranslation(true);
		tooltip.setHasTranslation(true);
		return *this;
	}
	
	void render(int pos);
	void renderSelected(int pos, std::string submenuName);
private:
	std::function<void(breakOption*)> onUpdateCallback;
};