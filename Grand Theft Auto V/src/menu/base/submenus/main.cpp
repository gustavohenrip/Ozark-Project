#include "main.hpp"
#include "menu/base/submenu_handler.hpp"
#include "main/player.hpp"
#include "main/weapon.hpp"
#include "main/vehicle.hpp"
#include "main/teleport.hpp"
#include "main/world.hpp"
#include "main/misc.hpp"
#include "main/object.hpp"
#include "main/network.hpp"
#include "main/ped.hpp"
#include "main/settings.hpp"
#include "menu/base/util/instructionals.hpp"
#include "menu/base/util/panels.hpp"
#include "menu/base/submenus/main/helpers/rainbow_config.hpp"
#include "utils/fiber_pool.hpp"
#include "utils/fibers.hpp"
#include "menu/base/util/helpers.hpp"
#include "rage/engine.hpp"
#include "menu/init.hpp"
#include "menu/base/util/global.hpp"
#include "utils/caller.hpp"
#include "utils/log.hpp"

using namespace MainMenuVars;

namespace MainMenuVars {
	Vars_ m_Vars;

	void ScaleformFadeOutTime(char* sParam0, float fParam1) {
		Native::_PushScaleformMovieFunction(m_Vars.m_ScaleformHandle, "HIDE");
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam0);
		Native::EndTextCommandScaleformString();
		Native::_PushScaleformMovieFunctionParameterFloat(fParam1);
		Native::_PopScaleformMovieFunctionVoid();
	}

	void ScaleformFadeInTime(char* sParam0, float fParam1) {
		Native::_PushScaleformMovieFunction(m_Vars.m_ScaleformHandle, "SHOW_SINGLE_LINE");
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam0);
		Native::EndTextCommandScaleformString();
		Native::_PushScaleformMovieFunctionParameterFloat(fParam1);
		Native::_PopScaleformMovieFunctionVoid();
	}

	void RenderLine(char* sParam0, char* sParam1, char* sParam2, char* sParam3, bool bParam4) {
		Native::_PushScaleformMovieFunction(m_Vars.m_ScaleformHandle, "ADD_TEXT_TO_SINGLE_LINE");
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam0);
		Native::EndTextCommandScaleformString();
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam1);
		Native::EndTextCommandScaleformString();
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam2);
		Native::EndTextCommandScaleformString();
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam3);
		Native::EndTextCommandScaleformString();
		Native::_PushScaleformMovieFunctionParameterBool(bParam4);
		Native::_PopScaleformMovieFunctionVoid();
	}

	void SetupSingleLine(char* sParam0, float fParam1, float fParam2, float fParam3, float fParam4, char* sParam5) {
		Native::_PushScaleformMovieFunction(m_Vars.m_ScaleformHandle, "SETUP_SINGLE_LINE");
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam0);
		Native::EndTextCommandScaleformString();
		Native::_PushScaleformMovieFunctionParameterFloat(fParam1);
		Native::_PushScaleformMovieFunctionParameterFloat(fParam2);
		Native::_PushScaleformMovieFunctionParameterFloat(fParam3);
		Native::_PushScaleformMovieFunctionParameterFloat(fParam4);
		Native::BeginTextCommandScaleformString("STRING");
		Native::AddTextComponentSubstringPlayerName(sParam5);
		Native::EndTextCommandScaleformString();
		Native::_PopScaleformMovieFunctionVoid();
	}

	void RenderWelcome() {
		if (!m_Vars.m_HasInitializedScaleform) {
			Native::_TransitionToBlurred(1000);
			m_Vars.m_ScaleformHandle = Native::RequestScaleformMovie("OPENING_CREDITS");
			m_Vars.m_HasInitializedScaleform = true;
		}

		if (Native::HasScaleformMovieLoaded(m_Vars.m_ScaleformHandle)) {
			static const DWORD TIMER = GetTickCount();
			switch (m_Vars.m_CurrentIndex) {
				case -1:
					if (GetTickCount() - TIMER > 750) {
						m_Vars.m_CurrentIndex++;
					}
					break;
				case 0:
					SetupSingleLine("presents", 1.5f, 1.5f, 20.f, 20.f, "left");
					RenderLine("presents", "OZARK CHEATS", "$font2", "HUD_COLOUR_WHITE", 1);
					RenderLine("presents", "presents", "$font5", "HUD_COLOUR_FREEMODE", 1);
					ScaleformFadeInTime("presents", 0.166f);
					m_Vars.m_CachedTime = GetTickCount();
					m_Vars.m_CurrentIndex++;
					break;
				case 1:
					if (GetTickCount() - m_Vars.m_CachedTime > 3250) {
						ScaleformFadeOutTime("presents", 0.166f);
						m_Vars.m_CurrentIndex++;
					}

					break;
				case 2:
					if (GetTickCount() - m_Vars.m_CachedTime > 5750) { // 70
						SetupSingleLine("presents", 1.5f, 1.5f, 0.f, 190.f, "left");
						RenderLine("presents", "a", "$font5", "HUD_COLOUR_MICHAEL", 1);
						RenderLine("presents", "NEW ERA", "$font2", "HUD_COLOUR_WHITE", 1);
						RenderLine("presents", "of modding", "$font5", "HUD_COLOUR_MICHAEL", 1);
						ScaleformFadeInTime("presents", 0.166f);
						m_Vars.m_CurrentIndex++;
					}
					break;
				case 3:
					if (GetTickCount() - m_Vars.m_CachedTime > 5750 + 3250) {
						ScaleformFadeOutTime("presents", 0.166f);
						m_Vars.m_CurrentIndex++;
					}

					break;

				case 4:
					if (GetTickCount() - m_Vars.m_CachedTime > 5750 + 3250 + 3250) {
						Native::SetScaleformMovieAsNoLongerNeeded(&m_Vars.m_ScaleformHandle);
						m_Vars.m_ScaleformHandle = -1;
						m_Vars.m_ShownWelcome = true;
						Native::_TransitionFromBlurred(1000);

						Utils::GetFiberPool()->Push([&] {
							Utils::WaitFiber(1500);
							Global::Vars::g_StopRendering = false;
						});
					}
					break;
			}

			if (m_Vars.m_ScaleformHandle != -1) {
				Native::DrawScaleformMovieFullscreen(m_Vars.m_ScaleformHandle, 255, 255, 255, 255, 0);
			}
		}
	}
}

