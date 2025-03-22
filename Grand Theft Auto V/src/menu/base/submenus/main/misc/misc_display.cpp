#include "misc_display.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "menu/base/util/stacked_text_renderer.hpp"
#include "menu/base/util/pools/pool_manager.hpp"
#include "menu/base/util/timers.hpp"
#include "utils/va.hpp"
#include "utils/caller.hpp"

using namespace MiscDisplayMenuVars;

namespace MiscDisplayMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> FPS[] = {
		{ TranslationString("FPS", true), 0, },
		{ TranslationString("Average", true), 1, },
		{ TranslationString("Both", true), 2 },
	};
}

void MiscDisplayMenu::Init() {
	SetName("Display");
	SetParentSubmenu<MiscMenu>();

	addOption(ScrollOption<int>(TOGGLE, "FPS")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_FPS)
		.addScroll(m_Vars.m_FPSVar, 0, NUMOF(FPS), FPS)
		.addOnClick([] {
			if (m_Vars.m_FPS) {
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_FPS", 0);
			} else {
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_FPS");
			}
		}).canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Position")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_Position)
		.addOnClick([] {
			if (m_Vars.m_Position) {
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_POS", 1);
			} else {
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_POS");
			}
		}).canBeSaved(m_ParentNameStack));

	// 4 -> 13

	addOption(ToggleOption("Free Slots")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_FreeSlots)
		.addOnClick([] {
			if (m_Vars.m_FreeSlots) {
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_FREESLOTS", 2);
			} else {
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_FREESLOTS");
			}
		})
		.canBeSaved(m_ParentNameStack));

	addOption(ToggleOption("Modder Count")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_ModderCount)
		.addOnClick([] {
			if (m_Vars.m_ModderCount) {
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_MODDERS", 3);
			} else {
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_MODDERS");
			}
		})
		.canBeSaved(m_ParentNameStack));
		
	addOption(ToggleOption("Entity Pools")
		.addTranslation().addHotkey()
		.addToggle(m_Vars.m_EntityPools)
		.addOnClick([] {
			if (m_Vars.m_EntityPools) {
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_AttachmentPool", 14);
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_ObjectPool", 15);
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_PedPool", 16);
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_PickupPool", 17);
				Menu::Util::GetStackedTextRenderer()->Alloc("SR_VehiclePool", 18);
			} else {
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_AttachmentPool");
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_ObjectPool");
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_PedPool");
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_PickupPool");
				Menu::Util::GetStackedTextRenderer()->Dealloc("SR_VehiclePool");
			}
		}).canBeSaved(m_ParentNameStack));

	addString("Modders");
	addString("Free Slots");
	addString("Position");
	addString("Attachment Pool");
	addString("Attachment Pool");
	addString("Object Pool");
	addString("Ped Pool");
	addString("Pickup Pool");
	addString("Vehicle Pool");
	addString("FPS");
	addString("Average FPS");
	addString("Avg");
}

void MiscDisplayMenu::Update() {}

/*Called once on submenu open*/
void MiscDisplayMenu::UpdateOnce() {}

