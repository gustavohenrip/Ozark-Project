#pragma once
#include "menu/base/submenu.hpp"

class MiscModelSwapAddMenu : public Menu::Submenu {
public:
	static MiscModelSwapAddMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscModelSwapAddMenu()
		: Menu::Submenu() {}

	~MiscModelSwapAddMenu();
};

namespace MiscModelSwapAddMenuVars {
	struct Vars_ {
		std::string m_Original;
		std::string m_New;
	};

	extern Vars_ m_Vars;
}