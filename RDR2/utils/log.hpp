#pragma once
#include "stdafx.hpp"

#define info(fmt, ...) logging::getLog()->print(consoleForeground::DARKCYAN, "I", fmt, ##__VA_ARGS__);
#define warn(fmt, ...) logging::getLog()->print(consoleForeground::DARKYELLOW, "W", fmt, ##__VA_ARGS__);
#define error(fmt, ...) logging::getLog()->print(consoleForeground::RED, "E", fmt, ##__VA_ARGS__);
#define success(fmt, ...) logging::getLog()->print(consoleForeground::GREEN, "S", fmt, ##__VA_ARGS__);
#define customInfo(name, fmt, ...) logging::getLog()->print(consoleForeground::DARKCYAN, name, fmt, ##__VA_ARGS__);
#define customWarn(name, fmt, ...) logging::getLog()->print(consoleForeground::DARKYELLOW, name, fmt, ##__VA_ARGS__);
#define customError(name, fmt, ...) logging::getLog()->print(consoleForeground::RED, name, fmt, ##__VA_ARGS__);
#define customSuccess(name, fmt, ...) logging::getLog()->print(consoleForeground::GREEN, name, fmt, ##__VA_ARGS__);
#define custom(colorr, name, fmt, ...) logging::getLog()->print(colorr, name, fmt, ##__VA_ARGS__);

#ifdef DEV
#define dev(fmt, ...) logging::getLog()->print(consoleForeground::DARKMAGENTA, "DEV", fmt, ##__VA_ARGS__);
#define customDev(name, fmt, ...) logging::getLog()->print(consoleForeground::DARKMAGENTA, name, fmt, ##__VA_ARGS__);
#else
#define dev(fmt, ...)
#define customDev(name, fmt, ...)
#endif

namespace consoleForeground {
	enum {
		BLACK = 0,
		DARKBLUE = FOREGROUND_BLUE,
		DARKGREEN = FOREGROUND_GREEN,
		DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DARKRED = FOREGROUND_RED,
		DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		GRAY = FOREGROUND_INTENSITY,
		BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};
}
namespace consoleBackground {
	enum {
		BLACK = 0,
		DARKBLUE = BACKGROUND_BLUE,
		DARKGREEN = BACKGROUND_GREEN,
		DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
		DARKRED = BACKGROUND_RED,
		DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
		DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
		DARKGRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		GRAY = BACKGROUND_INTENSITY,
		BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
		GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
		CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
		RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
		MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
		YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
		WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	};
}

namespace logging {
	class logger {
	public:
		void initialize();
		const char* getTimeString();
		void setColor(int color);
		void print(int color, const char* type, const char* fmt, ...);
		void printFile(std::string message);
		void setCanPrint(bool can) { canPrint = can; }
	private:
		bool canPrint = true;
		FILE* stream;
		HANDLE outputHandle;
	}; logger* getLog();
}