#include "rainbow_config.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../../main.hpp"
#include "rainbow_config_load.hpp"

using namespace RainbowConfigMenuVars;

namespace RainbowConfigMenuVars {
	Vars_ m_Vars;

	void SetColor() {
		if (m_Vars.m_Rainbow) {
			m_Vars.m_Rainbow->m_Min = m_Vars.m_LocalRainbow.m_Min;
			m_Vars.m_Rainbow->m_Max = m_Vars.m_LocalRainbow.m_Max;
			m_Vars.m_Rainbow->m_Steps = m_Vars.m_LocalRainbow.m_Steps;
		}
	}
}

void RainbowConfigMenu::Init() {
	SetName("Rainbow Config");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("Save and Load")
		.addTranslation()
		.addOnClick([] { RainbowConfigLoadMenuVars::m_Vars.m_Rainbow = m_Vars.m_Rainbow; })
		.addSubmenu<RainbowConfigLoadMenu>());

	m_Vars.m_LocalRainbow.Add(&m_Vars.m_LocalColor);
	m_Vars.m_LocalRainbow.Toggle(true);

	RainbowConfigLoadMenu::GetInstance();
}

void RainbowConfigMenu::Update() {
	if (m_Vars.m_Rainbow) {
		m_Vars.m_LocalColor = m_Vars.m_Rainbow->m_Color;
		m_Vars.m_LocalRainbow.m_Min = m_Vars.m_Rainbow->m_Min;
		m_Vars.m_LocalRainbow.m_Max = m_Vars.m_Rainbow->m_Max;
		m_Vars.m_LocalRainbow.m_Steps = m_Vars.m_Rainbow->m_Steps;
		m_Vars.m_LocalRainbow.Run();
	}
}

/*Called once on submenu open*/
void RainbowConfigMenu::UpdateOnce() {
	ClearOptionsOffset(1);

	addOption(NumberOption<int>(SCROLL, "Minimum")
		.addNumber(m_Vars.m_LocalRainbow.m_Min, "%i", 1).addMin(0).addMax(255).canLoop()
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_LocalColor.WithOpacity(255)); })
		.addOnClick(SetColor));

	addOption(NumberOption<int>(SCROLL, "Maximum")
		.addNumber(m_Vars.m_LocalRainbow.m_Max, "%i", 1).addMin(0).addMax(255).canLoop()
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_LocalColor.WithOpacity(255)); })
		.addOnClick(SetColor));

	addOption(NumberOption<int>(SCROLL, "Steps")
		.addNumber(m_Vars.m_LocalRainbow.m_Steps, "%i", 1).addMin(1).addMax(255).canLoop()
		.addOnHover([] { Menu::GetRenderer()->PreviewColor(m_Vars.m_LocalColor.WithOpacity(255)); })
		.addOnClick(SetColor));
}

/*Called always*/
void RainbowConfigMenu::FeatureUpdate() {}

RainbowConfigMenu* _instance;
RainbowConfigMenu* RainbowConfigMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new RainbowConfigMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

RainbowConfigMenu::~RainbowConfigMenu() { delete _instance; }