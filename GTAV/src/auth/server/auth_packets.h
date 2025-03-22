#pragma once
#include "stdafx.h"
#include "auth_types.h"

namespace auth::server {
	void populate_header(request::auth_header* header, uint8_t packet, uint32_t size);
	bool packet_ping();
	bool packet_welcome();
	bool packet_get_time();
	bool packet_get_signatures();
	bool packet_heartbeat();
}