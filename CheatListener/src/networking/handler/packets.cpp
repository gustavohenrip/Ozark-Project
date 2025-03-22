#include "packets.hpp"
#include "utils/io/binary_writer.hpp"
#include "../socket_encryption.hpp"
#include "utils/structs.hpp"
#include "utils/utils.hpp"
#include "mysql/mysql_funcs.hpp"
#include "utils/log.hpp"

namespace Packets {
	void PacketPing(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header) {
		Log::Connection(ServerWriter, "PacketPing");

		Structs::Response::PacketPing Packet;
		IO::BinaryWriter Writer = IO::BinaryWriter((uint8_t*)&Packet, sizeof(Packet));

		auto Encryption = Network::StartEncryptionProcess(&Writer, &Header);
		Writer.WriteByte(69);
		Writer.CleanAllocations();

		Network::SendEncryptedPacket(ServerWriter, (uint8_t*)&Packet, sizeof(Packet), Encryption);
	}

	void PacketWelcome(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header) {
		// Log::Connection(ServerWriter, "PacketWelcome");
		
		const char* Username = Reader.ReadChars(100);
		const char* ComputerName = Reader.ReadChars(32);

		uint8_t PasswordHash[0x40];
		Reader.CopyBytes(PasswordHash, sizeof(PasswordHash));

		Structs::Response::PacketWelcome Packet;
		IO::BinaryWriter Writer = IO::BinaryWriter((uint8_t*)&Packet, sizeof(Packet));
		
		Structs::eResponseStatus Status = Structs::RESPONSE_WELCOME_NO_ACCOUNT;
		Structs::eAccountStatus AccountStatus = Structs::ACCOUNT_OK;
		std::string HardwareID = "";
		Structs::ClientInfo ClientInfo;
		Structs::CheatInfo CheatInfo;

		Utils::GenerateRandomBytes(Packet.m_AuthToken, sizeof(Packet.m_AuthToken));

		if (!MySQL::GetClientInfo(Username, Utils::ConvertBytesToString(PasswordHash, sizeof(PasswordHash)).c_str(), &ClientInfo)) {
			Log::Warn(ServerWriter, "PacketWelcome", "Account doesn't exist");
			goto end;
		}

		/*if (ClientInfo.m_ID != 28825
			&& ClientInfo.m_ID != 1
			&& ClientInfo.m_ID != 308
			&& ClientInfo.m_ID != 9
			&& ClientInfo.m_ID != 6854) {
			Status = Structs::RESPONSE_WELCOME_DISABLED;
			goto end;
		}*/

		AccountStatus = (Structs::eAccountStatus)ClientInfo.m_Status["status"].get<int>();
		MySQL::UpdateClientIP(ClientInfo, ServerWriter.GetIP());

		if (AccountStatus == Structs::ACCOUNT_DISABLED || AccountStatus == Structs::ACCOUNT_BANNED) {
			Log::Error(ServerWriter, "PacketWelcome", "Account is " + std::string(AccountStatus == Structs::ACCOUNT_DISABLED ? "disabled" : "banned"));
			Status = AccountStatus == Structs::ACCOUNT_DISABLED ? Structs::RESPONSE_WELCOME_DISABLED_ACCOUNT : Structs::RESPONSE_WELCOME_BANNED_ACCOUNT;
			goto end;
		}

		if (ClientInfo.m_Rank == Structs::RANK_USER) {
			if (MySQL::GetCheatInfo(Header.m_Cheat.m_Cheat, &CheatInfo)) {
				if (CheatInfo.m_CurrentVersion > Header.m_Cheat.m_CheatVersion) {
					Log::Warn(ServerWriter, "PacketWelcome", "Cheat is outdated");
					Status = Structs::RESPONSE_WELCOME_OUTDATED;
					goto end;
				}

				if (!CheatInfo.m_Public) {
					bool Tester = ClientInfo.m_Flags["tester"].get<bool>();
					if (CheatInfo.m_Tester && Tester && Header.m_Cheat.m_Tester) {
						goto cont;
					}

					Log::Warn(ServerWriter, "PacketWelcome", "Cheat is disabled");
					Status = Structs::RESPONSE_WELCOME_DISABLED;
					goto end;
				}
			}
		}

	cont:
		HardwareID = Utils::ConvertBytesToString(Header.m_HardwareID, sizeof(Header.m_HardwareID));
		if (MySQL::IsHardwareIDBanned(HardwareID.c_str())) {
			Log::Error(ServerWriter, "PacketWelcome", "Hardware is banned");
			Status = Structs::RESPONSE_WELCOME_BANNED_ACCOUNT;
			MySQL::UpdateClientStatus(ClientInfo, Structs::ACCOUNT_BANNED, "Hardware has been previously banned");
			goto end;
		}

		if (ClientInfo.m_Hwid["hwid"].get<std::string>() == "-none-") {
			MySQL::UpdateClientHardwareID(ClientInfo, HardwareID.c_str());
		} else {
			if (ClientInfo.m_Hwid["hwid"].get<std::string>().compare(HardwareID)) {
				Log::Warn(ServerWriter, "PacketWelcome", "Hardware doesn't match");
				Status = Structs::RESPONSE_WELCOME_HWID_MISMATCH;
				goto end;
			}
		}

		Status = Structs::RESPONSE_SUCCESS;
		MySQL::AddAuthToken(ClientInfo, (Structs::eCheats)Header.m_Cheat.m_Cheat, Utils::ConvertBytesToString(Packet.m_AuthToken, sizeof(Packet.m_AuthToken)).c_str(), ComputerName);
		MySQL::UpdateClientAuthentications(ClientInfo);

		Log::Connection(ServerWriter, "PacketWelcome", "Token: " + Utils::ConvertBytesToString(Packet.m_AuthToken, sizeof(Packet.m_AuthToken)));
	end:
		auto Encryption = Network::StartEncryptionProcess(&Writer, &Header);
		Writer.WriteInt32(Status);
		Writer.WriteBytes(Packet.m_AuthToken, sizeof(Packet.m_AuthToken));
		Writer.CleanAllocations();
		
		Network::SendEncryptedPacket(ServerWriter, (uint8_t*)&Packet, sizeof(Packet), Encryption);
	}

