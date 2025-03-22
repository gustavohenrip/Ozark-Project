#pragma once
#include "stdafx.hpp"
#include "rage/types/base_types.hpp"

namespace Rage::Entity {
	uint64_t GetNetworkObject(::Entity entity);
	int GetNetworkObjectOwner(::Entity entity);
	bool UpdateNetworkObjectOwner(::Entity entity, int NewOwner);
}