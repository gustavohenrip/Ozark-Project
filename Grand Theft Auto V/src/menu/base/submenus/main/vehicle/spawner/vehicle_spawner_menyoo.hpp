#pragma once
#include "menu/base/submenu.hpp"

class VehicleSpawnerMenyooMenu : public Menu::Submenu {
public:
	static VehicleSpawnerMenyooMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleSpawnerMenyooMenu()
		: Menu::Submenu() {}

	~VehicleSpawnerMenyooMenu();
};

namespace VehicleSpawnerMenyooMenuVars {
	struct Vars_ {
		bool m_DeleteCurrent;
		bool m_UseCollision;
		
		Entity m_VehicleHandle;
		Entity m_InitialHandle;
		
		std::vector<Entity> m_SpawnedVehicles;
		std::vector<Entity> m_SpawnedAttachments;
	};

	extern Vars_ m_Vars;
}