#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"
#include <queue>

namespace menu::util {
	class fiberPool {
	public:
		void init();
		void update(std::function<void()>& func);
		void run(std::function<void()> func);
	private:
		std::queue<std::function<void()>> jobs;
	}; fiberPool* getFiberPool();
}