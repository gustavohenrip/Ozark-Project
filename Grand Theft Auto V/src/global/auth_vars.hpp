#pragma once
#include "stdafx.hpp"

struct PatternInfo {
	std::string m_Pattern;
	int m_Offset;
	std::string m_Name;
	bool m_Important;

	PatternInfo() : m_Pattern(""), m_Offset(0), m_Name(""), m_Important(false) {}

	PatternInfo(std::string p, int o, std::string n, bool b = false) : m_Pattern(p), m_Offset(o), m_Name(n), m_Important(b) {}

	void Reset() {
		for (std::size_t i = 0; i < m_Pattern.length(); i++) m_Pattern[i] = 'F';
		for (std::size_t i = 0; i < m_Name.length(); i++) m_Name[i] = 'F';

		m_Pattern.clear();
		m_Name.clear();
		m_Offset = -1;
		m_Important = false;
	}
};

namespace Global::Auth {
	extern bool g_HasToken;
	extern uint8_t g_HardwareID[64];
	extern uint8_t g_AuthToken[0x20];
	extern std::unordered_map<std::string, PatternInfo> g_Signatures;
	extern std::string g_Username;
	extern std::string g_Password;
	extern std::string g_Build;
	extern std::string g_SocialClubName;
	extern uint32_t g_SecondsLeft;
	extern bool g_HasLifetime;
	extern uint8_t g_SelectedCrash;
}