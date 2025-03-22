#include "menu_input.hpp"
#include "menu/base/base.hpp"
#include "utils/fibers.hpp"
#include "menu/base/util/input.hpp"
#include "menu/base/renderer.hpp"
#include "menu/base/util/hotkeys.hpp"

namespace Menu::Util {
	TranslationString T_EnterToAdd("enter to add, escape to cancel", true, true);

	Color MenuInput::HSVToRGB(float H, float S, float V, int A) {
		float r = 0, g = 0, b = 0;

		if (S == 0) {
			r = V;
			g = V;
			b = V;
		} else {
			int i;
			double f, p, q, t;

			if (H == 360) H = 0;
			else H = H / 60;

			i = H > 0 ? floor(H) : ceil(H);
			f = H - i;

			p = V * (1.0f - S);
			q = V * (1.0f - (S * f));
			t = V * (1.0f - (S * (1.0f - f)));

			switch (i) {
				case 0: r = V; g = t; b = p; break;
				case 1: r = q; g = V; b = p; break;
				case 2: r = p; g = V; b = t; break;
				case 3: r = p; g = q; b = V; break;
				case 4: r = t; g = p; b = V; break;
				default: r = V; g = p; b = q; break;
			}

		}

		return Color((int)(r * 255), (int)(g * 255), (int)(b * 255), A);
	}

	bool MenuInput::Col(std::string OptionName, Color Col) {
		return true;
	}

	void MenuInput::Hotkey(std::string OptionName, Option* Option) {
		char LocalName[0x80];
		strcpy(LocalName, OptionName.c_str());

		bool Exit = false;
		bool Accepted = false;
		int InputKey = Option->HasHotkey() && Option->GetHotkey() != -1 ? Option->GetHotkey() : 0;

		while (!Exit) {
			Utils::GetFiberManager()->GoToMainFiber();
			Menu::GetBase()->m_DisableInputThisFrame = true;

			float W = 0.16f;
			float H = 0.12f;
			float X = 0.5f - (W * 0.5f);
			float Y = 0.5f - (H * 0.5f);
			float BezzelSize = 0.007875f / 2.f;
			
			Menu::GetRenderer()->DrawRect({ X, Y }, { W, BezzelSize }, Global::UIVars::g_TitleHeaderColor); // header
			Menu::GetRenderer()->DrawRect({ X, Y + BezzelSize }, { W, H }, { 0, 0, 0, 180 }); // bg

			Menu::GetRenderer()->DrawText(LocalName, { X + ( W * 0.5f), Y + BezzelSize + (H * 0.15f) }, 0.315f, 0, Color(255, 255, 255), JUSTIFY_CENTER); // option name
			Menu::GetRenderer()->DrawRect({ X + ((W * 0.1f) * 0.5f), Y + BezzelSize + ((H * 0.7f) * 0.5f) + ((H * 0.2f) * 0.5f) }, { W * 0.9f, H * 0.3f }, { 40, 40, 40, 200 }); // input box

			Menu::GetRenderer()->DrawText(TRANSLATE(T_EnterToAdd), { X + (W * 0.5f), Y + BezzelSize + (H * 0.8f) }, 0.25f, 0, Color(255, 255, 255), JUSTIFY_CENTER); // help test

			int LastKey = Menu::Util::GetInput()->m_LastKey;
			if (LastKey != VK_RETURN && LastKey != VK_ESCAPE && LastKey != VK_BACK && LastKey != VK_F4) {
				InputKey = LastKey;
			}

			if (InputKey != VK_F12) {
				Menu::GetRenderer()->DrawText(g_KeyNames[InputKey], { X + (W * 0.5f), Y + BezzelSize + ((H * 0.7f) * 0.5f) + ((H * 0.3f) * 0.5f) }, 0.35f, 0, Color(255, 255, 255), JUSTIFY_CENTER);
			}

			if (Menu::Util::GetInput()->IsAJustPressed(true)) {
				Accepted = true;
				Exit = true;
			} else if (Menu::Util::GetInput()->IsJustReleased(true, VK_ESCAPE, true)) {
				Accepted = false;
				Exit = true;
			}
		}

		if (Accepted && InputKey && InputKey != VK_F12) {
			Menu::Util::GetHotkey()->UnregisterHotkey(Option);
			Option->setHotkey(InputKey);
			Menu::Util::GetHotkey()->RegisterHotkey(InputKey, Option);
		}
	}

	void MenuInput::Update() {
		if (!Menu::GetBase()->m_Open) return;
		for (auto& Function : m_FunctionQueue) Function();
		m_FunctionQueue.clear();
	}

	void MenuInput::Push(std::function<void()> Function) {
		m_FunctionQueue.push_back(Function);
	}

	MenuInput* GetMenuInput() {
		static MenuInput Instance;
		return &Instance;
	}
}