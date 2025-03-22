#pragma once
#include "rage/types/base_types.hpp"
#include "rage/engine.hpp"
#include "utils/singleton.hpp"

namespace rage::entity {
	class entityTask {
	public:
		uint64_t getNetObj(Entity entity);
		bool changeNetObjOwner(Entity entity, int index);
		int getNetObjOwner(Entity entity);
	}; entityTask* getEntityTask();
}