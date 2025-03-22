#pragma once
#include "stdafx.hpp"
#include "utils/translation_string.hpp"

class option {
public:
	option()
		: name(translationString("")), tooltip(translationString("")), canBeSaved_(false), isBreak_(false), isVisible_(true), requirementCallback([] () -> bool { return true; })
	{}

	option(std::string name)
		: name(translationString(name)), tooltip(translationString("")), canBeSaved_(false), isBreak_(false), isVisible_(true), requirementCallback([] () -> bool { return true; })
	{}

	option(std::string name, bool breaker)
		: name(translationString(name)), tooltip(translationString("")), canBeSaved_(false), isBreak_(breaker), isVisible_(true), requirementCallback([] () -> bool { return true; })
	{}

	virtual void render(int pos);
	virtual void renderSelected(int pos, std::string submenuName);

	bool isVisible() { return isVisible_ && requirementCallback(); }
	bool isBreak() { return isBreak_; }
	translationString& getTooltip() { return tooltip; }
	translationString& getName() { return name; }
	void setName(const char* n) { name.set(n); }
protected:
	translationString name;
	translationString tooltip;
	bool isBreak_;
	bool isVisible_;
	bool canBeSaved_;
	std::function<bool()> requirementCallback;
};