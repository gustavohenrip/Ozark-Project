#include "server_packets.hpp"
#include "global/auth_vars.hpp"
#include "utils/utils.hpp"
#include "../crypto/crypto.hpp"
#include "../sockets.hpp"
#include "utils/log.hpp"
#include "utils/va.hpp"
#include "menu/init.hpp"
#include "global/variables.hpp"

namespace Auth::Server {
	void PopulateHeader(Request::Header* Header, ePackets Packet, uint32_t Size) {
		LOG_CUSTOM_DEV("Auth", "Populating packet %i", Packet);

		Header->m_Magic = 'DOSB';
		Header->m_Size = Size;
		Header->m_Packet = Packet;
		Header->m_Cheat.m_Cheat = CHEAT_GRAND_THEFT_AUTO_V;
		Header->m_Cheat.m_CheatVersion = MENU_VERSION_INTERNAL;

		memcpy(Header->m_HardwareID, Global::Auth::g_HardwareID, 0x40);
		memcpy(Header->m_AuthToken, Global::Auth::g_AuthToken, 0x20);

		Header->m_EncryptionKeys.m_XorKey = (uint8_t)(rand() % 256);
		Utils::CreateRandomBytes(Header->m_EncryptionKeys.m_HardwareKey, 0x10);
		Utils::CreateRandomBytes(Header->m_EncryptionKeys.m_TokenKey, 0x10);
		Utils::CreateRandomBytes(Header->m_EncryptionKeys.m_RequestKey, 0x10);

		Crypto::RC4(Header->m_EncryptionKeys.m_HardwareKey, 0x10, Header->m_HardwareID, sizeof(Header->m_HardwareID));
		Crypto::RC4(Header->m_EncryptionKeys.m_TokenKey, 0x10, Header->m_AuthToken, sizeof(Header->m_AuthToken));
	}

	void EncryptPayload(Request::Header* Header) {
		if (Header->m_Size > sizeof(Request::Header)) {
			Crypto::RC4Modified(Header->m_EncryptionKeys.m_RequestKey, 0x10, (uint8_t*)Header, Header->m_Size, sizeof(Request::Header));
		}
	}

	bool PacketPing() {
		if (!Sockets::Network::Initialize()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize"_Protect);
			return false;
		}

		Request::PacketPing* PacketPing = Utils::AllocateMemoryType<Request::PacketPing>();
		PopulateHeader(PacketPing, PACKET_PING, sizeof(Request::PacketPing));
		Response::PacketPing* PacketPingResponse = Utils::AllocateMemoryType<Response::PacketPing>();

		int ReceivedBytes = 0;
		bool Success; const char* ErrorMsg;

		Sockets::Network Network;

		Network.Create(&Success, &ErrorMsg);
		if (Success) {
			Network.Connect(&Success, &ErrorMsg);
			if (Success) {
				Request::PacketPing PacketPingBackup;
				memcpy(&PacketPingBackup, PacketPing, sizeof(PacketPingBackup));

				EncryptPayload(PacketPing);
				LOG_CUSTOM_DEV("Auth", "Sending PACKET_PING");

				Network.Send(PacketPing, PacketPing->m_Size);
				Network.Receive(&PacketPingBackup, PacketPingResponse, sizeof(Response::PacketPing), &ReceivedBytes, &Success, &ErrorMsg);
				LOG_CUSTOM_DEV("Auth", "Received PACKET_PING");

				if (Success) {
					if (ReceivedBytes == sizeof(Response::PacketPing)) {
						if (PacketPingResponse->m_PingTest == 69) {
							free(PacketPing);
							free(PacketPingResponse);
							return true;
						} else {
							ErrorMsg = Utils::VA::VA("PING failed security checks"_Protect);
						}
					} else {
						ErrorMsg = Utils::VA::VA("Server didn't return enough data to pass security checks on %i"_Protect, PacketPing->m_Packet);
					}
				}
			}
		}

		if (ErrorMsg) {
			LOG_CUSTOM_ERROR("Auth"_Protect, ErrorMsg);
		}

		free(PacketPing);
		free(PacketPingResponse);
		return false;
	}

