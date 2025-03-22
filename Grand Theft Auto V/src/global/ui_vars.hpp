#pragma once
#include "stdafx.hpp"
#include "utils/math.hpp"

struct Color {
	int R;
	int G;
	int B;
	int A = 255;

	Color()
		: R(0), G(0), B(0), A(0) {}

	Color(int Red, int Green, int Blue, int Alpha = 255)
		: R(Red), G(Green), B(Blue), A(Alpha) {}

	Color(uint32_t Hex)
		: R((Hex >> 24)), G(((Hex >> 16) & 0x00FF)), B(((Hex >> 8) & 0x0000FF)), A((Hex & 0xFF)) {}

	Color WithOpacity(int Opacity) {
		return Color(R, G, B, Opacity);
	}

	void UpdateWithoutOpacity(Color Col) {
		R = Col.R;
		G = Col.G;
		B = Col.B;
	}

	bool Matches(Color& Col) {
		return Col.R == R && Col.G == G && Col.B == B;
	}

	Math::Vector3_<float> AsFloatVec() {
		return { (float)R, (float)G, (float)B };
	}

	std::initializer_list<int> AsInitializer() {
		return std::initializer_list<int> { R, G, B, A };
	}

	static Color Red() {
		return Color(0xdb2525ff);
	}

	static Color Green() {
		return Color(0x25db28ff);
	}
};

namespace Global::UIVars {
	extern Color g_TitleTextColor;
	extern Color g_MenuHeaderTextColor;
	extern Color g_OpenToolTipColor;
	extern Color g_MainTextColor;
	extern Color g_BreakTextColor;
	extern Color g_SelectedTextColor;
	extern Color g_ToggleOnColor;
	extern Color g_ToggleOffColor;
	extern Color g_TitleHeaderColor;
	extern Color g_MenuHeaderColor;
	extern Color g_MenuFooterColor;
	extern Color g_ScrollerColor;
	extern Color g_BackgroundColor;
	extern Color g_NotificationSuccess;
	extern Color g_NotificationError;

	extern int g_TitleFont;
	extern int g_HeaderFont;
	extern int g_OptionFont;
	extern int g_BreakFont;

	extern float g_MenuX;
	extern float g_MenuY;
	extern float g_MenuWrap;

	extern bool g_MenuGlobe;
	extern bool g_RenderingColor;
	extern bool g_InsideLSC;
	extern bool g_InsideBennys;
	extern bool g_InsideWeapon;
	extern bool g_InsideCasino;
	extern bool g_InsideCasinoLuckyWheel;
	extern bool g_InsideCasinoSlotMachines;
	extern bool g_InsideCasinoBlackjack;
	extern bool g_InsideCasinoBlackjackHigh;
	extern bool g_InsideCasinoPoker;
	extern bool g_InsideCasinoPokerHigh;
	extern bool g_InsideCasinoRoulette;
	extern bool g_InsideCasinoRouletteHigh;

	extern std::string g_RenderingTooltip;

	extern int g_Test;
}