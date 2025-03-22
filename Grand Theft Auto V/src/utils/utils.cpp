#include "utils.hpp"
#include <algorithm>
#include <iomanip>

namespace Utils {
	std::string CreateRandomString(size_t Length) {
		auto randchar = []() -> char {
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};

		std::string str(Length, 0);
		std::generate_n(str.begin(), Length, randchar);
		return str;
	}

	std::vector<std::string> Split(std::string Str, std::string Splitter) {
		size_t pos;
		std::vector<std::string> out;
		while ((pos = Str.find(Splitter)) != std::string::npos) {
			std::string token = Str.substr(0, pos);
			out.push_back(token);
			Str.erase(0, pos + Splitter.length());
		}
		return out;
	}

	std::vector<uint8_t> ConvertNumberToBytes(int ParamInt) {
		std::vector<BYTE> arrayOfByte(4);
		for (int i = 0; i < 4; i++)
			arrayOfByte[3 - i] = (ParamInt >> (i * 8));
		return arrayOfByte;
	}

	std::vector<char> ConvertStringToBytes(std::string Data) {
		std::vector<char> bytes;

		for (std::size_t i = 0; i < Data.length(); i += 2) {
			std::string byteString = Data.substr(i, 2);
			char byte = (char)strtol(byteString.c_str(), NULL, 16);
			bytes.push_back(byte);
		}

		return bytes;
	}

	std::string ConvertBytesToString(uint8_t* Data, int Length, bool Space) {
		std::stringstream str;
		str.setf(std::ios_base::hex, std::ios::basefield);
		str.setf(std::ios_base::uppercase);
		str.fill('0');

		for (int i = 0; i < Length; ++i) {
			str << std::setw(2) << (unsigned short)Data[i];

			if (Space && i != Length - 1) {
				str << " ";
			}
		}

		return str.str();
	}

	std::string Rot13(std::string Input) {
		for (std::size_t len = Input.length(), idx = 0; idx != len; ++idx) {
			if (Input[idx] >= 'a' && Input[idx] <= 'm') {
				Input[idx] = Input[idx] + 13;
			} else if (Input[idx] >= 'n' && Input[idx] <= 'z') {
				Input[idx] = Input[idx] - 13;
			} else if (Input[idx] >= 'A' && Input[idx] <= 'M') {
				Input[idx] = Input[idx] + 13;
			} else if (Input[idx] >= 'N' && Input[idx] <= 'Z') {
				Input[idx] = Input[idx] - 13;
			}
		}

		return Input;
	}

	std::string Rot13Number(std::string Input) {
		for (std::size_t len = Input.length(), idx = 0; idx != len; ++idx) {
			switch (Input[idx]) {
				case '0': Input[idx] = '3'; break;
				case '1': Input[idx] = '4'; break;
				case '2': Input[idx] = '5'; break;
				case '3': Input[idx] = '6'; break;
				case '4': Input[idx] = '7'; break;
				case '5': Input[idx] = '8'; break;
				case '6': Input[idx] = '9'; break;
				case '7': Input[idx] = '0'; break;
				case '8': Input[idx] = '1'; break;
				case '9': Input[idx] = '2'; break;
			}
		}

		return Input;
	}

	std::string Rot13NumberReverse(std::string Input) {
		for (std::size_t len = Input.length(), idx = 0; idx != len; ++idx) {
			switch (Input[idx]) {
				case '3': Input[idx] = '0'; break;
				case '4': Input[idx] = '1'; break;
				case '5': Input[idx] = '2'; break;
				case '6': Input[idx] = '3'; break;
				case '7': Input[idx] = '4'; break;
				case '8': Input[idx] = '5'; break;
				case '9': Input[idx] = '6'; break;
				case '0': Input[idx] = '7'; break;
				case '1': Input[idx] = '8'; break;
				case '2': Input[idx] = '9'; break;
			}
		}

		return Input;
	}

	void ReplaceString(std::string& Str, const std::string& From, const std::string& To) {
		size_t start_pos = 0;
		while ((start_pos = Str.find(From, start_pos)) != std::string::npos) {
			size_t end_pos = start_pos + From.length();
			while ((start_pos = Str.find(From, start_pos)) != std::string::npos) {
				Str.replace(start_pos, From.length(), To);
				start_pos += To.length();
			}
			start_pos += To.length();
		}
	}

	void CreateRandomBytes(uint8_t* Data, int Length) {
		for (int i = 0; i < Length; i++) {
			Data[i] = (uint8_t)(rand() % 256);
		}
	}

	std::string GetStackTrace() {
		std::string s("[");

		typedef USHORT(WINAPI* CaptureStackBackTraceType)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
		CaptureStackBackTraceType func = (CaptureStackBackTraceType)(GetProcAddress(LoadLibraryA("kernel32.dll"), "RtlCaptureStackBackTrace"));
		
		if (func != NULL) {
			HMODULE hMods[1024];
			DWORD cbNeeded;
			EnumProcessModules((HANDLE)-1, hMods, sizeof(hMods), &cbNeeded);
			const int kMaxCallers = 62;
			void* callers[kMaxCallers];
			int count = (func)(0, kMaxCallers, callers, NULL);
			for (int i = 1; i < count; i++) {
				DWORD64 callerAddr = (DWORD64)callers[i];
				HMODULE currentCallerModule = NULL;
				
				for (unsigned int i2 = 0; i2 < (cbNeeded / sizeof(HMODULE)); i2++) {
					MODULEINFO information = { 0 };
					GetModuleInformation((HANDLE)-1, hMods[i2], &information, sizeof(information));
					if (callerAddr >= (DWORD64)information.lpBaseOfDll && callerAddr <= (DWORD64)information.lpBaseOfDll + information.SizeOfImage) {
						currentCallerModule = hMods[i2];
						break;
					}
				}

				if (currentCallerModule != NULL) {
					char moduleName[MAX_PATH];
					GetModuleBaseNameA((HANDLE)-1, currentCallerModule, moduleName, MAX_PATH);
					char buf[512];
					sprintf_s(buf, "0x%llx [%s]%s", callerAddr - (DWORD64)currentCallerModule, moduleName, i != count - 1 ? ", " : "");
					s += buf;
				} else {
					char buf[512];
					sprintf_s(buf, "0x%llx %s", callerAddr, i != count - 1 ? ", " : "");
					s += buf;
				}
			}

			s += "]";
			return s;
		}

		return "Unk";
	}
}