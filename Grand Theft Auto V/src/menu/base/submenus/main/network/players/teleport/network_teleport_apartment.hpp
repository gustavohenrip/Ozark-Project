#pragma once
#include "menu/base/submenu.hpp"

class NetworkTeleportApartmentMenu : public Menu::Submenu {
public:
	static NetworkTeleportApartmentMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkTeleportApartmentMenu()
		: Menu::Submenu() {}

	~NetworkTeleportApartmentMenu();
};

namespace NetworkTeleportApartmentMenuVars {
	struct Vars_ {};

	const char* GetLocationFromID(int ID);
	extern Vars_ m_Vars;
}