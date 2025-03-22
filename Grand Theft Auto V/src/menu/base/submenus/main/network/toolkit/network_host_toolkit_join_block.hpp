#pragma once
#include "menu/base/submenu.hpp"

class NetworkHostToolkitJoinBlockingMenu : public Menu::Submenu {
public:
	static NetworkHostToolkitJoinBlockingMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	NetworkHostToolkitJoinBlockingMenu()
		: Menu::Submenu() {}

	~NetworkHostToolkitJoinBlockingMenu();
};

namespace NetworkHostToolkitJoinBlockingMenuVars {
	enum eHostToolkit {
		TOOLKIT_NEXT_SESSION_HOST,
		TOOLKIT_RID,
		TOOLKIT_NAME,
		TOOLKIT_IP
	};

	struct ToolkitBlock {
		bool m_Enabled;
		bool m_Notify = true;
		RadioContext m_Context = { "commonmenu", "shop_tick_icon" };
	};

	struct ToolkitBlockResponse {
		std::string m_Name;
		std::string m_Tooltip;
		eJoinResponses m_Response;
	};

	struct Vars_ {
		int m_WhoMayJoin = 0;
		ToolkitBlock m_Responses[8];
	};

	extern Vars_ m_Vars;
}