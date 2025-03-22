#pragma once
#include "menu/base/submenu.hpp"

class NetworkHostToolkitDesyncMenu : public Menu::Submenu {
public:
	static NetworkHostToolkitDesyncMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkHostToolkitDesyncMenu()
		: Menu::Submenu() {}

	~NetworkHostToolkitDesyncMenu();
};

namespace NetworkHostToolkitDesyncMenuVars {
	struct Vars_ {
		bool m_Block;
		bool m_Redirect = true;
		bool m_Notify = true;

		std::vector<std::pair<uint64_t, uint64_t>> m_Desyncs;
	};

	extern Vars_ m_Vars;
}