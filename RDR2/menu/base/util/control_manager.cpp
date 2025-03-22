#include "control_manager.hpp"
#include "rage/invoker/natives.hpp"
#include "rage/entity/entity.hpp"
#include "player_manager.hpp"
#include "utils/memory/pattern.hpp"

namespace menu {
	void controlManager::update() {
		if (controlRequests.size()) {
			requestControlStruct& request = controlRequests.front();

			int oldOwner = 0;
			if (request.fineseOwner) {
				if (rage::entity::getEntityTask()->getNetObjOwner(request.entity) != getLocalPlayer().id) {
					oldOwner = rage::entity::getEntityTask()->getNetObjOwner(request.entity);
					if (native::networkIsInSession()) {
						uint64_t obj = rage::engine::getEntityAddress(request.entity);
						auto vit = std::find(forceOwnerTries.begin(), forceOwnerTries.end(), obj);
						if (vit == forceOwnerTries.end()) {
							forceOwnerTries.push_back(obj);
							rage::entity::getEntityTask()->changeNetObjOwner(request.entity, getLocalPlayer().id);
						}
					}
				}
			}

			for (int i = 0; i < 30; i++) {
				native::networkRequestControlOfEntity(request.entity);
				if (rage::entity::getEntityTask()->getNetObjOwner(request.entity) == getLocalPlayer().id || native::networkHasControlOfEntity(request.entity)) {
					if (request.function) request.function(request.entity);
					if (request.functionWithOwner) request.functionWithOwner(request.entity, oldOwner);
					break;
				}
			}

			controlRequests.pop();
		}

		if (modelRequests.size()) {
			requestModelStruct& request = modelRequests.front();
			if (native::hasModelLoaded(request.model) || request.tries > 30) {
				if (request.tries < 30) {
					request.function(request.model);
					native::setModelAsNoLongerNeeded(request.model);
				}

				modelRequests.pop();
			} else {
				request.tries++;
				native::requestModel(request.model);
			}
		}
	}

	void controlManager::requestControl(Entity entity, std::function<void(Entity)> callback, bool fineseOwner) {
		if (!native::networkIsInSession()) {
			callback(entity);
		} else {
			controlRequests.push({ entity, callback, nullptr, 0, fineseOwner });
		}
	}

	void controlManager::requestControl(Entity entity, std::function<void(Entity, int)> callback, bool fineseOwner) {
		controlRequests.push({ entity, nullptr, callback, 0, fineseOwner });
	}

	void controlManager::requestModel(DWORD model, std::function<void(DWORD)> callback) {
		modelRequests.push({ model, callback, 0 });
	}

	controlManager* getControlManager() {
		static controlManager instance;
		return &instance;
	}
}