void MainMenu::Init() {
	SetName("Ozark");

	m_ParentNameStack.push("Ozark");
	m_DefaultInstructionals = false;

	Menu::Panels::GetPanelManager()->Initialize();

	m_Vars.m_ShownWelcome = Utils::GetConfig()->ReadBool(m_ParentNameStack, "Shown Welcome", false, { "Main" });
	if (!m_Vars.m_ShownWelcome) {
		Global::Vars::g_StopRendering = true;
		Utils::GetConfig()->WriteBool(m_ParentNameStack, "Shown Welcome", true, { "Main" });
	}

	addOption(SubmenuOption("Player")
		.addTranslation().addHotkey()
		.addSubmenu<PlayerMenu>());

	addOption(SubmenuOption("Network")
		.addTranslation().addHotkey()
		.addSubmenu<NetworkMenu>());

	addOption(SubmenuOption("Weapon")
		.addTranslation().addHotkey()
		.addSubmenu<WeaponMenu>()
		.addOnClick([] { Global::UIVars::g_InsideWeapon = true; }));

	addOption(SubmenuOption("Vehicle")
		.addTranslation().addHotkey()
		.addSubmenu<VehicleMenu>());

	addOption(SubmenuOption("Teleport")
		.addTranslation().addHotkey()
		.addSubmenu<TeleportMenu>());

	addOption(SubmenuOption("World")
		.addTranslation().addHotkey()
		.addSubmenu<WorldMenu>());

	addOption(SubmenuOption("NPC")
		.addTranslation().addHotkey()
		.addSubmenu<PedMenu>());

	addOption(SubmenuOption("Object")
		.addTranslation().addHotkey()
		.addSubmenu<ObjectMenu>());

	addOption(SubmenuOption("Miscellaneous")
		.addTranslation().addHotkey()
		.addSubmenu<MiscMenu>());

	addOption(SubmenuOption("Settings")
		.addTranslation().addHotkey()
		.addSubmenu<SettingsMenu>());
}

void MainMenu::Update() {
	Instructionals::MainMenu();
}

/*Called once on submenu open*/
void MainMenu::UpdateOnce() {
	Global::UIVars::g_InsideWeapon = false;
}

/*Called always*/
void MainMenu::FeatureUpdate() {
	if (!m_Vars.m_ShownWelcome) {
		RenderWelcome();
	}

	if (Global::Vars::g_NetworkBaseConfig) {
		Rage::Network::NetworkBaseConfig* NetworkBaseConfig = *(Rage::Network::NetworkBaseConfig**)(uint64_t)Global::Vars::g_NetworkBaseConfig;
		if (NetworkBaseConfig) {
			NetworkBaseConfig->SetNetworkSessionWasInvited(false);
		}
	}

	Menu::Global(1663921).At(7).As<bool>() = true;

#ifdef DEV
	if (Global::Vars::g_arxan_integ_calls[0]) {
		Global::Vars::g_arxan_integ_calls[0] = false;
		LOG_DEV("ARXAN - 1 START");
	}

	if (Global::Vars::g_arxan_integ_calls[1]) {
		Global::Vars::g_arxan_integ_calls[1] = false;
		LOG_DEV("ARXAN - 1 END");
	}

	if (Global::Vars::g_arxan_integ_calls[2]) {
		Global::Vars::g_arxan_integ_calls[2] = false;
		LOG_DEV("ARXAN - 2 START");
	}

	if (Global::Vars::g_arxan_integ_calls[3]) {
		Global::Vars::g_arxan_integ_calls[3] = false;
		LOG_DEV("ARXAN - 2 END");
	}

	if (Global::Vars::g_arxan_integ_calls[4]) {
		Global::Vars::g_arxan_integ_calls[4] = false;
		LOG_DEV("ARXAN - 3 START");
	}

	if (Global::Vars::g_arxan_integ_calls[5]) {
		Global::Vars::g_arxan_integ_calls[5] = false;
		LOG_DEV("ARXAN - 3 END");
	}

	if (Global::Vars::g_arxan_integ_calls[6]) {
		Global::Vars::g_arxan_integ_calls[6] = false;
		LOG_DEV("ARXAN - 4 START");
	}

	if (Global::Vars::g_arxan_integ_calls[7]) {
		Global::Vars::g_arxan_integ_calls[7] = false;
		LOG_DEV("ARXAN - 4 END");
	}
#endif

}

MainMenu* _instance;
MainMenu* MainMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MainMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MainMenu::~MainMenu() { delete _instance; }