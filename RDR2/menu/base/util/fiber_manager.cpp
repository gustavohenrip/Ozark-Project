#include "fiber_manager.hpp"
#include "utils/caller.hpp"
#include "global/variables.hpp"
#include "utils/log.hpp"

namespace menu::util {
	void fiberManager::init() {
		if (initialized) return;
		ConvertThreadToFiber(nullptr);
		mainFiberHandle = GetCurrentFiber();
		initialized = true;
	}

	void fiberManager::update() {
		if (!initialized || !currentFiber) return;

		for (std::size_t i = 0; i < fibers.size(); i++) {
			if (fibers.at(i).running) {
				if (timeGetTime() >= fibers.at(i).wakeTime) {
					SwitchToFiber(fibers.at(i).fiberHandle);
				}
			}
		}

		currentFiber = currentFiber->next;
	}

	void fiberManager::add(std::string name, void(*func)()) {
		uint64_t* args = new uint64_t[2];
		args[0] = (uint64_t)func;
		args[1] = (uint64_t)mainFiberHandle;

		auto handle = CreateFiber(0, [] (void* param) {
			uint64_t* params = (uint64_t*)param;
			try {
				while (!global::vars::g_unloading) {
					caller::call<int>(params[0]);
					SwitchToFiber((void*)params[1]);
				}
			} catch (...) {
				delete[] params;
			}
		}, args);

		fibers.push_back({ nullptr, handle, timeGetTime(), name, true });

		for (auto& f = fibers.begin(); f != fibers.end(); ++f) {
			auto& next = f + 1;
			if (next != fibers.end()) f->next = &*next;
			else f->next = &fibers.front();
		}

		currentFiber = &fibers.front();
	}

	void fiberManager::sleep(unsigned long time) {
		auto cond = find_if(fibers.begin(), fibers.end(), [] (fiber& f) -> bool { return f.fiberHandle == GetCurrentFiber(); });
		if (cond != fibers.end()) {
			cond->wakeTime = timeGetTime() + time;
			SwitchToFiber(mainFiberHandle);
		}
	}

	void fiberManager::unload() {
		for (fiber& f : fibers) {
			DeleteFiber(f.fiberHandle);
		}
	}

	void fiberManager::setRunning(std::string name, bool state) {
		for (auto& fiber : fibers) {
			if (!strcmp(name.c_str(), fiber.name.c_str())) {
				fiber.running = state;
				break;
			}
		}
	}

	fiberManager* getFiberManager() {
		static fiberManager instance;
		return &instance;
	}

	fiberManager* getScriptHookFiberManager() {
		static fiberManager instance;
		return &instance;
	}
}