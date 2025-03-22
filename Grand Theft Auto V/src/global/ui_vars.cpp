#include "ui_vars.hpp"

namespace Global::UIVars {
	Color g_TitleTextColor = Color(255, 255, 255, 255);
	Color g_MenuHeaderTextColor = Color(255, 255, 255, 255);
	Color g_OpenToolTipColor = Color(0x52003dff);
	Color g_MainTextColor = Color(255, 255, 255, 255);
	Color g_BreakTextColor = Color(255, 255, 255, 255);
	Color g_SelectedTextColor = Color(0, 0, 0, 255);
	Color g_ToggleOnColor = Color(130, 214, 157, 255);
	Color g_ToggleOffColor = Color(200, 55, 80, 255);
	Color g_TitleHeaderColor = Color(0x52003dff);
	Color g_MenuHeaderColor = Color(0, 0, 0, 235);
	Color g_MenuFooterColor = Color(0, 0, 0, 210);
	Color g_ScrollerColor = Color(255, 255, 255, 255);
	Color g_BackgroundColor = Color(0, 0, 0, 255);
	Color g_NotificationSuccess = Color(0x00785cff);
	Color g_NotificationError = Color(0x780000ff);

	int g_TitleFont = 1;
	int g_HeaderFont = 0;
	int g_OptionFont = 0;
	int g_BreakFont = 0;

	float g_MenuX = 0.70f;
	float g_MenuY = 0.327f;
	float g_MenuWrap = 0.068f;

	bool g_MenuGlobe = true;
	bool g_RenderingColor = false;
	bool g_InsideLSC = false;
	bool g_InsideBennys = false;
	bool g_InsideWeapon = false;
	bool g_InsideCasino = false;
	bool g_InsideCasinoLuckyWheel = false;
	bool g_InsideCasinoSlotMachines = false;
	bool g_InsideCasinoBlackjack = false;
	bool g_InsideCasinoBlackjackHigh = false;
	bool g_InsideCasinoPoker = false;
	bool g_InsideCasinoPokerHigh = false;
	bool g_InsideCasinoRoulette = false;
	bool g_InsideCasinoRouletteHigh = false;

	std::string g_RenderingTooltip;

	int g_Test = 0;
}