#pragma once
#include "stdafx.hpp"
#include "utils/structs.hpp"

namespace MySQL {
	bool IsAuthTokenValid(const char* Token);
	bool IsCheatInFreemode(Structs::eCheats Cheat);
	bool IsCheatOutdated(Structs::eCheats Cheat, uint32_t Version);
	bool IsHardwareIDBanned(const char* HardwareID);

	bool GetAuthToken(const char* Token, Structs::AuthToken* Info);
	bool GetClientInfo(const char* Username, const char* PasswordHash, Structs::ClientInfo* Info);
	bool GetClientInfoByID(int ID, Structs::ClientInfo* Info);
	bool GetClientSubscription(int ID, Structs::eCheats Cheat, Structs::ClientSubscription* Info);
	bool GetClientMetrics(int ID, std::vector<Structs::ClientMetric>& Metrics);
	bool GetCheatInfo(int ID, Structs::CheatInfo* Info);
	std::vector<Structs::AuthToken> GetAuthTokens();
	
	void UpdateClientIP(Structs::ClientInfo Info, const char* IP);
	void UpdateClientStatus(Structs::ClientInfo Info, Structs::eAccountStatus Status, const char* Reason, bool BanHardware = false);
	void UpdateClientHardwareID(Structs::ClientInfo Info, const char* HardwareID);
	void UpdateClientAuthentications(Structs::ClientInfo Info);
	void UpdateAuthTokenHeartbeat(const char* Token);

	void BanHardwareID(const char* HardwareID, const char* Reason);
	void AddAuthToken(Structs::ClientInfo Info, Structs::eCheats Cheat, const char* Token, const char* PCUsername);
	void DeleteAuthToken(const char* Token);
	void RevalidateAuthToken(const char* Token);
	void AddMetric(int ID, Structs::eCheats Cheat, int MetricID, int Type, const char* Info);
	void AddHeartbeat(int ID, Structs::eCheats Cheat, const char* IP, nlohmann::json Game);
	void DeleteOldHeartbeats();
}