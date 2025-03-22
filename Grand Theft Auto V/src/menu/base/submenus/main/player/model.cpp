#include "model.hpp"
#include "menu/base/submenu_handler.hpp"
#include "appearance.hpp"
#include "menu/base/util/control_manager.hpp"
#include "global/arrays.hpp"
#include "utils/fibers.hpp"

using namespace ModelMenuVars;

namespace ModelMenuVars {
	Vars_ m_Vars;

	void ChangeModel(uint32_t Hash) {
		if (Native::IsModelInCdimage(Hash)) {
			if (Native::IsModelValid(Hash)) {
				Menu::GetControlManager()->RequestModel(Hash, [](uint32_t ModelHash) {
					Native::SetPlayerModel(Menu::GetLocalPlayer().m_ID, ModelHash);
					m_Vars.m_LastModel = ModelHash;

					Utils::WaitFiber(2000);
					Global::Vars::g_ReapplyWeapons = true;
				});
			}
		}
	}
}

void ModelMenu::Init() {
	SetName("Model");
	SetParentSubmenu<AppearanceMenu>();

	addOption(ButtonOption("Refresh Model")
		.addTranslation()
		.addOnClick([] {
			Native::ResetPedVisibleDamage(Menu::GetLocalPlayer().m_Ped);
			Native::ClearPedBloodDamage(Menu::GetLocalPlayer().m_Ped);
			Native::ClearPedDecorations(Menu::GetLocalPlayer().m_Ped);
		}));

	addOption(ButtonOption("Refresh Model Cache")
		.addTranslation()
		.addOnClick([] {
			m_Vars.m_LastModel = 0;
		})
		.addTooltip("Refresh the cache that re-applies your model when you die"));

	addOption(ButtonOption("Input Custom Model")
		.addTranslation()
		.addKeyboard("Input the model! (Hint: name, hash or numeric)", 30, [](const char* ModelName) {
			if (ModelName) {
				if (ModelName[0] == '0' && (ModelName[1] == 'x' || ModelName[1] == 'X')) {
					// hash
					ChangeModel((uint32_t)_strtoui64(ModelName, 0, 0));
				} else {
					std::string StringName(ModelName);
					if (std::find_if(StringName.begin(), StringName.end(), [](unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
						// numeric
						ChangeModel((uint32_t)atoi(ModelName));
					} else {
						// name
						ChangeModel(Native::GetHashKey(ModelName));
					}
				}
			}
		}));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Popular")
		.addTranslation()
		.addScroll(m_Vars.m_Popular, 0, NUMOF(Global::Arrays::g_PedModelPopular), Global::Arrays::g_PedModelPopular).canLoop()
		.addOnClick([] { ChangeModel(Global::Arrays::g_PedModelPopular[m_Vars.m_Popular].m_Result); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Story")
		.addTranslation()
		.addScroll(m_Vars.m_Story, 0, NUMOF(Global::Arrays::g_PedModelStory), Global::Arrays::g_PedModelStory).canLoop()
		.addOnClick([] { ChangeModel(Global::Arrays::g_PedModelStory[m_Vars.m_Story].m_Result); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Animals")
		.addTranslation()
		.addScroll(m_Vars.m_Animals, 0, NUMOF(Global::Arrays::g_PedModelAnimal), Global::Arrays::g_PedModelAnimal).canLoop()
		.addOnClick([] { ChangeModel(Global::Arrays::g_PedModelAnimal[m_Vars.m_Animals].m_Result); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Emergency")
		.addTranslation()
		.addScroll(m_Vars.m_Emergency, 0, NUMOF(Global::Arrays::g_PedModelEmergency), Global::Arrays::g_PedModelEmergency).canLoop()
		.addOnClick([] { ChangeModel(Global::Arrays::g_PedModelEmergency[m_Vars.m_Emergency].m_Result); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Role")
		.addTranslation()
		.addScroll(m_Vars.m_Role, 0, NUMOF(Global::Arrays::g_PedModelRole), Global::Arrays::g_PedModelRole).canLoop()
		.addOnClick([] { ChangeModel(Global::Arrays::g_PedModelRole[m_Vars.m_Role].m_Result); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Ambient")
		.addTranslation()
		.addScroll(m_Vars.m_Ambient, 0, NUMOF(Global::Arrays::g_PedModelAmbient), Global::Arrays::g_PedModelAmbient).canLoop()
		.addOnClick([] { ChangeModel(Global::Arrays::g_PedModelAmbient[m_Vars.m_Ambient].m_Result); }));

	addOption(ScrollOption<uint32_t>(SCROLLSELECT, "Misc")
		.addTranslation()
		.addScroll(m_Vars.m_Misc, 0, NUMOF(Global::Arrays::g_PedModelMisc), Global::Arrays::g_PedModelMisc).canLoop()
		.addOnClick([] { ChangeModel(Global::Arrays::g_PedModelMisc[m_Vars.m_Misc].m_Result); }));
}

void ModelMenu::Update() {}

/*Called once on submenu open*/
void ModelMenu::UpdateOnce() {}

/*Called always*/
void ModelMenu::FeatureUpdate() {
	static bool Reapply = false;
	
	if (!Menu::GetLocalPlayer().m_Alive) {
		if (m_Vars.m_LastModel) {
			Reapply = true;
		}
	} else {
		if (Reapply) {
			Reapply = false;
			if (m_Vars.m_LastModel) {
				ChangeModel(m_Vars.m_LastModel);
			}
		}
	}
}

ModelMenu* _instance;
ModelMenu* ModelMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ModelMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

ModelMenu::~ModelMenu() { delete _instance; }