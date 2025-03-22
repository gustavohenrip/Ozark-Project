#include "security_vars.hpp"

namespace Global::Security {
	std::vector<uint8_t> g_PEHeader;
	uint8_t g_ModuleDigest[0x40];
}