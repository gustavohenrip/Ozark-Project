#include "fiber_pool.hpp"
#include "fibers.hpp"

namespace Utils {
	void FiberPool::Initialize() {
		static std::function<void()> Job1;
		static std::function<void()> Job2;
		static std::function<void()> Job3;
		static std::function<void()> Job4;
		static std::function<void()> Job5;
		static std::function<void()> Job6;
		static std::function<void()> Job7;
		static std::function<void()> Job8;

		GetFiberManager()->Add("P1"_Protect, [] { GetFiberPool()->Update(Job1); });
		GetFiberManager()->Add("P2"_Protect, [] { GetFiberPool()->Update(Job2); });
		GetFiberManager()->Add("P3"_Protect, [] { GetFiberPool()->Update(Job3); });
		GetFiberManager()->Add("P4"_Protect, [] { GetFiberPool()->Update(Job4); });
		GetFiberManager()->Add("P5"_Protect, [] { GetFiberPool()->Update(Job5); });
		GetFiberManager()->Add("P6"_Protect, [] { GetFiberPool()->Update(Job6); });
		GetFiberManager()->Add("P7"_Protect, [] { GetFiberPool()->Update(Job7); });
		GetFiberManager()->Add("P8"_Protect, [] { GetFiberPool()->Update(Job8); });
	}

	void FiberPool::Update(std::function<void()>& Func) {
		if (m_Jobs.empty()) return;
		Func = m_Jobs.front();
		m_Jobs.pop();
		Func();
	}

	void FiberPool::Push(std::function<void()> Func) {
		if (m_Jobs.size() >= 200) {
			return;
		}

		m_Jobs.emplace(Func);
	}

	FiberPool* GetFiberPool() {
		static FiberPool Instance;
		return &Instance;
	}
}