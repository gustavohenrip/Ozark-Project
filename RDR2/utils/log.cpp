#include "log.hpp"
#include "config.hpp"
#include "va.hpp"
#include <ctime>

namespace logging {
	void logger::initialize() {
		AllocConsole();
		SetConsoleTitleA("Ozark"_Protect);

		freopen_s(&stream, "CONIN$", "w", stdin);
		freopen_s(&stream, "CONOUT$", "w", stdout);
		freopen_s(&stream, "CONOUT$", "w", stderr);

		outputHandle = GetStdHandle((DWORD)-11);

		const int width = 130;
		const int height = 40;

		SMALL_RECT rectangle;
		rectangle.Left = rectangle.Top = 0;
		rectangle.Right = width - 1;
		rectangle.Bottom = height - 1;
		SetConsoleWindowInfo(outputHandle, true, &rectangle);

		FILE* fp = fopen(utils::getConfig()->getLogPath(), "w"_Protect);
		if (fp) fclose(fp);
	}

	const char* logger::getTimeString() {
		time_t rawtime;
		time(&rawtime);

		char* str = asctime(localtime(&rawtime));
		std::size_t n = strlen(str) - 1;

		if (n < 0)
			n = 0;

		str[n] = '\0';

		return str;
	}

	void logger::setColor(int color) {
		CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
		GetConsoleScreenBufferInfo(outputHandle, &screenBuffer);

		WORD attributes = screenBuffer.wAttributes & ~FOREGROUND_RED & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
		attributes |= color;
		SetConsoleTextAttribute(outputHandle, attributes);
	}

	void logger::print(int color, const char* type, const char* fmt, ...) {
		char buffer[512];
		char buffer2[512];

		memset(buffer, 0, 512);
		memset(buffer2, 0, 512);

		va_list val;
		va_start(val, fmt);
		vsprintf_s(buffer, fmt, val);
		va_end(val);

		if (canPrint) {
			setColor(color | consoleBackground::BLACK);
			printf("%s | %s\n"_Protect, type, buffer);
			setColor(consoleForeground::WHITE);
		}

		printFile(utils::va::va("[%s] %s | %s\n"_Protect, getTimeString(), type, buffer));
	}

	void logger::printFile(std::string message) {
		std::ofstream file(utils::getConfig()->getLogPath(), std::ios_base::app);
		if (file.good()) {
			file << message;
			file.close();
		}
	}

	logger* getLog() {
		static logger instance;
		return &instance;
	}
}