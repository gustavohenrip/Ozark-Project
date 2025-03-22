#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingPlayerRotationMenu : public Menu::Submenu {
public:
	static NetworkSpoofingPlayerRotationMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingPlayerRotationMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingPlayerRotationMenu();
};

namespace NetworkSpoofingPlayerRotationMenuVars {
	struct Vars_ {
		bool m_Spinbot;
		bool m_Jitter;
		bool m_FaceDirection;
		bool m_Relative;

		int m_FaceDirectionVal;
		float m_SpinbotSpeed = 10.f;

		Math::Vector2<float> m_HookValue;
	};

	extern Vars_ m_Vars;
}