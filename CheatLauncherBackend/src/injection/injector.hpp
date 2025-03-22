#pragma once
#include "stdafx.hpp"

namespace Injector {
	bool ManualMap(DWORD ProcessID, uint8_t* Buffer, int Size);
	bool NativeInject(DWORD ProcessID, std::string path);
	DWORD GetProcessIdByName(std::string name);
}