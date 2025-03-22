#include "mysql_funcs.hpp"
#include "mysql.hpp"
#include "utils/utils.hpp"

namespace MySQL {
	bool IsAuthTokenValid(const char* Token) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM auth_tokens WHERE `token` = ?");
			DB.BindParam(Token);

			if (DB.Execute()) {
				if (DB.IsReadable()) {
					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	bool IsHardwareIDBanned(const char* HardwareID) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM banned_hwid WHERE `hwid` = ?");
			DB.BindParam(HardwareID);

			if (DB.Execute()) {
				if (DB.IsReadable()) {
					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	bool IsCheatInFreemode(Structs::eCheats Cheat) {
		Structs::CheatInfo CheatInfo;
		if (GetCheatInfo(Cheat, &CheatInfo)) {
			return CheatInfo.m_Freemode;
		}

		return false;
	}

	bool IsCheatOutdated(Structs::eCheats Cheat, uint32_t Version) {
		Structs::CheatInfo CheatInfo;
		if (GetCheatInfo(Cheat, &CheatInfo)) {
			return CheatInfo.m_CurrentVersion > Version;
		}

		return false;
	}

	bool GetAuthToken(const char* Token, Structs::AuthToken* Info) {
		if (!Info) return false;
		
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM `auth_tokens` WHERE `token` = ?");
			DB.BindParam(Token);

			if (DB.Execute()) {
				if (DB.IsReadable()) {
					Info->m_ID = DB.GetInt("id");
					Info->m_AccountID = DB.GetInt("account_id");
					Info->m_Cheat = (Structs::eCheats)DB.GetInt("cheat_id");
					Info->m_Token = DB.GetString("token");
					Info->m_LastConnection = DB.GetInt("last_connection");
					Info->m_InitialConnection = DB.GetInt("initial_connection");
					Info->m_ConnectionIndex = DB.GetInt("connection_index");
					Info->m_InitialConnectionReceived = DB.GetBool("initial_connection_received");

					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	bool GetClientInfo(const char* Username, const char* PasswordHash, Structs::ClientInfo* Info) {
		if (!Info) return false;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM `users` WHERE `username` = ? AND password = ?");
			DB.BindParam(Username);
			DB.BindParam(PasswordHash);

			if (DB.Execute()) {
				if (DB.IsReadable()) {
					Info->m_ID = DB.GetInt("id");
					Info->m_Username = DB.GetString("username");
					Info->m_Password = DB.GetString("password");
					Info->m_Rank = (Structs::eRank)DB.GetInt("rank");
					Info->m_Hwid = nlohmann::json::parse(DB.GetString("hwid").c_str());
					Info->m_IP = nlohmann::json::parse(DB.GetString("ip").c_str());
					Info->m_Flags = nlohmann::json::parse(DB.GetString("flags").c_str());
					Info->m_Status = nlohmann::json::parse(DB.GetString("status").c_str());

					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	bool GetClientInfoByID(int ID, Structs::ClientInfo* Info) {
		if (!Info) return false;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM `users` WHERE `id` = ?");
			DB.BindParam(ID);

			if (DB.Execute()) {
				if (DB.IsReadable()) {
					Info->m_ID = DB.GetInt("id");
					Info->m_Username = DB.GetString("username");
					Info->m_Password = DB.GetString("password");
					Info->m_Rank = (Structs::eRank)DB.GetInt("rank");
					Info->m_Hwid = nlohmann::json::parse(DB.GetString("hwid").c_str());
					Info->m_IP = nlohmann::json::parse(DB.GetString("ip").c_str());
					Info->m_Flags = nlohmann::json::parse(DB.GetString("flags").c_str());
					Info->m_Status = nlohmann::json::parse(DB.GetString("status").c_str());

					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	bool GetClientSubscription(int ID, Structs::eCheats Cheat, Structs::ClientSubscription* Info) {
		if (!Info) return false;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM `subscriptions` WHERE account_id = ? AND cheat_id = ?");
			DB.BindParam(ID);
			DB.BindParam(Cheat);

			if (DB.Execute()) {
				if (DB.IsReadable()) {
					Info->m_ID = DB.GetInt("id");
					Info->m_AccountID = DB.GetInt("account_id");
					Info->m_Cheat = (Structs::eCheats)DB.GetInt("cheat_id");
					Info->m_Start = DB.GetUInt32("start");
					Info->m_End = DB.GetUInt32("end");

					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	bool GetClientMetrics(int ID, std::vector<Structs::ClientMetric>& Metrics) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM metrics WHERE account_id = ?");
			DB.BindParam(ID);

			if (DB.Execute()) {
				if (DB.GetRowCount() > 0) {
					while (DB.IsReadable()) {
						Structs::ClientMetric Metric;
						Metric.m_ID = DB.GetInt("id");
						Metric.m_AccountID = DB.GetInt("account_id");
						Metric.m_Cheat = (Structs::eCheats)DB.GetInt("cheat_id");
						Metric.m_MetricID = DB.GetInt("metric_id");
						Metric.m_Type = DB.GetInt("type");
						Metric.m_Info = DB.GetString("info");
						Metric.m_Time = DB.GetUInt32("time");
						Metrics.push_back(Metric);
					}

					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	bool GetCheatInfo(int ID, Structs::CheatInfo* Info) {
		if (!Info) return false;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM `cheats` WHERE `id` = ?");
			DB.BindParam(ID);

			if (DB.Execute()) {
				if (DB.IsReadable()) {
					Info->m_ID = DB.GetInt("id");
					Info->m_Name = DB.GetString("name");
					Info->m_Public = DB.GetBool("public");
					Info->m_Tester = DB.GetBool("tester");
					Info->m_Freemode = DB.GetBool("freemode");
					Info->m_CurrentVersion = DB.GetUInt32("current_version");

					DB.Disconnect();
					return true;
				}
			}

			DB.Disconnect();
		}

		return false;
	}

	std::vector<Structs::AuthToken> GetAuthTokens() {
		std::vector<Structs::AuthToken> AuthTokens;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("SELECT * FROM auth_tokens");

			if (DB.Execute()) {
				if (DB.GetRowCount() > 0) {
					while (DB.IsReadable()) {
						Structs::AuthToken AuthToken;
						AuthToken.m_ID = DB.GetInt("id");
						AuthToken.m_AccountID = DB.GetInt("account_id");
						AuthToken.m_Cheat = (Structs::eCheats)DB.GetInt("cheat_id");
						AuthToken.m_Token = DB.GetString("token");
						AuthToken.m_LastConnection = DB.GetUInt32("last_connection");
						AuthToken.m_InitialConnection = DB.GetUInt32("initial_connection");
						AuthToken.m_ConnectionIndex = DB.GetInt("connection_index");
						AuthToken.m_InitialConnectionReceived = DB.GetBool("initial_connection_received");
						AuthToken.m_ComputerName = DB.GetString("computer_name");
						AuthTokens.push_back(AuthToken);
					}
				}
			}

			DB.Disconnect();
		}

		return AuthTokens;
	}

	void UpdateClientIP(Structs::ClientInfo Info, const char* IP) {
		Info.m_IP["last"] = IP;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("UPDATE users SET `ip` = ? WHERE `id` = ?");
			DB.BindParam(Info.m_IP.dump().c_str());
			DB.BindParam(Info.m_ID);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void UpdateClientStatus(Structs::ClientInfo Info, Structs::eAccountStatus Status, const char* Reason, bool BanHardware) {
		Info.m_Status["status"] = Status;
		Info.m_Status["reason"] = Reason;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("UPDATE users SET `status` = ? WHERE `id` = ?");
			DB.BindParam(Info.m_Status.dump().c_str());
			DB.BindParam(Info.m_ID);
			DB.Execute();
			DB.Disconnect();
		}

		if (BanHardware) {
			std::string HardwareID = Info.m_Hwid["hwid"];
			if (!HardwareID.empty() && HardwareID != "-none-") {
				BanHardwareID(HardwareID.c_str(), Reason);
			}
		}
	}

	void UpdateClientHardwareID(Structs::ClientInfo Info, const char* HardwareID) {
		Info.m_Hwid["hwid"] = HardwareID;

		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("UPDATE users SET `hwid` = ? WHERE `id` = ?");
			DB.BindParam(Info.m_Hwid.dump().c_str());
			DB.BindParam(Info.m_ID);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void UpdateClientAuthentications(Structs::ClientInfo Info) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("UPDATE users SET authentications=authentications+1 WHERE `id` = ?");
			DB.BindParam(Info.m_ID);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void UpdateAuthTokenHeartbeat(const char* Token) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("UPDATE auth_tokens SET last_connection = ?, initial_connection_received = ? WHERE `token` = ?");
			DB.BindParam((uint32_t)std::time(0));
			DB.BindParam(true);
			DB.BindParam(Token);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void BanHardwareID(const char* HardwareID, const char* Reason) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("INSERT INTO banned_hwid (hwid, `reason`) VALUES (?, ?)");
			DB.BindParam(HardwareID);
			DB.BindParam(Reason);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void RevalidateAuthToken(const char* Token) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("UPDATE auth_tokens SET connection_index=connection_index+1 WHERE `token` = ?");
			DB.BindParam(Token);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void AddAuthToken(Structs::ClientInfo Info, Structs::eCheats Cheat, const char* Token, const char* PCUsername) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("DELETE FROM auth_tokens WHERE account_id = ? AND cheat_id = ?");
			DB.BindParam(Info.m_ID);
			DB.BindParam(Cheat);
			DB.Execute();

			DB.Prepare("INSERT INTO auth_tokens (account_id, cheat_id, `token`, last_connection, initial_connection, connection_index, computer_name) VALUES (?, ?, ?, ?, ?, ?, ?)");
			DB.BindParam(Info.m_ID);
			DB.BindParam(Cheat);
			DB.BindParam(Token);
			DB.BindParam((uint32_t)std::time(0));
			DB.BindParam((uint32_t)std::time(0));
			DB.BindParam(1);
			DB.BindParam(PCUsername);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void DeleteAuthToken(const char* Token) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("DELETE FROM auth_tokens WHERE `token` = ?");
			DB.BindParam(Token);
			DB.Execute();
			DB.Disconnect();
		}
	}

	void AddMetric(int ID, Structs::eCheats Cheat, int MetricID, int Type, const char* Info) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("INSERT INTO metrics (account_id, cheat_id, metric_id, `type`, `info`, `time`) VALUES (?, ?, ?, ?, ?, ?)");
			DB.BindParam(ID);
			DB.BindParam(Cheat);
			DB.BindParam(MetricID);
			DB.BindParam(Type);
			DB.BindParam(Info);
			DB.BindParam((uint32_t)std::time(0));
			DB.Execute();
			DB.Disconnect();
		}
	}

	void AddHeartbeat(int ID, Structs::eCheats Cheat, const char* IP, nlohmann::json Game) {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("INSERT INTO heartbeats (account_id, cheat_id, `ip`, game, `time`) VALUES (?, ?, ?, ?, ?)");
			DB.BindParam(ID);
			DB.BindParam(Cheat);
			DB.BindParam(IP);
			DB.BindParam(Game.dump().c_str());
			DB.BindParam((uint32_t)std::time(0));
			DB.Execute();
			DB.Disconnect();
		}
	}

	void DeleteOldHeartbeats() {
		MySQLConnection DB;
		if (DB.Connect()) {
			DB.Prepare("DELETE FROM heartbeats WHERE `time` < ?");
			DB.BindParam(((uint32_t)std::time(0)) - 172800); // 48hr
			DB.Execute();
			DB.Disconnect();
		}
	}
}