	void PacketGetTime(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header) {
		// Log::Connection(ServerWriter, "PacketGetTime");

		Structs::Response::PacketGetTime Packet;
		IO::BinaryWriter Writer = IO::BinaryWriter((uint8_t*)&Packet, sizeof(Packet));

		Structs::eResponseStatus Status = Structs::RESPONSE_SUCCESS;
		Structs::AuthToken AuthToken;
		Structs::ClientSubscription ClientSubscription;
		uint32_t SecondsLeft = 0;

		bool Freemode = MySQL::IsCheatInFreemode((Structs::eCheats)Header.m_Cheat.m_Cheat);

		if (MySQL::GetAuthToken(Utils::ConvertBytesToString(Header.m_AuthToken, sizeof(Header.m_AuthToken)).c_str(), &AuthToken)) {
			if (MySQL::GetClientSubscription(AuthToken.m_AccountID, AuthToken.m_Cheat, &ClientSubscription)) {
				if (ClientSubscription.m_End == 0x7FFFFFFF) {
					SecondsLeft = 0x7FFFFFFF;
					Log::Connection(ServerWriter, "PacketGetTime", "Lifetime");
				} else {
					// Doesn't have lifetime, let's check when the sub is supposed to expire! :)
					if (ClientSubscription.m_End > (uint32_t)time(0)) {
						SecondsLeft = ClientSubscription.m_End - (uint32_t)time(0);
					}

					if (SecondsLeft == 0) {
						if (Freemode) {
							Log::Connection(ServerWriter, "PacketGetTime", "No time, but we're in freemode");
							goto end;
						}

						Log::Connection(ServerWriter, "PacketGetTime", "No time");
						Status = Structs::RESPONSE_ERROR;
					} else {
						Structs::TimeCalculations Time(SecondsLeft);
						Log::Connection(ServerWriter, "PacketGetTime", std::to_string(Time.m_Days) + "D " + std::to_string(Time.m_Hours) + "H " + std::to_string(Time.m_Minutes) + "M " + std::to_string(Time.m_Seconds) + "S");
					}
				}
			} else {
				if (Freemode) {
					Log::Error(ServerWriter, "PacketGetTime", "Failed to find subscription, but we're in freemode");
					goto end;
				}

				Log::Error(ServerWriter, "PacketGetTime", "Failed to find subscription");
				Status = Structs::RESPONSE_ERROR;
			}
		} else {
			Log::Error(ServerWriter, "PacketGetTime", "Failed to find auth token");
			Status = Structs::RESPONSE_ERROR;
		}

	end:
		auto Encryption = Network::StartEncryptionProcess(&Writer, &Header);
		Writer.WriteInt32(Status);
		Writer.WriteUInt32(Freemode && SecondsLeft == 0 ? 86400 : SecondsLeft);
		Writer.CleanAllocations();

		Network::SendEncryptedPacket(ServerWriter, (uint8_t*)&Packet, sizeof(Packet), Encryption);

		if (Status != Structs::RESPONSE_SUCCESS) {
			MySQL::DeleteAuthToken(Utils::ConvertBytesToString(Header.m_AuthToken, sizeof(Header.m_AuthToken)).c_str());
		}
	}

