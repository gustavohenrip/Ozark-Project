#pragma once
#include "stdafx.hpp"

namespace WMI {
	bool Initialize();
	std::string Read(std::string Part, std::string Property);
	void Cleanup();
}