#pragma once
#include "stdafx.hpp"
#include "utils/translation_string.hpp"
#include "utils/json.hpp"

static TranslationString T_UsedHotkeyFor("Used hotkey for", true, true);

class Option {
public:
	Option() :
		m_Name(TranslationString("")),
		m_Tooltip(TranslationString("")),
		m_CanBeSaved(false),
		m_IsBreak(false),
		m_IsVisible(true),
		m_Slider(false),
		m_HasHotkey(false),
		m_RequirementCallback([]() -> bool { return true; }),
		m_Instructionals({}) {}

	Option(std::string m_Name, bool breaker = false, bool Slider = false, bool Input = false)
		: m_Name(TranslationString(m_Name)), m_Tooltip(TranslationString("")), m_Slider(Slider), m_Input(Input), m_CanBeSaved(false), m_IsBreak(breaker), m_IsVisible(true), m_RequirementCallback([]() -> bool { return true; }) {
	}

	virtual void Render(int pos);
	virtual void RenderSelected(int pos, std::stack<std::string> MenuStack);
	virtual void InvokeSave(std::stack<std::string> MenuStack);
	virtual void InvokeHotkey();
	virtual void WriteTranslation(nlohmann::json& JsobObject);
	virtual void ReadTranslation(nlohmann::json& JsobObject);
	virtual void ResetTranslation();

	bool IsVisible() { return m_IsVisible && m_RequirementCallback(); }
	bool IsBreak() { return m_IsBreak; }
	bool IsSlider() { return m_Slider; }
	bool IsInput() { return m_Input;}
	bool HasHotkey() { return m_HasHotkey; }
	int GetHotkey() { return m_Hotkey; }

	void setHotkey(int key) { m_Hotkey = key; }
	void setName(const char* n) { m_Name.Set(n); }
	void setTooltipInternal(std::string tip) { m_Tooltip.Set(tip.c_str()); }
	void SetSubmenuNameStackPtr(std::stack<std::string>* Ptr) { m_SubmenuNameStackPtr = Ptr; }

	TranslationString& GetTooltip() { return m_Tooltip; }
	TranslationString& GetName() { return m_Name; }
	std::stack<std::string>* GetSubmenuNameStackPtr() { return m_SubmenuNameStackPtr; }
protected:
	TranslationString m_Name;
	TranslationString m_Tooltip;
	bool m_IsBreak;
	bool m_IsVisible;
	bool m_CanBeSaved;
	bool m_Slider;
	bool m_Input;
	bool m_HasHotkey = false;
	float m_Offset = 0.f;
	int m_Hotkey = -1;
	std::function<bool()> m_RequirementCallback;
	std::vector<std::tuple<std::string, int, bool>>	m_Instructionals;
	std::stack<std::string>* m_SubmenuNameStackPtr = nullptr;
};