	void PacketMetric(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header) {
		Log::Connection(ServerWriter, "PacketMetric");

		uint8_t MetricID = Reader.ReadByte();
		const char* MetricInfo = Reader.ReadChars(200);
		if (!MetricInfo) MetricInfo = "-none-";

		Structs::Response::PacketMetric Packet;
		IO::BinaryWriter Writer = IO::BinaryWriter((uint8_t*)&Packet, sizeof(Packet));
		
		Structs::AuthToken AuthToken;
		Structs::ClientInfo ClientInfo;
		std::vector<Structs::ClientMetric> Metrics;
		bool Processed = false;
		bool Banned = false;

		static std::vector<std::pair<Structs::eMetricID, Structs::eMetricType>> MetricInfos = {
			{ Structs::METRIC_THREAD_CREATED_IN_UNMAPPED_MEMORY, Structs::METRIC_INFO },
			{ Structs::METRIC_DEBUGGING, Structs::METRIC_BAN },
			{ Structs::METRIC_THREAD_BEING_DEBUGGED, Structs::METRIC_BAN },
			{ Structs::METRIC_FUNCTION_INTEGRITY_FAILED, Structs::METRIC_WARNING },
			{ Structs::METRIC_BLACKLISTED_PROCESS, Structs::METRIC_WARNING },
			{ Structs::METRIC_MODULE_DIGEST_MISMATCH, Structs::METRIC_BAN },
			{ Structs::METRIC_CRASHED, Structs::METRIC_INFO },
		};

		if (MetricID > (uint8_t)MetricInfos.size()) {
			Log::Error(ServerWriter, "PacketMetric", "ID is too big");
			goto end;
		}

		if (MySQL::GetAuthToken(Utils::ConvertBytesToString(Header.m_AuthToken, sizeof(Header.m_AuthToken)).c_str(), &AuthToken)) {
			MySQL::AddMetric(AuthToken.m_AccountID, AuthToken.m_Cheat, (int)MetricID, MetricInfos[MetricID].second, MetricInfo);

			if (MySQL::GetClientInfoByID(AuthToken.m_AccountID, &ClientInfo)) {
				if (MySQL::GetClientMetrics(AuthToken.m_AccountID, Metrics)) {
					Processed = true;

					int WarningCount = 0;
					for (auto& Metric : Metrics) {
						if (Metric.m_Type == Structs::METRIC_WARNING) {
							WarningCount++;
						} else {
							if (Metric.m_Type == Structs::METRIC_BAN) {
								Banned = true;
								// MySQL::UpdateClientStatus(ClientInfo, Structs::ACCOUNT_BANNED, "Account activity is very suspicious");
								Log::Error(ServerWriter, "PacketMetric", "Account has been banned");
								goto end;
							}
						}
					}

					if (WarningCount >= 5) {
						Banned = true;
						// MySQL::UpdateClientStatus(ClientInfo, Structs::ACCOUNT_DISABLED, "Account activity is suspicious");
						Log::Warn(ServerWriter, "PacketMetric", "Account has been disabled");
						goto end;
					}
				}
			}
		}

	end:
		auto Encryption = Network::StartEncryptionProcess(&Writer, &Header);
		Writer.WriteByte(Processed);
		Writer.WriteByte(Banned);
		Writer.CleanAllocations();

		Network::SendEncryptedPacket(ServerWriter, (uint8_t*)&Packet, sizeof(Packet), Encryption);
	}

