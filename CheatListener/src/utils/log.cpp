#include "log.hpp"

namespace Log {
	namespace Color {
		enum eColorCode {
			FG_BLACK = 30,
			FG_RED = 31,
			FG_GREEN = 32,
			FG_YELLOW = 33,
			FG_BLUE = 34,
			FG_MAGENTA = 35,
			FG_CYAN = 36,
			FG_LIGHTGRAY = 37,
			FG_DEFAULT = 39,
			FG_DARKGRAY = 90,
			FG_LIGHTRED = 91,
			FG_LIGHTGREEN = 92,
			FG_LIGHTYELLOW = 93,
			FG_LIGHTBLUE = 94,
			FG_LIGHTMAGENTA = 95,
			FG_LIGHTCYAN = 96,
			FG_WHITE = 97
		};

		inline std::ostream& operator<<(std::ostream& os, eColorCode code) {
			return os << "\033[" << static_cast<int>(code) << "m";
		}
	}

	std::string GetTimeAsString() {
		tm TimeStruct;
		time_t CurrentTime = std::time(nullptr);
		localtime_r(&CurrentTime, &TimeStruct);

		static char Buffer[100];
		sprintf(Buffer, "[%02d-%02d-%02d %02d:%02d:%02d]", TimeStruct.tm_mday, TimeStruct.tm_mon + 1, TimeStruct.tm_year + 1900, TimeStruct.tm_hour, TimeStruct.tm_min, TimeStruct.tm_sec);
		return Buffer;
	}

	std::string GetIPPadding(std::string IP) {
		std::string Str = "";

		int Length = 15 - (int)IP.length();
		for (int i = 0; i < Length; i++) {
			Str += " ";
		}

		return Str;
	}

	void GenerateDefaultResponse(std::string Type, Color::eColorCode Color, Network::Socket Socket, std::string PacketName, std::string Values) {
		std::cout << Color::FG_LIGHTGRAY << GetTimeAsString() << " " << Color::FG_DEFAULT
			<< "[" << Color::FG_CYAN << Socket.GetIP() << Color::FG_DEFAULT << "] " << Color
			<< GetIPPadding(Socket.GetIP()) << Type << Color::FG_DEFAULT << " [" << PacketName << "]";

		if (Values != "") {
			std::cout << " " << Values;
		}

		std::cout << std::endl;
	}

	void Misc(std::string Values) {
		std::cout << Color::FG_LIGHTGRAY << GetTimeAsString() << " " << Color::FG_DEFAULT << Values << std::endl;
	}

	void Misc(std::string Values, Network::Socket Client) {
		std::cout << Color::FG_LIGHTGRAY << GetTimeAsString() << " " << Color::FG_DEFAULT 
			<< "[" << Color::FG_CYAN << Client.GetIP() << Color::FG_DEFAULT << "] "
			<< GetIPPadding(Client.GetIP()) << Values << std::endl;
	}

	void Connection(Network::Socket Client, std::string PacketName, std::string Values) {
		GenerateDefaultResponse("Connection", Color::FG_BLUE, Client, PacketName, Values);
	}

	void Success(Network::Socket Client, std::string PacketName, std::string Values) {
		GenerateDefaultResponse("Success   ", Color::FG_GREEN, Client, PacketName, Values);
	}

	void Error(Network::Socket Client, std::string PacketName, std::string Values) {
		GenerateDefaultResponse("Error     ", Color::FG_RED, Client, PacketName, Values);
	}

	void Warn(Network::Socket Client, std::string PacketName, std::string Values) {
		GenerateDefaultResponse("Warn      ", Color::FG_YELLOW, Client, PacketName, Values);
	}

	void Info(Network::Socket Client, std::string PacketName, std::string Values) {
		GenerateDefaultResponse("Info      ", Color::FG_MAGENTA, Client, PacketName, Values);
	}
}