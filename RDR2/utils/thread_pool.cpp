#include "thread_pool.hpp"
#include "global/variables.hpp"

namespace utils {
	void threadPool::addJob(std::function<void()> func) {
		struct data {
			std::function<void()> f;
		};

		data* d = new data();
		d->f = func;

		CreateThread(0, 0, [] (void* lpParam) -> DWORD {
			data* t = (data*)lpParam;
			t->f();
			delete t;
			return 0;
		}, d, 0, 0);
	}

	void threadPool::add(const char* name, void(*func)()) {
		thread* instance = new thread();
		instance->name = name;
		instance->running = true;
		instance->func = func;
		
		instance->handle = CreateThread(0, 0, [] (void* lpParam) -> DWORD {
			thread* t = (thread*)lpParam;

			while (t->running && !global::vars::g_unloading) {
				t->func();
			}

			t->running = false;
			auto vit = std::find(getThreadPool()->getThreads().begin(), getThreadPool()->getThreads().end(), t);
			if (vit != getThreadPool()->getThreads().end()) {
				getThreadPool()->getThreads().erase(vit);
			}

			delete t;
			return 0;
		}, instance, 0, 0);

		threads.push_back(instance);
	}

	void threadPool::remove(const char* name) {
		for (auto& obj : threads) {
			if (obj) {
				if (obj->name) {
					if (!strcmp(obj->name, name)) {
						obj->running = false;
						return;
					}
				}
			}
		}
	}

	threadPool* getThreadPool() {
		static threadPool instance;
		return &instance;
	}
}