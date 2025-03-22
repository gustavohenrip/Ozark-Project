#pragma once
#include "json.hpp"

namespace Structs {
	enum eCheats : uint8_t {
		CHEAT_GRAND_THEFT_AUTO_V = 1,
		CHEAT_RED_DEAD_REDEMPTION_2,
		CHEAT_END
	};

	enum ePackets : uint8_t {
		PACKET_PING = 1,
		PACKET_HEARTBEAT,
		PACKET_GET_SIGNATURES,
		PACKET_GET_TIME,
		PACKET_WELCOME,
		PACKET_METRIC,
		PACKET_END
	};

	enum eSocketErrorCodes : uint8_t {
		ERROR_CANT_READ_MAGIC,
		ERROR_INVALID_MAGIC,
		ERROR_CANT_READ_REQUIRED_HEADER,
		ERROR_INVALID_SIZE,
		ERROR_INVALID_PACKET,
		ERROR_INVALID_CHEAT,
		ERROR_CANT_READ_HEADER_DATA,
		ERROR_INVALID_PACKET_SIZE,
		ERROR_AUTH_TOKEN_NOT_FOUND,
		ERROR_ALLOCATING_MEMORY
	};

	enum eResponseStatus {
		RESPONSE_ERROR,
		RESPONSE_SUCCESS,

		// welcome
		RESPONSE_WELCOME_NO_ACCOUNT,
		RESPONSE_WELCOME_DISABLED_ACCOUNT,
		RESPONSE_WELCOME_BANNED_ACCOUNT,
		RESPONSE_WELCOME_OUTDATED,
		RESPONSE_WELCOME_PTB_DISABLED,
		RESPONSE_WELCOME_DISABLED,
		RESPONSE_WELCOME_HWID_MISMATCH,
	};

	enum eAccountStatus {
		ACCOUNT_OK,
		ACCOUNT_DISABLED,
		ACCOUNT_BANNED
	};

	enum eRank {
		RANK_USER,
		RANK_STAFF,
		RANK_ADMIN
	};

	enum eMetricID {
		METRIC_THREAD_CREATED_IN_UNMAPPED_MEMORY = 1,	// i.e co-loading
		METRIC_DEBUGGING,
		METRIC_THREAD_BEING_DEBUGGED,
		METRIC_FUNCTION_INTEGRITY_FAILED,
		METRIC_BLACKLISTED_PROCESS,
		METRIC_MODULE_DIGEST_MISMATCH,
		METRIC_CRASHED,
	};

	enum eMetricType {
		METRIC_INFO = 1,
		METRIC_WARNING,
		METRIC_BAN
	};

	struct ConnectionInfo {
		int m_Socket;
		char m_IP[16];
	};

	struct ClientInfo {
		int m_ID;
		std::string m_Username;
		std::string m_Password;
		eRank m_Rank;
		nlohmann::json m_Hwid;		// { 'hwid': 'xxxxx', 'last_reset': 1234 }
		nlohmann::json m_IP;		// { 'registered': '127.0.0.1', 'last': '127.0.0.1' }
		nlohmann::json m_Flags;		// { 'reseller': true, 'tester': true }
		nlohmann::json m_Status;	// { 'status': 1, 'reason': 'gay' }
	};

	struct CheatInfo {
		int m_ID;
		std::string m_Name;
		bool m_Public;
		bool m_Tester;
		bool m_Freemode;
		uint32_t m_CurrentVersion;
	};

	struct AuthToken {
		int m_ID;
		int m_AccountID;
		eCheats m_Cheat;
		std::string m_Token;
		uint32_t m_LastConnection;
		uint32_t m_InitialConnection;
		int m_ConnectionIndex;
		bool m_InitialConnectionReceived;
		std::string m_ComputerName;
	};

	struct ClientSubscription {
		int m_ID;
		int m_AccountID;
		eCheats m_Cheat;
		uint32_t m_Start;
		uint32_t m_End;
	};

	struct ClientMetric {
		int m_ID;
		int m_AccountID;
		eCheats m_Cheat;
		int m_MetricID;
		int m_Type;
		std::string m_Info;
		uint32_t m_Time;
	};

	struct TimeCalculations {
		int m_Years;
		int m_Days;
		int m_Hours;
		int m_Minutes;
		int m_Seconds;

		TimeCalculations(int Seconds) {
			m_Years = abs(Seconds / (60 * 60 * 24 * 365));
			m_Days = Seconds / 86400;
			m_Hours = (Seconds % 86400) / 3600;
			m_Minutes = m_Hours / 60;
			m_Seconds = (m_Hours % 60) / 1;
		}
	};

	namespace Request {
#pragma pack(push)
#pragma pack(1)
		struct EncryptionKeys {
			uint8_t m_XorKey; // Applied to whole buffer
			uint8_t m_TokenKey[0x10]; // m_AuthToken RC4
			uint8_t m_HardwareKey[0x10]; // m_HardwareID RC4
			uint8_t m_RequestKey[0x10]; // Stays the same in req->resp
		};

		struct CheatHeader {
			uint8_t m_Cheat;
			uint32_t m_CheatVersion;
			bool m_Tester; // Whether it's public test build
		};

		struct Header {
			uint32_t m_Magic; // 'D1CC'
			uint16_t m_Size;
			uint8_t m_Packet;
			CheatHeader m_Cheat;
			uint8_t m_AuthToken[0x20]; // Used in encryption method
			uint8_t m_HardwareID[0x40]; // SHA-512
			EncryptionKeys m_EncryptionKeys;
		};

		struct PacketPing : Header {};

		struct PacketWelcome : Header {
			char m_Username[100];
			char m_PCName[32]; // PC Username
			uint8_t m_PasswordHash[0x40]; // SHA-512
		};

		struct PacketGetTime : Header {};

		struct PacketMetric : Header {
			uint8_t m_MetricID;
			char m_MetricInfo[200];
		};

		struct PacketGetSignatures : Header {};

		struct PacketHeartbeat : Header {
			char m_GameUsername[32];
			char m_GameBuild[32];
			uint32_t m_RockstarID;
		};
#pragma pack(pop)
	}

	namespace Response {
#pragma pack(push)
#pragma pack(1)
		struct EncryptionKeys {
			uint8_t m_XorKey; // Applied to whole buffer
			uint8_t m_FinalKey[0x10]; // RC4'd at the end
			uint8_t m_FirstKey[0x10];
			uint8_t m_SecondKey[0x10];
			uint32_t m_Hash;
		};

		struct PacketPing : EncryptionKeys {
			uint8_t m_PingTest; // Should be 69
		};

		struct PacketWelcome : EncryptionKeys {
			eResponseStatus m_ResponseStatus;
			uint8_t m_AuthToken[0x20];
		};

		struct PacketGetTime : EncryptionKeys {
			eResponseStatus m_ResponseStatus;
			uint32_t m_SecondsLeft;
		};

		struct PacketGetSignaturesLength : EncryptionKeys {
			eResponseStatus m_ResponseStatus;
			uint32_t m_Length;
		};

		struct PacketGetSignatures : EncryptionKeys {
			eResponseStatus m_ResponseStatus;
			uint8_t m_Signatures[0];
		};

		struct PacketHeartbeat : EncryptionKeys {
			eResponseStatus m_ResponseStatus;
			uint32_t m_SecondsLeft;
			bool m_UpdateAvailable;
		};

		struct PacketMetric : EncryptionKeys {
			bool m_Processed;
			bool m_Banned;
		};
#pragma pack(pop)
	}
}