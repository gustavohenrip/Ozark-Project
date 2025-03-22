#include "object_building.hpp"
#include "menu/base/submenu_handler.hpp"
#include "menu/base/util/control_manager.hpp"
#include "../object.hpp"

using namespace ObjectBuildingMenuVars;

namespace ObjectBuildingMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Types[] = {
		{ TranslationString("Half Pipe - Straight", true), 0 },
		{ TranslationString("Half Pipe - Curved Wide", true), 1 },
		{ TranslationString("Half Pipe - Curved Small", true), 2 },
		{ TranslationString("Forest", true), 3 },
	};

	std::vector<Object> HalfPipeTest;

	void BuildStraightHalfPipe() {
		Menu::GetControlManager()->RequestModel(0xF66F582F, [] (uint32_t Model) {
			Math::Vector3<float> MinDimensions;
			Math::Vector3<float> MaxDimensions;

			Native::GetModelDimensions(Model, &MinDimensions, &MaxDimensions);
			Math::Vector3<float> Diameter = MaxDimensions - MinDimensions;

			Math::Vector3<float> SpawnCoords = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Ped) * 10.f);
			Native::GetGroundZFor3DCoord(SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, &SpawnCoords.m_Z, false);

			Object FirstObject = Native::CreateObject(Model, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, true, true, false);
			if (Native::DoesEntityExist(FirstObject)) {
				HalfPipeTest.push_back(FirstObject);
				Native::SetEntityRotation(FirstObject, -m_Vars.m_VerticalAngle, 0.f, Menu::GetLocalPlayer().m_Heading, 2, true);
				Native::FreezeEntityPosition(FirstObject, true);

				Object NextObject = FirstObject;
				for (int i = 0; i < m_Vars.m_Count[0]; i++) {
					Math::Vector3<float> Next = Native::GetOffsetFromEntityInWorldCoords(NextObject, 0.f, Diameter.m_Y - 0.15f, 0.f);
					NextObject = Native::CreateObject(Model, Next.m_X, Next.m_Y, Next.m_Z, true, true, false);
					if (Native::DoesEntityExist(NextObject)) {
						Native::SetEntityRotation(NextObject, -m_Vars.m_VerticalAngle, 0.f, Menu::GetLocalPlayer().m_Heading, 2, true);
						Native::FreezeEntityPosition(NextObject, true);

						HalfPipeTest.push_back(NextObject);
					}
				}
			}
		});
	}

	void BuildCurvedHalfPipeWide() {
		Menu::GetControlManager()->RequestModel(0xF66F582F, [] (uint32_t Model) {
			Math::Vector3<float> MinDimensions;
			Math::Vector3<float> MaxDimensions;

			Native::GetModelDimensions(Model, &MinDimensions, &MaxDimensions);
			Math::Vector3<float> Diameter = MaxDimensions - MinDimensions;

			Math::Vector3<float> SpawnCoords = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Ped) * 10.f);
			Native::GetGroundZFor3DCoord(SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, &SpawnCoords.m_Z, false);

			Object FirstObject = Native::CreateObject(Model, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, true, true, false);
			if (Native::DoesEntityExist(FirstObject)) {
				Native::SetEntityHeading(FirstObject, Menu::GetLocalPlayer().m_Heading);
				Native::FreezeEntityPosition(FirstObject, true);

				Object NextObject = FirstObject;
				for (int i = 0; i < m_Vars.m_Count[1]; i++) {
					Math::Vector3<float> Next = Native::GetOffsetFromEntityInWorldCoords(NextObject, 0.4f, Diameter.m_Y - 0.15f - 0.7f, 0.f);
					NextObject = Native::CreateObject(Model, Next.m_X, Next.m_Y, Next.m_Z, true, true, false);
					if (Native::DoesEntityExist(NextObject)) {
						Native::SetEntityHeading(NextObject, Menu::GetLocalPlayer().m_Heading - ((i + 1) * 5.f));
						Native::FreezeEntityPosition(NextObject, true);
					}
				}
			}
		});
	}

	void BuildCurvedHalfPipeSmall() {
		Menu::GetControlManager()->RequestModel(0xF66F582F, [] (uint32_t Model) {
			Math::Vector3<float> MinDimensions;
			Math::Vector3<float> MaxDimensions;

			Native::GetModelDimensions(Model, &MinDimensions, &MaxDimensions);
			Math::Vector3<float> Diameter = MaxDimensions - MinDimensions;

			Math::Vector3<float> SpawnCoords = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Ped) * 10.f);
			Native::GetGroundZFor3DCoord(SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, &SpawnCoords.m_Z, false);

			Object FirstObject = Native::CreateObject(Model, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, true, true, false);
			if (Native::DoesEntityExist(FirstObject)) {
				Native::SetEntityHeading(FirstObject, Menu::GetLocalPlayer().m_Heading);
				Native::FreezeEntityPosition(FirstObject, true);

				Object NextObject = FirstObject;
				for (int i = 0; i < m_Vars.m_Count[2]; i++) {
					Math::Vector3<float> Next = Native::GetOffsetFromEntityInWorldCoords(NextObject, 0.7f, Diameter.m_Y - 0.15f - 1.4f, 0.f);
					NextObject = Native::CreateObject(Model, Next.m_X, Next.m_Y, Next.m_Z, true, true, false);
					if (Native::DoesEntityExist(NextObject)) {
						Native::SetEntityHeading(NextObject, Menu::GetLocalPlayer().m_Heading - ((i + 1) * 10.5f));
						Native::FreezeEntityPosition(NextObject, true);
					}
				}
			}
		});
	}

	void BuildForest() {
		uint32_t Trees[] = {
			0xE43D331B,
			0x16770e68,
			0xb355b730,
			0xEDF5CD3C,
			0x4D11F2B,
			0x49e472aa
		};

		Menu::GetControlManager()->RequestModel(0xEDF5CD3C, [=] (uint32_t Model) {Math::Vector3<float> MinDimensions;
			Math::Vector3<float> SpawnCoords = Menu::GetLocalPlayer().m_Coords;
			Native::GetGroundZFor3DCoord(SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, &SpawnCoords.m_Z, false);

			Object FirstObject = Native::CreateObject(Model, SpawnCoords.m_X, SpawnCoords.m_Y, SpawnCoords.m_Z, true, true, false);
			if (Native::DoesEntityExist(FirstObject)) {
				Native::FreezeEntityPosition(FirstObject, true);

				Object NextObject = FirstObject;
				for (int i = 0; i < m_Vars.m_Count[3]; i++) {
					Math::Vector3<float> Next = Native::GetOffsetFromEntityInWorldCoords(NextObject, Native::GetRandomFloatInRange(-10.f, 10.f), Native::GetRandomFloatInRange(-10.f, 10.f), 0.f);
					uint32_t ModelHash = Trees[Native::GetRandomIntInRange(0, 5)];
					
					NextObject = Native::CreateObject(ModelHash, Next.m_X, Next.m_Y, SpawnCoords.m_Z - (ModelHash == 0xb355b730 ? 0.f : 3.f), true, true, false);
					if (Native::DoesEntityExist(NextObject)) {
						Native::FreezeEntityPosition(NextObject, true);
					}
				}
			}
		});
	}

	void Create() {
		switch (m_Vars.m_Type) {
			case 0: BuildStraightHalfPipe(); break;
			case 1: BuildCurvedHalfPipeWide(); break;
			case 2: BuildCurvedHalfPipeSmall(); break;
			case 3: BuildForest(); break;
		}
	}
}

