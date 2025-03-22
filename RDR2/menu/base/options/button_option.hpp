#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"

class buttonOption : public option {
public:
	buttonOption(std::string name)
		: option(name), hasKeyboard(false), isKeyboardActive(false), keyboardMaxChars(0), onUpdateCallback([] (buttonOption*) {}), onClickCallback([] () {}), onHoverCallback([] () {}), onKeyboardCallback([] (const char*) {})
	{}

	buttonOption& addOnClick(std::function<void()> func) {
		onClickCallback = func;
		return *this;
	}

	buttonOption& addRequirement(std::function<bool()> func) {
		requirementCallback = func;
		return *this;
	}

	buttonOption& addOnUpdate(std::function<void(buttonOption*)> func) {
		onUpdateCallback = func;
		return *this;
	}

	buttonOption& addTooltip(std::string tip) {
		tooltip = tip;
		return *this;
	}

	buttonOption& addKeyboard(const char* title, int maxChars, std::function<void(const char*)> callback) {
		hasKeyboard = true;
		keyboardMaxChars = maxChars;
		onKeyboardCallback = callback;
		keyboardTitle = title;
		return *this;
	}

	buttonOption& addKeyboardThis(const char* title, int maxChars, std::function<void(buttonOption*, const char*)> callback) {
		hasKeyboard = true;
		keyboardMaxChars = maxChars;
		onKeyboardCallbackThis = callback;
		keyboardTitle = title;
		return *this;
	}

	buttonOption& addOnHover(std::function<void()> func) {
		onHoverCallback = func;
		return *this;
	}

	buttonOption& addTranslation() {
		name.setHasTranslation(true);
		tooltip.setHasTranslation(true);
		return *this;
	}

	void render(int pos);
	void renderSelected(int pos, std::string submenuName);
private:
	std::function<void(buttonOption*)> onUpdateCallback;
	std::function<void()> onClickCallback;
	std::function<void()> onHoverCallback;
	std::function<void(const char*)> onKeyboardCallback;
	std::function<void(buttonOption*, const char*)> onKeyboardCallbackThis;

	bool hasKeyboard;
	bool isKeyboardActive;
	int keyboardMaxChars;
	const char* keyboardTitle;
};