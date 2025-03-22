#pragma once
#include "stdafx.hpp"
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

namespace wmi {
	bool initialize();
	std::string getData(std::string part, std::string property);
	void cleanup();
}