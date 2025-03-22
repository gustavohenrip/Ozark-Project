#include "wardrobe_save_load.hpp"
#include "menu/base/submenu_handler.hpp"
#include "wardrobe.hpp"
#include "utils/json.hpp"
#include "utils/va.hpp"
#include "utils/log.hpp"

using namespace WardrobeSaveLoadMenuVars;

namespace WardrobeSaveLoadMenuVars {
	Vars_ m_Vars;

	void SaveOutfit(const char* OutfitName) {
		auto Location = Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetOutfitPath().c_str(), OutfitName);

		try {
			std::ofstream Output(Location);
			nlohmann::json Object;

			auto& Clothing = Object["Clothing"];
			for (auto& Component : WardrobeMenuVars::Components) {
				Clothing[Component.m_Name.GetOriginal() + " Drawable"] = Native::GetPedDrawableVariation(Menu::GetLocalPlayer().m_Ped, Component.m_Result);
				Clothing[Component.m_Name.GetOriginal() + " Texture"] = Native::GetPedTextureVariation(Menu::GetLocalPlayer().m_Ped, Component.m_Result);
			}

			auto& Accessories = Object["Accessories"];
			for (auto& Prop : WardrobeMenuVars::Accessories) {
				Accessories[Prop.m_Name.GetOriginal() + " Drawable"] = Native::GetPedPropIndex(Menu::GetLocalPlayer().m_Ped, Prop.m_Result);
				Accessories[Prop.m_Name.GetOriginal() + " Texture"] = Native::GetPedPropTextureIndex(Menu::GetLocalPlayer().m_Ped, Prop.m_Result);
			}

			Output << Object.dump(4);
			Output.close();
			Menu::GetSubmenuHandler()->GetCurrentSubmenu()->UpdateOnce();
		} catch(std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}

	void LoadOutfit(const char* OutfitName) {
		auto Location = Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetOutfitPath().c_str(), OutfitName);

		try {
			std::ifstream Input(Location);
			if (!Input.good()) return;

			nlohmann::json Object;
			Input >> Object;

			Native::ClearAllPedProps(Menu::GetLocalPlayer().m_Ped);

			auto& Clothing = Object["Clothing"];
			for (auto& Component : WardrobeMenuVars::Components) {
				Native::SetPedComponentVariation(Menu::GetLocalPlayer().m_Ped, Component.m_Result, Clothing[Component.m_Name.GetOriginal() + " Drawable"].get<int>(), Clothing[Component.m_Name.GetOriginal() + " Texture"].get<int>(), 0);
			}

			auto& Accessories = Object["Accessories"];
			for (auto& Prop : WardrobeMenuVars::Accessories) {
				Native::SetPedPropIndex(Menu::GetLocalPlayer().m_Ped, Prop.m_Result, Accessories[Prop.m_Name.GetOriginal() + " Drawable"].get<int>(), Accessories[Prop.m_Name.GetOriginal() + " Texture"].get<int>(), true);
			}

			Input.close();
		} catch(std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}
}

void WardrobeSaveLoadMenu::Init() {
	SetName("Save and Load");
	SetParentSubmenu<WardrobeMenu>();

	addOption(ButtonOption("Save Outfit")
		.addTranslation()
		.addKeyboard("Enter the outfit name!", 30, SaveOutfit));

	addOption(BreakOption("Outfits")
		.addTranslation());

	addString("~m~None");
}

void WardrobeSaveLoadMenu::Update() {}

/*Called once on submenu open*/
void WardrobeSaveLoadMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	auto Outfits = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetOutfitPath(), ".json");
	if (Outfits.size()) {
		for (auto& Item : Outfits) {
			addOption(ButtonOption(Item)
				.addOnClick([=] {
					LoadOutfit(Item.c_str());
				}));
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void WardrobeSaveLoadMenu::FeatureUpdate() {}

WardrobeSaveLoadMenu* _instance;
WardrobeSaveLoadMenu* WardrobeSaveLoadMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WardrobeSaveLoadMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WardrobeSaveLoadMenu::~WardrobeSaveLoadMenu() { delete _instance; }