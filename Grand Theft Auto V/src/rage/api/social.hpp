#pragma once
#include "stdafx.hpp"
#include <queue>

namespace Rage::API {
	struct RockstarAccount {
		std::string m_Name;
		uint64_t m_RockstarID;
		int m_Total;

		RockstarAccount()
			: m_Name(""), m_RockstarID(0), m_Total(0) {}

		RockstarAccount(std::string n, uint64_t rid, int t)
			: m_Name(n), m_RockstarID(rid), m_Total(t) {}
	};

	class SocialClub {
	public:
		void RequestThread();
		void SendSocialClub(std::string URL, std::string Data, std::function<void(std::wstring)> Callback);
		void GetProfileFromRockstarID(uint64_t RockstarID, std::function<void(bool, RockstarAccount)> Callback, bool Notify = true);
		void GetProfileFromName(std::string Name, std::function<void(bool, RockstarAccount)> Callback);
		std::queue<std::pair<std::pair<std::string, std::string>, std::function<void(std::wstring)>>>& GetQueue() { return m_Queue; }
	private:
		std::queue<std::pair<std::pair<std::string, std::string>, std::function<void(std::wstring)>>> m_Queue;
	}; SocialClub* GetSocialClub();
}