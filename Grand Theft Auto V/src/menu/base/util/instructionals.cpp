#include "instructionals.hpp"
#include "rage/invoker/natives.hpp"
#include "global/ui_vars.hpp"
#include "menu/base/util/global.hpp"

namespace Instructionals {
	TranslationString T_OpenCategory("Open Category", true, true);
	TranslationString T_CloseMenu("Close Menu", true, true);
	TranslationString T_Scroll("Scroll", true, true);
	TranslationString T_Select("Select", true, true);
	TranslationString T_Back("Back", true, true);
	TranslationString T_RainbowConfig("Rainbow Config", true, true);
	TranslationString T_Adjust("Adjust", true, true);
	TranslationString T_Input("Input", true, true);
	TranslationString T_AddHotkey("Add Hotkey", true, true);

	static const char* g_KeyNamesInstructional[254] = {
		"F10",
		"F10",
		"F10",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"BACKSPACE",
		"TAB",
		"TAB",
		"TAB",
		"NUM 5",
		"ENTER",
		"ENTER",
		"ENTER",
		"SHIFT",
		"CTRL",
		"ALT",
		"ALT",
		"CAPSLOCK",
		"CAPSLOCK",
		"CAPSLOCK",
		"CAPSLOCK",
		"CAPSLOCK",
		"CAPSLOCK",
		"CAPSLOCK",
		"ESC",
		"ESC",
		"ESC",
		"ESC",
		"ESC",
		"SPACE",
		"NUM 9",
		"NUM 3",
		"NUM 1",
		"NUM 7",
		"LEFT",
		"UP",
		"RIGHT",
		"DOWN",
		"NUM 2",
		"NUM 2",
		"NUM 2",
		"NUM 2",
		"NUM 0",
		"DELETE",
		"NUM DECIMAL",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"9",
		"9",
		"9",
		"9",
		"9",
		"9",
		"9",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"Z",
		"Z",
		"Z",
		"Z",
		"Z",
		"N0",
		"N1",
		"N2",
		"N3",
		"N4",
		"N5",
		"N6",
		"N7",
		"N8",
		"N9",
		"*",
		"NUM PLUS",
		"NUM PLUS",
		"NUM SUB",
		"NUM DECIMAL",
		"/",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"F12",
		"Pause",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SCROLL LOCK",
		"SHIFT",
		"RIGHT SHIFT",
		"CTRL",
		"CTRL",
		"ALT",
		"ALT",
		"ALT",
		"ALT",
		"ALT",
		"ALT",
		"ALT",
		"ALT",
		"M",
		"D",
		"C",
		"B",
		"P",
		"Q",
		"J",
		"G",
		"G",
		"G",
		"G",
		"F",
		"F",
		"F",
		";",
		"=",
		",",
		"-",
		".",
		"/",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"GRAVE",
		"[",
		"null",
		"]",
		"ACUTE/CEDILLA",
		"ACUTE/CEDILLA",
		"ACUTE/CEDILLA",
		"ACUTE/CEDILLA",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null",
		"null"
	};

	void Instructionals::Setup() {
		if (!Native::HasScaleformMovieLoaded(m_Handle)) {
			m_Handle = Native::RequestScaleformMovie("instructional_buttons");
			return;
		}

		Native::_PushScaleformMovieFunction(m_Handle, "CLEAR_ALL");
		Native::_PopScaleformMovieFunctionVoid();

		Native::_PushScaleformMovieFunction(m_Handle, "TOGGLE_MOUSE_BUTTONS");
		Native::_PushScaleformMovieFunctionParameterBool(1);
		Native::_PopScaleformMovieFunctionVoid();

		Native::_PushScaleformMovieFunction(m_Handle, "SET_MAX_WIDTH");
		Native::_PushScaleformMovieFunctionParameterFloat(1.f);
		Native::_PopScaleformMovieFunctionVoid();
	}

	void Instructionals::AddInstructional(std::string Text, eControls Control) {
		Native::_PushScaleformMovieFunction(m_Handle, "SET_DATA_SLOT");
		Native::_PushScaleformMovieFunctionParameterInt(m_InstructionalCount++);

		Native::_0xE83A3E3557A56640(Native::GetControlInstructionalButton(0, Control, true)); // problem!

		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(Text.c_str());
		Native::EndTextCommandScaleformString();

		Native::_PushScaleformMovieFunctionParameterBool(true);
		Native::_PushScaleformMovieFunctionParameterInt(Control);

		Native::_PopScaleformMovieFunctionVoid();
	}

