#include "misc_hud.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/timers.hpp"

using namespace MiscHudMenuVars;

namespace MiscHudMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> HUDColorLabels[] = {
		{ TranslationString("Pure White", true), 0 },
		{ TranslationString("White", true), 0 },
		{ TranslationString("Black", true), 0 },
		{ TranslationString("Grey", true), 0 },
		{ TranslationString("Grey Light", true), 0 },
		{ TranslationString("Grey Dark", true), 0 },
		{ TranslationString("Red", true), 0 },
		{ TranslationString("Red Light", true), 0 },
		{ TranslationString("Red Dark", true), 0 },
		{ TranslationString("Blue", true), 0 },
		{ TranslationString("Blue Light", true), 0 },
		{ TranslationString("Blue Dark", true), 0 },
		{ TranslationString("Yellow", true), 0 },
		{ TranslationString("Yellow Light", true), 0 },
		{ TranslationString("Yellow Dark", true), 0 },
		{ TranslationString("Orange", true), 0 },
		{ TranslationString("Orange Light", true), 0 },
		{ TranslationString("Orange Dark", true), 0 },
		{ TranslationString("Green", true), 0 },
		{ TranslationString("Green Light", true), 0 },
		{ TranslationString("Green Dark", true), 0 },
		{ TranslationString("Purple", true), 0 },
		{ TranslationString("Purple Light", true), 0 },
		{ TranslationString("Purple Dark", true), 0 },
		{ TranslationString("Pink", true), 0 },
		{ TranslationString("Radar Health", true), 0 },
		{ TranslationString("Radar Armour", true), 0 },
		{ TranslationString("Radar Damage", true), 0 },
		{ TranslationString("Player 1", true), 0 },
		{ TranslationString("Player 2", true), 0 },
		{ TranslationString("Player 3", true), 0 },
		{ TranslationString("Player 4", true), 0 },
		{ TranslationString("Player 5", true), 0 },
		{ TranslationString("Player 6", true), 0 },
		{ TranslationString("Player 7", true), 0 },
		{ TranslationString("Player 8", true), 0 },
		{ TranslationString("Player 9", true), 0 },
		{ TranslationString("Player 10", true), 0 },
		{ TranslationString("Player 11", true), 0 },
		{ TranslationString("Player 12", true), 0 },
		{ TranslationString("Player 13", true), 0 },
		{ TranslationString("Player 14", true), 0 },
		{ TranslationString("Player 15", true), 0 },
		{ TranslationString("Player 16", true), 0 },
		{ TranslationString("Player 17", true), 0 },
		{ TranslationString("Player 18", true), 0 },
		{ TranslationString("Player 19", true), 0 },
		{ TranslationString("Player 20", true), 0 },
		{ TranslationString("Player 21", true), 0 },
		{ TranslationString("Player 22", true), 0 },
		{ TranslationString("Player 23", true), 0 },
		{ TranslationString("Player 24", true), 0 },
		{ TranslationString("Player 25", true), 0 },
		{ TranslationString("Player 26", true), 0 },
		{ TranslationString("Player 27", true), 0 },
		{ TranslationString("Player 28", true), 0 },
		{ TranslationString("Player 29", true), 0 },
		{ TranslationString("Player 30", true), 0 },
		{ TranslationString("Player 31", true), 0 },
		{ TranslationString("Player 32", true), 0 },
		{ TranslationString("Simple Blip Default", true), 0 },
		{ TranslationString("Menu Blue", true), 0 },
		{ TranslationString("Menu Grey Light", true), 0 },
		{ TranslationString("Menu Blue Extra Dark", true), 0 },
		{ TranslationString("Menu Yellow", true), 0 },
		{ TranslationString("Menu Yellow Dark", true), 0 },
		{ TranslationString("Menu Green", true), 0 },
		{ TranslationString("Menu Grey", true), 0 },
		{ TranslationString("Menu Grey Dark", true), 0 },
		{ TranslationString("Menu Highlight", true), 0 },
		{ TranslationString("Menu Standard", true), 0 },
		{ TranslationString("Menu Dimmed", true), 0 },
		{ TranslationString("Menu Extra Dimmed", true), 0 },
		{ TranslationString("Brief Title", true), 0 },
		{ TranslationString("Mid Grey MP", true), 0 },
		{ TranslationString("Player 1 Dark", true), 0 },
		{ TranslationString("Player 2 Dark", true), 0 },
		{ TranslationString("Player 3 Dark", true), 0 },
		{ TranslationString("Player 4 Dark", true), 0 },
		{ TranslationString("Player 5 Dark", true), 0 },
		{ TranslationString("Player 6 Dark", true), 0 },
		{ TranslationString("Player 7 Dark", true), 0 },
		{ TranslationString("Player 8 Dark", true), 0 },
		{ TranslationString("Player 9 Dark", true), 0 },
		{ TranslationString("Player 10 Dark", true), 0 },
		{ TranslationString("Player 11 Dark", true), 0 },
		{ TranslationString("Player 12 Dark", true), 0 },
		{ TranslationString("Player 13 Dark", true), 0 },
		{ TranslationString("Player 14 Dark", true), 0 },
		{ TranslationString("Player 15 Dark", true), 0 },
		{ TranslationString("Player 16 Dark", true), 0 },
		{ TranslationString("Player 17 Dark", true), 0 },
		{ TranslationString("Player 18 Dark", true), 0 },
		{ TranslationString("Player 19 Dark", true), 0 },
		{ TranslationString("Player 20 Dark", true), 0 },
		{ TranslationString("Player 21 Dark", true), 0 },
		{ TranslationString("Player 22 Dark", true), 0 },
		{ TranslationString("Player 23 Dark", true), 0 },
		{ TranslationString("Player 24 Dark", true), 0 },
		{ TranslationString("Player 25 Dark", true), 0 },
		{ TranslationString("Player 26 Dark", true), 0 },
		{ TranslationString("Player 27 Dark", true), 0 },
		{ TranslationString("Player 28 Dark", true), 0 },
		{ TranslationString("Player 29 Dark", true), 0 },
		{ TranslationString("Player 30 Dark", true), 0 },
		{ TranslationString("Player 31 Dark", true), 0 },
		{ TranslationString("Player 32 Dark", true), 0 },
		{ TranslationString("Bronze", true), 0 },
		{ TranslationString("Silver", true), 0 },
		{ TranslationString("Gold", true), 0 },
		{ TranslationString("Platinum", true), 0 },
		{ TranslationString("Gang 1", true), 0 },
		{ TranslationString("Gang 2", true), 0 },
		{ TranslationString("Gang 3", true), 0 },
		{ TranslationString("Gang 4", true), 0 },
		{ TranslationString("Same Crew", true), 0 },
		{ TranslationString("Freemode", true), 0 },
		{ TranslationString("Pause BG", true), 0 },
		{ TranslationString("Friendly", true), 0 },
		{ TranslationString("Enemy", true), 0 },
		{ TranslationString("Location", true), 0 },
		{ TranslationString("Pickup", true), 0 },
		{ TranslationString("Pause Singleplayer", true), 0 },
		{ TranslationString("Freemode Dark", true), 0 },
		{ TranslationString("Inactive Mission", true), 0 },
		{ TranslationString("Damage", true), 0 },
		{ TranslationString("Pink Light", true), 0 },
		{ TranslationString("PM Item Highlight", true), 0 },
		{ TranslationString("Script Variable", true), 0 },
		{ TranslationString("Yoga", true), 0 },
		{ TranslationString("Tennis", true), 0 },
		{ TranslationString("Golf", true), 0 },
		{ TranslationString("Shooting Range", true), 0 },
		{ TranslationString("Flight School", true), 0 },
		{ TranslationString("North Blue", true), 0 },
		{ TranslationString("Social Club", true), 0 },
		{ TranslationString("Platform Blue", true), 0 },
		{ TranslationString("Platform Green", true), 0 },
		{ TranslationString("Platform Grey", true), 0 },
		{ TranslationString("Facebook Blue", true), 0 },
		{ TranslationString("In-Game BG", true), 0 },
		{ TranslationString("Darts", true), 0 },
		{ TranslationString("Waypoint", true), 0 },
		{ TranslationString("Michael", true), 0 },
		{ TranslationString("Franklin", true), 0 },
		{ TranslationString("Trevor", true), 0 },
		{ TranslationString("Golf P1", true), 0 },
		{ TranslationString("Golf P2", true), 0 },
		{ TranslationString("Golf P3", true), 0 },
		{ TranslationString("Golf P4", true), 0 },
		{ TranslationString("Waypoint Light", true), 0 },
		{ TranslationString("Waypoint Dark", true), 0 },
		{ TranslationString("Panel Light", true), 0 },
		{ TranslationString("Michael Dark", true), 0 },
		{ TranslationString("Franklin Dark", true), 0 },
		{ TranslationString("Trevor Dark", true), 0 },
		{ TranslationString("Objective Route", true), 0 },
		{ TranslationString("Pause Map Tint", true), 0 },
		{ TranslationString("Pause Deselect", true), 0 },
		{ TranslationString("PM Weapons Purchasable", true), 0 },
		{ TranslationString("PM Weapons Locked", true), 0 },
		{ TranslationString("End Screen BG", true), 0 },
		{ TranslationString("Chop", true), 0 },
		{ TranslationString("Pausemap Tint Half", true), 0 },
		{ TranslationString("North Blue Official", true), 0 },
		{ TranslationString("Script Variable 2", true), 0 },
		{ TranslationString("H", true), 0 },
		{ TranslationString("H Dark", true), 0 },
		{ TranslationString("T", true), 0 },
		{ TranslationString("T Dark", true), 0 },
		{ TranslationString("HS Hard", true), 0 },
		{ TranslationString("Controller Michael", true), 0 },
		{ TranslationString("Controller Franklin", true), 0 },
		{ TranslationString("Controller Trevor", true), 0 },
		{ TranslationString("Controller Chop", true), 0 },
		{ TranslationString("Video Editor Video", true), 0 },
		{ TranslationString("Video Editor Audio", true), 0 },
		{ TranslationString("Video Editor Text", true), 0 },
		{ TranslationString("HB Blue", true), 0 },
		{ TranslationString("HB Yellow", true), 0 },
		{ TranslationString("Video Editor Score", true), 0 },
		{ TranslationString("Video Editor Audio Fadeout", true), 0 },
		{ TranslationString("Video Editor Text Fadeout", true), 0 },
		{ TranslationString("Video Editor Score Fadeout", true), 0 },
		{ TranslationString("Heist Background", true), 0 },
		{ TranslationString("Video Editor Ambient", true), 0 },
		{ TranslationString("Video Editor Ambient Fadeout", true), 0 },
		{ TranslationString("GB", true), 0 },
		{ TranslationString("G", true), 0 },
		{ TranslationString("B", true), 0 },
		{ TranslationString("Low Flow", true), 0 },
		{ TranslationString("Low Flow Dark", true), 0 },
		{ TranslationString("G1", true), 0 },
		{ TranslationString("G2", true), 0 },
		{ TranslationString("G3", true), 0 },
		{ TranslationString("G4", true), 0 },
		{ TranslationString("G5", true), 0 },
		{ TranslationString("G6", true), 0 },
		{ TranslationString("G7", true), 0 },
		{ TranslationString("G8", true), 0 },
		{ TranslationString("G9", true), 0 },
		{ TranslationString("G10", true), 0 },
		{ TranslationString("G11", true), 0 },
		{ TranslationString("G12", true), 0 },
		{ TranslationString("G13", true), 0 },
		{ TranslationString("G14", true), 0 },
		{ TranslationString("G15", true), 0 },
		{ TranslationString("Adversary", true), 0 },
		{ TranslationString("Degen Red", true), 0 },
		{ TranslationString("Degen Yellow", true), 0 },
		{ TranslationString("Degen Green", true), 0 },
		{ TranslationString("Degen Cyan", true), 0 },
		{ TranslationString("Degen Blue", true), 0 },
		{ TranslationString("Degen Magenta", true), 0 },
		{ TranslationString("Stunt 1", true), 0 },
		{ TranslationString("Stunt 2", true), 0 }
	};

	void SetColor(MiscHudMenu* pThis) {
		Native::_SetHudColour(m_Vars.m_Hud, m_Vars.m_Color.R, m_Vars.m_Color.G, m_Vars.m_Color.B, m_Vars.m_Color.A);

		// Save color
		Utils::GetConfig()->WriteColor(pThis->m_ParentNameStack, HUDColorLabels[m_Vars.m_Hud].m_Name.GetOriginal(), m_Vars.m_Color);
	}

	void RefreshList(MiscHudMenu* pThis) {
		pThis->ClearOptionsOffset(1);

		Native::GetHudColour(m_Vars.m_Hud, &m_Vars.m_Color.R, &m_Vars.m_Color.G, &m_Vars.m_Color.B, &m_Vars.m_Color.A);

		pThis->addOption(NumberOption<int>(SCROLL, pThis->getString("[R]ed"))
			.addNumber(m_Vars.m_Color.R, "%i", 1).addMin(0).addMax(255).canLoop().showMax()
			.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_Color); })
			.addOnClick([=] { SetColor(pThis); }));

		pThis->addOption(NumberOption<int>(SCROLL, pThis->getString("[G]reen"))
			.addNumber(m_Vars.m_Color.G, "%i", 1).addMin(0).addMax(255).canLoop().showMax()
			.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_Color); })
			.addOnClick([=] { SetColor(pThis); }));

		pThis->addOption(NumberOption<int>(SCROLL, pThis->getString("[B]lue"))
			.addNumber(m_Vars.m_Color.B, "%i", 1).addMin(0).addMax(255).canLoop().showMax()
			.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_Color); })
			.addOnClick([=] { SetColor(pThis); }));

		pThis->addOption(NumberOption<int>(SCROLL, pThis->getString("Opacity"))
			.addNumber(m_Vars.m_Color.A, "%i", 1).addMin(0).addMax(255).canLoop().showMax()
			.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_Color); })
			.addOnClick([=] { SetColor(pThis); }));
	}
}

void MiscHudMenu::Init() {
	SetName("HUD");
	SetParentSubmenu<MiscMenu>();

	addOption(ScrollOption<int>(SCROLL, "HUD")
		.addTranslation()
		.addScroll(m_Vars.m_Hud, 0, NUMOF(HUDColorLabels), HUDColorLabels)
		.addOnClick([this] { RefreshList(this); }));

	addString("[R]ed");
	addString("[G]reen");
	addString("[B]lue");
	addString("Opacity");

	// Load colors
	for (int i = 0; i < NUMOF(HUDColorLabels); i++) {
		Color Color;
		if (Utils::GetConfig()->ReadColor(m_ParentNameStack, HUDColorLabels[i].m_Name.GetOriginal(), &Color)) {
			Native::_SetHudColour(m_Vars.m_Hud, Color.R, Color.G, Color.B, Color.A);
		}
	}
}

void MiscHudMenu::Update() {}

/*Called once on submenu open*/
void MiscHudMenu::UpdateOnce() {
	RefreshList(this);
}

/*Called always*/
void MiscHudMenu::FeatureUpdate() {}

MiscHudMenu* _instance;
MiscHudMenu* MiscHudMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscHudMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscHudMenu::~MiscHudMenu() { delete _instance; }