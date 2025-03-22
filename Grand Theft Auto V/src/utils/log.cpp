#include "log.hpp"
#include "config.hpp"
#include "va.hpp"

namespace Logger {
	std::unordered_map<uint32_t, char*> g_LogStorage;

	void Log::Initialize() {
		AllocConsole();
		SetConsoleTitleA("Ozark"_Protect);

		freopen_s(&m_Stream, "CONIN$", "w", stdin);
		freopen_s(&m_Stream, "CONOUT$", "w", stdout);
		freopen_s(&m_Stream, "CONOUT$", "w", stderr);

		m_OutputHandle = GetStdHandle((uint32_t)-11);

		constexpr int Width = 130;
		constexpr int Height = 40;

		SMALL_RECT Rectangle;
		Rectangle.Left = Rectangle.Top = 0;
		Rectangle.Right = Width - 1;
		Rectangle.Bottom = Height - 1;
		SetConsoleWindowInfo(m_OutputHandle, true, &Rectangle);

		FILE* FilePtr = fopen(Utils::GetConfig()->GetLogPath().c_str(), "w"_Protect);
		if (FilePtr) fclose(FilePtr);
	}

	void Log::Cleanup() {
		for (auto Ptr : g_LogStorage) {
			delete[] Ptr.second;
		}
	}

	void Log::SetColor(int Color) {
		CONSOLE_SCREEN_BUFFER_INFO screenBuffer;
		GetConsoleScreenBufferInfo(m_OutputHandle, &screenBuffer);

		WORD attributes = screenBuffer.wAttributes & ~FOREGROUND_RED & ~FOREGROUND_GREEN & ~FOREGROUND_BLUE & ~FOREGROUND_INTENSITY;
		attributes |= Color;
		SetConsoleTextAttribute(m_OutputHandle, attributes);
	}

	void Log::Print(int Color, const char* Type, const char* Format, ...) {
		if (g_LogStorage.find(GetCurrentThreadId()) == end(g_LogStorage)) {
			g_LogStorage[GetCurrentThreadId()] = new char[0x1000];
			printf("Creating VA buffer for thread %i\n", GetCurrentThreadId());
		}

		char* Buffer = g_LogStorage[GetCurrentThreadId()];
		memset(Buffer, 0, 0x1000);

		va_list val;
		va_start(val, Format);
		vsprintf(Buffer, Format, val);
		va_end(val);

		SetColor(Color | ConsoleBackground::BLACK);
		printf("%s | %s\n"_Protect, Type, Buffer);
		SetColor(ConsoleForeground::WHITE);

		time_t RawTime;
		time(&RawTime);
		tm* Time = localtime(&RawTime);

		char TimeString[100];
		strftime(TimeString, sizeof(TimeString), "%a %b %d %T %Y", Time);

		char PrintBuffer[0x1000];
		snprintf(PrintBuffer, 0x1000, "[%s] %s | %s\n"_Protect, TimeString, Type, Buffer);
		PrintFile(PrintBuffer);
	}

	void Log::PrintFile(std::string Message) {
		std::ofstream file(Utils::GetConfig()->GetLogPath(), std::ios_base::app);
		if (file.good()) {
			file << Message;
			file.close();
		}
	}

	Log* GetLog() {
		static Log Instance;
		return &Instance;
	}
}