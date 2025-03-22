#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "menu/base/submenu.hpp"

static Color g_EmptyColor = Color(-1, -1, -1, -1);

class SubmenuOption : public Option {
public:
	SubmenuOption(std::string name)
		: Option(name), m_UpdateColor(&g_EmptyColor), m_Submenu(nullptr), m_OnUpdateCallback([] (SubmenuOption*, int) {}), m_OnClickCallback([] () {}), m_OnHoverCallback([] () {}) {}

	template<typename T>
	SubmenuOption& addSubmenu() {
		m_Submenu = T::GetInstance();
		return *this;
	}

	SubmenuOption& addColor(Color& Col) {
		m_UpdateColor = &Col;
		return *this;
	}

	SubmenuOption& addTextOffset(float Offset) {
		m_Offset = Offset;
		return *this;
	}

	SubmenuOption& addOnClick(std::function<void()> func) {
		m_OnClickCallback = func;
		return *this;
	}

	SubmenuOption& addRequirement(std::function<bool()> func) {
		m_RequirementCallback = func;
		return *this;
	}

	SubmenuOption& addOnUpdate(std::function<void(SubmenuOption*, int)> func) {
		m_OnUpdateCallback = func;
		return *this;
	}

	SubmenuOption& addOnHover(std::function<void()> func) {
		m_OnHoverCallback = func;
		return *this;
	}

	SubmenuOption& addTooltip(std::string tip) {
		m_Tooltip.Set(tip.c_str());
		return *this;
	}

	SubmenuOption& addTranslation() {
		m_Name.SetHasTranslation(true);
		m_Tooltip.SetHasTranslation(true);
		return *this;
	}

	SubmenuOption& addSideText(std::string Text) {
		m_SideText = Text;
		m_HasSideText = true;
		return *this;
	}

	SubmenuOption& addHotkey() {
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
	std::function<void(SubmenuOption*, int)> m_OnUpdateCallback;
	std::function<void()> m_OnClickCallback;
	std::function<void()> m_OnHoverCallback;

	Color* m_UpdateColor;
	Menu::Submenu* m_Submenu;

	bool m_HasSideText = false;
	std::string m_SideText = "";
};