#pragma once
#include "menu/base/submenu.hpp"

class GiveWeaponAttachmentMenuSelected : public Menu::Submenu {
public:
	static GiveWeaponAttachmentMenuSelected* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	GiveWeaponAttachmentMenuSelected()
		: Menu::Submenu() {}

	~GiveWeaponAttachmentMenuSelected();
};

namespace GiveWeaponAttachmentMenuSelectedVars {
	struct Vars_ {
		uint32_t m_SelectedWeapon;
	};

	extern Vars_ m_Vars;
}