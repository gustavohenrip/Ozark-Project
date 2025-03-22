#include "world_trains.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../world.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/pools/pool_manager.hpp"

using namespace WorldTrainMenuVars;

namespace WorldTrainMenuVars {
	Vars_ m_Vars;

	ScrollStruct<float> TrainSpeeds[] = {
		{ TranslationString("-500", false), -500.f },
		{ TranslationString("-250", false), -250.f },
		{ TranslationString("-100", false), -100.f },
		{ TranslationString("-50", false), -50.f },
		{ TranslationString("-25", false), -25.f },
		{ TranslationString("-10", false), -10.f },
		{ TranslationString("-1", false), -1.f },
		{ TranslationString("0", false), 0.f },
		{ TranslationString("1", false), 1.f },
		{ TranslationString("10", false), 10.f },
		{ TranslationString("25", false), 25.f },
		{ TranslationString("50", false), 50.f },
		{ TranslationString("100", false), 100.f },
		{ TranslationString("250", false), 2500.f },
		{ TranslationString("500", false), 500.f }
	};

	ScrollStruct<int> TrainPaints[] = {
		{ TranslationString("Chrome", true), 120 },
		{ TranslationString("Gold", true), 158 },
		{ TranslationString("Pink", true), 135 },
		{ TranslationString("Blue", true), 140 }
	};

	void EnterTrain() {
		if (!Native::IsVehicleSeatFree(m_Vars.m_Train, -1, 0)) {
			Ped P = Native::GetPedInVehicleSeat(m_Vars.m_Train, -1, 0);
			Native::SetEntityAsMissionEntity(P, true, true);
			Native::DeleteEntity(&P);
		}

		Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, m_Vars.m_Train, -1);
	}

	void PaintPaint() {
		Menu::GetControlManager()->SimpleRequestControl(m_Vars.m_Train);
		Native::SetVehicleColours(m_Vars.m_Train, TrainPaints[m_Vars.m_Paint].m_Result, TrainPaints[m_Vars.m_Paint].m_Result);

		for (int i = 1; i < 50; i++) {
			Vehicle Carriage = Native::GetTrainCarriage(m_Vars.m_Train, i);
			if (Native::DoesEntityExist(Carriage)) {
				Native::SetVehicleColours(Carriage, TrainPaints[m_Vars.m_Paint].m_Result, TrainPaints[m_Vars.m_Paint].m_Result);
			}
		}
	}

	void DeleteCarriage() {
		std::vector<Vehicle> Carriages;

		for (int i = 1; i < 50; i++) {
			Vehicle Carriage = Native::GetTrainCarriage(m_Vars.m_Train, i);
			if (Native::DoesEntityExist(Carriage)) {
				Carriages.push_back(Carriage);
			}
		}

		for (auto& Carriage : Carriages) {
			Menu::GetControlManager()->SimpleRequestControl(Carriage);
			Native::SetEntityAsMissionEntity(Carriage, true, true);
			Native::DeleteEntity(&Carriage);
		}
	}

	void DeleteTrain() {
		DeleteCarriage();
		Menu::GetControlManager()->SimpleRequestControl(m_Vars.m_Train);
		Native::SetEntityAsMissionEntity(m_Vars.m_Train, true, true);
		Native::DeleteMissionTrain(&m_Vars.m_Train);
	}

	void FindTrain() {
		float ClosestDistance = std::numeric_limits<float>::max();
		Entity ClosestEntity = 0;

		Menu::Pools::GetVehiclePool()->Run([&] (Entity Veh) {
			if (Native::GetEntityModel(Veh) == 0x3D6AAA9B) {
				float Distance = Menu::GetLocalPlayer().m_Coords.GetMathmaticalDistance(Native::GetEntityCoords(Veh, true));
				if (Distance < ClosestDistance) {
					ClosestDistance = Distance;
					ClosestEntity = Veh;
				}
			}
		});

		if (ClosestEntity != 0) {
			m_Vars.m_Train = ClosestEntity;
		} else {
			Menu::GetNotify()->NotifyStacked(WorldTrainMenu::GetInstance()->getString("Couldn't find a train"), Global::UIVars::g_NotificationError);
		}
	}

	bool Requirement() {
		return m_Vars.m_Train && Native::DoesEntityExist(m_Vars.m_Train);
	}
}

