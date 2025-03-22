#include "teleport_store_selected.hpp"
#include "menu/base/submenu_handler.hpp"
#include "teleport_store.hpp"
#include "../teleport.hpp"

using namespace TeleportStoreSelectedMenuVars;

namespace TeleportStoreSelectedMenuVars {
	Vars_ m_Vars;
	
	std::pair<std::string, Math::Vector3_<float>> TeleportLocStores[9][12] = {
		{ // Ammunation
			{ "Sandy Shores",{ 1705.173f, 3747.373f, 33.922f } },
			{ "Hawick",{ 234.312f, -42.553f, 69.676f } },
			{ "La Mesa",{ 843.569f, -1018.228f, 27.561f } },
			{ "Paleto Bay",{ -320.263f, 6071.031f, 31.337f } },
			{ "Little Seoul",{ -663.388f, -950.879f, 21.399f } },
			{ "Morningwood",{ -1324.082f, -388.411f, 36.545f } },
			{ "Great Chaparral",{ -1108.773f, 2685.568f, 18.875f } },
			{ "Chumash",{ -3158.208f, 1078.877f, 20.691f } },
			{ "Tataviam",{ 2568.549f, 313.032f, 108.461f } },
			{ "Pillbox Hill",{ 15.288f, -1122.648f, 28.816f } },
			{ "Cypress Flats",{ 812.893f, -2139.652f, 29.292f } },
		},
		{ // Barber Shop
			{ "Rockford Hills",{ -827.346f, -190.451f, 37.604f } },
			{ "Davis",{ 130.348f, -1715.111f, 29.234f } },
			{ "Vespucci",{ -1295.014f, -1116.923f, 6.655f } },
			{ "Sandy Shores",{ 1936.458f, 3720.811f, 32.672f } },
			{ "Mirror Park",{ 1202.648f, -470.297f, 66.246f } },
			{ "Hawick",{ -30.615f, -142.411f, 57.051f } },
			{ "Paleto Bay",{ -284.387f, 6236.210f, 31.460f } },
		},
		{ // Binco
			{ "Textile City",{ 411.758f, -808.082f, 29.144f } },
			{ "Vespucci Canals",{ -814.987f, -1083.856f, 11.012f } },
		},
		{ // Discount
			{ "Strawberry",{ 89.663f, -1390.938f, 29.249f } },
			{ "Grapeseed",{ 1678.675f, 4821.034f, 42.007f } },
			{ "Great Chaparral",{ -1091.677f, 2700.770f, 19.625f } },
			{ "Senora Desert",{ 1200.602f, 2696.959f, 37.927f } },
			{ "Paleto Bay",{ -5.078f, 6521.567f, 31.270f } },
		},
		{ // LSC
			{ "Burton",{ -365.425f, -131.809f, 37.873f } },
			{ "LS Airport",{ -1134.224f, -1984.387f, 13.166f } },
			{ "La Mesa",{ 709.797f, -1082.649f, 22.398f } },
			{ "Senora Desert",{ 1178.653f, 2666.179f, 37.881f } },
		},
		{ // Ponsonbys
			{ "Rockford Hills",{ -718.441f, -162.860f, 37.013f } },
			{ "Burton",{ -150.952f, -304.549f, 38.925f } },
			{ "Morningwood",{ -1461.290f, -226.524f, 49.249f } },
		},
		{ // Suburban
			{ "Del Perro",{ -1209.446f, -783.510f, 17.169f } },
			{ "Harmony",{ 617.782f, 2736.849f, 41.999f } },
			{ "Harwick",{ 130.452f, -202.726f, 54.505f } },
			{ "Chumash",{ -3165.330f, 1062.592f, 20.840f } },
		},
		{ // Tattoo
			{ "Downtown",{ 318.079f, 170.586f, 103.767f } },
			{ "Sandy Shores",{ 1853.978f, 3745.352f, 33.082f } },
			{ "Paleto Bay",{ -286.602f, 6202.248f, 31.322f } },
			{ "Vespucci Canals",{ -1159.103f, -1417.739f, 4.706f } },
			{ "El Burro Heights",{ 1319.359f, -1643.693f, 52.145f } },
			{ "Chumash",{ -3162.709f, 1071.733f, 20.681f } },
		},
		{ // Other
			{ "Beekers Garage",{ 126.219f, 6608.142f, 31.866f } },
			{ "Vespucci Movie Masks",{ -1339.926f, -1279.063f, 4.870f } },
		}
	};

	int TeleportLocStoreCount[] = {
		11,
		7,
		2,
		5,
		4,
		3,
		4,
		6,
		2,
	};
}

void TeleportStoreSelectedMenu::Init() {
	SetName("Store");
	SetParentSubmenu<TeleportStoreMenu>();

	for (int i = 0; i < 9; i++) {
		for (auto& Yes : TeleportLocStores[i]) {
			addString(Yes.first);
		}
	}
}

void TeleportStoreSelectedMenu::Update() {}

/*Called once on submenu open*/
void TeleportStoreSelectedMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	for (int i = 0; i < TeleportLocStoreCount[m_Vars.m_Selected]; i++) {
		auto Loc = TeleportLocStores[m_Vars.m_Selected][i];
		addOption(ButtonOption(getString(Loc.first))
			.addOnClick([=] { TeleportMenuVars::Teleport(Math::Vector3_<float>::ToPadded(Loc.second)); }));
	}
}

/*Called always*/
void TeleportStoreSelectedMenu::FeatureUpdate() {}

TeleportStoreSelectedMenu* _instance;
TeleportStoreSelectedMenu* TeleportStoreSelectedMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TeleportStoreSelectedMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TeleportStoreSelectedMenu::~TeleportStoreSelectedMenu() { delete _instance; }