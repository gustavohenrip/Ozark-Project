#pragma once
#include "menu/base/submenu.hpp"

class NetworkAccountTimesMenu : public Menu::Submenu {
public:
	static NetworkAccountTimesMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkAccountTimesMenu()
		: Menu::Submenu() {}

	~NetworkAccountTimesMenu();
};

namespace NetworkAccountTimesMenuVars {
	struct Vars_ {
		int m_Type;

		int m_Days;
		int m_Hours;
		int m_Minutes;
		int m_Seconds;
		int m_Milliseconds;
	};

	extern Vars_ m_Vars;
}