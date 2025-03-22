#include "world_advanced_ui_struct.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world_advanced_ui.hpp"
#include "world_advanced_ui_color.hpp"

using namespace WorldAdvancedUIStructMenuVars;

namespace WorldAdvancedUIStructMenuVars {
	Vars_ m_Vars;

	// uint32_t m_Flags; // 0x200 = using sun, 0x140 = using dynamic shadows

	void ReadStructFromConfig(CustomColor* Ptr, std::string Section, std::string Key) {
		std::stack<std::string> Stack = WorldAdvancedUIStructMenu::GetInstance()->m_ParentNameStack;

		Ptr->m_SaveToConfig = Utils::GetConfig()->ReadBool(Stack, "Save", Ptr->m_SaveToConfig, { Key });
		if (Ptr->m_SaveToConfig) {
			Ptr->m_RainbowToggle = Utils::GetConfig()->ReadBool(Stack, "Rainbow", Ptr->m_RainbowToggle, { Key });
			Ptr->m_Color->m_Intensity = Utils::GetConfig()->ReadFloat(Stack, "Intensity", Ptr->m_Color->m_Intensity, { Key });
			Ptr->m_Color->m_Radius = Utils::GetConfig()->ReadFloat(Stack, "Radius", Ptr->m_Color->m_Radius, { Key });
			Ptr->m_Color->m_FalloffExp = Utils::GetConfig()->ReadFloat(Stack, "FalloffExp", Ptr->m_Color->m_FalloffExp, { Key });
			Ptr->m_Color->m_InnerConeAngle = Utils::GetConfig()->ReadFloat(Stack, "InnerConeAngle", Ptr->m_Color->m_InnerConeAngle, { Key });
			Ptr->m_Color->m_OuterConeAngle = Utils::GetConfig()->ReadFloat(Stack, "OuterConeAngle", Ptr->m_Color->m_OuterConeAngle, { Key });
			Ptr->m_Color->m_CoronaHDR = Utils::GetConfig()->ReadFloat(Stack, "CoronaHDR", Ptr->m_Color->m_CoronaHDR, { Key });
			Ptr->m_Color->m_CoronaSize = Utils::GetConfig()->ReadFloat(Stack, "CoronaSize", Ptr->m_Color->m_CoronaSize, { Key });
			Ptr->m_Color->m_ShadowBlur = Utils::GetConfig()->ReadFloat(Stack, "ShadowBlur", Ptr->m_Color->m_ShadowBlur, { Key });
			Ptr->m_Color->m_CapsuleLength = Utils::GetConfig()->ReadFloat(Stack, "CapsuleLength", Ptr->m_Color->m_CapsuleLength, { Key });
			Utils::GetConfig()->ReadVector(Stack, "Color", &Ptr->m_Color->m_Color, { Key });

			if (Ptr->m_RainbowToggle) {
				Ptr->m_Rainbow.Add(&Ptr->m_InternalColor);
				Ptr->m_Rainbow.Toggle(true);
			}
		}
	}

	void SaveStructToConfig(std::string Key) {
		std::stack<std::string> Stack = WorldAdvancedUIStructMenu::GetInstance()->m_ParentNameStack;

		Utils::GetConfig()->WriteBool(Stack, "Save", m_Vars.m_Color->m_SaveToConfig, { Key });
		Utils::GetConfig()->WriteBool(Stack, "Rainbow", m_Vars.m_Color->m_RainbowToggle, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "Intensity", m_Vars.m_Color->m_Color->m_Intensity, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "Radius", m_Vars.m_Color->m_Color->m_Radius, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "FalloffExp", m_Vars.m_Color->m_Color->m_FalloffExp, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "InnerConeAngle", m_Vars.m_Color->m_Color->m_InnerConeAngle, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "OuterConeAngle", m_Vars.m_Color->m_Color->m_OuterConeAngle, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "CoronaHDR", m_Vars.m_Color->m_Color->m_CoronaHDR, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "CoronaSize", m_Vars.m_Color->m_Color->m_CoronaSize, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "ShadowBlur", m_Vars.m_Color->m_Color->m_ShadowBlur, { Key });
		Utils::GetConfig()->WriteFloat(Stack, "CapsuleLength", m_Vars.m_Color->m_Color->m_CapsuleLength, { Key });
		Utils::GetConfig()->WriteVector(Stack, "Color", m_Vars.m_Color->m_Color->m_Color, { Key });
	}
}