	void PacketGetSignatures(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header) {
		// Log::Connection(ServerWriter, "PacketGetSignatures");

		uint8_t* Response = nullptr;
		uint8_t* Data = nullptr;
		uint32_t Size = 0;
		Structs::eResponseStatus Status = Structs::RESPONSE_ERROR;
		Structs::AuthToken AuthToken;
		IO::BinaryWriter Writer;
		IO::BinaryWriter Writer2;

		struct Signature {
			std::string m_Name;
			std::vector<uint8_t> m_Signature;
			int m_Offset;
			bool m_Important;
		};

		struct SignatureByte {
			SignatureByte()
				: m_Data(0x3F)
			{}

			SignatureByte(std::string Pattern)
				: m_Data(StringToUint8(Pattern))
			{}

			uint8_t m_Data;
		private:
			uint8_t StringToUint8(std::string Pattern) {
				std::istringstream Stream(Pattern);
				uint32_t Current = 0;

				if (Stream >> std::hex >> Current) {
					return (uint8_t)Current;
				}

				return 0;
			}
		};

		auto GetSignatures = [=] (Structs::eCheats Cheat) {
			std::vector<Signature> Signatures;

			std::ifstream Input(Utils::GetCurrentPath() + "CHEAT_" + std::to_string(Header.m_Cheat.m_Cheat) + "_SIGS.txt");
			if (Input.good()) {
				std::string Line;
				while (std::getline(Input, Line)) {
					auto Split = Utils::Split(Line, ':');
					if (Split.size()) {
						Signature Sig;

						std::istringstream Stream(Split[3]);
						std::string Current;

						while (Stream >> Current) {
							if (Current.data()[0] == '?') {
								Sig.m_Signature.push_back(SignatureByte().m_Data);
							} else if (Current.length() == 2 && isxdigit(Current.data()[0]) && isxdigit(Current.data()[1])) {
								Sig.m_Signature.push_back(SignatureByte(Current).m_Data);
							}
						}

						Sig.m_Name = Split[0];
						Sig.m_Offset = atoi(Split[1].c_str());
						Sig.m_Important = Split[2] == "true";
						Signatures.push_back(Sig);
					}
				}
			}

			return Signatures;
		};

		if (MySQL::GetAuthToken(Utils::ConvertBytesToString(Header.m_AuthToken, sizeof(Header.m_AuthToken)).c_str(), &AuthToken)) {
			auto Signatures = GetSignatures(AuthToken.m_Cheat);
			if (Signatures.size()) {
				for (std::size_t i = 0; i < Signatures.size(); i++) {
					Size += (uint32_t)Signatures[i].m_Signature.size() + 0x10;
				}

				Data = (uint8_t*)malloc(Size);
				if (Data) {
					memset(Data, 0, Size);
					IO::BinaryWriter SignatureWriter = IO::BinaryWriter(Data, Size);

					for (std::size_t i = 0; i < Signatures.size(); i++) {
						uint8_t Name[10];
						memset(Name, 0, sizeof(Name));
						memcpy(Name, Signatures[i].m_Name.c_str(), Signatures[i].m_Name.size());

						SignatureWriter.WriteBytes(Name, sizeof(Name));
						SignatureWriter.WriteInt32(Signatures[i].m_Offset);
						SignatureWriter.WriteByte(Signatures[i].m_Important);
						SignatureWriter.WriteByte((uint8_t)Signatures[i].m_Signature.size());
						SignatureWriter.WriteBytes(Signatures[i].m_Signature.data(), (uint32_t)Signatures[i].m_Signature.size());
					}

					SignatureWriter.CleanAllocations();

					Status = Structs::RESPONSE_SUCCESS;
					Log::Success(ServerWriter, "PacketGetSignatures", "Sending " + std::to_string(Signatures.size()) + " signatures");
				} else {
					Log::Error(ServerWriter, "PacketGetSignatures", "Failed to allocate memory for signatures");
					ServerWriter.SendError(Structs::ERROR_ALLOCATING_MEMORY);
					return;
				}
			} else {
				Log::Warn(ServerWriter, "PacketGetSignatures", "Failed to find signatures - " + std::to_string(Header.m_Cheat.m_Cheat) + " - " + Utils::GetCurrentPath());
			}
		} else {
			Log::Error(ServerWriter, "PacketGetSignatures", "Failed to find auth token");
		}

		// keys + status + size
		uint32_t FirstSize = (uint32_t)sizeof(Structs::Response::EncryptionKeys) + 4 + 4 + Size;
		Response = (uint8_t*)malloc(FirstSize);
		if (!Response) {
			if (Data) free(Data);
			ServerWriter.SendError(Structs::ERROR_ALLOCATING_MEMORY);
			return;
		}

		memset(Response, 0, FirstSize);
		Writer = IO::BinaryWriter(Response, FirstSize - Size);

		auto Encryption = Network::StartEncryptionProcess(&Writer, &Header);

		Writer.WriteInt32(Status);
		Writer.WriteUInt32(Size);

		Writer.CleanAllocations();
		Network::SendEncryptedPacket(ServerWriter, Response, FirstSize - Size, Encryption);

		memset(Response, 0, FirstSize);
		Writer2 = IO::BinaryWriter(Response, Size + (uint32_t)sizeof(Structs::Response::EncryptionKeys));

		Encryption = Network::StartEncryptionProcess(&Writer2, &Header);

		Writer2.WriteBytes(Data, Size);
		Writer2.CleanAllocations();

		Network::SendEncryptedPacket(ServerWriter, Response, Size + (uint32_t)sizeof(Structs::Response::EncryptionKeys), Encryption);

		free(Response);
		if (Data) free(Data);
	}

