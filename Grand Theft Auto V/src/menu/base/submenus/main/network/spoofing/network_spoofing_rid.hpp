#pragma once
#include "menu/base/submenu.hpp"

class NetworkSpoofingRockstarIDMenu : public Menu::Submenu {
public:
	static NetworkSpoofingRockstarIDMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkSpoofingRockstarIDMenu()
		: Menu::Submenu() {}

	~NetworkSpoofingRockstarIDMenu();
};

namespace NetworkSpoofingRockstarIDMenuVars {
	struct Vars_ {
		uint64_t m_TempSpoofingRockstarID;
		uint64_t m_SpoofingRockstarID;

		bool m_ConfigSaveRockstarID;
		std::shared_ptr<ButtonOption> m_ApplyButton;
		char m_PreviewName[50];

	};

	void SpoofRockstarID(NetworkSpoofingRockstarIDMenu* This, uint64_t RockstarID, bool Def = false);
	extern Vars_ m_Vars;
}