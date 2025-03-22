#include "auth_vars.hpp"

namespace Global::Auth {
	bool g_HasToken;
	uint8_t g_HardwareID[64];
	uint8_t g_AuthToken[0x20];
	std::unordered_map<std::string, PatternInfo> g_Signatures;
	std::string g_Username;
	std::string g_Password;
	std::string g_Build;
	std::string g_SocialClubName;
	uint32_t g_SecondsLeft;
	bool g_HasLifetime;
	uint8_t g_SelectedCrash = 0;
}