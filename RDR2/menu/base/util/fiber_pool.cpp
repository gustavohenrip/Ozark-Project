#include "fiber_pool.hpp"
#include "fiber_manager.hpp"

namespace menu::util {
	void fiberPool::init() {
		static std::function<void()> job1;
		static std::function<void()> job2;
		static std::function<void()> job3;
		static std::function<void()> job4;
		static std::function<void()> job5;
		static std::function<void()> job6;
		static std::function<void()> job7;
		static std::function<void()> job8;
		static std::function<void()> job9;
		static std::function<void()> job10;
		static std::function<void()> job11;
		static std::function<void()> job12;

		getFiberManager()->add("P1"_Protect, [] { getFiberPool()->update(job1); });
		getFiberManager()->add("P2"_Protect, [] { getFiberPool()->update(job2); });
		getFiberManager()->add("P3"_Protect, [] { getFiberPool()->update(job3); });
		getFiberManager()->add("P4"_Protect, [] { getFiberPool()->update(job4); });
		getFiberManager()->add("P5"_Protect, [] { getFiberPool()->update(job5); });
		getFiberManager()->add("P6"_Protect, [] { getFiberPool()->update(job6); });
		getFiberManager()->add("P7"_Protect, [] { getFiberPool()->update(job7); });
		getFiberManager()->add("P8"_Protect, [] { getFiberPool()->update(job8); });
		getFiberManager()->add("P9"_Protect, [] { getFiberPool()->update(job9); });
		getFiberManager()->add("P10"_Protect, [] { getFiberPool()->update(job10); });
		getFiberManager()->add("P11"_Protect, [] { getFiberPool()->update(job11); });
		getFiberManager()->add("P12"_Protect, [] { getFiberPool()->update(job12); });
	}

	void fiberPool::update(std::function<void()>& func) {
		if (jobs.empty()) return;
		func = std::move(jobs.front());
		jobs.pop();
		func();
	}

	void fiberPool::run(std::function<void()> func) {
		jobs.emplace(std::move(func));
	}

	fiberPool* getFiberPool() {
		static fiberPool instance;
		return &instance;
	}
}