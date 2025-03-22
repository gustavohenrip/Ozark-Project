#pragma once
#include "stdafx.h"
#include "rage/types/network_types.h"
#include "rage/types/ros_types.h"

namespace rage::ros {
	void send_stat_requests(uint32_t* targets, int count);
	void download_player_image(uint32_t rid);
}