	bool PacketWelcome() {
		if (!Sockets::Network::Initialize()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize"_Protect);
			return false;
		}

		Request::PacketWelcome* PacketWelcome = Utils::AllocateMemoryType<Request::PacketWelcome>();
		PopulateHeader(PacketWelcome, PACKET_WELCOME, sizeof(Request::PacketWelcome));
		Response::PacketWelcome* PacketWelcomeResponse = Utils::AllocateMemoryType<Response::PacketWelcome>();

		memcpy(PacketWelcome->m_PasswordHash, Utils::ConvertStringToBytes(Utils::Rot13(Utils::Rot13NumberReverse(Global::Auth::g_Password))).data(), 0x40);
		memcpy(PacketWelcome->m_Username, Global::Auth::g_Username.c_str(), Global::Auth::g_Username.length());

		DWORD ComputerNameSize = sizeof(PacketWelcome->m_PCName);
		GetUserNameA(PacketWelcome->m_PCName, &ComputerNameSize);

		int ReceivedBytes = 0;
		bool Success; const char* ErrorMsg;

		Sockets::Network Network;

		Network.Create(&Success, &ErrorMsg);
		if (Success) {
			Network.Connect(&Success, &ErrorMsg);
			if (Success) {
				Request::PacketWelcome PacketWelcomeBackup;
				memcpy(&PacketWelcomeBackup, PacketWelcome, sizeof(PacketWelcomeBackup));

				EncryptPayload(PacketWelcome);
				LOG_CUSTOM_DEV("Auth", "Sending PACKET_WELCOME");
				Network.Send(PacketWelcome, PacketWelcome->m_Size);
				Network.Receive(&PacketWelcomeBackup, PacketWelcomeResponse, sizeof(Response::PacketWelcome), &ReceivedBytes, &Success, &ErrorMsg);
				LOG_CUSTOM_DEV("Auth", "Received PACKET_WELCOME");

				if (Success) {
					if (ReceivedBytes == sizeof(Response::PacketWelcome)) {
						memcpy(Global::Auth::g_AuthToken, PacketWelcomeResponse->m_AuthToken, 0x20);
						
						switch (PacketWelcomeResponse->m_ResponseStatus) {
							case RESPONSE_WELCOME_DISABLED_ACCOUNT:
								ErrorMsg = "This cheat is currently disabled. Check our Discord for more information"_Protect;
								break;

							case RESPONSE_WELCOME_NO_ACCOUNT:
								ErrorMsg = "Please check your credentials"_Protect;
								break;

							case RESPONSE_WELCOME_OUTDATED:
								ErrorMsg = "This cheat is outdated"_Protect;
								break;

							case RESPONSE_WELCOME_BANNED_ACCOUNT:
								ErrorMsg = "Your account is banned. Check the website for more information"_Protect;
								break;

							case RESPONSE_WELCOME_DISABLED:
								ErrorMsg = "Your account is disabled. Check the website for more information"_Protect;
								break;

							case RESPONSE_WELCOME_HWID_MISMATCH:
								ErrorMsg = "Your HWID doesn't match. Please go to ozark.gg and reset it"_Protect;
								break;

							case RESPONSE_WELCOME_PTB_DISABLED:
								ErrorMsg = "PTB is currently disabled"_Protect;
								break;

							case RESPONSE_SUCCESS:
								Global::Auth::g_HasToken = true;
								free(PacketWelcome);
								free(PacketWelcomeResponse);
								return true;
						}
					} else {
						ErrorMsg = Utils::VA::VA("Server didn't return enough data to pass security checks on %i"_Protect, PacketWelcome->m_Packet);
					}
				}
			}
		}

		if (ErrorMsg) {
			LOG_CUSTOM_ERROR("Auth"_Protect, ErrorMsg);
		}

		free(PacketWelcome);
		free(PacketWelcomeResponse);
		return false;
	}

