#pragma once
#include "menu/base/submenu.hpp"
#include "network_host_toolkit_join_block.hpp"

class NetworkHostToolkitJoinBlockingSelectedMenu : public Menu::Submenu {
public:
	static NetworkHostToolkitJoinBlockingSelectedMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkHostToolkitJoinBlockingSelectedMenu()
		: Menu::Submenu() {}

	~NetworkHostToolkitJoinBlockingSelectedMenu();
};

namespace NetworkHostToolkitJoinBlockingSelectedMenuVars {
	struct Vars_ {
		NetworkHostToolkitJoinBlockingMenuVars::ToolkitBlock* m_Selected;
	};

	extern NetworkHostToolkitJoinBlockingMenuVars::ToolkitBlockResponse m_Responses[23];
	extern Vars_ m_Vars;
}