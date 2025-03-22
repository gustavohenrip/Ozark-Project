#pragma once
#include "menu/base/submenu.hpp"

class NetworkAbusiveAttackersMenu : public Menu::Submenu {
public:
	static NetworkAbusiveAttackersMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAbusiveAttackersMenu()
		: Menu::Submenu() {}

	~NetworkAbusiveAttackersMenu();
};

namespace NetworkAbusiveAttackersMenuVars {
	struct Vars_ {
		int m_AttackerType;
		bool m_AttackerInvincibility;
		bool m_AttackerInvisibility;
		int m_AttackerCount;
	};

	extern Vars_ m_Vars;
}