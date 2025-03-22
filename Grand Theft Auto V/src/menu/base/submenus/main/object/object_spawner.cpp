#include "object_spawner.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../object.hpp"
#include "spawner/object_spawner_settings.hpp"
#include "spawner/object_spawner_select.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "spawner/object_spawner_manage.hpp"

using namespace ObjectSpawnerMenuVars;

namespace ObjectSpawnerMenuVars {
	Vars_ m_Vars;

	void SpawnObject(uint32_t ModelHash, const char* Name) {
		if (Native::IsModelInCdimage(ModelHash)) {
			Menu::GetControlManager()->RequestModel(ModelHash, [=] (uint32_t Model) {
				auto Coords = Menu::GetLocalPlayer().m_Coords + (Native::GetEntityForwardVector(Menu::GetLocalPlayer().m_Entity) * 2.f);
				Entity Object = Native::CreateObject(Model, Coords.m_X, Coords.m_Y, Coords.m_Z, true, true, false);

				if (Native::DoesEntityExist(Object)) {
					std::string N = Name;
					if (Name == nullptr) {
						N = std::to_string(Model);
					}

					if (ObjectSpawnerSettingsMenuVars::m_Vars.m_Fade) {
						Native::NetworkFadeInEntity(Object, true, false);
					}

					if (ObjectSpawnerSettingsMenuVars::m_Vars.m_Blip) {
						Blip B = Native::AddBlipForEntity(Object);
						Native::SetBlipSprite(B, 408);
						Native::SetBlipColour(B, 17);
						Native::SetBlipDisplay(B, 8);
						Native::SetBlipCategory(B, 4);
					}

					if (ObjectSpawnerSettingsMenuVars::m_Vars.m_ParticleFX) {
						Native::RequestNamedPtfxAsset("proj_indep_firework_v2");
						Native::_UseParticleFxAssetNextCall("proj_indep_firework_v2");
						Native::_StartParticleFxNonLoopedAtCoord2("scr_firework_indep_repeat_burst_rwb", Coords.m_X, Coords.m_Y, Coords.m_Z, 0, 0, 0, 1.2f, true, true, true);
					}

					if (ObjectSpawnerSettingsMenuVars::m_Vars.m_Notification) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s!", N.c_str(), ObjectSpawnerMenu::GetInstance()->getString("spawned").c_str()));
					}

					ObjectSpawnerManageMenuVars::m_Vars.m_SpawnedObjects.push_back({ Object, Model, N, Native::GetGameTimer() });
				}
			});
		}
	}
}

void ObjectSpawnerMenu::Init() {
	SetName("Object Spawner");
	SetParentSubmenu<ObjectMenu>();

	addString("spawned");

	addOption(SubmenuOption("Settings")
		.addTranslation()
		.addSubmenu<ObjectSpawnerSettingsMenu>());

	addOption(SubmenuOption("Manage Spawned Objects")
		.addTranslation()
		.addSubmenu<ObjectSpawnerManageMenu>());

	addOption(ButtonOption("Input Custom Object")
		.addTranslation()
		.addKeyboard("Input the model! (Hint: name, hash or numeric)", 30, [] (const char* ModelName) {
			if (ModelName) {
				if (ModelName[0] == '0' && (ModelName[1] == 'x' || ModelName[1] == 'X')) {
					// hash
					SpawnObject((uint32_t)_strtoui64(ModelName, 0, 0), ModelName);
				} else {
					std::string StringName(ModelName);
					if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
						// numeric
						SpawnObject((uint32_t)atoi(ModelName), ModelName);
					} else {
						// name
						SpawnObject(Native::GetHashKey(ModelName), ModelName);
					}
				}
			}
		}));

	addOption(BreakOption("Categories")
		.addTranslation());

	addOption(SubmenuOption("Small")
		.addTranslation()
		.addOnClick([] { ObjectSpawnerSelectMenuVars::m_Vars.m_ListPtr = Global::Arrays::g_ObjectModelSmall; ObjectSpawnerSelectMenuVars::m_Vars.m_ListSize = NUMOF(Global::Arrays::g_ObjectModelSmall); })
		.addSubmenu<ObjectSpawnerSelectMenu>());

	addOption(SubmenuOption("Medium")
		.addTranslation()
		.addOnClick([] { ObjectSpawnerSelectMenuVars::m_Vars.m_ListPtr = Global::Arrays::g_ObjectModelMedium; ObjectSpawnerSelectMenuVars::m_Vars.m_ListSize = NUMOF(Global::Arrays::g_ObjectModelMedium); })
		.addSubmenu<ObjectSpawnerSelectMenu>());

	addOption(SubmenuOption("Large")
		.addTranslation()
		.addOnClick([] { ObjectSpawnerSelectMenuVars::m_Vars.m_ListPtr = Global::Arrays::g_ObjectModelLarge; ObjectSpawnerSelectMenuVars::m_Vars.m_ListSize = NUMOF(Global::Arrays::g_ObjectModelLarge); })
		.addSubmenu<ObjectSpawnerSelectMenu>());
}

void ObjectSpawnerMenu::Update() {}

/*Called once on submenu open*/
void ObjectSpawnerMenu::UpdateOnce() {}

/*Called always*/
void ObjectSpawnerMenu::FeatureUpdate() {}

ObjectSpawnerMenu* _instance;
ObjectSpawnerMenu* ObjectSpawnerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ObjectSpawnerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ObjectSpawnerMenu::~ObjectSpawnerMenu() { delete _instance; }