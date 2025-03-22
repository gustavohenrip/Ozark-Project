#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingMenu : public Menu::Submenu {
public:
	static NetworkSpoofingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingMenu();
};

namespace NetworkSpoofingMenuVars {
	struct Vars_ {
		bool m_MoveAnimation;
		bool m_RockstarDeveloper;
		bool m_PeerToken;
		bool m_Position;

		int m_PositionVal;
		Math::Vector3<float> m_CachedPosition;

		uint64_t m_Peer;
	};

	extern Vars_ m_Vars;
}