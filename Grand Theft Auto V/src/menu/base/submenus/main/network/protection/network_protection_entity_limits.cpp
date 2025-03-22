#include "network_protection_entity_limits.hpp"
#include "menu/base/submenu_handler.hpp"
#include "network_protection_entity_events.hpp"
#include "utils/json.hpp"
#include "utils/va.hpp"
#include "limits/network_protection_entity_limits_selected.hpp"
#include "limits/network_protection_entity_limits_add.hpp"
#include "utils/log.hpp"

using namespace NetworkProtectionEntityLimitsMenuVars;

namespace NetworkProtectionEntityLimitsMenuVars {
	Vars_ m_Vars;

	bool CacheLimits() {
		try {
			std::ifstream Input(Utils::GetConfig()->GetEntityLimitsPath());
			if (Input.good()) {
				nlohmann::json Json = nlohmann::json::parse(Input);
				Input.close();

				if (Json.size()) {
					m_Vars.m_Limits.clear();

					for (auto it = Json.begin(); it != Json.end(); ++it) {
						nlohmann::json Elements = *it;

						EntityLimit Element;
						Element.m_Model = Elements[0].get<uint32_t>();
						Element.m_Delay = Elements[1].get<int>();
						Element.m_Enabled = Elements[2].get<bool>();
						strcpy(Element.m_Comment, it.key().c_str());
						m_Vars.m_Limits.push_back(Element);
					}

					return true;
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}

		return false;
	}
}

void NetworkProtectionEntityLimitsMenu::Init() {
	SetName("Entity Limits");
	SetParentSubmenu<NetworkProtectionEntityEventsMenu>();

	addOption(SubmenuOption("+ Add Limit")
		.addTranslation()
		.addSubmenu<NetworkProtectionEntityLimitsAddMenu>());

	/*addOption(ToggleOption("Auto Detect")
		.addToggle(m_Vars.m_AutoDetect).addHotkey()
		.canBeSaved(m_ParentNameStack)
		.addTooltip("Auto add models to the limit that are detected as spam"));*/

	addOption(BreakOption("Limits")
		.addTranslation());

	addString("~m~None");

	NetworkProtectionEntityLimitsSelectedMenu::GetInstance();
	CacheLimits();
}

void NetworkProtectionEntityLimitsMenu::Update() {}

/*Called once on submenu open*/
void NetworkProtectionEntityLimitsMenu::UpdateOnce() {
	// save edits
	if (Menu::GetSubmenuHandler()->GetPreviousSubmenu() == NetworkProtectionEntityLimitsSelectedMenu::GetInstance()) {
		if (m_Vars.m_Limits.size()) {
			try {
				std::ofstream Output(Utils::GetConfig()->GetEntityLimitsPath());
				if (Output.good()) {
					nlohmann::json Json;

					for (auto& J : m_Vars.m_Limits) {
						Json[J.m_Comment] = { J.m_Model, J.m_Delay, J.m_Enabled };
					}

					Output << Json.dump(4);
					Output.close();
				}
			} catch (std::exception& E) {
				LOG_WARN("[JSON] %s", E.what());
			}
		}
	}

	ClearOptionsOffset(2);

Start:
	if (!CacheLimits()) {
		try {
			std::ofstream Output(Utils::GetConfig()->GetEntityLimitsPath());
			if (Output.good()) {
				nlohmann::json Json;
				Json["Jet"] = { 0x3f119114, 6, true };
				Json["Another Jet"] = { 0x15F27762, 6, true };

				Output << Json.dump(4);
				Output.close();

				goto Start;
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	} else {
		for (std::size_t i = 0; i < m_Vars.m_Limits.size(); i++) {
			EntityLimit Limit = m_Vars.m_Limits[i];

			addOption(SubmenuOption(Utils::VA::VA("%X - %s", Limit.m_Model, Limit.m_Comment))
				.addSubmenu<NetworkProtectionEntityLimitsSelectedMenu>()
				.addOnClick([=] { NetworkProtectionEntityLimitsSelectedMenuVars::m_Vars.m_Selected = i; }));
		}
	}
}

/*Called always*/
void NetworkProtectionEntityLimitsMenu::FeatureUpdate() {}

NetworkProtectionEntityLimitsMenu* _instance;
NetworkProtectionEntityLimitsMenu* NetworkProtectionEntityLimitsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkProtectionEntityLimitsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkProtectionEntityLimitsMenu::~NetworkProtectionEntityLimitsMenu() { delete _instance; }