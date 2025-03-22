#pragma once
#include "stdafx.hpp"
#include "utils/structs.hpp"
#include "utils/io/binary_writer.hpp"
#include "sockets.hpp"

namespace Network {
	struct EncryptionProcess {
		Structs::Response::EncryptionKeys m_Keys;

		// Encryption only
		uint32_t m_Time;
		Structs::Request::Header* m_Header;
	};

	EncryptionProcess* StartEncryptionProcess(IO::BinaryWriter* Writer, Structs::Request::Header* Header);
	void GenerateEncryptionKeys(EncryptionProcess* Process);
	void FinalizeEncryptionKeys(EncryptionProcess* Process);
	void FinalizeEncryptionHash(EncryptionProcess* Process);
	void SendEncryptedPacket(Socket Writer, uint8_t* Packet, uint32_t Length, EncryptionProcess* Process);
}