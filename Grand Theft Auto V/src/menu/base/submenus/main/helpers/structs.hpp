#pragma once
#include "stdafx.hpp"

namespace Menu::Structs {
	struct ProtectionEventContext {
		bool m_Block;
		bool m_Redirect;
		bool m_Notify;

		bool m_CanBlock;
		bool m_CanRedirect;
		bool m_CanNotify;

		std::string m_Tooltip = "";
	};
}