	void Instructionals::AddInstructional(std::string Text, eScaleformButtons Button) {
		Native::_PushScaleformMovieFunction(m_Handle, "SET_DATA_SLOT");
		Native::_PushScaleformMovieFunctionParameterInt(m_InstructionalCount++);

		Native::_PushScaleformMovieFunctionParameterInt(Button);

		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(Text.c_str());
		Native::EndTextCommandScaleformString();

		Native::_PushScaleformMovieFunctionParameterBool(true);
		Native::_PushScaleformMovieFunctionParameterInt(Button);

		Native::_PopScaleformMovieFunctionVoid();
	}

	void Instructionals::AddInstructional(std::string Text, int VK) {
		Native::_PushScaleformMovieFunction(m_Handle, "SET_DATA_SLOT");
		Native::_PushScaleformMovieFunctionParameterInt(m_InstructionalCount++);

		Native::_0xE83A3E3557A56640(("t_" + (std::string)g_KeyNamesInstructional[VK]).c_str());
		Native::_PushScaleformMovieFunctionParameterString(Text.c_str());
		Native::_PushScaleformMovieFunctionParameterBool(true);
		Native::_PushScaleformMovieFunctionParameterInt(VK);

		Native::_PopScaleformMovieFunctionVoid();
	}

	void Instructionals::Close() {
		Native::_PushScaleformMovieFunction(m_Handle, "SET_BACKGROUND_COLOUR");
		Native::_PushScaleformMovieFunctionParameterInt(Global::UIVars::g_BackgroundColor.R);
		Native::_PushScaleformMovieFunctionParameterInt(Global::UIVars::g_BackgroundColor.G);
		Native::_PushScaleformMovieFunctionParameterInt(Global::UIVars::g_BackgroundColor.B);
		Native::_PushScaleformMovieFunctionParameterInt(80);
		Native::_PopScaleformMovieFunctionVoid();

		Native::_PushScaleformMovieFunction(m_Handle, "DRAW_INSTRUCTIONAL_BUTTONS");
		Native::_PushScaleformMovieFunctionParameterInt(0);
		Native::_PopScaleformMovieFunctionVoid();

		Native::DrawScaleformMovieFullscreen(m_Handle, 255, 255, 255, 255, 0);
		m_InstructionalCount = 0;
	}

	void MainMenu() {
		GetInstructionals()->Setup();
		GetInstructionals()->AddInstructional(TRANSLATE(T_OpenCategory), ControlFrontendAccept);
		GetInstructionals()->AddInstructional(TRANSLATE(T_CloseMenu), ControlFrontendCancel);
		GetInstructionals()->AddInstructional(TRANSLATE(T_Scroll), ARROW_UP_DOWN);
		GetInstructionals()->Close();
	}

	void Standard(bool Scroller, bool Keyboard, bool Hotkey) {
		GetInstructionals()->Setup();
		GetInstructionals()->AddInstructional(TRANSLATE(T_Select), ControlFrontendAccept);
		GetInstructionals()->AddInstructional(TRANSLATE(T_Back), ControlFrontendCancel);
		GetInstructionals()->AddInstructional(TRANSLATE(T_Scroll), ARROW_UP_DOWN);
		
		if (Scroller) GetInstructionals()->AddInstructional(TRANSLATE(T_Adjust), ARROW_LEFT_RIGHT);
		if (Keyboard) GetInstructionals()->AddInstructional(TRANSLATE(T_Input), ControlPhoneSelect);
		if (Hotkey) GetInstructionals()->AddInstructional(TRANSLATE(T_AddHotkey), VK_F12);

		GetInstructionals()->Close();
	}

	void Rainbow() {
		GetInstructionals()->Setup();
		GetInstructionals()->AddInstructional(TRANSLATE(T_Select), ControlFrontendAccept);
		GetInstructionals()->AddInstructional(TRANSLATE(T_Back), ControlFrontendCancel);
		GetInstructionals()->AddInstructional(TRANSLATE(T_Scroll), ARROW_UP_DOWN);
		GetInstructionals()->AddInstructional(TRANSLATE(T_RainbowConfig), ControlFrontendX);
		GetInstructionals()->Close();
	}

	void Drone() {
		GetInstructionals()->Setup();
		GetInstructionals()->Close();
	}

	Instructionals* GetInstructionals() {
		static Instructionals Instance;
		return &Instance;
	}
}