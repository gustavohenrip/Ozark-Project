#include "world_advanced_ui_sky.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"
#include "utils/memory/memory.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "menu/base/util/instructionals.hpp"
#include "menu/base/submenus/main/helpers/rainbow_config.hpp"
#include "utils/log.hpp"

using namespace WorldAdvancedUISkyMenuVars;

namespace WorldAdvancedUISkyMenuVars {
	Vars_ m_Vars;

	ScrollStruct<SkyColor*> SkyColors[] = {
		{ TranslationString("Azimuth East Color", true), &m_Vars.m_AzimuthEastColor },
		{ TranslationString("Azimuth West Color", true), &m_Vars.m_AzimuthWestColor },
		{ TranslationString("Azimuth Transition Color", true), &m_Vars.m_AzimuthTransitionColor },
		{ TranslationString("Cloud Base Minus Mid Color", true), &m_Vars.m_CloudBaseMinusMidColour },
		{ TranslationString("Cloud Mid Color", true), &m_Vars.m_CloudMidColor },
		{ TranslationString("Cloud Shadow Base Color", true), &m_Vars.m_CloudShadowMinusBaseColourTimesShadowStrength },
		{ TranslationString("Moon Color", true), &m_Vars.m_MoonColor },
		{ TranslationString("Sky Plane Color", true), &m_Vars.m_SkyPlaneColor },
		{ TranslationString("Sun Color", true), &m_Vars.m_SunColor },
		{ TranslationString("Zenith Color", true), &m_Vars.m_ZenithColor },
		{ TranslationString("Zenith Transition Color", true), &m_Vars.m_ZenithTransitionColor }
	};

	std::pair<int, uint64_t> Offsets[] = {
		{ 1, 0x39 },  // [1] 0x39
		{ 1, 0x6D },  // [1] 0x6D
		{ 1, 0xA1 },  // [1] 0xA1
		{ 2, 0x2E1 }, // [2] 0x2E1
		{ 1, 0x461 }, // [1] 0x461
		{ 2, 0x2CA }, // [2] 0x2CA
		{ 1, 0x36B }, // [1] 0x36B
		{ 1, 0x19F }, // [1] 0x19F
		{ 1, 0x261 }, // [1] 0x261
		{ 1, 0xE7 },  // [1] 0xE7
		{ 1, 0x121 }, // [1] 0x121
	};

	void HandlePatch(int Index = 0, bool Reset = false) {
		if (Reset) {
			// Just patch back the cache, the game will auto-update for us
			Memory::WriteVector(Offsets[Index].second, SkyColors[Index].m_Result->m_Cache);

			Utils::WaitFiber(1000);

			SkyColors[Index].m_Result->m_Color.m_X = SkyColors[Index].m_Result->m_MemoryColor->m_X;
			SkyColors[Index].m_Result->m_Color.m_Y = SkyColors[Index].m_Result->m_MemoryColor->m_Y;
			SkyColors[Index].m_Result->m_Color.m_Z = SkyColors[Index].m_Result->m_MemoryColor->m_Z;
			return;
		}

		if (*(uint8_t*)(Offsets[Index].second) != 0x90) {
			Memory::Nop(Offsets[Index].second, (int)SkyColors[Index].m_Result->m_Cache.size());
		}

		Math::Vector3_<float> Floats = SkyColors[Index].m_Result->m_Color;
		SkyColors[Index].m_Result->m_MemoryColor->m_X = Floats.m_X * SkyColors[Index].m_Result->m_Luminance;
		SkyColors[Index].m_Result->m_MemoryColor->m_Y = Floats.m_Y * SkyColors[Index].m_Result->m_Luminance;
		SkyColors[Index].m_Result->m_MemoryColor->m_Z = Floats.m_Z * SkyColors[Index].m_Result->m_Luminance;
	}
}

