#pragma once
#include "menu/base/submenu.hpp"

class VideosMenu : public Menu::Submenu {
public:
	static VideosMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VideosMenu()
		: Menu::Submenu() {}

	~VideosMenu();
};

namespace VideosMenuVars {
	struct Vars_ {
		bool m_HideNames;
		bool m_HideMe;
		bool m_HideIps;
		bool m_HideRockstarIDs;
		std::unordered_map<uint64_t, std::string> m_CachedData;
	};

	extern Vars_ m_Vars;
}