void WorldAdvancedUIStructMenu::Init() {
	SetName("Color");
	SetParentSubmenu<WorldAdvancedUIMenu>();

	addString("Color");
	addString("Intensity");
	addString("Radius");
	addString("Falloff Exponent");
	addString("Inner Cone Angle");
	addString("Outer Cone Angle");
	addString("Corona HDR");
	addString("Corona Size");
	addString("Shadow Blur");
	addString("Capsule Length");
	addString("Save to Config");
	addString("Reset to Default");
}

void WorldAdvancedUIStructMenu::Update() {
}

/*Called once on submenu open*/
void WorldAdvancedUIStructMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	addOption(SubmenuOption(getString("Color"))
		.addSubmenu<WorldAdvancedUIColorMenu>()
		.addOnClick([this] {
			WorldAdvancedUIColorMenu::GetInstance()->SetName(m_Name.GetOriginal());
			WorldAdvancedUIColorMenu::GetInstance()->SetParentSubmenu<WorldAdvancedUIStructMenu>();
			WorldAdvancedUIColorMenuVars::m_Vars.m_Color = nullptr;
			WorldAdvancedUIColorMenuVars::m_Vars.m_Color2 = m_Vars.m_Color;
		}));

	addOption(NumberOption<float>(SCROLL, getString("Intensity"))
		.addNumber(m_Vars.m_Color->m_Color->m_Intensity, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Radius"))
		.addNumber(m_Vars.m_Color->m_Color->m_Radius, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Falloff Exponent"))
		.addNumber(m_Vars.m_Color->m_Color->m_FalloffExp, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Inner Cone Angle"))
		.addNumber(m_Vars.m_Color->m_Color->m_InnerConeAngle, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Outer Cone Angle"))
		.addNumber(m_Vars.m_Color->m_Color->m_OuterConeAngle, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Corona HDR"))
		.addNumber(m_Vars.m_Color->m_Color->m_CoronaHDR, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Corona Size"))
		.addNumber(m_Vars.m_Color->m_Color->m_CoronaSize, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Shadow Blur"))
		.addNumber(m_Vars.m_Color->m_Color->m_ShadowBlur, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(NumberOption<float>(SCROLL, getString("Capsule Length"))
		.addNumber(m_Vars.m_Color->m_Color->m_CapsuleLength, "%.2f", 0.05f).addMin(-100.f).addMax(10000.f).setScrollSpeed(0));

	addOption(ToggleOption(getString("Save to Config"))
		.addToggle(m_Vars.m_Color->m_SaveToConfig)
		.addOnUpdate([] (ToggleOption* Option) { Option->addToggle(m_Vars.m_Color->m_SaveToConfig); }));

	addOption(ButtonOption(getString("Reset to Default"))
		.addTranslation()
		.addSprite("Ozark", "spinner")
		.addSpriteScale(0.01232f, 0.02184f)
		.addSpriteRotate()
		.addOnClick([] {
			m_Vars.m_Color->m_RainbowToggle = false;
			m_Vars.m_Color->m_Rainbow.Stop();

			memcpy(m_Vars.m_Color->m_Color, &m_Vars.m_Color->m_DefaultColor, sizeof(Rage::Types::UIElementColorStruct));
		}));

	SaveStructToConfig(m_Name.GetOriginal());
}

/*Called always*/
void WorldAdvancedUIStructMenu::FeatureUpdate() {}

WorldAdvancedUIStructMenu* _instance;
WorldAdvancedUIStructMenu* WorldAdvancedUIStructMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldAdvancedUIStructMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldAdvancedUIStructMenu::~WorldAdvancedUIStructMenu() { delete _instance; }