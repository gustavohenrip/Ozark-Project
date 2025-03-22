#include "timers.hpp"
#include "rage/invoker/natives.hpp"

namespace menu::util {
	void timers::runTimedFunction(int* timer, int ms, std::function<void()> callback) {
		if (*timer < native::getGameTimer()) {
			*timer = native::getGameTimer() + ms;
			callback();
		}
	}

	timers* getTimers() {
		static timers instance;
		return &instance;
	}
}