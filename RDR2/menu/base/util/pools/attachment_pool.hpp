#pragma once
#include "rage/engine.hpp"
#include "utils/math.hpp"
#include "utils/singleton.hpp"

namespace menu::pools {
	class attachmentPool {
	public:
		void update();
		void run(std::function<void(Entity)> callback);
		entityPool* getPoolPointer();
		std::vector<Entity>& getStorage() { return storage; }
	private:
		std::vector<Entity> storage;
	}; attachmentPool* getAttachmentPool();
}