#include "vehicle_spawner_garage.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_spawner.hpp"
#include "menu/base/util/stats.hpp"
#include "menu/base/submenus/main/network/players/teleport/network_teleport_apartment.hpp"
#include "menu/base/util/global.hpp"
#include "garage/vehicle_spawner_garage_selected.hpp"

using namespace VehicleSpawnerGarageMenuVars;

namespace VehicleSpawnerGarageMenuVars {
	Vars_ m_Vars;

	const char* GarageLabels[] = {
		"PROPERTY_HOUSE",
		"MULTI_PROPERTY_1",
		"MULTI_PROPERTY_2",
		"MULTI_PROPERTY_3",
		"MULTI_PROPERTY_4",
		"PROP_OFFICE",
		"PROP_CLUBHOUSE",
		"MULTI_PROPERTY_5",
		"PROP_OFFICE_GAR1",
		"PROP_OFFICE_GAR2",
		"PROP_OFFICE_GAR3",
		"PROP_IE_WAREHOUSE",
		"PROP_HANGAR",
		"PROP_DEFUNCBASE",
		"PROP_NIGHTCLUB",
		"PROP_MEGAWARE_GAR1",
		"PROP_MEGAWARE_GAR2",
		"PROP_MEGAWARE_GAR3",
		"PROP_ARENAWARS_GAR1",
		"PROP_ARENAWARS_GAR2",
		"PROP_ARENAWARS_GAR3",
		"PROP_CASINO_GAR1",
		"PROP_ARCADE_GAR1",
		"MULTI_PROPERTY_6",
		"MULTI_PROPERTY_7"
	};

	namespace Scripts {
		int func_603(int iParam0, int iParam1) {
			if (iParam1 == -1) {
				switch (iParam0) {
					case 91:
					case 92:
					case 93:
					case 94:
					case 95:
					case 96:
					case 97:
					case 98:
					case 99:
					case 100:
					case 101:
					case 102:
						return 1;
						break;
				}
			} else if (iParam1 == 91) {
				switch (iParam0) {
					case 91:
					case 92:
					case 93:
					case 94:
					case 95:
					case 96:
						return 1;
						break;
				}
			} else if (iParam1 == 97) {
				switch (iParam0) {
					case 97:
					case 98:
					case 99:
					case 100:
					case 101:
					case 102:
						return 1;
						break;
				}
			}
			return 0;
		}

		int func_604(int iParam0) {
			switch (iParam0) {
				case 87:
				case 88:
				case 89:
				case 90:
					return 1;
					break;
			}
			return 0;
		}

		int func_607(int iParam0) {
			if (iParam0 != -1) {
				return Native::IsBitSet(Menu::Global(1590682).At(iParam0, 883).At(274).At(351).As<int>(), 2);
			}

			return 0;
		}

		int func_285(int iParam0) {
			if (iParam0 != -1) {
				return Native::IsBitSet(Menu::Global(2426097).At(iParam0, 443).At(199).As<int>(), 9);
			}
			return 0;
		}

		bool func_606(int iParam0, bool bParam1) {
			if (Menu::Global(1590518).As<int>() != -1) {
				if (!func_607(Menu::Global(1590518).As<int>())) {
					return 0;
				}

				if (bParam1) {
					if (Menu::GetLocalPlayer().m_ID != Menu::Global(1590518).As<int>()) {
						if (Native::IsBitSet(Menu::Global(2426097).At(Menu::Global(1590518).As<int>(), 443).At(199).As<int>(), 24) || func_285(Menu::Global(1590518).As<int>())) {
							return 1;
						}
					}
				}
			}

			return Native::IsBitSet(Menu::Global(2426097).At(iParam0, 443).At(199).As<int>(), 24);
		}

		int func_605(int iParam0, bool bParam1, bool bParam2) {
			if (bParam2) {
				return func_606(Menu::GetLocalPlayer().m_ID, 0);
			}
			if (bParam1) {
				if (func_606(Menu::GetLocalPlayer().m_ID, 0)) {
					return 0;
				}
				switch (iParam0) {
					case 103:
					case 106:
					case 109:
					case 112:
					case 104:
					case 107:
					case 110:
					case 113:
					case 105:
					case 108:
					case 111:
					case 114:
						return 1;
						break;
				}
			}
			switch (iParam0) {
				case 103:
				case 106:
				case 109:
				case 112:
				case 104:
				case 107:
				case 110:
				case 113:
				case 105:
				case 108:
				case 111:
				case 114:
					return 1;
					break;
			}
			return 0;
		}

