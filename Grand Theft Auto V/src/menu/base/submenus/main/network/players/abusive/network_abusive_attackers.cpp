#include "network_abusive_attackers.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_players_abusive.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/submenus/main/network/network_session.hpp"

using namespace NetworkAbusiveAttackersMenuVars;

namespace NetworkAbusiveAttackersMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Type[] = {
		{ TranslationString("Military Jets", true), 0 },
		{ TranslationString("Military Buzzards", true), 1 },
	};

	void SpawnAttackers(Menu::PlayerVars Player) {
		std::vector<Entity> Entities;

		if (Player.m_AttackerType == 0) { // Military Jets
			Menu::GetControlManager()->SimpleRequestModel(0xB39B0AE6); // lazer

			for (int i = 0; i < Player.m_AttackerCount; i++) {
				Math::Vector3_<float> SpawnCoords = { Player.m_Coords.m_X + Native::GetRandomFloatInRange(0.f, 100.f), Player.m_Coords.m_Y + Native::GetRandomFloatInRange(0.f, 100.f), Player.m_Coords.m_Z + 100.f + (i * 10.f) };

				Vehicle Vehicle = Native::CreateVehicle(0xB39B0AE6, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, Player.m_Heading, true, true, 0);
				if (Native::DoesEntityExist(Vehicle)) {
					Native::ApplyForceToEntity(Vehicle, 1, 0.f, 0.f, 50.f, 0.f, 0.f, 0.f, 0, true, true, true, false, true);

					Entities.push_back(Vehicle);

					Ped Ped = Native::CreateRandomPedAsDriver(Vehicle, true);
					if (Native::DoesEntityExist(Ped)) {
						Native::TaskPlaneChase(Ped, Player.m_Ped, 0.f, 0.f, 50.f);
						Native::TaskCombatPed(Ped, Player.m_Ped, 0, 16);
						Native::SetPedKeepTask(Ped, true);
						Entities.push_back(Ped);
					}
				}
			}
		}

		if (Player.m_AttackerType == 1) { // Military Buzzards
			Menu::GetControlManager()->SimpleRequestModel(0x2f03547b); // buzzard
			Menu::GetControlManager()->SimpleRequestModel(0x613E626C); // mw

			for (int i = 0; i < Player.m_AttackerCount; i++) {
				Math::Vector3_<float> SpawnCoords = { Player.m_Coords.m_X + Native::GetRandomFloatInRange(0.f, 100.f), Player.m_Coords.m_Y + Native::GetRandomFloatInRange(0.f, 100.f), Player.m_Coords.m_Z + 100.f + (i * 10.f) };

				Vehicle Vehicle = Native::CreateVehicle(0x2f03547b, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, Player.m_Heading, true, true, 0);
				if (Native::DoesEntityExist(Vehicle)) {
					Native::ApplyForceToEntity(Vehicle, 1, 0.f, 0.f, 50.f, 0.f, 0.f, 0.f, 0, true, true, true, false, true);

					Entities.push_back(Vehicle);

					Ped Ped = Native::CreateRandomPedAsDriver(Vehicle, true);
					if (Native::DoesEntityExist(Ped)) {
						Entities.push_back(Ped);

						Native::TaskHeliChase(Ped, Player.m_Ped, 0.f, 0.f, 50.f);
						Native::TaskCombatPed(Ped, Player.m_Ped, 0, 16);
						Native::SetPedKeepTask(Ped, true);

						for (int j = 0; j < Native::GetVehicleMaxNumberOfPassengers(Vehicle); j++) {
							Ped = Native::CreatePedInsideVehicle(Vehicle, 29, 0x613E626C, i, true, false);
							if (Native::DoesEntityExist(Ped)) {
								Entities.push_back(Ped);

								Native::SetPedCombatAbility(Ped, 100);
								Native::GiveDelayedWeaponToPed(Ped, 0x394f415c, 9999, true);
								Native::TaskCombatPed(Ped, Player.m_Ped, 0, 16);
								Native::SetPedKeepTask(Ped, true);
							}
						}
					}
				}
			}
		}

		for (auto& E : Entities) {
			Native::SetEntityInvincible(E, Player.m_AttackerInvincibility);
			Native::SetEntityVisible(E, !Player.m_AttackerInvisibility, true);
		}
	}
}

void NetworkAbusiveAttackersMenu::Init() {
	SetName("Attackers");
	SetParentSubmenu<NetworkPlayersAbusiveMenu>();

	addString("Type");
	addString("Invincibility");
	addString("Invisibility");
	addString("Count");
	addString("Spawn Attackers");
}

void NetworkAbusiveAttackersMenu::Update() {}

/*Called once on submenu open*/
void NetworkAbusiveAttackersMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (Global::Vars::g_NetworkMenuType == SELECTED_PLAYER) {
		SetParentSubmenu<NetworkPlayersAbusiveMenu>();

		addOption(ScrollOption<int>(SCROLL, getString("Type"))
			.addScroll(Menu::GetSelectedPlayer().m_AttackerType, 0, NUMOF(Type), Type)
			.addOnUpdate([](ScrollOption<int>* Option) { Option->addScroll(Menu::GetSelectedPlayer().m_AttackerType, 0, NUMOF(Type), Type); }));

		addOption(ToggleOption(getString("Invincibility"))
			.addToggle(Menu::GetSelectedPlayer().m_AttackerInvincibility)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_AttackerInvincibility); }));

		addOption(ToggleOption(getString("Invisibility"))
			.addToggle(Menu::GetSelectedPlayer().m_AttackerInvisibility)
			.addOnUpdate([](ToggleOption* Option) { Option->addToggle(Menu::GetSelectedPlayer().m_AttackerInvisibility); }));

		addOption(NumberOption<int>(SCROLL, getString("Count"))
			.addNumber(Menu::GetSelectedPlayer().m_AttackerCount, "%i", 1).addMin(1).addMax(100)
			.addOnUpdate([](NumberOption<int>* Option) { Option->addNumber(Menu::GetSelectedPlayer().m_AttackerCount, "%i", 1); }));

		addOption(ButtonOption(getString("Spawn Attackers"))
			.addOnClick([] { SpawnAttackers(Menu::GetSelectedPlayer()); }));
	}
	else {
		SetParentSubmenu<NetworkPlayersAbusiveMenu>();

		addOption(ScrollOption<int>(SCROLL, getString("Type"))
			.addScroll(m_Vars.m_AttackerType, 0, NUMOF(Type), Type));

		addOption(ToggleOption(getString("Invincibility"))
			.addToggle(m_Vars.m_AttackerInvincibility));

		addOption(ToggleOption(getString("Invisibility"))
			.addToggle(m_Vars.m_AttackerInvisibility));

		addOption(NumberOption<int>(SCROLL, getString("Count"))
			.addNumber(m_Vars.m_AttackerCount, "%i", 1).addMin(1).addMax(100));

		addOption(ButtonOption(getString("Spawn Attackers"))
			.addOnClick([] {
				Menu::GetPlayerManager()->Foreach([](Menu::PlayerVars& Player) {
					SpawnAttackers(Player);
					});
			}));
	}
}

/*Called always*/
void NetworkAbusiveAttackersMenu::FeatureUpdate() {}

NetworkAbusiveAttackersMenu* _instance;
NetworkAbusiveAttackersMenu* NetworkAbusiveAttackersMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAbusiveAttackersMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAbusiveAttackersMenu::~NetworkAbusiveAttackersMenu() { delete _instance; }