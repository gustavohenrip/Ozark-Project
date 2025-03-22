#include "network_protection_entity_limits_add.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_protection_entity_limits.hpp"
#include "global/arrays.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "utils/log.hpp"

using namespace NetworkProtectionEntityLimitsAddMenuVars;

namespace NetworkProtectionEntityLimitsAddMenuVars {
	Vars_ m_Vars;

	void AddLimit() {
		if (m_Vars.m_Model == 0) return Menu::GetNotify()->NotifyStacked(NetworkProtectionEntityLimitsAddMenu::GetInstance()->getString("You need to input a model!"), Global::UIVars::g_NotificationError);

		try {
			std::ifstream Input(Utils::GetConfig()->GetEntityLimitsPath());
			if (Input.good()) {
				nlohmann::json Json = nlohmann::json::parse(Input);
				Input.close();

				Json[m_Vars.m_Comment] = { m_Vars.m_Model, m_Vars.m_Delay, true };

				std::ofstream Output(Utils::GetConfig()->GetEntityLimitsPath());
				if (Output.good()) {
					Output << Json.dump(4);
					Output.close();
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void NetworkProtectionEntityLimitsAddMenu::Init() {
	SetName("+ Add Limit");
	SetParentSubmenu<NetworkProtectionEntityLimitsMenu>();

	addString("You need to input a model!");
	addString("Set comment to");
	addString("Set model to");

	addOption(ButtonOption("Input Model")
		.addTranslation()
		.addKeyboard("Input the model! (Hint: name, hash or numeric)", 30, [this] (const char* ModelName) {
			if (ModelName) {
				m_Vars.m_Comment = ModelName;
				Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", getString("Set comment to").c_str(), m_Vars.m_Comment));

				if (ModelName[0] == '0' && (ModelName[1] == 'x' || ModelName[1] == 'X')) {
					// hash
					m_Vars.m_Model = (uint32_t)_strtoui64(ModelName, 0, 0);
				} else {
					std::string StringName(ModelName);
					if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
						// numeric
						m_Vars.m_Model = (uint32_t)atoi(ModelName);
					} else {
						// name
						m_Vars.m_Model = Native::GetHashKey(ModelName);
					}
				}

				Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %X", getString("Set model to").c_str(), m_Vars.m_Model));
			}
		}));

	addOption(ButtonOption("Input Comment")
		.addTranslation()
		.addKeyboard("Input custom comment", 100, [this] (const char* Comment) {
			m_Vars.m_Comment = Comment;
			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s %s", getString("Set comment to").c_str(), Comment));
		}));

	addOption(ScrollOption<int>(SCROLL, "Delay Per Spawn")
		.addTranslation()
		.addScroll(m_Vars.m_Delay, 0, NUMOF(Global::Arrays::g_Timers), Global::Arrays::g_Timers)
		.addTooltip("Delay between processing the spawn of this model"));

	addOption(ButtonOption("Add Limit")
		.addTranslation()
		.addOnClick(AddLimit));
}

void NetworkProtectionEntityLimitsAddMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEntityLimitsAddMenu::UpdateOnce() {
	m_Vars.m_Comment.clear();
	m_Vars.m_Model = 0;
	m_Vars.m_Delay = 0;
}

/*Called always*/
void NetworkProtectionEntityLimitsAddMenu::FeatureUpdate() {}

NetworkProtectionEntityLimitsAddMenu* _instance;
NetworkProtectionEntityLimitsAddMenu* NetworkProtectionEntityLimitsAddMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEntityLimitsAddMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEntityLimitsAddMenu::~NetworkProtectionEntityLimitsAddMenu() { delete _instance; }