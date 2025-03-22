#pragma once
#include "menu/base/submenu.hpp"

class TeleportMenu : public Menu::Submenu {
public:
	static TeleportMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	TeleportMenu()
		: Menu::Submenu() {}

	~TeleportMenu();
};

namespace TeleportMenuVars {
	struct Vars_ {
		bool m_TeleportTransition;
		bool m_AutoWaypoint;
		bool m_HasTeleportedFlag;

		uint32_t m_WaypointTimer;

		int m_TeleportInto;
		int m_TeleportTo;
	};

	void TeleportToWaypoint();
	void Teleport(Math::Vector3<float> Coords);
	extern Vars_ m_Vars;
}