/*Called always*/
void MiscDisplayMenu::FeatureUpdate() {
	static int Timer = 0;
	Menu::Timers::RunTimedFunction(&Timer, 1000, [] {
		m_Vars.m_DrawFPSCache[1] = m_Vars.m_DrawFPSCache[0];
		m_Vars.m_DrawFPSCache[0] = Native::GetFrameCount();
	});

	if (m_Vars.m_ModderCount) {
		int Count = 0;

		Menu::GetPlayerManager()->Foreach([&](Menu::PlayerVars& Player) {
			if (Player.m_IsModder) Count++;
		}, true);

		Menu::Util::GetStackedTextRenderer()->Update("SR_MODDERS", getString("Modders"), std::to_string(Count));
	}

	if (m_Vars.m_FreeSlots) {
		Menu::Util::GetStackedTextRenderer()->Update("SR_FREESLOTS", getString("Free Slots"), std::to_string(32 - Native::NetworkGetNumConnectedPlayers()));
	}

	if (m_Vars.m_Position) {
		auto Coords = Menu::GetLocalPlayer().m_Coords;
		Menu::Util::GetStackedTextRenderer()->Update("SR_POS", getString("Position"), Utils::VA::VA("[%.2f, %.2f, %.2f]", Coords.m_X, Coords.m_Y, Coords.m_Z));
	}

	if (m_Vars.m_EntityPools) {
		std::vector<std::pair<std::string, std::pair<int, int>>> PoolInfo;

		std::pair<std::string, void*> Pools[] = {
			{ "Attachment Pool", Global::Vars::g_AttachmentPool },
			{ "Object Pool", Global::Vars::g_ObjectPool },
			{ "Ped Pool", Global::Vars::g_PedPool },
			{ "Pickup Pool", Global::Vars::g_PickupPool },
			{ "Vehicle Pool", Global::Vars::g_VehiclePool }
		};

		for (int i = 0; i < NUMOF(Pools); i++) {
			auto Pool = Pools[i];
			if (Pool.second) {
				if (Pool.first == "Vehicle Pool") {
					auto P = (Rage::Types::VehicleEntityPool*)Pool.second;
					if (!IsValidPtr(P->m_Address)) continue;

					int Populated = 0;
					for (uint32_t j = 0; j < P->m_Count; j++) {
						if (P->GetEntity(j)) {
							Populated++;
						}
					}

					PoolInfo.push_back(std::make_pair(getString(Pool.first), std::make_pair(P->m_Size, Populated)));
				} else {
					auto P = (Rage::Types::GenericPool*)Pool.second;
					if (!IsValidPtr(P->m_MaskArray)) continue;
					if (!IsValidPtr(P->m_Address)) continue;

					int Populated = 0;
					for (uint32_t j = 0; j < P->m_Count; j++) {
						if (P->GetEntity(j)) {
							Populated++;
						}
					}

					PoolInfo.push_back(std::make_pair(getString(Pool.first), std::make_pair(P->m_Count, Populated)));
				}
			}
		}

		for (std::size_t i = 0; i < PoolInfo.size(); i++) {
			std::string Name = Pools[i].first;
			Name.erase(remove(Name.begin(), Name.end(), ' '), Name.end());
			Menu::Util::GetStackedTextRenderer()->Update(("SR_" + Name), PoolInfo[i].first.c_str(), Utils::VA::VA("%i/%i", PoolInfo[i].second.second, PoolInfo[i].second.first));
		}
	}

	if (m_Vars.m_FPS) {
		int FPS = m_Vars.m_DrawFPSCache[0] - m_Vars.m_DrawFPSCache[1] - 1;

		if (m_Vars.m_DrawFPSAverageCount < 900) {
			m_Vars.m_DrawFPSAverageTotal += FPS;
			m_Vars.m_DrawFPSAverageCount++;
		} else {
			m_Vars.m_DrawFPSAverageTotal = m_Vars.m_DrawFPSAverageCount = 0;
		}

		if (m_Vars.m_DrawFPSAverageTotal != 0 && m_Vars.m_DrawFPSAverageCount != 0) {
			m_Vars.m_DrawFPSAverage = m_Vars.m_DrawFPSAverageTotal / m_Vars.m_DrawFPSAverageCount;
		}

		char buffer[50];
		if (m_Vars.m_FPSVar == 0) {
			sprintf(buffer, "%s%i", FPS < 60 ? "~r~" : FPS > 100 ? "~g~" : "", FPS);
		} else if (m_Vars.m_FPSVar == 1) {
			sprintf(buffer, "%s%i", m_Vars.m_DrawFPSAverage < 60 ? "~r~" : m_Vars.m_DrawFPSAverage > 100 ? "~g~" : "", m_Vars.m_DrawFPSAverage);
			Menu::Util::GetStackedTextRenderer()->Update("SR_FPS", getString("Average FPS"), buffer);
			return;
		} else {
			sprintf(buffer, "%s%i~w~ (%s. %s%i~w~)", FPS < 60 ? "~r~" : FPS > 100 ? "~g~" : "", FPS, getString("Avg").c_str(), m_Vars.m_DrawFPSAverage < 60 ? "~r~" : m_Vars.m_DrawFPSAverage > 100 ? "~g~" : "", m_Vars.m_DrawFPSAverage);
		}

		Menu::Util::GetStackedTextRenderer()->Update("SR_FPS", getString("FPS"), buffer);
	}
}

MiscDisplayMenu* _instance;
MiscDisplayMenu* MiscDisplayMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscDisplayMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscDisplayMenu::~MiscDisplayMenu() { delete _instance; }