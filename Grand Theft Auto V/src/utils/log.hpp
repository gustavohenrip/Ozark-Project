#pragma once
#include "stdafx.hpp"

#define LOG_INFO(fmt, ...) Logger::GetLog()->Print(ConsoleForeground::DARKCYAN, "I", fmt, ##__VA_ARGS__);
#define LOG_WARN(fmt, ...) Logger::GetLog()->Print(ConsoleForeground::DARKYELLOW, "W", fmt, ##__VA_ARGS__);
#define LOG_ERROR(fmt, ...) Logger::GetLog()->Print(ConsoleForeground::RED, "E", fmt, ##__VA_ARGS__);
#define LOG_SUCCESS(fmt, ...) Logger::GetLog()->Print(ConsoleForeground::GREEN, "S", fmt, ##__VA_ARGS__);

#define LOG_CUSTOM_INFO(name, fmt, ...) Logger::GetLog()->Print(ConsoleForeground::DARKCYAN, name, fmt, ##__VA_ARGS__);
#define LOG_CUSTOM_WARN(name, fmt, ...) Logger::GetLog()->Print(ConsoleForeground::DARKYELLOW, name, fmt, ##__VA_ARGS__);
#define LOG_CUSTOM_ERROR(name, fmt, ...) Logger::GetLog()->Print(ConsoleForeground::RED, name, fmt, ##__VA_ARGS__);
#define LOG_CUSTOM_SUCCESS(name, fmt, ...) Logger::GetLog()->Print(ConsoleForeground::GREEN, name, fmt, ##__VA_ARGS__);
#define LOG_CUSTOM(colorr, name, fmt, ...) Logger::GetLog()->Print(colorr, name, fmt, ##__VA_ARGS__);

#ifdef DEV
#define LOG_DEV(fmt, ...) Logger::GetLog()->Print(ConsoleForeground::DARKMAGENTA, "DEV", fmt, ##__VA_ARGS__);
#define LOG_CUSTOM_DEV(name, fmt, ...) Logger::GetLog()->Print(ConsoleForeground::DARKMAGENTA, name, fmt, ##__VA_ARGS__);
#else
#define LOG_DEV(fmt, ...)
#define LOG_CUSTOM_DEV(name, fmt, ...)
#endif

namespace ConsoleForeground {
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

namespace ConsoleBackground {
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

namespace Logger {
	class Log {
	public:
		void Initialize();
		void SetColor(int Color);
		void Print(int Color, const char* Type, const char* Format, ...);
		void PrintFile(std::string Message);
		void Cleanup();
	private:
		FILE* m_Stream;
		HANDLE m_OutputHandle;
	};

	Log* GetLog();
}