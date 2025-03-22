#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "utils/config.hpp"

template<typename Type>
class NumberOption : public Option {
public:
	NumberOption(ScrollOptionType typ, std::string m_Name)
		: Option(m_Name, false, true, typ == SCROLL), m_Type(typ), m_Format(""), m_Number(nullptr), m_IsKeyboardActive(false), m_Toggle(nullptr), m_HasMin(false), m_HasMax(false), m_ShowMax(false), m_Loop(false), m_ScrollSpeed(100), m_OnUpdateCallback([](NumberOption*) {}), m_OnClickCallback([]() {}), m_OnHoverCallback([]() {}) {}

	NumberOption& addToggle(bool& t) {
		m_Toggle = &t;
		return *this;
	}

	NumberOption& addNumber(Type& num, std::string fmt, Type step_) {
		m_Number = &num;
		m_Format = fmt;
		m_Step = (Type)step_;
		return *this;
	}

	NumberOption& addMin(Type m) {
		m_Min = m;
		m_HasMin = true;
		return *this;
	}

	NumberOption& addMax(Type m) {
		m_Max = m;
		m_HasMax = true;
		return *this;
	}

	NumberOption& addOnClick(std::function<void()> func) {
		m_OnClickCallback = func;
		return *this;
	}

	NumberOption& addOnHover(std::function<void()> func) {
		m_OnHoverCallback = func;
		return *this;
	}

	NumberOption& addRequirement(std::function<bool()> func) {
		m_RequirementCallback = func;
		return *this;
	}

	NumberOption& showMax() {
		m_ShowMax = true;
		return *this;
	}

	NumberOption& addOnUpdate(std::function<void(NumberOption*)> func) {
		m_OnUpdateCallback = func;
		return *this;
	}

	NumberOption& addTooltip(std::string tip) {
		m_Tooltip.Set(tip.c_str());
		return *this;
	}

	NumberOption& setScrollSpeed(DWORD speed) {
		m_ScrollSpeed = speed;
		return *this;
	}

	NumberOption& canLoop() {
		m_Loop = true;
		return *this;
	}

	NumberOption& addTranslation() {
		m_Name.SetHasTranslation(true);
		m_Tooltip.SetHasTranslation(true);
		return *this;
	}

	NumberOption& canBeSaved(std::stack<std::string> MenuStack) {
		m_CanBeSaved = true;
		if (IsValidPtr(m_Number) && m_RequirementCallback()) {
			if (std::is_same<Type, int>::value) *m_Number = (Type)Utils::GetConfig()->ReadInt(MenuStack, m_Name.GetOriginal().c_str(), (int)*m_Number, { "Values" });
			else if (std::is_same<Type, float>::value) *m_Number = (Type)Utils::GetConfig()->ReadFloat(MenuStack, m_Name.GetOriginal().c_str(), (float)*m_Number, { "Values" });
			if (m_Type == TOGGLE && IsValidPtr(m_Toggle)) {
				*m_Toggle = Utils::GetConfig()->ReadBool(MenuStack, m_Name.GetOriginal().c_str(), *m_Toggle);

				if (*m_Toggle) {
					m_OnClickCallback();
				}
			}

			if (m_HasMin && *m_Number < m_Min) *m_Number = m_Min;
			if (m_HasMax && *m_Number > m_Max) *m_Number = m_Max;
		}

		return *this;
	}

	NumberOption& addHotkey() {
		m_HasHotkey = true;
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos, std::stack<std::string> MenuStack);
	void InvokeSave(std::stack<std::string> MenuStack);
	void InvokeHotkey();
	void WriteTranslation(nlohmann::json& JsobObject) {}
	void ReadTranslation(nlohmann::json& JsobObject) {}
	void ResetTranslation() {}
private:
	std::function<void(NumberOption*)> m_OnUpdateCallback;
	std::function<void()> m_OnClickCallback;
	std::function<void()> m_OnHoverCallback;

	ScrollOptionType m_Type;
	std::string m_Format;
	Type* m_Number;
	Type m_Step;
	Type m_Min;
	Type m_Max;

	bool* m_Toggle;
	bool m_HasMin;
	bool m_HasMax;
	bool m_Loop;
	bool m_ShowMax;
	bool m_IsKeyboardActive;
	uint32_t m_ScrollSpeed;
};