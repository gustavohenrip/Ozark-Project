#include "misc.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../main.hpp"
#include "menu/base/util/global.hpp"
#include "misc/misc_camera.hpp"
#include "misc/misc_radio.hpp"
#include "misc/misc_hud.hpp"
#include "misc/misc_tunables.hpp"
#include "misc/misc_visions.hpp"
#include "misc/misc_display.hpp"
#include "misc/misc_panels.hpp"
#include "misc/misc_scripthook.hpp"
#include "misc/misc_disables.hpp"
#include "misc/misc_model_swap.hpp"
#include "menu/base/util/panels.hpp"
#include "utils/memory/memory.hpp"
#include "misc/misc_texture_swap.hpp"

using namespace MiscMenuVars;

namespace MiscMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> MetricSystem[] = {
		{ TranslationString("Metric", true), 0 },
		{ TranslationString("Imperial", true), 1 }
	};
}

void MiscMenu::Init() {
	SetName("Miscellaneous");
	SetParentSubmenu<MainMenu>();

	addOption(SubmenuOption("ScriptHook")
		.addTranslation().addHotkey()
		.addSubmenu<MiscScriptHookMenu>());

	addOption(SubmenuOption("Camera")
		.addTranslation().addHotkey()
		.addSubmenu<MiscCameraMenu>());

	addOption(SubmenuOption("Radio")
		.addTranslation().addHotkey()
		.addSubmenu<MiscRadioMenu>());

	addOption(SubmenuOption("HUD")
		.addTranslation().addHotkey()
		.addSubmenu<MiscHudMenu>());

	addOption(SubmenuOption("Tunables")
		.addTranslation().addHotkey()
		.addSubmenu<MiscTunablesMenu>());

	addOption(SubmenuOption("Visions")
		.addTranslation().addHotkey()
		.addSubmenu<MiscVisionsMenu>());

	addOption(SubmenuOption("Display")
		.addTranslation().addHotkey()
		.addSubmenu<MiscDisplayMenu>());

	addOption(SubmenuOption("Panels")
		.addTranslation().addHotkey()
		.addSubmenu<MiscPanelsMenu>());

	addOption(SubmenuOption("Disables")
		.addTranslation().addHotkey()
		.addSubmenu<MiscDisablesMenu>());

	addOption(SubmenuOption("Model Swap")
		.addTranslation().addHotkey()
		.addSubmenu<MiscModelSwapMenu>());

	addOption(SubmenuOption("Texture Swap")
		.addTranslation().addHotkey()
		.addSubmenu<MiscTextureSwapMenu>());

	addOption(ScrollOption<int>(SCROLL, "Speed Calculations")
		.addTranslation().addHotkey()
		.addScroll(m_Vars.m_MetricSystem, 0, NUMOF(MetricSystem), MetricSystem)
		.addOnClick([] {
			float F = m_Vars.m_MetricSystem == 0 ? 2.236936f : 3.6f;
			std::string N = m_Vars.m_MetricSystem == 0 ? "MPH" : "KPH";
			Global::Vars::m_SelectedMetricSystem = { F, N };
		}).canBeSaved(m_ParentNameStack)
		.addTooltip("Metric to be used in features such as Speedometer"));

	addOption(ToggleOption("Show Join Events")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ShowJoinEvents).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Rockstar Developer Perks")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_RockstarDeveloperPerks).canBeSaved(m_ParentNameStack)
		.addTooltip("You NEED to use RID spoofing for this, otherwise you have the chance to get banned"));

	addOption(ButtonOption("Bail to SP")
		.addTranslation().addHotkey()
		.addOnClick([] {
			Native::NetworkBail(0, 0, 0);
		})
		.addTooltip("Bail to singleplayer"));
}

void MiscMenu::Update() {}

/*Called once on submenu open*/
void MiscMenu::UpdateOnce() {
	for (auto P : Menu::Panels::GetPanelManager()->GetPanels()) {
		P->m_Render = false;
	}
}

/*Called always*/
void MiscMenu::FeatureUpdate() {}

MiscMenu* _instance;
MiscMenu* MiscMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscMenu::~MiscMenu() { delete _instance; }