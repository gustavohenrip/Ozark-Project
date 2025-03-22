#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountOutfitsMenu : public Menu::Submenu {
public:
	static NetworkAccountOutfitsMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountOutfitsMenu()
		: Menu::Submenu() {}

	~NetworkAccountOutfitsMenu();
};

namespace NetworkAccountOutfitsMenuVars {
	struct Vars_ {};

	bool DoesOutfitSlotExist(int OutfitIndex);
	char* GetOutfitSlotNamePtr(int OutfitIndex);
	int GetMaxSlots();
	int GetFreeSlots();
	int GetFreeSlotIndex();
	void DeleteSlot(int Slot);
	void SaveOutfitEdit();

	extern Vars_ m_Vars;
}