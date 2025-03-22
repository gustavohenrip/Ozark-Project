#pragma once
#include "menu/base/submenu.hpp"

class PurchaseWeaponMenu : public Menu::Submenu {
public:
	static PurchaseWeaponMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	PurchaseWeaponMenu()
		: Menu::Submenu() {}

	~PurchaseWeaponMenu();
};

namespace PurchaseWeaponMenuVars {
	struct Vars_ {
		std::shared_ptr<Option> m_Button;
	};

	namespace Scripts {
		const char* func_181(int iParam0, int iParam1);
		void func_116(char* sParam0, const char* cParam1, uint32_t iParam5, int iParam6, int iParam7, int iParam8, int iParam9, int iParam10, bool bParam11, bool bParam12);
		int func_57(int iParam0);
		int func_117(int iParam0);
	}

	extern Vars_ m_Vars;
}