void WorldTrainMenu::Init() {
	SetName("Train");
	SetParentSubmenu<WorldMenu>();

	addOption(ToggleOption("Allow Random Trains")
		.addTranslation()
		.addToggle(m_Vars.m_AllowRandom)
		.addOnClick([] { Native::SetRandomTrains(m_Vars.m_AllowRandom); }));

	addOption(ButtonOption("Find Train")
		.addTranslation()
		.addOnClick(FindTrain)
		.addTooltip("Must be near train tracks"));

	addOption(ButtonOption("Spawn Train")
		.addTranslation()
		.addOnClick([] { m_Vars.m_Spawn = true; })
		.addTooltip("Must be near train tracks"));

	addOption(BreakOption("Spawned Train")
		.addTranslation()
		.addRequirement(Requirement));

	addOption(ButtonOption("Enter Train")
		.addTranslation()
		.addOnClick(EnterTrain)
		.addRequirement(Requirement));

	addOption(ScrollOption<float>(TOGGLE, "Speed")
		.addTranslation()
		.addToggle(m_Vars.m_Speed)
		.addScroll(m_Vars.m_SpeedVal, 0, NUMOF(TrainSpeeds), TrainSpeeds)
		.addOnClick([] { Menu::GetControlManager()->SimpleRequestControl(m_Vars.m_Train); })
		.addRequirement(Requirement));

	addOption(ScrollOption<int>(SCROLLSELECT, "Paint")
		.addTranslation()
		.addScroll(m_Vars.m_Paint, 0, NUMOF(TrainPaints), TrainPaints)
		.addOnClick(PaintPaint)
		.addRequirement(Requirement));

	addOption(ToggleOption("Derail")
		.addTranslation()
		.addToggle(m_Vars.m_Derail)
		.addOnClick([] { Native::SetRenderTrainAsDerailed(m_Vars.m_Train, m_Vars.m_Derail); })
		.addRequirement(Requirement));

	addOption(ButtonOption("Delete Carriages")
		.addTranslation()
		.addOnClick(DeleteCarriage)
		.addRequirement(Requirement));

	addOption(ButtonOption("Delete Train")
		.addTranslation()
		.addOnClick(DeleteTrain)
		.addRequirement(Requirement));

	addString("Couldn't find a train");
	addString("Manager reset, cached train too far");
	addString("You must delete the existing train first");
}

void WorldTrainMenu::Update() {
	if (Requirement()) {
		if (Menu::GetLocalPlayer().m_Coords.GetMathmaticalDistance(Native::GetEntityCoords(m_Vars.m_Train, false)) > 100.f) {
			m_Vars.m_Train = 0;
			Menu::GetNotify()->NotifyStacked(getString("Manager reset, cached train too far"), Global::UIVars::g_NotificationError);
		}
	} else {
		if (Menu::GetBase()->m_CurrentOption > 3) Menu::GetBase()->m_CurrentOption = 0;
	}
}

/*Called once on submenu open*/
void WorldTrainMenu::UpdateOnce() {}

/*Called always*/
void WorldTrainMenu::FeatureUpdate() {
	if (m_Vars.m_Spawn) {
		m_Vars.m_Spawn = false;

		if (m_Vars.m_Train && Native::DoesEntityExist(m_Vars.m_Train)) {
			Menu::GetNotify()->NotifyStacked(getString("You must delete the existing train first"), Global::UIVars::g_NotificationError);
			return;
		}

		Menu::GetControlManager()->SimpleRequestModel(0x3D6AAA9B);
		Menu::GetControlManager()->SimpleRequestModel(0x0AFD22A6);
		Menu::GetControlManager()->SimpleRequestModel(0x36DCFF98);
		Menu::GetControlManager()->SimpleRequestModel(0x0E512E79);
		Menu::GetControlManager()->SimpleRequestModel(0x264D9262);
		Menu::GetControlManager()->SimpleRequestModel(0xD1ABB666);

		m_Vars.m_Train = Native::CreateMissionTrain(15, Menu::GetLocalPlayer().m_Coords.m_X, Menu::GetLocalPlayer().m_Coords.m_Y, Menu::GetLocalPlayer().m_Coords.m_Z, true);
	}

	if (m_Vars.m_Speed) {
		if (m_Vars.m_Train) {
			if (Native::DoesEntityExist(m_Vars.m_Train)) {
				if (Native::GetPedInVehicleSeat(m_Vars.m_Train, -1, 0) == Menu::GetLocalPlayer().m_Ped) {
					Native::SetTrainSpeed(m_Vars.m_Train, TrainSpeeds[m_Vars.m_SpeedVal].m_Result);
					Native::SetTrainCruiseSpeed(m_Vars.m_Train, TrainSpeeds[m_Vars.m_SpeedVal].m_Result);
					return;
				}
			}
		}

		m_Vars.m_Speed = false;
	}
}

WorldTrainMenu* _instance;
WorldTrainMenu* WorldTrainMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WorldTrainMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WorldTrainMenu::~WorldTrainMenu() { delete _instance; }