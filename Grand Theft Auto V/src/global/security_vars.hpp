#pragma once
#include "stdafx.hpp"

namespace Global::Security {
	extern std::vector<uint8_t> g_PEHeader;
	extern uint8_t g_ModuleDigest[0x40];
}