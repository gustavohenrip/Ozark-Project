#include "wardrobe.hpp"
#include "menu/base/submenu_handler.hpp"
#include "appearance.hpp"
#include "wardrobe_save_load.hpp"

using namespace WardrobeMenuVars;

namespace WardrobeMenuVars {
	Vars_ m_Vars;

	ScrollStruct<int> Components[] = {
		{ TranslationString("Face", true), 0, },
		{ TranslationString("Mask", true), 1, },
		{ TranslationString("Hair", true), 2, },
		{ TranslationString("Torso", true), 3, },
		{ TranslationString("Leg", true), 4, },
		{ TranslationString("Misc", true), 5, },
		{ TranslationString("Shoes", true), 6, },
		{ TranslationString("Accessories", true), 7, },
		{ TranslationString("Undershirt", true), 8, },
		{ TranslationString("Kevlar", true), 9, },
		{ TranslationString("Badge", true), 10, },
		{ TranslationString("Torso 2", true), 11 },
	};

	ScrollStruct<int> Accessories[] = {
		{ TranslationString("Hats", true), 0, },
		{ TranslationString("Glasses", true), 1, },
		{ TranslationString("Ears", true), 2, },
		{ TranslationString("Watches", true), 6, },
		{ TranslationString("Bracelets", true), 7 },
	};

	void RemoveCamera() {
		if (Native::DoesCamExist(m_Vars.m_PreviewCam)) {
			Native::SetCamActive(m_Vars.m_PreviewCam, false);
			Native::RenderScriptCams(false, true, 1000, true, false, 0);
			Native::DestroyCam(m_Vars.m_PreviewCam, false);
		}
	}
}

