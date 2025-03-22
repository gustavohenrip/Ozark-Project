#include "network_casino_rigging_roulette.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_casino_rigging.hpp"
#include "../../../network.hpp"
#include "rage/engine.hpp"
#include "network_casino_rigging_roulette_selected.hpp"

using namespace NetworkCasinoRiggingRouletteMenuVars;

namespace NetworkCasinoRiggingRouletteMenuVars {
	Vars_ m_Vars;

	Math::Vector3<float> GetRouletteTableCoords(int TableIndex) {
		switch (TableIndex) {
			case 0: return { 1144.814f, 268.2634f, -52.8409f };
			case 1: return { 1150.355f, 262.7224f, -52.8409f };
			case 2: return { 1133.958f, 262.1071f, -52.0409f };
			case 3: return { 1129.595f, 267.2637f, -52.0409f };
			case 4: return { 1144.618f, 252.2411f, -52.0409f };
			case 5: return { 1148.981f, 247.0846f, -52.0409f };
		}
		return { 0.0f, 0.0f, 0.0f };
	}

	char* GetRouletteTableName(int TableIndex) {
		switch (TableIndex) {
			case 0: return "Standard Table";
			case 1: return "Standard Table";
			case 2: return "VIP Table";
			case 3: return "VIP Table";
			case 4: return "VIP Table";
			case 5: return "VIP Table";
		}

		return "Unknown";
	}

	void DrawESP(int TableIndex) {
		Math::Vector3<float> MachineCoords = GetRouletteTableCoords(TableIndex);
		Math::Vector3<float> StartCoords = Menu::GetLocalPlayer().m_Coords;
		Menu::GetRenderer()->DrawLine({ StartCoords.m_X, StartCoords.m_Y, StartCoords.m_Z }, { MachineCoords.m_X, MachineCoords.m_Y, MachineCoords.m_Z }, Global::UIVars::g_TitleHeaderColor);

		Native::SetDrawOrigin(MachineCoords.m_X, MachineCoords.m_Y, MachineCoords.m_Z, NULL);
		if (*(uint32_t*)Global::Vars::g_DrawOriginIndex != 0xFFFFFFFF) {
			Menu::GetRenderer()->DrawText(GetRouletteTableName(TableIndex), { 0.f, 0.f }, 0.3f, 0, { 255, 255, 255 }, JUSTIFY_CENTER);
			Native::ClearDrawOrigin();
		}
	}
}

void NetworkCasinoRiggingRouletteMenu::Init() {
	SetName("Roulette");
	SetParentSubmenu<NetworkCasinoRiggingMenu>();

	addString("~m~No Tables Found");
	addString("Standard Table");
	addString("VIP Table");
	addString("Unknown");
}

void NetworkCasinoRiggingRouletteMenu::Update() {
	if (Native::_GetNumberOfInstancesOfScriptWithNameHash(joaat("am_mp_casino")) <= 0) {
		Menu::GetSubmenuHandler()->SetSubmenu(NetworkMenu::GetInstance());
	}
}

/*Called once on submenu open*/
void NetworkCasinoRiggingRouletteMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	uint64_t RouletteLocals = Rage::Engine::GetScriptLocals("casinoroulette");
	if (RouletteLocals) {
		for (int i = 0; i < 6; i++) {
			addOption(SubmenuOption(Utils::VA::VA("[%i] %s", i + 1, getString(GetRouletteTableName(i)).c_str()))
				.addSubmenu<NetworkCasinoRiggingRouletteSelectedMenu>()
				.addOnClick([=] { NetworkCasinoRiggingRouletteSelectedMenuVars::m_Vars.m_Selected = i; })
				.addOnHover([=] {
					if (i > 1) {
						Global::UIVars::g_InsideCasinoRoulette = false;
						Global::UIVars::g_InsideCasinoRouletteHigh = true;
					} else {
						Global::UIVars::g_InsideCasinoRoulette = true;
						Global::UIVars::g_InsideCasinoRouletteHigh = false;
					}

					DrawESP(i);
				}));
		}
	} else {
		addOption(ButtonOption(getString("~m~No Tables Found")));
	}
}

/*Called always*/
void NetworkCasinoRiggingRouletteMenu::FeatureUpdate() {}

NetworkCasinoRiggingRouletteMenu* _instance;
NetworkCasinoRiggingRouletteMenu* NetworkCasinoRiggingRouletteMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkCasinoRiggingRouletteMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkCasinoRiggingRouletteMenu::~NetworkCasinoRiggingRouletteMenu() { delete _instance; }