	void PacketHeartbeat(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header) {
		//Log::Connection(ServerWriter, "PacketHeartbeat");

		const char* GameUsername = Reader.ReadChars(32);
		const char* GameBuild = Reader.ReadChars(32);
		uint32_t RockstarID = Reader.ReadUInt32();

		Structs::Response::PacketHeartbeat Packet;
		IO::BinaryWriter Writer = IO::BinaryWriter((uint8_t*)&Packet, sizeof(Packet));

		Structs::eResponseStatus Status = Structs::RESPONSE_SUCCESS;
		Structs::AuthToken AuthToken;
		Structs::ClientSubscription ClientSubscription;
		uint32_t SecondsLeft = 0;
		bool UpdateAvailable = false;

		auto Token = Utils::ConvertBytesToString(Header.m_AuthToken, sizeof(Header.m_AuthToken));
		if (MySQL::GetAuthToken(Token.c_str(), &AuthToken)) {
			MySQL::UpdateAuthTokenHeartbeat(Token.c_str());
			
			nlohmann::json Json;
			Json["u"] = GameUsername;
			Json["b"] = GameBuild;
			Json["r"] = RockstarID;
			MySQL::AddHeartbeat(AuthToken.m_AccountID, AuthToken.m_Cheat, ServerWriter.GetIP(), Json);

			bool Freemode = MySQL::IsCheatInFreemode((Structs::eCheats)Header.m_Cheat.m_Cheat);
			if (MySQL::GetClientSubscription(AuthToken.m_AccountID, AuthToken.m_Cheat, &ClientSubscription)) {
				UpdateAvailable = MySQL::IsCheatOutdated(AuthToken.m_Cheat, Header.m_Cheat.m_CheatVersion);

				if (ClientSubscription.m_End == 0x7FFFFFFF) {
					SecondsLeft = 0x7FFFFFFF;
					Log::Connection(ServerWriter, "PacketHeartbeat", "Lifetime");
				} else {
					if (ClientSubscription.m_End > (uint32_t)time(0)) {
						SecondsLeft = ClientSubscription.m_End - (uint32_t)time(0);
					}

					if (SecondsLeft == 0) {
						if (Freemode) {
							Log::Connection(ServerWriter, "PacketHeartbeat", "No time, but we're in freemode");
							goto end;
						}

						Log::Connection(ServerWriter, "PacketHeartbeat", "No time");
						Status = Structs::RESPONSE_ERROR;
					} else {
						Structs::TimeCalculations Time(SecondsLeft);
						Log::Connection(ServerWriter, "PacketHeartbeat", std::to_string(Time.m_Days) + "D " + std::to_string(Time.m_Hours) + "H " + std::to_string(Time.m_Minutes) + "M " + std::to_string(Time.m_Seconds) + "S");
					}
				}
			} else {
				if (Freemode) {
					Log::Error(ServerWriter, "PacketHeartbeat", "Failed to find subscription, but we're in freemode");
					goto end;
				}

				Log::Error(ServerWriter, "PacketHeartbeat", "Failed to find subscription");
				Status = Structs::RESPONSE_ERROR;
			}
		} else {
			Log::Error(ServerWriter, "PacketHeartbeat", "Failed to find auth token");
			Status = Structs::RESPONSE_ERROR;
		}

	end:
		auto Encryption = Network::StartEncryptionProcess(&Writer, &Header);

		Writer.WriteInt32(Status);
		Writer.WriteUInt32(SecondsLeft);
		Writer.WriteByte(UpdateAvailable);

		Network::SendEncryptedPacket(ServerWriter, (uint8_t*)&Packet, sizeof(Packet), Encryption);

		if (Status != Structs::RESPONSE_SUCCESS) {
			MySQL::DeleteAuthToken(Token.c_str());
		}
	}
}