#include "hooks.hpp"
#include "rage/invoker/natives.hpp"
#include "utils/fibers.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/util/input.hpp"
#include "menu/init.hpp"
#include "utils/caller.hpp"
#include "utils/memory/pattern.hpp"
#include "menu/base/util/control_manager.hpp"
#include "utils/fiber_pool.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/panels.hpp"
#include "utils/log.hpp"
#include "menu/base/submenus/main/network/browser/network_browser.hpp"
#include "rage/engine.hpp"
#include <mutex>
#include "utils/memory/memory.hpp"
#include "menu/base/util/menu_input.hpp"
#include "menu/base/util/hotkeys.hpp"

using namespace Menu;

int Menu::Hooks::StatGetIntHook(Rage::Invoker::NativeContext* Context) {
	static int FrameCache = 0;
	static bool InitFlag = false;

#ifdef DEV
	if (Menu::Util::GetInput()->IsPressed(true, VK_DELETE)) {
		GetInit()->Unload();
	}
#endif

	if (Global::Vars::g_UnloadingQueue) {
		Global::Vars::g_UnloadingQueue = false;
		GetInit()->Unload();
	}

	if (FrameCache < Native::GetFrameCount()) {
		FrameCache = Native::GetFrameCount();

		if (!InitFlag) {
			InitFlag = true;

			GetPlayerManager()->Update();

			GetSubmenuHandler()->InitSubmenus();
			Utils::GetFiberManager()->Initialize();
			Utils::GetScriptHookFiberManager()->Initialize();
			Utils::GetFiberPool()->Initialize();

			Utils::GetFiberManager()->Add("CMU"_Protect, [] {
				GetControlManager()->Update();
				GetNotify()->Update();
			});

			Utils::GetFiberManager()->Add("BU"_Protect, [] {
				GetBase()->Update();
				Panels::GetPanelManager()->Update();
			});

			Utils::GetFiberManager()->Add("SU"_Protect, [] {
				GetSubmenuHandler()->AsyncUpdate();
				GetBase()->AsyncUpdate();
			});

			Utils::GetFiberManager()->Add("MI"_Protect, [] {
				Menu::Util::GetMenuInput()->Update();
			});

			Utils::GetFiberManager()->Add("HKM"_Protect, [] {
				Menu::Util::GetHotkey()->Update();
			});

			Utils::GetFiberManager()->Add("SDSD"_Protect, [] {
				if (NetworkBrowserMenuVars::m_Vars.m_Populate) {
					if (Menu::GetSubmenuHandler()->GetCurrentSubmenu() == NetworkBrowserMenu::GetInstance()) {
						Rage::Engine::SendMatchmakingRequest();
					}
				}
			});

			Rage::Engine::RegisterTexture((std::string(Utils::GetConfig()->GetGTAPath()) + "\\Ozark.ytd"_Protect).c_str(), "Ozark.ytd"_Protect);
			Rage::Engine::RegisterTexture((std::string(Utils::GetConfig()->GetGTAPath()) + "\\OzarkBlips.ytd"_Protect).c_str(), "OzarkBlips.ytd"_Protect);

			Native::RequestStreamedTextureDict("Ozark", false);
			Native::RequestStreamedTextureDict("OzarkBlips", false);
			Native::RequestStreamedTextureDict("commonmenu", false);
			Native::RequestStreamedTextureDict("ShopUI_Title_Supermod", false);
			Native::RequestStreamedTextureDict("shopui_title_carmod", false);
			Native::RequestStreamedTextureDict("shopui_title_gunclub", false);
			Native::RequestStreamedTextureDict("shopui_title_casino", false);
			Native::RequestStreamedTextureDict("casinoui_lucky_wheel", false);
			Native::RequestStreamedTextureDict("casinoui_cards_blackjack", false);
			Native::RequestStreamedTextureDict("casinoui_cards_blackjack_high", false);
			Native::RequestStreamedTextureDict("casinoui_roulette", false);
			Native::RequestStreamedTextureDict("casinoui_roulette_high", false);
			Native::RequestStreamedTextureDict("casinoui_cards_three", false);
			Native::RequestStreamedTextureDict("casinoui_cards_three_high", false);
			Native::RequestStreamedTextureDict("casinoui_slots_diamond", false);
		}

		if (!Global::Vars::g_Unloading) {
			Utils::GetFiberManager()->Update();
			Utils::GetScriptHookFiberManager()->Update();
		}
	}
	
	return Native::StatGetInt(Context->GetArgument<uint32_t>(0), Context->GetArgument<int*>(1), Context->GetArgument<int>(2));
}