void WorldAdvancedUISkyMenu::Init() {
	SetName("UI: Sky");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	m_DefaultInstructionals = false;

	if (!IsValidPtr(Global::Vars::g_UIWeather)) return;

	for (auto& A : Offsets) {
		if (A.first == 1) {
			A.second += Global::Vars::g_UpdateUIValues1;
		} else A.second += Global::Vars::g_UpdateUIValues2;
	}

	// Initialize cache
	for (int i = 0; i < 7; i++) {
		uint64_t Sevens = Offsets[2].second;
		SkyColors[2].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[3].second;
		SkyColors[3].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[4].second;
		SkyColors[4].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[5].second;
		SkyColors[5].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[6].second;
		SkyColors[6].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[7].second;
		SkyColors[7].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[8].second;
		SkyColors[8].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[9].second;
		SkyColors[9].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));

		Sevens = Offsets[10].second;
		SkyColors[10].m_Result->m_Cache.push_back(*(uint8_t*)(Sevens + i));
	}

	for (int i = 0; i < 4; i++) {
		uint64_t Fours = Offsets[0].second;
		SkyColors[0].m_Result->m_Cache.push_back(*(uint8_t*)(Fours + i));

		Fours = Offsets[1].second;
		SkyColors[1].m_Result->m_Cache.push_back(*(uint8_t*)(Fours + i));
	}

	// Initialize color ptrs
	SkyColors[0].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_AzimuthEastColor.m_Value;
	SkyColors[1].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_AzimuthWestColor.m_Value;
	SkyColors[2].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_AzimuthTransitionColor.m_Value;
	SkyColors[3].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_CloudBaseMinusMidColour.m_Value;
	SkyColors[4].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_CloudMidColor.m_Value;
	SkyColors[5].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_CloudShadowMinusBaseColourTimesShadowStrength.m_Value;
	SkyColors[6].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_MoonColor.m_Value;
	SkyColors[7].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_SkyPlaneColor.m_Value;
	SkyColors[8].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_SunColor.m_Value;
	SkyColors[9].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_ZenithColor.m_Value;
	SkyColors[10].m_Result->m_MemoryColor = &Global::Vars::g_UIWeather->m_ZenithTransitionColor.m_Value;

	// Update the menu's color to the memory ptr colors
	for (auto& C : SkyColors) {
		if (IsValidPtr(C.m_Result)) {
			if (IsValidPtr(C.m_Result->m_MemoryColor)) {
				C.m_Result->m_InternalColor.R = (int)(C.m_Result->m_MemoryColor->m_X * 255.f);
				C.m_Result->m_InternalColor.G = (int)(C.m_Result->m_MemoryColor->m_Y * 255.f);
				C.m_Result->m_InternalColor.B = (int)(C.m_Result->m_MemoryColor->m_Z * 255.f);

				if (C.m_Result->m_InternalColor.R < 0) C.m_Result->m_InternalColor.R = 0;
				if (C.m_Result->m_InternalColor.G < 0) C.m_Result->m_InternalColor.G = 0;
				if (C.m_Result->m_InternalColor.B < 0) C.m_Result->m_InternalColor.B = 0;

				C.m_Result->m_Color.m_X = C.m_Result->m_MemoryColor->m_X;
				C.m_Result->m_Color.m_Y = C.m_Result->m_MemoryColor->m_Y;
				C.m_Result->m_Color.m_Z = C.m_Result->m_MemoryColor->m_Z;
			}
		}
	}

	for (int i = 0; i < NUMOF(SkyColors); i++) {
		auto& C = SkyColors[i];

		if (IsValidPtr(C.m_Result)) {
			C.m_Result->m_SaveToConfig = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Save", false, { C.m_Name.GetOriginal() });
			if (C.m_Result->m_SaveToConfig) {
				Utils::GetConfig()->ReadVector(m_ParentNameStack, C.m_Name.GetOriginal(), &C.m_Result->m_Color, { C.m_Name.GetOriginal() });
				C.m_Result->m_Luminance = Utils::GetConfig()->ReadFloat(m_ParentNameStack, "Luminance", 1.f, { C.m_Name.GetOriginal() });
				C.m_Result->m_RainbowToggle = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Rainbow", false, { C.m_Name.GetOriginal() });

				C.m_Result->m_Rainbow.Add(&C.m_Result->m_InternalColor);
				C.m_Result->m_Rainbow.Toggle(C.m_Result->m_RainbowToggle);

				HandlePatch(i);
			}
		}
	}

	addOption(ScrollOption<SkyColor*>(SCROLLSELECT, "Element")
		.addTranslation()
		.addScroll(m_Vars.m_Selected, 0, NUMOF(SkyColors), SkyColors));

	addOption(NumberOption<float>(SCROLL, "[R]ed")
		.addTranslation()
		.addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Color.m_X, "%.2f", 0.01f).addMin(-100.f).addMax(100.f).canLoop().setScrollSpeed(5)
		.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Color.m_X, "%.2f", 0.01f); })
		.addOnClick([] { HandlePatch(m_Vars.m_Selected); }));

	addOption(NumberOption<float>(SCROLL, "[G]reen")
		.addTranslation()
		.addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Color.m_Y, "%.2f", 0.01f).addMin(-100.f).addMax(100.f).canLoop().setScrollSpeed(5)
		.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Color.m_Y, "%.2f", 0.01f); })
		.addOnClick([] { HandlePatch(m_Vars.m_Selected); }));

	addOption(NumberOption<float>(SCROLL, "[B]lue")
		.addTranslation()
		.addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Color.m_Z, "%.2f", 0.01f).addMin(-100.f).addMax(100.f).canLoop().setScrollSpeed(5)
		.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Color.m_Z, "%.2f", 0.01f); })
		.addOnClick([] { HandlePatch(m_Vars.m_Selected); }));

	addOption(NumberOption<float>(SCROLL, "Luminance")
		.addTranslation()
		.addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Luminance, "%.1f", 0.1f).addMin(0.f).addMax(100.f)
		.addOnUpdate([] (NumberOption<float>* Option) { Option->addNumber(SkyColors[m_Vars.m_Selected].m_Result->m_Luminance, "%.1f", 0.1f); })
		.addOnClick([] { HandlePatch(m_Vars.m_Selected); }));

	addOption(ToggleOption("Rainbow")
		.addTranslation()
		.addToggle(SkyColors[m_Vars.m_Selected].m_Result->m_RainbowToggle)
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(SkyColors[m_Vars.m_Selected].m_Result->m_RainbowToggle); })
		.addOnClick([] {
			SkyColors[m_Vars.m_Selected].m_Result->m_Rainbow.Toggle(SkyColors[m_Vars.m_Selected].m_Result->m_RainbowToggle);
			if (SkyColors[m_Vars.m_Selected].m_Result->m_RainbowToggle) {
				SkyColors[m_Vars.m_Selected].m_Result->m_Rainbow.Add(&SkyColors[m_Vars.m_Selected].m_Result->m_InternalColor);
			} else {
				SkyColors[m_Vars.m_Selected].m_Result->m_Rainbow.Remove(&SkyColors[m_Vars.m_Selected].m_Result->m_InternalColor);
			}
		}));

	addOption(ToggleOption("Save to Config")
		.addTranslation()
		.addToggle(SkyColors[m_Vars.m_Selected].m_Result->m_SaveToConfig)
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(SkyColors[m_Vars.m_Selected].m_Result->m_SaveToConfig); }));

	addOption(ButtonOption("Reset to Default")
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([] {
			SkyColors[m_Vars.m_Selected].m_Result->m_RainbowToggle = false;
			SkyColors[m_Vars.m_Selected].m_Result->m_Rainbow.Stop();
			Utils::GetFiberPool()->Push([] { HandlePatch(m_Vars.m_Selected, true); });
		}));
}

