#include "handler.hpp"
#include "../sockets.hpp"
#include "utils/utils.hpp"
#include "utils/io/binary_reader.hpp"
#include "networking/crypto/crypto.hpp"
#include "packets.hpp"
#include "mysql/mysql_funcs.hpp"
#include "utils/log.hpp"

namespace Network::Handler {
	void HeartbeatListener(void*) {
		while (true) {
			// Delete inactive tokens
			auto Tokens = MySQL::GetAuthTokens();
			std::vector<std::pair<std::string, std::string>> RemovalQueue;

			if (Tokens.size()) {
				uint32_t Timestamp = (uint32_t)std::time(0);
				for (auto& Token : Tokens) {
					if (!Token.m_InitialConnectionReceived) {
						if ((Timestamp - Token.m_InitialConnection) > 120) {
							RemovalQueue.push_back({ Token.m_Token, "Initial HB not received" });
							continue;
						}
					}

					if ((Timestamp - Token.m_LastConnection) > 420) {
						RemovalQueue.push_back({ Token.m_Token, "HB not updated" });
					}
				}

				for (auto& Token : RemovalQueue) {
					Log::Misc("Deleting token " + Token.first + " - " + Token.second);
					MySQL::DeleteAuthToken(Token.first.c_str());
				}

				Tokens.clear();
				RemovalQueue.clear();
			}

			// Delete heartbeats over 48 hours old
			MySQL::DeleteOldHeartbeats();

			sleep(60);
		}
	}

	void ClientListener(void*) {
		Network::Socket Server = Network::GetServerSocket();
		if (Server.Listen()) {
			sockaddr_in Address;
			while (true) {
				socklen_t Length = sizeof(Address);
				int Socket = accept(Server.GetSocket(), (sockaddr*)&Address, &Length);

				Structs::ConnectionInfo* Info = (Structs::ConnectionInfo*)malloc(sizeof(Structs::ConnectionInfo));
				if (!Info) {
					printf("failed to alloc\n");
					close(Socket);
					continue;
				}

				Info->m_Socket = Socket;
				snprintf(Info->m_IP, 16, "%d.%d.%d.%d", (Address.sin_addr.s_addr & 0xFF), ((Address.sin_addr.s_addr & 0xFF00) >> 8), ((Address.sin_addr.s_addr & 0xFF0000) >> 16), ((Address.sin_addr.s_addr & 0xFF000000) >> 24));
			
				Utils::CreateThread((void*)Handler, Info);
				memset(&Address, 0, sizeof(Address));
			}
		} else {
			printf("failed to start listener\n");
		}
	}

	bool PacketSizeVerification(Structs::Request::Header Header) {
		Structs::ePackets Packet = (Structs::ePackets)Header.m_Packet;
		switch (Packet) {
			case Structs::PACKET_PING:
				return Header.m_Size == sizeof(Structs::Request::PacketPing);

			case Structs::PACKET_WELCOME:
				return Header.m_Size == sizeof(Structs::Request::PacketWelcome);

			case Structs::PACKET_GET_TIME:
				return Header.m_Size == sizeof(Structs::Request::PacketGetTime);

			case Structs::PACKET_METRIC:
				return Header.m_Size == sizeof(Structs::Request::PacketMetric);

			case Structs::PACKET_GET_SIGNATURES:
				return Header.m_Size == sizeof(Structs::Request::PacketGetSignatures);

			case Structs::PACKET_HEARTBEAT:
				return Header.m_Size == sizeof(Structs::Request::PacketHeartbeat);

			case Structs::PACKET_END:
				return false;
		}

		return false;
	}

	bool AuthTokenValidation(Structs::Request::Header Header) {
		if (Header.m_Packet == Structs::PACKET_PING || Header.m_Packet == Structs::PACKET_WELCOME) return true;

		auto AuthToken = Utils::ConvertBytesToString(Header.m_AuthToken, sizeof(Header.m_AuthToken));
		if (MySQL::IsAuthTokenValid(AuthToken.c_str())) {
			MySQL::RevalidateAuthToken(AuthToken.c_str());
			return true;
		}

		return false;
	}

	void BlockIP(const char* IP) {
		char yes[50];
		snprintf(yes, 50, "ipset add pog %s", IP); // old: losers
		system(yes);
	}

	void Handler(Structs::ConnectionInfo* Info) {
		Network::Socket Socket(Info->m_Socket);
		Socket.SetIP(Info->m_IP);

		// Attempt to read the magic from our header in the payload
		uint8_t Magic[4];
		if (!Socket.Receive(Magic, sizeof(Magic))) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to receive magic - " + std::to_string(Info->m_Socket), Socket);
			Log::Misc("Manual attempt to read - " + std::to_string(recv(Info->m_Socket, Magic, 4, MSG_NOSIGNAL)), Socket);
			Socket.SendError(Structs::ERROR_CANT_READ_MAGIC);
			Socket.Close({ Info });
			return;
		}

		// Attempt to parse the magic into a checkable format
		IO::BinaryReader MagicReader = IO::BinaryReader(Magic, sizeof(Magic));
		uint32_t MagicValue = MagicReader.ReadUInt32();
		MagicReader.CleanAllocations();

