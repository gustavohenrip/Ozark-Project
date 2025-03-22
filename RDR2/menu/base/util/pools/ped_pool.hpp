#pragma once
#include "rage/engine.hpp"
#include "utils/math.hpp"
#include "utils/singleton.hpp"

namespace menu::pools {
	class pedPool {
	public:
		void update();
		void run(std::function<void(Ped)> callback);
		entityPool* getPoolPointer();
		std::vector<Ped>& getStorage() { return storage; }
	private:
		std::vector<Ped> storage;
	}; pedPool* getPedPool();
}