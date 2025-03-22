#pragma once
#include "stdafx.hpp"
#include "server_types.hpp"
#include "security/security.hpp"

namespace Auth::Server {
	void PopulateHeader(Request::Header* Header, ePackets Packet, uint32_t Size);
	bool PacketPing();
	bool PacketWelcome();
	bool PacketGetTime();
	bool PacketGetSignatures();
	bool PacketHeartbeat();
	bool PacketMetric(Security::Metrics::Metric* Metric, std::string Info);
}