	bool PacketGetTime() {
		if (!Sockets::Network::Initialize()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize"_Protect);
			return false;
		}

		Request::PacketGetTime* PacketGetTime = Utils::AllocateMemoryType<Request::PacketGetTime>();
		PopulateHeader(PacketGetTime, PACKET_GET_TIME, sizeof(Request::PacketGetTime));
		Response::PacketGetTime* PacketGetTimeResponse = Utils::AllocateMemoryType<Response::PacketGetTime>();

		int ReceivedBytes = 0;
		bool Success; const char* ErrorMsg;

		Sockets::Network Network;

		Network.Create(&Success, &ErrorMsg);
		if (Success) {
			Network.Connect(&Success, &ErrorMsg);
			if (Success) {
				Request::PacketGetTime PacketGetTimeBackup;
				memcpy(&PacketGetTimeBackup, PacketGetTime, sizeof(PacketGetTimeBackup));

				EncryptPayload(PacketGetTime);
				LOG_CUSTOM_DEV("Auth", "Sending PACKET_GET_TIME");
				Network.Send(PacketGetTime, PacketGetTime->m_Size);
				Network.Receive(&PacketGetTimeBackup, PacketGetTimeResponse, sizeof(Response::PacketGetTime), &ReceivedBytes, &Success, &ErrorMsg);
				LOG_CUSTOM_DEV("Auth", "Received PACKET_GET_TIME");

				if (Success) {
					if (ReceivedBytes == sizeof(Response::PacketGetTime)) {
						Global::Auth::g_HasLifetime = PacketGetTimeResponse->m_SecondsLeft == std::numeric_limits<int>::max();
						Global::Auth::g_SecondsLeft = PacketGetTimeResponse->m_SecondsLeft;

						switch (PacketGetTimeResponse->m_ResponseStatus) {
							case RESPONSE_ERROR:
								ErrorMsg = "An error occured getting your subscriptions"_Protect;
								break;

							case RESPONSE_SUCCESS:
								free(PacketGetTime);
								free(PacketGetTimeResponse);
								return true;
						}
					} else {
						ErrorMsg = Utils::VA::VA("Server didn't return enough data to pass security checks on %i"_Protect, PacketGetTime->m_Packet);
					}
				}
			}
		}

		if (ErrorMsg) {
			LOG_CUSTOM_ERROR("Auth"_Protect, ErrorMsg);
		}

		free(PacketGetTime);
		free(PacketGetTimeResponse);
		return false;
	}

	bool PacketGetSignatureData(uint32_t Size) {
		if (!Sockets::Network::Initialize()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize"_Protect);
			return false;
		}

		uint32_t TotalSize = sizeof(Response::PacketGetSignatures) + Size;

		Request::PacketGetSignatures* PacketGetSignatures = Utils::AllocateMemoryType<Request::PacketGetSignatures>();
		PopulateHeader(PacketGetSignatures, PACKET_GET_SIGNATURES, sizeof(Request::PacketGetSignatures));
		Response::PacketGetSignatures* PacketGetSignaturesResponse = (Response::PacketGetSignatures*)malloc(TotalSize);

		PacketGetSignatures->m_SizeOnly = false;

		int ReceivedBytes = 0;
		bool Success; const char* ErrorMsg;

		Sockets::Network Network;

		Network.Create(&Success, &ErrorMsg);
		if (Success) {
			Network.Connect(&Success, &ErrorMsg);
			if (Success) {
				Request::PacketGetSignatures PacketGetSignaturesBackup;
				memcpy(&PacketGetSignaturesBackup, PacketGetSignatures, sizeof(PacketGetSignaturesBackup));

				EncryptPayload(PacketGetSignatures);
				LOG_CUSTOM_DEV("Auth", "Sending PACKET_GET_SIGNATURES");
				Network.Send(PacketGetSignatures, PacketGetSignatures->m_Size);
				Network.Receive(&PacketGetSignaturesBackup, PacketGetSignaturesResponse, TotalSize, &ReceivedBytes, &Success, &ErrorMsg);
				LOG_CUSTOM_DEV("Auth", "Received PACKET_GET_SIGNATURES");

				if (Success) {
					if (ReceivedBytes == TotalSize) {
						switch (PacketGetSignaturesResponse->m_ResponseStatus) {
							case RESPONSE_ERROR:
								ErrorMsg = "An error occured getting menu data (#2)"_Protect;
								break;

							case RESPONSE_SUCCESS:
								uint8_t* Data = new uint8_t[Size];
								memcpy(Data, (uint8_t*)PacketGetSignaturesResponse + sizeof(Response::PacketGetSignatures), Size);

								for (uint32_t i = 0; i < Size; i++) {
									Request::BaseSignature* Signature = (Request::BaseSignature*)((uint64_t)Data + i);

									std::string SignatureStr = Utils::ConvertBytesToString(&Signature->m_Signature, Signature->m_SignatureLength, true);
									Utils::ReplaceString(SignatureStr, "3F"_Protect, "?"_Protect);
									Global::Auth::g_Signatures.insert(std::make_pair(Signature->m_Name, PatternInfo(SignatureStr, Signature->m_Offset, Signature->m_Name, Signature->m_Important)));

									i += sizeof(Request::BaseSignature) + Signature->m_SignatureLength - 2;
								}

								memset(Data, 0, Size);
								delete[] Data;

								free(PacketGetSignatures);
								free(PacketGetSignaturesResponse);
								return true;
						}
					} else {
						ErrorMsg = Utils::VA::VA("Server didn't return enough data to pass security checks on %i"_Protect, PacketGetSignatures->m_Packet);
					}
				}
			}
		}

		if (ErrorMsg) {
			LOG_CUSTOM_ERROR("Auth"_Protect, ErrorMsg);
		}

		free(PacketGetSignatures);
		free(PacketGetSignaturesResponse);
		return false;
	}

