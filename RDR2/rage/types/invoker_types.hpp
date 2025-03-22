#pragma once
#include "stdafx.hpp"

namespace rage::invoker {
	struct nativeRegistration {
		nativeRegistration* nextRegistration;
		uint64_t functions[7];
		uint32_t count;
		char _0x0044[0x4];
		uint64_t hashes[7];
	};
}