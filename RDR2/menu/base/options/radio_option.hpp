#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"

struct radioContext {

	std::unordered_map<int, bool> toggles;
	int selected;
	int count;
	std::pair<const char*, const char*> sprite;

	radioContext(const char* spriteDict, const char* spriteTexture) {
		sprite = std::make_pair(spriteDict, spriteTexture);
		selected = 0;
		count = 0;
	}

	void reset() {
		count = 0;

		for (auto& it : toggles) {
			it.second = false;
		}
	}
};

class radioOption : public option {
public:
	radioOption(std::string name)
		: option(name), onUpdateCallback([](radioOption*) {}), onClickCallback([]() {}), onHoverCallback([]() {})
	{}

	radioOption& addRadio(radioContext& radio) {
		context = &radio;
		index = context->count;
		ID = context->count;
		context->count++;

		if (ID == 0) {
			context->toggles[index] = true;
		}

		return *this;
	}
	radioOption& addOnClick(std::function<void()> func) {
		onClickCallback = func;
		return *this;
	}

	radioOption& addRequirement(std::function<bool()> func) {
		requirementCallback = func;
		return *this;
	}

	radioOption& addOnUpdate(std::function<void(radioOption*)> func) {
		onUpdateCallback = func;
		return *this;
	}

	radioOption& addTooltip(std::string tip) {
		tooltip = tip;
		return *this;
	}

	radioOption& addOnHover(std::function<void()> func) {
		onHoverCallback = func;
		return *this;
	}

	radioOption& addTranslation() {
		name.setHasTranslation(true);
		tooltip.setHasTranslation(true);
		return *this;
	}

	void render(int pos);
	void renderSelected(int pos, std::string submenuName);
private:
	std::function<void(radioOption*)> onUpdateCallback;
	std::function<void()> onClickCallback;
	std::function<void()> onHoverCallback;

	int ID;
	int index;
	radioContext* context;
};