		int func_609(int iParam0) {
			iParam0 = (iParam0 - 1000);
			if (iParam0 >= 0 && iParam0 <= 4) {
				return iParam0;
			}
			return -1;
		}

		int func_1452(int iParam0) {
			switch (iParam0) {
				case 0:
					return 156;

				case 2:
					return 223;

				case 1:
					return 224;

				case 3:
					return 278;
			}
			return -1;
		}

		// func_602
		int GetGarageVehicleMaxCount(int iParam0, int iParam1 = -1, bool bParam2 = true) {
			if (iParam0 == -1) {
				if (iParam1 >= 1) {
					if (func_605(iParam1, 0, 0)) {
						return 20;
					} else if (func_604(iParam1)) {
						return 0;
					} else if (func_603(iParam1, -1)) {
						return 10;
					} else if (iParam1 == 115) {
						return 8;
					} else if (iParam1 == 116) {
						return 20;
					} else if (iParam1 == 117) {
						return 7;
					} else if (iParam1 == 118) {
						return 1;
					} else if ((iParam1 == 119 || iParam1 == 120) || iParam1 == 121) {
						return 10;
					} else if (iParam1 == 122) {
						return 9;
					} else if (iParam1 == 123 || iParam1 == 124) {
						return 10;
					} else if (iParam1 == 125) {
						return 10;
					} else if (iParam1 == 126) {
						return 10;
					} else if (iParam1 <= 126 && iParam1 > 0) {
						if (Menu::Global(1049924).At(iParam1, 1951).At(33).As<int>() == 2) {
							if (bParam2) {
								return 3;
							} else {
								return 2;
							}
						} else if (Menu::Global(1049924).At(iParam1, 1951).At(33).As<int>() == 6) {
							if (bParam2) {
								return 8;
							} else {
								return 6;
							}
						} else if (Menu::Global(1049924).At(iParam1, 1951).At(33).As<int>() == 10) {
							if (bParam2) {
								return 13;
							} else {
								return 10;
							}
						}
					}
				}
			}
			switch (iParam0) {
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 7:
				case 23:
				case 24:
					return 13;
					break;

				case 5:
					return 0;
					break;

				case 6:
					return 10;
					break;

				case 8:
				case 9:
				case 10:
					return 20;
					break;

				case 11:
					return 8;
					break;

				case 12:
					return 20;
					break;

				case 13:
					return 7;
					break;

				case 14:
					return 1;
					break;

				case 15:
				case 16:
				case 17:
					return 10;
					break;

				case 18:
				case 19:
				case 20:
					return 10;
					break;

				case 21:
					return 10;
					break;

				case 22:
					return 10;
					break;
			}
			return 0;
		}

		int func_1451(int iParam0) {
			int iVar0;

			switch (iParam0) {
				case 8:
					return 88;
					break;

				case 9:
					return 108;
					break;

				case 10:
					return 128;
					break;

				case 11:
					return 148;
					break;

				case 6:
					return 65;
					break;

				case 7:
					return 75;
					break;

				case 5:
					return -1;
					break;

				case 12:
					return 159;
					break;

				case 13:
					return 179;
					break;

				case 14:
					return 191;
					break;

				case 15:
					return 192;
					break;

				case 16:
					return 202;
					break;

				case 17:
					return 212;
					break;

				case 18:
					return 227;
					break;

				case 19:
					return 237;
					break;

				case 20:
					return 247;
					break;

				case 21:
					return 258;
					break;

				case 22:
					return 268;
					break;

				case 23:
					return 281;
					break;

				case 24:
					return 294;
					break;
			}
			if (iParam0 >= 1000) {
				iVar0 = func_609(iParam0);
				return func_1452(iVar0);
			}
			return (GetGarageVehicleMaxCount(iParam0, -1, 1) * iParam0);
		}

		void func_1149(int iParam0, int* iParam1, bool bParam2) {
			if (Menu::Global(262145).At(10165).As<int>()) { // good
				*iParam1 = iParam0;
			}

			if (iParam0 >= 0) {
				*iParam1 = (Menu::Global(1672965).At(iParam0).As<int>() - 1);
			} else {
				*iParam1 = -1;
			}
		}

		char* func_2031(int iParam0) {
			switch (iParam0) {
				case 1:
					return "MP_HANGAR_1";
					break;

				case 2:
					return "MP_HANGAR_2";
					break;

				case 3:
					return "MP_HANGAR_3";
					break;

				case 4:
					return "MP_HANGAR_4";
					break;

				case 5:
					return "MP_HANGAR_5";
					break;
			}
			return "";
		}