void ObjectBuildingMenu::Init() {
	SetName("Object Building");
	SetParentSubmenu<ObjectMenu>();

	addOption(ScrollOption<int>(SCROLL, "Type")
		.addTranslation()
		.addScroll(m_Vars.m_Type, 0, NUMOF(Types), Types));

	addOption(NumberOption<int>(SCROLL, "Count")
		.addTranslation()
		.addNumber(m_Vars.m_Count[0], "%i", 1).addMin(1).addMax(50)
		.addOnUpdate([] (NumberOption<int>* Option) { Option->addNumber(m_Vars.m_Count[m_Vars.m_Type], "%i", 1); }));
	
	addOption(NumberOption<float>(SCROLL, "Vertical Angle")
		.addTranslation()
		.addNumber(m_Vars.m_VerticalAngle, "%.0f", 1.f).addMin(-90.f).addMax(90.f)
		.addOnHover([] { Menu::GetRenderer()->DrawSprite("Ozark", "half_pipe_side", { 0.5f, 0.5f }, { 0.1f, 0.1f }, -m_Vars.m_VerticalAngle, { 255, 255, 255, 255 }); })
		.addRequirement([] { return m_Vars.m_Type == 0; }));

	addOption(ButtonOption("Create Building")
		.addTranslation()
		.addOnClick(Create));
}

void ObjectBuildingMenu::Update() {}

/*Called once on submenu open*/
void ObjectBuildingMenu::UpdateOnce() {}

/*Called always*/
void ObjectBuildingMenu::FeatureUpdate() {}

ObjectBuildingMenu* _instance;
ObjectBuildingMenu* ObjectBuildingMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectBuildingMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectBuildingMenu::~ObjectBuildingMenu() { delete _instance; }