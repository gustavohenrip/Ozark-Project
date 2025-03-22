#include "network_account_outfits.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../network_account.hpp"
#include "menu/base/util/global.hpp"
#include "utils/va.hpp"
#include "outfits/network_account_outfits_edit.hpp"

using namespace NetworkAccountOutfitsMenuVars;

namespace NetworkAccountOutfitsMenuVars {
	Vars_ m_Vars;

	void SaveOutfitEdit() {
		Native::StatSave(0, false, 3);
	}

	bool DoesOutfitSlotExist(int OutfitIndex) {
		return Menu::Global(2097152).At(6175).At(2300).At(OutfitIndex).At(2).As<bool>(); // good
	}

	char* GetOutfitSlotNamePtr(int OutfitIndex) {
		return Menu::Global(2097152).At(6175).At(2454).At(OutfitIndex, 8).At(1).Get<char>(); // ?
	}

	int GetMaxSlots() {
		return Menu::Global(2529977).At(9146).As<int>(); // ?
	}

	int GetFreeSlots() {
		int Count = 0;

		for (int i = 0; i < GetMaxSlots(); i++) {
			if (DoesOutfitSlotExist(i)) {
				if (!Native::IsStringNullOrEmpty(GetOutfitSlotNamePtr(i))) {
					continue;
				}
			}

			Count++;
		}

		return Count;
	}

	int GetFreeSlotIndex() {
		for (int i = 0; i < GetMaxSlots(); i++) {
			if (Native::IsStringNullOrEmpty(GetOutfitSlotNamePtr(i))) {
				return i;
			}
		}

		return -1;
	}

	void DeleteSlot(int Slot) { 
		Menu::Global(2097152).At(6175).At(2300).At(Slot).At(2).As<int>() = 0; // good
		*GetOutfitSlotNamePtr(Slot) = 0;
	}
}

void NetworkAccountOutfitsMenu::Init() {
	SetName("Outfits");
	SetParentSubmenu<NetworkAccountMenu>();

	addString("Create Outfit");
	addString("Outfits");
	addString("New Outfit");
}

void NetworkAccountOutfitsMenu::Update() {}

/*Called once on submenu open*/
void NetworkAccountOutfitsMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	if (GetFreeSlots()) {
		addOption(SubmenuOption(getString("Create Outfit"))
			.addOnClick([this] {
				NetworkAccountOutfitsEditMenu::GetInstance()->SetName(getString("New Outfit"));
				NetworkAccountOutfitsEditMenuVars::m_Vars.m_Slot = GetFreeSlotIndex();
			})
			.addSubmenu<NetworkAccountOutfitsEditMenu>());

		addOption(BreakOption(getString("Outfits")));
	}
	
	for (int i = 0; i < GetMaxSlots(); i++) {
		if (DoesOutfitSlotExist(i)) {
			char* Name = GetOutfitSlotNamePtr(i);
			if (Name) {
				if (!Native::IsStringNullOrEmpty(Name)) {
					addOption(SubmenuOption(Name)
						.addSubmenu<NetworkAccountOutfitsEditMenu>()
						.addOnClick([=] {
						NetworkAccountOutfitsEditMenu::GetInstance()->SetName(Name);
						NetworkAccountOutfitsEditMenuVars::m_Vars.m_Slot = i;
					}));
				}
			}
		}
	}
}

/*Called always*/
void NetworkAccountOutfitsMenu::FeatureUpdate() {}

NetworkAccountOutfitsMenu* _instance;
NetworkAccountOutfitsMenu* NetworkAccountOutfitsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new NetworkAccountOutfitsMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

NetworkAccountOutfitsMenu::~NetworkAccountOutfitsMenu() { delete _instance; }