	bool PacketGetSignatures() {
		if (!Sockets::Network::Initialize()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize"_Protect);
			return false;
		}

		Request::PacketGetSignatures* PacketGetSignatures = Utils::AllocateMemoryType<Request::PacketGetSignatures>();
		PopulateHeader(PacketGetSignatures, PACKET_GET_SIGNATURES, sizeof(Request::PacketGetSignatures));
		Response::PacketGetSignaturesLength* PacketGetSignaturesResponse = Utils::AllocateMemoryType<Response::PacketGetSignaturesLength>();

		PacketGetSignatures->m_SizeOnly = true;

		int ReceivedBytes = 0;
		bool Success; const char* ErrorMsg;

		Sockets::Network Network;

		Network.Create(&Success, &ErrorMsg);
		if (Success) {
			Network.Connect(&Success, &ErrorMsg);
			if (Success) {
				Request::PacketGetSignatures PacketGetSignaturesBackup;
				memcpy(&PacketGetSignaturesBackup, PacketGetSignatures, sizeof(PacketGetSignaturesBackup));

				EncryptPayload(PacketGetSignatures);
				LOG_CUSTOM_DEV("Auth", "Sending PACKET_GET_SIGNATURES_LENGTH");
				Network.Send(PacketGetSignatures, PacketGetSignatures->m_Size);
				Network.Receive(&PacketGetSignaturesBackup, PacketGetSignaturesResponse, sizeof(Response::PacketGetSignaturesLength), &ReceivedBytes, &Success, &ErrorMsg);
				LOG_CUSTOM_DEV("Auth", "Received PACKET_GET_SIGNATURES_LENGTH");

				if (Success) {
					if (ReceivedBytes == sizeof(Response::PacketGetSignaturesLength)) {
						switch (PacketGetSignaturesResponse->m_ResponseStatus) {
							case RESPONSE_ERROR:
								ErrorMsg = "An error occured getting menu data (#1)"_Protect;
								break;

							case RESPONSE_SUCCESS:
								uint32_t Size = PacketGetSignaturesResponse->m_Length;
								free(PacketGetSignatures);
								free(PacketGetSignaturesResponse);
								return PacketGetSignatureData(Size);
						}
					} else {
						ErrorMsg = Utils::VA::VA("Server didn't return enough data to pass security checks on %i"_Protect, PacketGetSignatures->m_Packet);
					}
				}
			}
		}

		if (ErrorMsg) {
			LOG_CUSTOM_ERROR("Auth"_Protect, ErrorMsg);
		}

		free(PacketGetSignatures);
		free(PacketGetSignaturesResponse);
		return false;
	}