		if (MagicValue != 0x434F434B) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to validate magic - " + std::to_string(MagicValue), Socket);
			Socket.SendError(Structs::ERROR_INVALID_MAGIC);
			Socket.Close({ Info });
			return;
		}

		uint8_t RequiredHeader[4];
		if (!Socket.Receive(RequiredHeader, sizeof(RequiredHeader))) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to read required header", Socket);
			Socket.SendError(Structs::ERROR_CANT_READ_REQUIRED_HEADER);
			Socket.Close({ Info });
			return;
		}

		Structs::Request::Header Header;

		IO::BinaryReader RequiredHeaderReader = IO::BinaryReader(RequiredHeader, sizeof(RequiredHeader));
		Header.m_Size = RequiredHeaderReader.ReadUInt16();
		Header.m_Packet = RequiredHeaderReader.ReadByte();
		Header.m_Cheat.m_Cheat = RequiredHeaderReader.ReadByte();
		RequiredHeaderReader.CleanAllocations();

		if (!PacketSizeVerification(Header)) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to verify packet size", Socket);
			Socket.SendError(Structs::ERROR_INVALID_PACKET_SIZE);
			Socket.Close({ Info });
			return;
		}

		if (Header.m_Size < sizeof(Structs::Request::Header) || Header.m_Size > 0x500) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to verify header size", Socket);
			Socket.SendError(Structs::ERROR_INVALID_SIZE);
			Socket.Close({ Info });
			return;
		}

		if (Header.m_Packet < Structs::PACKET_PING || Header.m_Packet >= Structs::PACKET_END) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to verify packet id", Socket);
			Socket.SendError(Structs::ERROR_INVALID_PACKET);
			Socket.Close({ Info });
			return;
		}

		if (Header.m_Cheat.m_Cheat < Structs::CHEAT_GRAND_THEFT_AUTO_V || Header.m_Cheat.m_Cheat >= Structs::CHEAT_END) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to verify cheat id", Socket);
			Socket.SendError(Structs::ERROR_INVALID_CHEAT);
			Socket.Close({ Info });
			return;
		}

		uint32_t DataSize = Header.m_Size - (uint32_t)sizeof(RequiredHeader) - (uint32_t)sizeof(Magic);
		
		uint8_t* Data = (uint8_t*)malloc(DataSize);
		if (!Socket.Receive(Data, DataSize)) {
			BlockIP(Info->m_IP);
			Log::Misc("Failed to read header data", Socket);
			Socket.SendError(Structs::ERROR_CANT_READ_HEADER_DATA);
			Socket.Close({ Info, Data });
			return;
		}
		
		IO::BinaryReader DataReader = IO::BinaryReader(Data, DataSize);
		Header.m_Cheat.m_CheatVersion = DataReader.ReadUInt32();
		Header.m_Cheat.m_Tester = DataReader.ReadBool();
		DataReader.CopyBytes(Header.m_AuthToken, sizeof(Header.m_AuthToken));
		DataReader.CopyBytes(Header.m_HardwareID, sizeof(Header.m_HardwareID));
		Header.m_EncryptionKeys.m_XorKey = DataReader.ReadByte();
		DataReader.CopyBytes(Header.m_EncryptionKeys.m_TokenKey, sizeof(Header.m_EncryptionKeys.m_TokenKey));
		DataReader.CopyBytes(Header.m_EncryptionKeys.m_HardwareKey, sizeof(Header.m_EncryptionKeys.m_HardwareKey));
		DataReader.CopyBytes(Header.m_EncryptionKeys.m_RequestKey, sizeof(Header.m_EncryptionKeys.m_RequestKey));

		Crypto::RC4(Header.m_EncryptionKeys.m_TokenKey, sizeof(Header.m_EncryptionKeys.m_TokenKey), Header.m_AuthToken, sizeof(Header.m_AuthToken));
		Crypto::RC4(Header.m_EncryptionKeys.m_HardwareKey, sizeof(Header.m_EncryptionKeys.m_HardwareKey), Header.m_HardwareID, sizeof(Header.m_HardwareID));

		if (Header.m_Size > sizeof(Header)) {
			Crypto::RC4Modified(Header.m_EncryptionKeys.m_RequestKey, sizeof(Header.m_EncryptionKeys.m_RequestKey), Data, DataSize, DataReader.GetCurrentIndex());
		}

		if (AuthTokenValidation(Header)) {
			switch (Header.m_Packet) {
				case Structs::PACKET_PING:
					Packets::PacketPing(DataReader, Socket, Header);
					break;

				case Structs::PACKET_WELCOME:
					Packets::PacketWelcome(DataReader, Socket, Header);
					break;

				case Structs::PACKET_GET_TIME:
					Packets::PacketGetTime(DataReader, Socket, Header);
					break;

				case Structs::PACKET_METRIC:
					Packets::PacketMetric(DataReader, Socket, Header);
					break;

				case Structs::PACKET_GET_SIGNATURES:
					Packets::PacketGetSignatures(DataReader, Socket, Header);
					break;

				case Structs::PACKET_HEARTBEAT:
					Packets::PacketHeartbeat(DataReader, Socket, Header);
					break;
			}
		} else {
			Socket.SendError(Structs::ERROR_AUTH_TOKEN_NOT_FOUND);
			BlockIP(Info->m_IP);
		}

		DataReader.CleanAllocations();
		Socket.Close({ Info, Data });
	}
}