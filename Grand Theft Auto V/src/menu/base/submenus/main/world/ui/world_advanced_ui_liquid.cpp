#include "world_advanced_ui_liquid.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"

using namespace WorldAdvancedUILiquidMenuVars;

namespace WorldAdvancedUILiquidMenuVars {
	Vars_ m_Vars;

	ScrollStruct<eLiquid> Liquid[] = {
		{ TranslationString("Puddle", true), LIQUID_PUDDLE },
		{ TranslationString("Blood", true), LIQUID_BLOOD },
		{ TranslationString("Oil", true), LIQUID_OIL },
		{ TranslationString("Petrol", true), LIQUID_PETROL },
		{ TranslationString("Mud", true), LIQUID_MUD }
	};

	void Save() {
		for (int i = 0; i < NUMOF(Liquid); i++) {
			auto& L = Liquid[i];

			Color C;
			C.R = (int)Global::Vars::g_VfxLiquid[i].m_Red;
			C.G = (int)Global::Vars::g_VfxLiquid[i].m_Green;
			C.B = (int)Global::Vars::g_VfxLiquid[i].m_Blue;
			C.A = (int)Global::Vars::g_VfxLiquid[i].m_Alpha;

			Utils::GetConfig()->WriteColor(WorldAdvancedUILiquidMenu::GetInstance()->m_ParentNameStack, "Color", C, { Liquid[i].m_Name.GetOriginal() });
		}
	}

	void Load() {
		for (int i = 0; i < NUMOF(Liquid); i++) {
			auto& L = Liquid[i];

			Color C;
			if (Utils::GetConfig()->ReadColor(WorldAdvancedUILiquidMenu::GetInstance()->m_ParentNameStack, "Color", &C, { Liquid[i].m_Name.GetOriginal() })) {
				Global::Vars::g_VfxLiquid[i].m_Red = (uint8_t)C.R;
				Global::Vars::g_VfxLiquid[i].m_Green = (uint8_t)C.G;
				Global::Vars::g_VfxLiquid[i].m_Blue = (uint8_t)C.B;
				Global::Vars::g_VfxLiquid[i].m_Alpha = (uint8_t)C.A;
			}
		}
	}
}

void WorldAdvancedUILiquidMenu::Init() {
	SetName("UI: Liquid");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	memcpy(m_Vars.m_Backup, Global::Vars::g_VfxLiquid, sizeof(m_Vars.m_Backup));

	Load();

	addOption(ScrollOption<eLiquid>(SCROLL, "Liquid Type")
		.addScroll(m_Vars.m_Selected, 0, NUMOF(Liquid), Liquid)
		.addOnClick([this] { UpdateOnce(); }));

	addString("[R]ed");
	addString("[G]reen");
	addString("[B]lue");
	addString("Opacity");
}

void WorldAdvancedUILiquidMenu::Update() {}

/*Called once on submenu open*/
void WorldAdvancedUILiquidMenu::UpdateOnce() {
	ClearOptionsOffset(1);

	addOption(NumberOption<uint8_t>(SCROLL, getString("[R]ed"))
		.addNumber(Global::Vars::g_VfxLiquid[m_Vars.m_Selected].m_Red, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5)
		.addOnClick(Save));

	addOption(NumberOption<uint8_t>(SCROLL, getString("[G]reen"))
		.addNumber(Global::Vars::g_VfxLiquid[m_Vars.m_Selected].m_Green, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5)
		.addOnClick(Save));

	addOption(NumberOption<uint8_t>(SCROLL, getString("[B]lue"))
		.addNumber(Global::Vars::g_VfxLiquid[m_Vars.m_Selected].m_Blue, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5)
		.addOnClick(Save));

	addOption(NumberOption<uint8_t>(SCROLL, getString("Opacity"))
		.addNumber(Global::Vars::g_VfxLiquid[m_Vars.m_Selected].m_Alpha, "%i", 1).addMin(0).addMax(255).canLoop().setScrollSpeed(5)
		.addOnClick(Save));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([] {
			memcpy(Global::Vars::g_VfxLiquid, &m_Vars.m_Backup, sizeof(m_Vars.m_Backup));
			Save();
		}));
}

/*Called always*/
void WorldAdvancedUILiquidMenu::FeatureUpdate() {}

WorldAdvancedUILiquidMenu* _instance;
WorldAdvancedUILiquidMenu* WorldAdvancedUILiquidMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUILiquidMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUILiquidMenu::~WorldAdvancedUILiquidMenu() { delete _instance; }