	bool PacketHeartbeat() {
		if (!Sockets::Network::Initialize()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize"_Protect);
			return false;
		}

		Request::PacketHeartbeat* PacketHeartbeat = Utils::AllocateMemoryType<Request::PacketHeartbeat>();
		PopulateHeader(PacketHeartbeat, PACKET_HEARTBEAT, sizeof(Request::PacketHeartbeat));
		Response::PacketHeartbeat* PacketHeartbeatResponse = Utils::AllocateMemoryType<Response::PacketHeartbeat>();

		strcpy(PacketHeartbeat->m_GameUsername, IsValidPtr(Global::Vars::g_RsInfo) ? Utils::VA::VA("%s:%i", Global::Vars::g_RsInfo->m_Name, Global::Vars::g_RsInfo->m_RockstarID) : "");
		strcpy(PacketHeartbeat->m_GameBuild, Utils::Rot13(Global::Auth::g_Build).c_str());

		int ReceivedBytes = 0;
		bool Success; const char* ErrorMsg;

		Sockets::Network Network;

		Network.Create(&Success, &ErrorMsg);
		if (Success) {
			Network.Connect(&Success, &ErrorMsg);
			if (Success) {
				Request::PacketHeartbeat PacketHeartbeatBackup;
				memcpy(&PacketHeartbeatBackup, PacketHeartbeat, sizeof(PacketHeartbeatBackup));

				EncryptPayload(PacketHeartbeat);
				LOG_CUSTOM_DEV("Auth", "Sending PACKET_HEARTBEAT");
				Network.Send(PacketHeartbeat, PacketHeartbeat->m_Size);
				Network.Receive(&PacketHeartbeatBackup, PacketHeartbeatResponse, sizeof(Response::PacketHeartbeat), &ReceivedBytes, &Success, &ErrorMsg);
				LOG_CUSTOM_DEV("Auth", "Received PACKET_HEARTBEAT");

				if (Success) {
					if (ReceivedBytes == sizeof(Response::PacketHeartbeat)) {
						switch (PacketHeartbeatResponse->m_ResponseStatus) {
							case RESPONSE_ERROR:
								ErrorMsg = "An error occured sending a heartbeat"_Protect;
								break;

							case RESPONSE_SUCCESS:
								Global::Auth::g_HasLifetime = PacketHeartbeatResponse->m_SecondsLeft == std::numeric_limits<int>::max();
								Global::Auth::g_SecondsLeft = PacketHeartbeatResponse->m_SecondsLeft;

								if (PacketHeartbeatResponse->m_UpdateAvailable) {
									// TODO: HotSwap
								}

								free(PacketHeartbeat);
								free(PacketHeartbeatResponse);
								return true;
						}
					} else {
						ErrorMsg = Utils::VA::VA("Server didn't return enough data to pass security checks on %i"_Protect, PacketHeartbeat->m_Packet);
					}
				}
			}
		}

		if (ErrorMsg) {
			LOG_CUSTOM_ERROR("Auth"_Protect, ErrorMsg);
		}

		free(PacketHeartbeat);
		free(PacketHeartbeatResponse);
		return false;
	}

	bool PacketMetric(Security::Metrics::Metric* Metric, std::string Info) {
		if (!Global::Auth::g_HasToken) return true;

		if (!Sockets::Network::Initialize()) {
			LOG_CUSTOM_ERROR("Auth"_Protect, "Failed to initialize"_Protect);
			return false;
		}

		Request::PacketMetric* PacketMetric = Utils::AllocateMemoryType<Request::PacketMetric>();
		PopulateHeader(PacketMetric, PACKET_METRIC, sizeof(Request::PacketMetric));
		Response::PacketMetric* PacketMetricResponse = Utils::AllocateMemoryType<Response::PacketMetric>();

		PacketMetric->m_MetricID = Metric->GetMetricID();
		if (!Info.empty()) {
			strcpy(PacketMetric->m_MetricInfo, Info.c_str());
		}

		int ReceivedBytes = 0;
		bool Success; const char* ErrorMsg;

		Sockets::Network Network;

		Network.Create(&Success, &ErrorMsg);
		if (Success) {
			Network.Connect(&Success, &ErrorMsg);
			if (Success) {
				Request::PacketMetric PacketMetricBackup;
				memcpy(&PacketMetricBackup, PacketMetric, sizeof(PacketMetricBackup));

				EncryptPayload(PacketMetric);
				LOG_CUSTOM_DEV("Auth", "Sending PACKET_METRIC");
				Network.Send(PacketMetric, PacketMetric->m_Size);
				Network.Receive(&PacketMetricBackup, PacketMetricResponse, sizeof(Response::PacketMetric), &ReceivedBytes, &Success, &ErrorMsg);
				LOG_CUSTOM_DEV("Auth", "Received PACKET_METRIC");

				if (Success) {
					if (ReceivedBytes == sizeof(Response::PacketMetric)) {
						if (PacketMetricResponse->m_Processed) {
							if (PacketMetricResponse->m_Banned) {
								Menu::GetInit()->Unload();
							}

							free(PacketMetric);
							free(PacketMetricResponse);
							return true;
						}
					} else {
						ErrorMsg = Utils::VA::VA("Server didn't return enough data to pass security checks on %i"_Protect, PacketMetric->m_Packet);
					}
				}
			}
		}

		if (ErrorMsg) {
			LOG_CUSTOM_ERROR("Auth"_Protect, ErrorMsg);
		}

		free(PacketMetric);
		free(PacketMetricResponse);
		return false;
	}
}