#pragma once
#include "stdafx.hpp"
#include "utils/structs.hpp"

namespace Network::Handler {
	void Handler(Structs::ConnectionInfo*);
	void ClientListener(void*);
	void HeartbeatListener(void*);
}