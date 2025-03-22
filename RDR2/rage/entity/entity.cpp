#include "entity.hpp"

namespace rage::entity {
	uint64_t entityTask::getNetObj(Entity entity) {
		auto entityAddress = engine::getEntityAddress(entity);
		if (entityAddress) {
			return *(uint64_t*)(entityAddress + 0xE0);
		}

		return 0;
	}

	bool entityTask::changeNetObjOwner(Entity entity, int index) {
		if (getNetObjOwner(entity) != index) {
			auto net = engine::getNetGamePlayerFromIndex(index);
			if (net) {
				auto netEntityAddress = getNetObj(entity);
				if (netEntityAddress) {
					return engine::changeNetObjOwner(netEntityAddress, net);
				}
			}

			return false;
		}

		return true;
	}

	int entityTask::getNetObjOwner(Entity entity) {
		auto netEntityAddress = getNetObj(entity);
		if (netEntityAddress) {
			return *(uint8_t*)(netEntityAddress + 0x45);
		}

		return -1;
	}

	entityTask* getEntityTask() {
		static entityTask instance;
		return &instance;
	}
}