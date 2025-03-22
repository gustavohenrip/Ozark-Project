#include "network_account_outfits_edit.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account_outfits.hpp"
#include "menu/base/util/global.hpp"
#include "menu/base/submenus/main/player/wardrobe.hpp"
#include "network_account_outfits_edit_name.hpp"

using namespace NetworkAccountOutfitsEditMenuVars;

namespace NetworkAccountOutfitsEditMenuVars {
	Vars_ m_Vars;

	void SetAccessoryModel(int Slot, int Index, int Value) {
		Menu::Global(2097152).At(6175).At(1878).At(Slot, 10).At(Index).At(2).As<int>() = Value;
	}

	void SetAccessoryVariation(int Slot, int Index, int Value) {
		Menu::Global(2097152).At(6175).At(2089).At(Slot, 10).At(Index).At(2).As<int>() = Value;
	}

	void SetClothingModel(int Slot, int Index, int Value) {
		Menu::Global(2097152).At(6175).At(1330).At(Slot, 13).At(Index).At(2).As<int>() = Value;
	}

	void SetClothingVariation(int Slot, int Index, int Value) {
		Menu::Global(2097152).At(6175).At(1604).At(Slot, 13).At(Index).At(2).As<int>() = Value;
	}

	int GetAccessoryModel(int Slot, int Index) {
		return Menu::Global(2097152).At(6175).At(1878).At(Slot, 10).At(Index).At(2).As<int>();
	}

	int GetAccessoryVariation(int Slot, int Index) {
		return Menu::Global(2097152).At(6175).At(2089).At(Slot, 10).At(Index).At(2).As<int>();
	}

	int GetClothingModel(int Slot, int Index) {
		return Menu::Global(2097152).At(6175).At(1330).At(Slot, 13).At(Index).At(2).As<int>();
	}

	int GetClothingVariation(int Slot, int Index) {
		return Menu::Global(2097152).At(6175).At(1604).At(Slot, 13).At(Index).At(2).As<int>();
	}

	void UpdateSlotToCurrent(int Slot) {
		for (int i = 0; i < 12; i++) {
			SetClothingModel(Slot, i, Native::GetPedDrawableVariation(Menu::GetLocalPlayer().m_Ped, i));
			SetClothingVariation(Slot, i, Native::GetPedTextureVariation(Menu::GetLocalPlayer().m_Ped, i));
		}

		for (int i = 0; i < 9; i++) {
			SetAccessoryModel(Slot, i, Native::GetPedPropIndex(Menu::GetLocalPlayer().m_Ped, i));
			SetAccessoryVariation(Slot, i, Native::GetPedPropTextureIndex(Menu::GetLocalPlayer().m_Ped, i));
		}
	}

	void CacheCurrentOutfit() {
		for (int i = 0; i < 12; i++) {
			m_Vars.m_ClothingCache[0][i] = Native::GetPedDrawableVariation(Menu::GetLocalPlayer().m_Ped, i);
			m_Vars.m_ClothingCache[1][i] = Native::GetPedTextureVariation(Menu::GetLocalPlayer().m_Ped, i);
		}

		for (int i = 0; i < 9; i++) {
			m_Vars.m_AccessoryCache[0][i] = Native::GetPedPropIndex(Menu::GetLocalPlayer().m_Ped, i);
			m_Vars.m_AccessoryCache[1][i] = Native::GetPedPropTextureIndex(Menu::GetLocalPlayer().m_Ped, i);
		}
	}

	void SetCachedOutfit() {
		for (int i = 0; i < 12; i++) {
			Native::SetPedComponentVariation(Menu::GetLocalPlayer().m_Ped, i, m_Vars.m_ClothingCache[0][i], m_Vars.m_ClothingCache[1][i], 0);
		}

		for (int i = 0; i < 9; i++) {
			Native::SetPedPropIndex(Menu::GetLocalPlayer().m_Ped, i, m_Vars.m_AccessoryCache[0][i], m_Vars.m_AccessoryCache[1][i], true);
		}
	}

	void SetCurrentOutfitToSlot(int Slot) {
		for (int i = 0; i < 12; i++) {
			Native::SetPedComponentVariation(Menu::GetLocalPlayer().m_Ped, i, GetClothingModel(m_Vars.m_Slot, i), GetClothingVariation(m_Vars.m_Slot, i), 0);
		}

		for (int i = 0; i < 9; i++) {
			Native::SetPedPropIndex(Menu::GetLocalPlayer().m_Ped, i, GetAccessoryModel(m_Vars.m_Slot, i), GetAccessoryVariation(m_Vars.m_Slot, i), true);
		}
	}

	void CreateOutfit(int Slot) {
		UpdateSlotToCurrent(Slot);

		char* Name = NetworkAccountOutfitsMenuVars::GetOutfitSlotNamePtr(Slot);
		strcpy(Name, ("Ozark - " + std::to_string(Slot)).c_str());

		Menu::Global(2097152).At(6175).At(2300).At(Slot).At(2).As<int>() = 1;

		NetworkAccountOutfitsMenuVars::SaveOutfitEdit();
	}
}

void NetworkAccountOutfitsEditMenu::Init() {
	SetName("Outfit");
	SetParentSubmenu<NetworkAccountOutfitsMenu>();

	addOption(SubmenuOption("Name")
		.addTranslation()
		.addSubmenu<NetworkAccountOutfitsEditNameMenu>());

	addOption(SubmenuOption("Wardrobe")
		.addTranslation()
		.addSubmenu<WardrobeMenu>()
		.addOnClick([] {
			WardrobeMenu::GetInstance()->SetParentSubmenu<NetworkAccountOutfitsEditMenu>();
			CacheCurrentOutfit();
			SetCurrentOutfitToSlot(m_Vars.m_Slot);
		}));

	addOption(ButtonOption("Delete")
		.addTranslation()
		.addOnClick([] {
			NetworkAccountOutfitsMenuVars::DeleteSlot(m_Vars.m_Slot);
			NetworkAccountOutfitsMenuVars::SaveOutfitEdit();
			Menu::GetSubmenuHandler()->SetSubmenuPrevious(true);
		}));
}

void NetworkAccountOutfitsEditMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountOutfitsEditMenu::UpdateOnce() {
	if (WardrobeMenuVars::m_Vars.m_PreviewCam) {
		WardrobeMenuVars::RemoveCamera();
	}

	if (Menu::GetSubmenuHandler()->GetPreviousSubmenu() == WardrobeMenu::GetInstance()) {
		// Update the slots components to our current outfit that we just edited
		UpdateSlotToCurrent(m_Vars.m_Slot);

		// Restore our current outfit to our real outfit we cached
		SetCachedOutfit();

		NetworkAccountOutfitsMenuVars::SaveOutfitEdit();
	}

	// Creating an outfit
	if (m_Vars.m_Slot == NetworkAccountOutfitsMenuVars::GetFreeSlotIndex()) {
		CreateOutfit(m_Vars.m_Slot);
	}
}

/*Called always*/
void NetworkAccountOutfitsEditMenu::FeatureUpdate() {}

NetworkAccountOutfitsEditMenu* _instance;
NetworkAccountOutfitsEditMenu* NetworkAccountOutfitsEditMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountOutfitsEditMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountOutfitsEditMenu::~NetworkAccountOutfitsEditMenu() { delete _instance; }