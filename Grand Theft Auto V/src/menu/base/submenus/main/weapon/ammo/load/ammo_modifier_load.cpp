#include "ammo_modifier_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../ammo_modifier.hpp"
#include "utils/json.hpp"
#include "../ammo_swap.hpp"
#include "../ammo_modifier_edit.hpp"
#include "utils/log.hpp"

using namespace AmmoModifierLoadMenuVars;

namespace AmmoModifierLoadMenuVars {
	Vars_ m_Vars;

	struct ModifierSave {
		std::string m_Name;
		uint32_t m_Model;
		uint32_t m_Swap;
		float m_Damage;
		float m_Force;
		float m_Speed;
	};

	void LoadModifier(ModifierSave Modifier) {
		if (Modifier.m_Model != Modifier.m_Swap) {
			AmmoSwapMenuVars::Swap(Modifier.m_Model, Modifier.m_Swap);
		}

		AmmoModifierEditMenuVars::m_Vars.m_CurrentHash = Modifier.m_Model;

		Rage::Types::Weapon* Weapon = AmmoModifierEditMenuVars::GetActiveWeapon();
		if (Weapon) {
			Weapon->m_Damage = Modifier.m_Damage;
			Weapon->m_Force = Modifier.m_Force;
			Weapon->m_Speed = Modifier.m_Speed;
		}
	}

	void SaveModifier(const char* Name, uint32_t Model) {
		uint32_t SwapModel = Model;
		
		auto Vit = std::find_if(AmmoModifierEditMenuVars::m_Vars.m_Modifiers.begin(), AmmoModifierEditMenuVars::m_Vars.m_Modifiers.end(), [=] (AmmoModifierEditMenuVars::EditContext& Context) {
			return Context.m_Hash == Model;
		});

		if (Vit != AmmoModifierEditMenuVars::m_Vars.m_Modifiers.end()) {
			SwapModel = Vit->m_Swap;
		}

		AmmoModifierEditMenuVars::m_Vars.m_CurrentHash = Model;
		Rage::Types::Weapon* Weapon = AmmoModifierEditMenuVars::GetActiveWeapon();
		if (Weapon) {
			try {
				nlohmann::json Json;

				std::ifstream Input(Utils::GetConfig()->GetAmmoModifiersPath());
				if (Input.good()) {
					Input >> Json;
					Input.close();
				}

				std::ofstream Output(Utils::GetConfig()->GetAmmoModifiersPath());
				if (Output.good()) {
					Json[Name] = nlohmann::json::array({
						Model,
						SwapModel,
						Weapon->m_Damage,
						Weapon->m_Force,
						Weapon->m_Speed
					});

					Output << Json.dump(4);
					Output.close();
				}
			} catch (std::exception& E) {
				LOG_WARN("[JSON] %s", E.what());
			}
		}
	}
}

void AmmoModifierLoadMenu::Init() {
	SetName("Load Modifier");
	SetParentSubmenu<AmmoModifierMenu>();

	addString("~m~None");
}

void AmmoModifierLoadMenu::Update() {}

/*Called once on submenu open*/
void AmmoModifierLoadMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	try {
		std::ifstream Input(Utils::GetConfig()->GetAmmoModifiersPath());
		if (Input.good()) {
			nlohmann::json Json;
			Input >> Json;
			Input.close();

			if (Json.size() > 0) {
				std::vector<ModifierSave> Modifiers;

				for (auto& it = Json.begin(); it != Json.end(); ++it) {
					nlohmann::json Data = *it;
					Modifiers.push_back({ it.key(), Data[0].get<uint32_t>(), Data[1].get<uint32_t>(), Data[2].get<float>(), Data[3].get<float>(), Data[4].get<float>() });
				}

				for (auto& Modifier : Modifiers) {
					addOption(ButtonOption(Modifier.m_Name)
						.addOnClick([=] { LoadModifier(Modifier); }));
				}
			} else {
				addOption(ButtonOption(getString("~m~None")));
			}
		} else {
			addOption(ButtonOption(getString("~m~None")));
		}
	} catch (std::exception& E) {
		LOG_WARN("[JSON] %s", E.what());
	}
}

/*Called always*/
void AmmoModifierLoadMenu::FeatureUpdate() {}

AmmoModifierLoadMenu* _instance;
AmmoModifierLoadMenu* AmmoModifierLoadMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AmmoModifierLoadMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

AmmoModifierLoadMenu::~AmmoModifierLoadMenu() { delete _instance; }