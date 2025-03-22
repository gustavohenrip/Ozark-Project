#pragma once
#include "rage/invoker/natives.hpp"
#include "utils/singleton.hpp"

namespace menu {
	class notify {
	public:
		void bottom(std::string text);
		void right(std::string text);
	}; notify* getNotify();
}