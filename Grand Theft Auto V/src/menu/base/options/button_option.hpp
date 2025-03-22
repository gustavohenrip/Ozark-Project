#pragma once
#include "stdafx.hpp"
#include "option.hpp"
#include "menu/base/base.hpp"
#include "rage/types/base_types.hpp"

class ButtonOption : public Option {
public:
	ButtonOption(std::string name)
		: Option(name),
		m_HasSprite(false),
		m_HasKeyboard(false),
		m_IsKeyboardActive(false),
		m_SpriteRotate(false),
		m_HasSideText(false),
		m_KeyboardMaxChars(0),
		m_KeyboardTitle(nullptr),
		m_SpriteRotation(0.f),
		m_Sprite({ "", "" }),
		m_SpriteScale({ 0.02112f, 0.03744f }),
		m_OnUpdateCallback([](ButtonOption*) {}),
		m_OnClickCallback([]() {}),
		m_OnHoverCallback([]() {}),
		m_OnKeyboardCallback([](const char*) {}),
		m_OnKeyboardCallbackThis([] (ButtonOption*, const char*) {}),
		m_OnSpriteRenderRequirement([] { return true; })
	{}

	ButtonOption& addOnClick(std::function<void()> func) {
		m_OnClickCallback = func;
		return *this;
	}

	ButtonOption& addRequirement(std::function<bool()> func) {
		m_RequirementCallback = func;
		return *this;
	}

	ButtonOption& addOnUpdate(std::function<void(ButtonOption*)> func) {
		m_OnUpdateCallback = func;
		return *this;
	}

	ButtonOption& addTooltip(std::string tip) {
		m_Tooltip.Set(tip.c_str());
		return *this;
	}

	ButtonOption& addKeyboard(const char* title, int maxChars, std::function<void(const char*)> callback) {
		m_HasKeyboard = true;
		m_KeyboardMaxChars = maxChars;
		m_OnKeyboardCallback = callback;
		m_KeyboardTitle = title;
		return *this;
	}

	ButtonOption& addOnHover(std::function<void()> func) {
		m_OnHoverCallback = func;
		return *this;
	}

	ButtonOption& addTranslation() {
		m_Name.SetHasTranslation(true);
		m_Tooltip.SetHasTranslation(true);
		return *this;
	}

	ButtonOption& removeSprite() {
		m_HasSprite = false;
		return *this;
	}

	ButtonOption& addSprite(const char* Dict, const char* Name, std::function<bool()> Requirement = [] { return true; }) {
		m_HasSprite = true;
		m_Sprite = { Dict, Name };
		m_OnSpriteRenderRequirement = Requirement;
		return *this;
	}

	ButtonOption& addSpriteScale(float X, float Y) {
		m_SpriteScale = { X, Y };
		return *this;
	}

	ButtonOption& addSpriteRotate() {
		m_SpriteRotate = true;
		return *this;
	}

	ButtonOption& addInstructional(std::string Text, eScaleformButtons Button) {
		m_Instructionals.push_back({ Text, Button, false });
		return *this;
	}

	ButtonOption& addInstructional(std::string Text, eControls Button) {
		m_Instructionals.push_back({ Text, Button, true });
		return *this;
	}

	ButtonOption& addSideText(std::string Text) {
		m_SideText = Text;
		m_HasSideText = true;
		return *this;
	}

	ButtonOption& addHotkey() {
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
	std::function<void(ButtonOption*)> m_OnUpdateCallback;
	std::function<void()> m_OnClickCallback;
	std::function<void()> m_OnHoverCallback;
	std::function<void(const char*)> m_OnKeyboardCallback;
	std::function<void(ButtonOption*, const char*)> m_OnKeyboardCallbackThis;
	std::function<bool()> m_OnSpriteRenderRequirement;

	std::pair<const char*, const char*> m_Sprite;
	Math::Vector2<float> m_SpriteScale;

	bool m_HasSprite;
	bool m_HasKeyboard;
	bool m_IsKeyboardActive;
	bool m_SpriteRotate;
	bool m_HasSideText;

	int m_KeyboardMaxChars;
	const char* m_KeyboardTitle;
	float m_SpriteRotation;
	std::string m_SideText;
};