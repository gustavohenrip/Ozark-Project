#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"
#include "rage/types/base_types.hpp"
#include <queue>

namespace menu {
	struct requestControlStruct {
		Entity entity;
		std::function<void(Entity)> function;
		std::function<void(Entity, int)> functionWithOwner;
		int tries;
		bool fineseOwner;
	};

	struct requestModelStruct {
		DWORD model;
		std::function<void(DWORD)> function;
		int tries;
	};

	class controlManager {
	public:
		void update();
		void requestControl(Entity entity, std::function<void(Entity)> callback, bool fineseOwner = false);
		void requestControl(Entity entity, std::function<void(Entity, int oldOwner)> callback, bool fineseOwner = false);
		void requestModel(DWORD model, std::function<void(DWORD)> callback);

		std::vector<uint64_t>& getForceOwnerTries() { return forceOwnerTries; }
	private:
		std::queue<requestControlStruct> controlRequests;
		std::queue<requestModelStruct> modelRequests;
		std::vector<uint64_t> forceOwnerTries;
	}; controlManager* getControlManager();
}