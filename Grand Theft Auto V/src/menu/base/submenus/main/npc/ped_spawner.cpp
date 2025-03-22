#include "ped_spawner.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../ped.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/control_manager.hpp"
#include "utils/va.hpp"
#include "menu/base/util/notify.hpp"
#include "spawner/ped_spawner_settings.hpp"
#include "spawner/ped_spawner_manage.hpp"

using namespace PedSpawnerMenuVars;

namespace PedSpawnerMenuVars {
	Vars_ m_Vars;

	void SpawnPed(uint32_t ModelHash, const char* Name) {
		if (Native::IsModelInCdimage(ModelHash)) {
			Menu::GetControlManager()->RequestModel(ModelHash, [=] (uint32_t Model) {
				Menu::PlayerVars Target;
				if (Menu::GetSubmenuHandler()->GetPreviousSubmenu() == PedMenu::GetInstance()) {
					Target = Menu::GetLocalPlayer();
				}
				else {
					Target = Menu::GetSelectedPlayer();
				}
				auto Coords = Target.m_Coords + (Native::GetEntityForwardVector(Target.m_Entity) * 2.f);
				Entity Ped = Native::CreatePed(21, Model, Coords.m_X, Coords.m_Y, Coords.m_Z, 0.f, true, false);

				if (Native::DoesEntityExist(Ped)) {
					std::string N = Name;
					if (Name == nullptr) {
						N = std::to_string(Model);
					}

					if (PedSpawnerSettingsMenuVars::m_Vars.m_Fade) {
						Native::NetworkFadeInEntity(Ped, true, false);
					}

					if (PedSpawnerSettingsMenuVars::m_Vars.m_Blip) {
						Blip B = Native::AddBlipForEntity(Ped);
						Native::SetBlipSprite(B, 280);
						Native::SetBlipColour(B, 2);
						Native::SetBlipDisplay(B, 8);
						Native::SetBlipCategory(B, 4);
					}

					if (PedSpawnerSettingsMenuVars::m_Vars.m_ParticleFX) {
						Native::RequestNamedPtfxAsset("proj_indep_firework_v2");
						Native::_UseParticleFxAssetNextCall("proj_indep_firework_v2");
						Native::_StartParticleFxNonLoopedAtCoord2("scr_firework_indep_repeat_burst_rwb", Coords.m_X, Coords.m_Y, Coords.m_Z, 0, 0, 0, 1.2f, true, true, true);
					}

					if (PedSpawnerSettingsMenuVars::m_Vars.m_Notification) {
						Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s!", N.c_str(), PedSpawnerMenu::GetInstance()->getString("spawned").c_str()));
					}

					PedSpawnerManageMenuVars::m_Vars.m_SpawnedPeds.push_back({ Ped, Model, N, Native::GetGameTimer() });
				}
			});
		}
	}
}

void PedSpawnerMenu::Init() {
	SetName("NPC Spawner");
	SetParentSubmenu<PedMenu>();

	addString("Spawned");

	addOption(SubmenuOption("Settings")
		.addTranslation()
		.addSubmenu<PedSpawnerSettingsMenu>());

	addOption(SubmenuOption("Manage Spawned NPCs")
		.addTranslation()
		.addSubmenu<PedSpawnerManageMenu>());

	addOption(ButtonOption("Input Custom NPC")
		.addTranslation()
		.addKeyboard("Input the model! (Hint: name, hash or numeric)", 30, [] (const char* ModelName) {
		if (ModelName) {
			if (ModelName[0] == '0' && (ModelName[1] == 'x' || ModelName[1] == 'X')) {
				// hash
				SpawnPed((uint32_t)_strtoui64(ModelName, 0, 0), ModelName);
			} else {
				std::string StringName(ModelName);
				if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
					// numeric
					SpawnPed((uint32_t)atoi(ModelName), ModelName);
				} else {
					// name
					SpawnPed(Native::GetHashKey(ModelName), ModelName);
				}
			}
		}
	}));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Popular")
		.addTranslation()
		.addScroll(m_Vars.m_Popular, 0, NUMOF(Global::Arrays::g_PedModelPopular), Global::Arrays::g_PedModelPopular).canLoop()
		.addOnClick([] { SpawnPed(Global::Arrays::g_PedModelPopular[m_Vars.m_Popular].m_Result, Global::Arrays::g_PedModelPopular[m_Vars.m_Popular].m_Name.Get().c_str()); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Story")
		.addTranslation()
		.addScroll(m_Vars.m_Story, 0, NUMOF(Global::Arrays::g_PedModelStory), Global::Arrays::g_PedModelStory).canLoop()
		.addOnClick([] { SpawnPed(Global::Arrays::g_PedModelStory[m_Vars.m_Story].m_Result, Global::Arrays::g_PedModelStory[m_Vars.m_Story].m_Name.Get().c_str()); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Animals")
		.addTranslation()
		.addScroll(m_Vars.m_Animals, 0, NUMOF(Global::Arrays::g_PedModelAnimal), Global::Arrays::g_PedModelAnimal).canLoop()
		.addOnClick([] { SpawnPed(Global::Arrays::g_PedModelAnimal[m_Vars.m_Animals].m_Result, Global::Arrays::g_PedModelAnimal[m_Vars.m_Animals].m_Name.Get().c_str()); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Emergency")
		.addTranslation()
		.addScroll(m_Vars.m_Emergency, 0, NUMOF(Global::Arrays::g_PedModelEmergency), Global::Arrays::g_PedModelEmergency).canLoop()
		.addOnClick([] { SpawnPed(Global::Arrays::g_PedModelEmergency[m_Vars.m_Emergency].m_Result, Global::Arrays::g_PedModelEmergency[m_Vars.m_Emergency].m_Name.Get().c_str()); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Role")
		.addTranslation()
		.addScroll(m_Vars.m_Role, 0, NUMOF(Global::Arrays::g_PedModelRole), Global::Arrays::g_PedModelRole).canLoop()
		.addOnClick([] { SpawnPed(Global::Arrays::g_PedModelRole[m_Vars.m_Role].m_Result, Global::Arrays::g_PedModelRole[m_Vars.m_Role].m_Name.Get().c_str()); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Ambient")
		.addTranslation()
		.addScroll(m_Vars.m_Ambient, 0, NUMOF(Global::Arrays::g_PedModelAmbient), Global::Arrays::g_PedModelAmbient).canLoop()
		.addOnClick([] { SpawnPed(Global::Arrays::g_PedModelAmbient[m_Vars.m_Ambient].m_Result, Global::Arrays::g_PedModelAmbient[m_Vars.m_Ambient].m_Name.Get().c_str()); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Misc")
		.addTranslation()
		.addScroll(m_Vars.m_Misc, 0, NUMOF(Global::Arrays::g_PedModelMisc), Global::Arrays::g_PedModelMisc).canLoop()
		.addOnClick([] { SpawnPed(Global::Arrays::g_PedModelMisc[m_Vars.m_Misc].m_Result, Global::Arrays::g_PedModelMisc[m_Vars.m_Misc].m_Name.Get().c_str()); }));
}

void PedSpawnerMenu::Update() {}

/*Called once on submenu open*/
void PedSpawnerMenu::UpdateOnce() {}

/*Called always*/
void PedSpawnerMenu::FeatureUpdate() {}

PedSpawnerMenu* _instance;
PedSpawnerMenu* PedSpawnerMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PedSpawnerMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

PedSpawnerMenu::~PedSpawnerMenu() { delete _instance; }