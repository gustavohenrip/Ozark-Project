#include "network_teleport_apartment.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players_teleport.hpp"
#include "menu/base/util/helpers.hpp"
#include "rage/types/global_types.hpp"

using namespace NetworkTeleportApartmentMenuVars;

namespace NetworkTeleportApartmentMenuVars {
	Vars_ m_Vars;

	const char* GetLocationFromID(int ID) {
		const char* Return = "";
		switch (ID) {
			case 0:
				Return = "MP_REP_PROP_1";
				break;
			case 1:
				Return = "MP_PROP_1";
				break;

			case 2:
				Return = "MP_PROP_2";
				break;

			case 3:
				Return = "MP_PROP_3";
				break;

			case 4:
				Return = "MP_PROP_4";
				break;

			case 5:
				Return = "MP_PROP_5";
				break;

			case 6:
				Return = "MP_PROP_6";
				break;

			case 7:
				Return = "MP_PROP_7";
				break;

			case 8:
				Return = "MP_PROP_8";
				break;

			case 9:
				Return = "MP_PROP_9";
				break;

			case 10:
				Return = "MP_PROP_10";
				break;

			case 11:
				Return = "MP_PROP_11";
				break;

			case 12:
				Return = "MP_PROP_12";
				break;

			case 13:
				Return = "MP_PROP_13";
				break;

			case 14:
				Return = "MP_PROP_14";
				break;

			case 15:
				Return = "MP_PROP_15";
				break;

			case 16:
				Return = "MP_PROP_16";
				break;

			case 17:
				Return = "MP_PROP_17";
				break;

			case 18:
				Return = "MP_PROP_18";
				break;

			case 19:
				Return = "MP_PROP_19";
				break;

			case 20:
				Return = "MP_PROP_20";
				break;

			case 21:
				Return = "MP_PROP_21";
				break;

			case 22:
				Return = "MP_PROP_22";
				break;

			case 23:
				Return = "MP_PROP_23";
				break;

			case 24:
				Return = "MP_PROP_24";
				break;

			case 25:
				Return = "MP_PROP_25";
				break;

			case 26:
				Return = "MP_PROP_26";
				break;

			case 27:
				Return = "MP_PROP_27";
				break;

			case 28:
				Return = "MP_PROP_28";
				break;

			case 29:
				Return = "MP_PROP_29";
				break;

			case 30:
				Return = "MP_PROP_30";
				break;

			case 31:
				Return = "MP_PROP_31";
				break;

			case 32:
				Return = "MP_PROP_32";
				break;

			case 33:
				Return = "MP_PROP_33";
				break;

			case 34:
				Return = "MP_PROP_34";
				break;

			case 35:
				Return = "MP_PROP_35";
				break;

			case 36:
				Return = "MP_PROP_36";
				break;

			case 37:
				Return = "MP_PROP_37";
				break;

			case 38:
				Return = "MP_PROP_38";
				break;

			case 39:
				Return = "MP_PROP_39";
				break;

			case 40:
				Return = "MP_PROP_40";
				break;

			case 41:
				Return = "MP_PROP_41";
				break;

			case 42:
				Return = "MP_PROP_42";
				break;

			case 43:
				Return = "MP_PROP_43";
				break;

			case 44:
				Return = "MP_PROP_44";
				break;

			case 45:
				Return = "MP_PROP_45";
				break;

			case 46:
				Return = "MP_PROP_46";
				break;

			case 47:
				Return = "MP_PROP_48";
				break;

			case 48:
				Return = "MP_PROP_49";
				break;

			case 49:
				Return = "MP_PROP_50";
				break;

			case 50:
				Return = "MP_PROP_51";
				break;

			case 51:
				Return = "MP_PROP_52";
				break;

			case 52:
				Return = "MP_PROP_57";
				break;

			case 53:
				Return = "MP_PROP_59";
				break;

			case 54:
				Return = "MP_PROP_60";
				break;

			case 55:
				Return = "MP_PROP_61";
				break;

			case 56:
				Return = "MP_PROP_62";
				break;

			case 57:
				Return = "MP_PROP_63";
				break;

			case 58:
				Return = "MP_PROP_64";
				break;

			case 59:
				Return = "MP_PROP_65";
				break;

			case 60:
				Return = "MP_PROP_66";
				break;

			case 61:
				Return = "MP_PROP_67";
				break;

			case 62:
				Return = "MP_PROP_68";
				break;

			case 63:
				Return = "MP_PROP_69";
				break;

			case 64:
				Return = "MP_PROP_70";
				break;

			case 65:
				Return = "MP_PROP_71";
				break;

			case 66:
				Return = "MP_PROP_72";
				break;

			case 67:
				Return = "MP_PROP_73";
				break;

			case 68:
				Return = "MP_PROP_74";
				break;

			case 69:
				Return = "MP_PROP_75";
				break;

			case 70:
				Return = "MP_PROP_76";
				break;

			case 71:
				Return = "MP_PROP_77";
				break;

			case 72:
				Return = "MP_PROP_78";
				break;

			case 83:
				Return = "MP_PROP_79";
				break;

			case 84:
				Return = "MP_PROP_80";
				break;

			case 85:
				Return = "MP_PROP_81";
				break;

			case 73:
				Return = "MP_PROP_83";
				break;

			case 74:
				Return = "MP_PROP_84";
				break;

			case 75:
				Return = "MP_PROP_85";
				break;

			case 76:
				Return = "MP_PROP_86";
				break;

			case 77:
				Return = "MP_PROP_87";
				break;

			case 78:
				Return = "MP_PROP_89";
				break;

			case 79:
				Return = "MP_PROP_90";
				break;

			case 80:
				Return = "MP_PROP_92";
				break;

			case 81:
				Return = "MP_PROP_94";
				break;

			case 82:
				Return = "MP_PROP_95";
				break;

			case 86:
				Return = "PM_SPAWN_Y";
				break;

			case 87:
				Return = "MP_PROP_OFF1";
				break;

			case 88:
				Return = "MP_PROP_OFF2";
				break;

			case 89:
				Return = "MP_PROP_OFF3";
				break;

			case 90:
				Return = "MP_PROP_OFF4";
				break;

			case 91:
				Return = "MP_PROP_CLUBH1";
				break;

			case 92:
				Return = "MP_PROP_CLUBH2";
				break;

			case 93:
				Return = "MP_PROP_CLUBH3";
				break;

			case 94:
				Return = "MP_PROP_CLUBH4";
				break;

			case 95:
				Return = "MP_PROP_CLUBH5";
				break;

			case 96:
				Return = "MP_PROP_CLUBH6";
				break;

			case 97:
				Return = "MP_PROP_CLUBH7";
				break;

			case 98:
				Return = "MP_PROP_CLUBH8";
				break;

			case 99:
				Return = "MP_PROP_CLUBH9";
				break;

			case 100:
				Return = "MP_PROP_CLUBH10";
				break;

			case 101:
				Return = "MP_PROP_CLUBH11";
				break;

			case 102:
				Return = "MP_PROP_CLUBH12";
				break;

			case 103:
			case 106:
			case 109:
			case 112:
				Return = "MP_PROP_OFFG1";
				break;

			case 104:
			case 107:
			case 110:
			case 113:
				Return = "MP_PROP_OFFG2";
				break;

			case 105:
			case 108:
			case 111:
			case 114:
				Return = "MP_PROP_OFFG3";
				break;

			case 115:
				Return = "IE_WARE_1";
				break;
		}

		return Native::_GetLabelText(Return);
	}

