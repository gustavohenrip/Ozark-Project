#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"

class BreakOption : public Option {
public:
	BreakOption(std::string name)
		: Option(name), m_OnUpdateCallback([](BreakOption*) {}) {}

	BreakOption& addRequirement(std::function<bool()> func) {
		m_RequirementCallback = func;
		return *this;
	}

	BreakOption& addOnUpdate(std::function<void(BreakOption*)> func) {
		m_OnUpdateCallback = func;
		return *this;
	}

	BreakOption& addTranslation() {
		m_Name.SetHasTranslation(true);
		m_Tooltip.SetHasTranslation(true);
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos, std::stack<std::string> MenuStack);
	void InvokeSave(std::stack<std::string> MenuStack) {}
	void InvokeHotkey() {}
	void WriteTranslation(nlohmann::json& JsobObject) {}
	void ReadTranslation(nlohmann::json& JsobObject) {}
	void ResetTranslation() {}
private:
	std::function<void(BreakOption*)> m_OnUpdateCallback;
};