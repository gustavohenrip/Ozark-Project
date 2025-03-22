#pragma once
#include "stdafx.hpp"

namespace utils::va {
	const char* va(const char* fmt, ...);
	const wchar_t* vaw(const char* fmt, ...);
	const char* vaBuffer(char* buffer, std::size_t size, const char* fmt, ...);
}