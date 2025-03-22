#pragma once
#include "menu/base/submenu.hpp"

class GiveWeaponAttachmentMenu : public Menu::Submenu {
public:
	static GiveWeaponAttachmentMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	GiveWeaponAttachmentMenu()
		: Menu::Submenu() {}

	~GiveWeaponAttachmentMenu();
};

namespace GiveWeaponAttachmentMenuVars {
	struct Vars_ {
		int m_GiveAll;
		int m_RemoveAll;
	};

	extern Vars_ m_Vars;
}