		int func_981(int iParam0) {
			if (iParam0 != -1) {
				return Menu::Global(1590682).At(iParam0, 883).At(274).At(264).As<int>();
			}
			return 0;
		}

		int func_979(int iParam0) {
			if (iParam0 != -1) {
				return Menu::Global(1590682).At(iParam0, 883).At(274).At(271).As<int>();
			}
			return 0;
		}

		char* func_2030(int iParam0) {
			switch (iParam0) {
				case 1:
					return "MP_DBASE_1";
					break;

				case 2:
					return "MP_DBASE_2";
					break;

				case 3:
					return "MP_DBASE_3";
					break;

				case 4:
					return "MP_DBASE_4";
					break;

				case 5:
					return "MP_DBASE_6";
					break;

				case 6:
					return "MP_DBASE_7";
					break;

				case 7:
					return "MP_DBASE_8";
					break;

				case 8:
					return "MP_DBASE_9";
					break;

				case 9:
					return "MP_DBASE_10";
					break;
			}
			return "";
		}
	}

	template<typename T>
	T GetSlotTrait(int Slot, int Trait) {
		return *Menu::Global(1323678).At(Slot, 141).At(Trait).Get<T>();
	}

	const char* GetGarageName(int ID) {
		const char* Label = "Garage";

		int Index = Stats::Interface<int>::Get(GarageLabels[ID]);
		if (Index > 0) {
			if (Index > 115) {
				if (Index == 116) {
					Label = (Scripts::func_2031(Scripts::func_981(Menu::GetLocalPlayer().m_ID)));
				}

				if (Index == 117) {
					Label = (Scripts::func_2030(Scripts::func_979(Menu::GetLocalPlayer().m_ID)));
				}

				if (Index == 118) {
					Label = ("MP_BHUB_CLUBG");
				}

				if (Index == 119) {
					Label = "MP_BHUB_GAR1";
				}

				if (Index == 120) {
					Label = "MP_BHUB_GAR2";
				}

				if (Index == 121) {
					Label = "MP_BHUB_GAR3";
				}

				if (Index == 122) {
					Label = "ARENA_GAR_F0";
				}

				if (Index == 123) {
					Label = "ARENA_GAR_F1";
				}

				if (Index == 124) {
					Label = "ARENA_GAR_F2";
				}

				if (Index == 125) {
					Label = "CASINO_GARNAME";
				}

				if (Index == 126) {
					Label = "ARCADE_GARNAME";
				}

				Label = Native::_GetLabelText(Label);
			} else {
				Label = NetworkTeleportApartmentMenuVars::GetLocationFromID(Index);
			}
		}

		return Label;
	}
}

void VehicleSpawnerGarageMenu::Init() {
	SetName("Garage Vehicles");
	SetParentSubmenu<VehicleSpawnerMenu>();

	addString("No Garages");
}

void VehicleSpawnerGarageMenu::Update() {}

/*Called once on submenu open*/
void VehicleSpawnerGarageMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	// Get garage names
	for (auto& V : m_Vars.m_ValidSlots) V.clear();

	for (int i = 0; i < NUMOF(GarageLabels); i++) {
		int iVar4 = Scripts::func_1451(i);
		for (int j = 0; j < Scripts::GetGarageVehicleMaxCount(i, -1, 1); j++) {
			int Slot = 0;
			Scripts::func_1149((j + iVar4), &Slot, 1);

			if (Slot >= 0 && GetSlotTrait<uint32_t>(Slot, 66) != 0 && Native::IsModelInCdimage(GetSlotTrait<uint32_t>(Slot, 66))) {
				m_Vars.m_ValidSlots[i].push_back(Slot);
			}
		}

		if (!m_Vars.m_ValidSlots[i].empty()) {
			addOption(SubmenuOption(GetGarageName(i))
				.addTranslation()
				.addSubmenu<VehicleSpawnerGarageSelectedMenu>()
				.addOnClick([=] {
					VehicleSpawnerGarageSelectedMenu::GetInstance()->SetName(GetGarageName(i));
					VehicleSpawnerGarageSelectedMenuVars::m_Vars.m_Slot = i;
				}));
		}
	}

	if (m_Options.size() == 0) {
		addOption(ButtonOption(getString("No Garages")));
	}
}

/*Called always*/
void VehicleSpawnerGarageMenu::FeatureUpdate() {}

VehicleSpawnerGarageMenu* _instance;
VehicleSpawnerGarageMenu* VehicleSpawnerGarageMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerGarageMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerGarageMenu::~VehicleSpawnerGarageMenu() { delete _instance; }