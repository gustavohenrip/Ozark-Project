#pragma once
#include "stdafx.hpp"
#include "utils/singleton.hpp"

namespace menu::util {
	class timers {
	public:
		void runTimedFunction(int* timer, int ms, std::function<void()> callback); // call this in loops and it'll only run this func within w/e
	}; timers* getTimers();
}