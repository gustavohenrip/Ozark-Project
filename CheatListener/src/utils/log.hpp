#pragma once
#include "stdafx.hpp"
#include "networking/sockets.hpp"

namespace Log {
	void Misc(std::string Values);
	void Misc(std::string Values, Network::Socket Client);
	void Connection(Network::Socket Client, std::string PacketName, std::string Values = "");
	void Success(Network::Socket Client, std::string PacketName, std::string Values = "");
	void Error(Network::Socket Client, std::string PacketName, std::string Values = "");
	void Warn(Network::Socket Client, std::string PacketName, std::string Values = "");
	void Info(Network::Socket Client, std::string PacketName, std::string Values = "");
}