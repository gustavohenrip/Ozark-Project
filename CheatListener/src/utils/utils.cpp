#include "utils.hpp"

namespace Utils {
	uint32_t GetTickCount() {
		struct timeval tv;
		if (gettimeofday(&tv, 0) != 0) {
			return 0;
		}

		return ((uint32_t)tv.tv_sec * 1000) + ((uint32_t)tv.tv_usec / 1000);
	}

	void CreateThread(void* Thread, void* Param) {
		pthread_t ThreadHandle;
		pthread_create(&ThreadHandle, 0, (void*(*)(void*))Thread, Param);
		pthread_detach(ThreadHandle);
	}

	void GenerateRandomBytes(uint8_t* Buffer, uint32_t Length) {
		for (uint32_t i = 0; i < Length; i++) {
			Buffer[i] = (uint8_t)(rand() % 256);
		}
	}

	std::vector<uint8_t> ConvertIntegerToBytes(uint32_t Integer) {
		std::vector<uint8_t> ArrayOfByte(4);

		for (int i = 0; i < 4; i++) {
			ArrayOfByte[3 - i] = (uint8_t)(Integer >> (i * 8));
		}

		return ArrayOfByte;
	}

	std::string ConvertBytesToString(uint8_t* Buffer, uint32_t Length) {
		std::stringstream str;
		str.setf(std::ios_base::hex, std::ios::basefield);
		str.setf(std::ios_base::uppercase);
		str.fill('0');

		for (size_t i = 0; i < Length; ++i) {
			str << std::setw(2) << (unsigned short)Buffer[i];
		}

		return str.str();
	}

	std::string GetCurrentPath() {
#ifdef NEW_SERVER
		return "/home/ozark/new/";
#else
		return "/home/ozark/";
#endif

		char Result[256];
		auto Count = readlink("/proc/self/exe", Result, 256);
		auto String = std::string(Result, Count > 0 ? Count : 0);
		return String.substr(0, String.size() - 17);
	}

	std::vector<std::string> Split(std::string& s, char c) {
		std::vector<std::string> v;
		std::string::size_type i = 0;
		std::string::size_type j = s.find(c);

		while (j != std::string::npos) {
			v.push_back(s.substr(i, j - i));
			i = ++j;
			j = s.find(c, j);

			if (j == std::string::npos)
				v.push_back(s.substr(i, s.length()));
		}

		return v;
	}
}