void WorldAdvancedUISkyMenu::Update() {
	if (Menu::GetBase()->m_CurrentOption == 5) {
		Instructionals::Rainbow();

		if (Menu::Util::GetInput()->IsJustPressed(false, ControlFrontendX)) {
			RainbowConfigMenuVars::m_Vars.m_Rainbow = &SkyColors[m_Vars.m_Selected].m_Result->m_Rainbow;
			RainbowConfigMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUISkyMenu>();
			Menu::GetSubmenuHandler()->SetSubmenu(RainbowConfigMenu::GetInstance());
		}
	} else Instructionals::Standard();


}

/*Called once on submenu open*/
void WorldAdvancedUISkyMenu::UpdateOnce() {
	for (auto& C : SkyColors) {
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Save", C.m_Result->m_SaveToConfig, { C.m_Name.GetOriginal() });

		if (C.m_Result->m_SaveToConfig) {
			Utils::GetConfig()->WriteBool(m_ParentNameStack, "Rainbow", C.m_Result->m_RainbowToggle, { C.m_Name.GetOriginal() });
			Utils::GetConfig()->WriteVector(m_ParentNameStack, C.m_Name.GetOriginal(), C.m_Result->m_Color, { C.m_Name.GetOriginal() });
			Utils::GetConfig()->WriteFloat(m_ParentNameStack, "Luminance", C.m_Result->m_Luminance, { C.m_Name.GetOriginal() });
		}
	}
}

/*Called always*/
void WorldAdvancedUISkyMenu::FeatureUpdate() {
	for (int i = 0; i < NUMOF(SkyColors); i++) {
		auto& C = SkyColors[i];
		if (C.m_Result->m_RainbowToggle) {
			C.m_Result->m_Rainbow.Run();

			C.m_Result->m_Color.m_X = C.m_Result->m_InternalColor.R / 255.f;
			C.m_Result->m_Color.m_Y = C.m_Result->m_InternalColor.G / 255.f;
			C.m_Result->m_Color.m_Z = C.m_Result->m_InternalColor.B / 255.f;

			HandlePatch(i);
		}
	}
}

WorldAdvancedUISkyMenu* _instance;
WorldAdvancedUISkyMenu* WorldAdvancedUISkyMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUISkyMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUISkyMenu::~WorldAdvancedUISkyMenu() { delete _instance; }