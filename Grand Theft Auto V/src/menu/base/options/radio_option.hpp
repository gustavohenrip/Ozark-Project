#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"

struct RadioContext {
	std::unordered_map<int, bool> m_Toggles;
	int m_Selected = 0;
	int m_Count;
	std::pair<const char*, const char*> m_Sprite;

	RadioContext(const char* SpriteDict, const char* SpriteTexture) {
		m_Sprite = std::make_pair(SpriteDict, SpriteTexture);
		m_Selected = 0;
		m_Count = 0;
	}

	void Reset() {
		m_Count = 0;

		for (auto& it : m_Toggles) {
			it.second = false;
		}
	}
};

class RadioOption : public Option {
public:
	RadioOption(std::string name)
		: Option(name), m_ID(0), m_Context(nullptr), m_OnClickCallback([] {}), m_OnUpdateCallback([](RadioOption* option) {}), m_OnHoverCallback([] {}) {}

	RadioOption& addRadio(RadioContext& radio) {
		m_Context = &radio;
		m_Index = m_Context->m_Count;
		m_ID = m_Context->m_Count;
		m_Context->m_Count++;

		if (m_ID == 0) {
			m_Context->m_Toggles[m_Index] = true;
		}

		return *this;
	}

	RadioOption& addTranslation() {
		m_Tooltip.SetHasTranslation(true);
		m_Name.SetHasTranslation(true);
		return *this;
	}

	RadioOption& addOnClick(std::function<void()> func) {
		m_OnClickCallback = func;
		return *this;
	}

	RadioOption& addRequirement(std::function<bool()> func) {
		m_RequirementCallback = func;
		return *this;
	}

	RadioOption& addOnUpdate(std::function<void(RadioOption*)> update) {
		m_OnUpdateCallback = update;
		return *this;
	}

	RadioOption& addOnHover(std::function<void()> func) {
		m_OnHoverCallback = func;
		return *this;
	}

	RadioOption& addTooltip(std::string tip) {
		m_Tooltip.Set(tip.c_str());
		return *this;
	}

	void UpdateSelected() {
		m_Context->m_Toggles[m_Index] = true;
		m_Context->m_Toggles[m_Context->m_Selected] = false;
		m_Context->m_Selected = m_ID;
	}

	bool isSelected() {
		return m_Context->m_Selected == m_ID;
	}

	void Render(int pos);
	void RenderSelected(int pos, std::stack<std::string> MenuStack);
	void InvokeSave(std::stack<std::string> MenuStack) {}
	void InvokeHotkey() {}
	void WriteTranslation(nlohmann::json& JsobObject) {}
	void ReadTranslation(nlohmann::json& JsobObject) {}
	void ResetTranslation() {}
private:
	std::function<void(RadioOption*)> m_OnUpdateCallback;
	std::function<void()> m_OnClickCallback;
	std::function<void()> m_OnHoverCallback;

	int m_ID;
	int m_Index;
	RadioContext* m_Context;
};