void WardrobeMenu::Init() {
	SetName("Wardrobe");
	SetParentSubmenu<AppearanceMenu>();

	addOption(SubmenuOption("Save and Load")
		.addTranslation().addHotkey()
		.addSubmenu<WardrobeSaveLoadMenu>());

	addOption(ToggleOption("Preview Camera")
		.addTranslation()
		.addToggle(m_Vars.m_PreviewCamera).canBeSaved(m_ParentNameStack)
		.addOnClick([] { if (!m_Vars.m_PreviewCamera) RemoveCamera(); }));

	addOption(BreakOption("Customization")
		.addTranslation());

	addOption(ScrollOption<int>(SCROLL, "Clothing")
		.addTranslation()
		.addScroll(m_Vars.m_Clothing, 0, NUMOF(Components), Components)
		.addOnClick([] {
			m_Vars.m_ClothingModel = Native::GetPedDrawableVariation(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result);
			m_Vars.m_ClothingVariation = Native::GetPedTextureVariation(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result);
		}));

	addOption(NumberOption<int>(SCROLL, "Clothing Model") // drawableID
		.addTranslation()
		.addNumber(m_Vars.m_ClothingModel, "%i", 1).addMin(-1).addMax(0).showMax()
		.addOnClick([] {
			Native::SetPedComponentVariation(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result, m_Vars.m_ClothingModel, Native::GetPedTextureVariation(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result), 0);
			m_Vars.m_ClothingVariation = Native::GetPedTextureVariation(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result);
		})
		.addOnUpdate([](NumberOption<int>* Option) {
			Option->addMax(Native::GetNumberOfPedDrawableVariations(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result) - 1);
		}));

	addOption(NumberOption<int>(SCROLL, "Clothing Variation") // textureID
		.addTranslation()
		.addNumber(m_Vars.m_ClothingVariation, "%i", 1).addMin(-1).addMax(0).showMax()
		.addOnClick([] {
			Native::SetPedComponentVariation(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result, m_Vars.m_ClothingModel, m_Vars.m_ClothingVariation, 0);
		})
		.addOnUpdate([](NumberOption<int>* Option) {
			Option->addMax(Native::GetNumberOfPedTextureVariations(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result, m_Vars.m_ClothingModel));
			
			if (m_Vars.m_ClothingVariation > Native::GetNumberOfPedTextureVariations(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result, m_Vars.m_ClothingModel)) {
				m_Vars.m_ClothingVariation = 0;
			}
		})
		.addRequirement([] { return Native::GetNumberOfPedDrawableVariations(Menu::GetLocalPlayer().m_Ped, Components[m_Vars.m_Clothing].m_Result) > 0; }));

	addOption(ScrollOption<int>(SCROLL, "Accessory")
		.addTranslation()
		.addScroll(m_Vars.m_Accessory, 0, NUMOF(Accessories), Accessories)
		.addOnClick([] {
			m_Vars.m_AccessoryModel = Native::GetPedPropIndex(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result);
			m_Vars.m_AccessoryVariation = Native::GetPedPropTextureIndex(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result);
		}));

	addOption(NumberOption<int>(SCROLL, "Accessory Model") // drawableID
		.addTranslation()
		.addNumber(m_Vars.m_AccessoryModel, "%i", 1).addMin(-1).addMax(0).showMax()
		.addOnClick([] {
			Native::SetPedPropIndex(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result, m_Vars.m_AccessoryModel, 0, true);
			m_Vars.m_AccessoryVariation = Native::GetPedPropTextureIndex(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result);
		})
		.addOnUpdate([](NumberOption<int>* Option) {
			Option->addMax(Native::GetNumberOfPedPropDrawableVariations(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result));
		}));

	addOption(NumberOption<int>(SCROLL, "Accessory Variation") // textureID
		.addTranslation()
		.addNumber(m_Vars.m_AccessoryVariation, "%i", 1).addMin(-1).addMax(0).showMax()
		.addOnClick([] {
			Native::SetPedPropIndex(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result, m_Vars.m_AccessoryModel, m_Vars.m_AccessoryVariation, true);
		})
		.addOnUpdate([](NumberOption<int>* Option) {
			Option->addMax(Native::GetNumberOfPedPropTextureVariations(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result, m_Vars.m_AccessoryVariation));

			if (m_Vars.m_AccessoryVariation > Native::GetNumberOfPedPropTextureVariations(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result, m_Vars.m_AccessoryVariation)) {
				m_Vars.m_ClothingVariation = 0;
			}
		})
		.addRequirement([] { return Native::GetNumberOfPedPropTextureVariations(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result, m_Vars.m_Accessory) > 0; }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Clear Accessory")
		.addTranslation()
		.addScroll(m_Vars.m_AccessoryClear, 0, NUMOF(Accessories), Accessories)
		.addOnClick([] {
			Native::ClearPedProp(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result);
			m_Vars.m_AccessoryModel = Native::GetPedPropIndex(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result);
			m_Vars.m_AccessoryVariation = Native::GetPedPropTextureIndex(Menu::GetLocalPlayer().m_Ped, Accessories[m_Vars.m_Accessory].m_Result);
		}));
}

void WardrobeMenu::Update() {}

/*Called once on submenu open*/
void WardrobeMenu::UpdateOnce() {}

/*Called always*/
void WardrobeMenu::FeatureUpdate() {
	if (m_Vars.m_PreviewCamera 
		&& (Menu::GetSubmenuHandler()->GetCurrentSubmenu() == this
			|| Menu::GetSubmenuHandler()->GetCurrentSubmenu() == WardrobeSaveLoadMenu::GetInstance())) {
		if (!Native::DoesCamExist(m_Vars.m_PreviewCam)) {
			m_Vars.m_PreviewCam = Native::CreateCam("DEFAULT_SCRIPTED_CAMERA", false);
		}

		Native::SetCamActive(m_Vars.m_PreviewCam, true);
		Native::RenderScriptCams(true, true, 1000, true, 0, 0);
		Native::AttachCamToPedBone(m_Vars.m_PreviewCam, Menu::GetLocalPlayer().m_Ped, 0, 0.f, 1.5f, 0.f, true);
		Native::PointCamAtPedBone(m_Vars.m_PreviewCam, Menu::GetLocalPlayer().m_Ped, 0, 0.f, 0.f, 0.3f, true);
		Native::TaskStandStill(Menu::GetLocalPlayer().m_Ped, 1);
	}
}

WardrobeMenu* _instance;
WardrobeMenu* WardrobeMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WardrobeMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

WardrobeMenu::~WardrobeMenu() { delete _instance; }