	void TeleportPlayerToApartment(Menu::PlayerVars Player, int ID) {
		if (Global::Vars::g_NetworkMenuType == ALL_PLAYERS) {
			Menu::GetPlayerManager()->Foreach([=] (Menu::PlayerVars& P) {
				Menu::Helpers::TriggerScriptEvent({ Rage::Global::ApartmentTeleport, P.m_ID, 0, 0, ID, ID }, 6, P.m_ID);
			});
		} else {
			Menu::Helpers::TriggerScriptEvent({ Rage::Global::ApartmentTeleport, Player.m_ID, 0, 0, ID, ID }, 6, Player.m_ID);
		}
	}
}

void NetworkTeleportApartmentMenu::Init() {
	SetName("Apartments");
	SetParentSubmenu<NetworkPlayersTeleportMenu>();
}

void NetworkTeleportApartmentMenu::Update() {}

/*Called once on submenu open*/
void NetworkTeleportApartmentMenu::UpdateOnce() {
	ClearOptionsOffset(0);
	
	for (int i = 1; i < 116; i++) {
		const char* Location = GetLocationFromID(i);
		if (Location == nullptr || Location == "") continue;

		addOption(ButtonOption(Location)
			.addOnClick([=] { TeleportPlayerToApartment(Menu::GetSelectedPlayer(), i); }));
	}
}

/*Called always*/
void NetworkTeleportApartmentMenu::FeatureUpdate() {}

NetworkTeleportApartmentMenu* _instance;
NetworkTeleportApartmentMenu* NetworkTeleportApartmentMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkTeleportApartmentMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkTeleportApartmentMenu::~NetworkTeleportApartmentMenu() { delete _instance; }