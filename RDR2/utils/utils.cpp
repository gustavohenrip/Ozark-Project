#include "utils.hpp"
#include <algorithm>
#include <iomanip>

namespace utils {
	void* allocateMemory(int len) {
		void* mem = (void*)malloc(len);
		memset(mem, 0, len);
		return mem;
	}

	std::string createRandomString(size_t length) {
		auto randchar = [] () -> char {
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};

		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}

	std::vector<std::string> split(std::string s, std::string splitter) {
		size_t pos;
		std::vector<std::string> out;
		while ((pos = s.find(splitter)) != std::string::npos) {
			std::string token = s.substr(0, pos);
			out.push_back(token);
			s.erase(0, pos + splitter.length());
		}
		return out;
	}

	std::vector<BYTE> convertNumberToBytes(int paramInt) {
		std::vector<BYTE> arrayOfByte(4);
		for (int i = 0; i < 4; i++)
			arrayOfByte[3 - i] = (paramInt >> (i * 8));
		return arrayOfByte;
	}

	std::vector<char> convertStringToBytes(std::string data) {
		std::vector<char> bytes;

		for (std::size_t i = 0; i < data.length(); i += 2) {
			std::string byteString = data.substr(i, 2);
			char byte = (char)strtol(byteString.c_str(), NULL, 16);
			bytes.push_back(byte);
		}

		return bytes;
	}

	std::string convertBytesToString(BYTE* data, int len, bool space) {
		std::stringstream str;
		str.setf(std::ios_base::hex, std::ios::basefield);
		str.setf(std::ios_base::uppercase);
		str.fill('0');

		for (int i = 0; i < len; ++i) {
			str << std::setw(2) << (unsigned short)data[i];

			if (space && i != len - 1) {
				str << " ";
			}
		}

		return str.str();
	}

	std::string rot13(std::string input) {
		for (std::size_t len = input.length(), idx = 0; idx != len; ++idx) {
			if (input[idx] >= 'a' && input[idx] <= 'm') {
				input[idx] = input[idx] + 13;
			} else if (input[idx] >= 'n' && input[idx] <= 'z') {
				input[idx] = input[idx] - 13;
			} else if (input[idx] >= 'A' && input[idx] <= 'M') {
				input[idx] = input[idx] + 13;
			} else if (input[idx] >= 'N' && input[idx] <= 'Z') {
				input[idx] = input[idx] - 13;
			}
		}

		return input;
	}

	std::string rot13Number(std::string input) {
		for (std::size_t len = input.length(), idx = 0; idx != len; ++idx) {
			switch (input[idx]) {
				case '0': input[idx] = '3'; break;
				case '1': input[idx] = '4'; break;
				case '2': input[idx] = '5'; break;
				case '3': input[idx] = '6'; break;
				case '4': input[idx] = '7'; break;
				case '5': input[idx] = '8'; break;
				case '6': input[idx] = '9'; break;
				case '7': input[idx] = '0'; break;
				case '8': input[idx] = '1'; break;
				case '9': input[idx] = '2'; break;
			}
		}

		return input;
	}

	std::string rot13NumberReverse(std::string input) {
		for (std::size_t len = input.length(), idx = 0; idx != len; ++idx) {
			switch (input[idx]) {
				case '3': input[idx] = '0'; break;
				case '4': input[idx] = '1'; break;
				case '5': input[idx] = '2'; break;
				case '6': input[idx] = '3'; break;
				case '7': input[idx] = '4'; break;
				case '8': input[idx] = '5'; break;
				case '9': input[idx] = '6'; break;
				case '0': input[idx] = '7'; break;
				case '1': input[idx] = '8'; break;
				case '2': input[idx] = '9'; break;
			}
		}

		return input;
	}

	void replaceString(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			size_t end_pos = start_pos + from.length();
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // ...
			}
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	void createRandomBytes(BYTE* data, int len) {
		for (int i = 0; i < len; i++) {
			data[i] = (BYTE)(rand() % 256);
		}
	}


	DWORD findProcessId(std::string processName) {
#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof(processInfo);

		HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (processesSnapshot == INVALID_HANDLE_VALUE)
			return 0;

		Process32First(processesSnapshot, &processInfo);
		if (!processName.compare(processInfo.szExeFile)) {
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}

		while (Process32Next(processesSnapshot, &processInfo)) {
			if (!processName.compare(processInfo.szExeFile)) {
				CloseHandle(processesSnapshot);
				return processInfo.th32ProcessID;
			}
		}

		CloseHandle(processesSnapshot);
		return 0;
	}
}