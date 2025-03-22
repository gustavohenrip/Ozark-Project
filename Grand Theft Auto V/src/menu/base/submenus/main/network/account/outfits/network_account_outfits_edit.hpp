#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountOutfitsEditMenu : public Menu::Submenu {
public:
	static NetworkAccountOutfitsEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountOutfitsEditMenu()
		: Menu::Submenu() {}

	~NetworkAccountOutfitsEditMenu();
};

namespace NetworkAccountOutfitsEditMenuVars {
	struct Vars_ {
		int m_Slot;
		int m_ClothingCache[2][12];
		int m_AccessoryCache[2][9];
	};

	extern Vars_ m_Vars;
}