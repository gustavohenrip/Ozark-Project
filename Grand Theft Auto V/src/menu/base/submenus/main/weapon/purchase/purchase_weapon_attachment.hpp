#pragma once
#include "menu/base/submenu.hpp"

class PurchaseWeaponAttachmentMenu : public Menu::Submenu {
public:
	static PurchaseWeaponAttachmentMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PurchaseWeaponAttachmentMenu()
		: Menu::Submenu() {}

	~PurchaseWeaponAttachmentMenu();
};

namespace PurchaseWeaponAttachmentMenuVars {
	struct Vars_ {
		std::shared_ptr<Option> m_Button;
	};

	extern Vars_ m_Vars;
}