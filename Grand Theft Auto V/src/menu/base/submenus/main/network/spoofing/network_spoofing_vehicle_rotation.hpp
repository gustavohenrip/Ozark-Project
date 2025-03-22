#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingVehicleRotationMenu : public Menu::Submenu {
public:
	static NetworkSpoofingVehicleRotationMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingVehicleRotationMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingVehicleRotationMenu();
};

namespace NetworkSpoofingVehicleRotationMenuVars {
	struct Vars_ {
		bool m_Ghost;
		bool m_Spinbot;
		bool m_Jitter;
		bool m_Static;
		bool m_Relative;

		float m_SpinbotSpeed = 10.f;

		int m_SpinbotAxis;
		int m_JitterAxis;

		Math::Vector3_<float> m_StaticValue;
		Math::Vector3_<bool> m_StaticToggle;
		Math::Vector3_<float> m_HookValue;

		Vehicle m_GhostHandle;
	};

	extern Vars_ m_Vars;
}