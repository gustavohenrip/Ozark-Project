#pragma once
#include "stdafx.hpp"
#include "utils/io/binary_reader.hpp"
#include "networking/sockets.hpp"
#include "utils/structs.hpp"

namespace Packets {
	void PacketPing(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header);
	void PacketWelcome(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header);
	void PacketGetTime(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header);
	void PacketMetric(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header);
	void PacketGetSignatures(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header);
	void PacketHeartbeat(IO::BinaryReader Reader, Network::Socket ServerWriter, Structs::Request::Header Header);
}