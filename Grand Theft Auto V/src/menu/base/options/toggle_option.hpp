#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "utils/config.hpp"

class ToggleOption : public Option {
public:
	ToggleOption(std::string name)
		: Option(name), m_Toggle(nullptr), m_OnUpdateCallback([](ToggleOption*) {}), m_OnClickCallback([]() {}), m_OnHoverCallback([]() {}) {}

	ToggleOption& addToggle(bool& tog) {
		m_Toggle = &tog;
		return *this;
	}

	ToggleOption& addOnClick(std::function<void()> func) {
		m_OnClickCallback = func;
		return *this;
	}

	ToggleOption& addRequirement(std::function<bool()> func) {
		m_RequirementCallback = func;
		return *this;
	}

	ToggleOption& addOnUpdate(std::function<void(ToggleOption*)> func) {
		m_OnUpdateCallback = func;
		return *this;
	}

	ToggleOption& addTooltip(std::string tip) {
		m_Tooltip.Set(tip.c_str());
		return *this;
	}

	ToggleOption& addOnHover(std::function<void()> func) {
		m_OnHoverCallback = func;
		return *this;
	}

	ToggleOption& canBeSaved(std::stack<std::string> MenuStack) {
		if (MenuStack.size() <= 0) return *this;

		m_CanBeSaved = true;
		if (IsValidPtr(m_Toggle) && m_RequirementCallback()) {
			*m_Toggle = Utils::GetConfig()->ReadBool(MenuStack, m_Name.GetOriginal().c_str(), *m_Toggle);
			if (*m_Toggle) {
				m_OnClickCallback();
			}
		}

		return *this;
	}

	ToggleOption& addTranslation() {
		m_Name.SetHasTranslation(true);
		m_Tooltip.SetHasTranslation(true);
		return *this;
	}

	ToggleOption& addHotkey() {
		m_HasHotkey = true;
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos, std::stack<std::string> MenuStack);
	void InvokeSave(std::stack<std::string> MenuStack) {}
	void InvokeHotkey();
	void WriteTranslation(nlohmann::json& JsobObject) {}
	void ReadTranslation(nlohmann::json& JsobObject) {}
	void ResetTranslation() {}
private:
	std::function<void(ToggleOption*)> m_OnUpdateCallback;
	std::function<void()> m_OnClickCallback;
	std::function<void()> m_OnHoverCallback;

	bool* m_Toggle = nullptr;
};