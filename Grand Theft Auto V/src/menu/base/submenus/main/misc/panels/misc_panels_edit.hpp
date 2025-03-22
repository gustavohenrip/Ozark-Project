#pragma once
#include "menu/base/submenu.hpp"

class MiscPanelsEditMenu : public Menu::Submenu {
public:
	static MiscPanelsEditMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MiscPanelsEditMenu()
		: Menu::Submenu() {}

	~MiscPanelsEditMenu();
};

namespace MiscPanelsEditMenuVars {
	struct Vars_ {
		std::size_t m_Index;
		std::string m_ID;
		int m_LocalColumn;
		int m_LocalIndex;
	};

	extern Vars_ m_Vars;
}