#include "stdafx.hpp"
#include "global/vars.hpp"
#include "network/network.hpp"
#include "crypto/crypto.hpp"
#include "injection/injector.hpp"
#include <shlobj.h>
#include <shlwapi.h>
#include <direct.h>
#include <filesystem>

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

void Encode(std::string& data) {
	std::string buffer;
	buffer.reserve(data.size());

	for (size_t pos = 0; pos != data.size(); ++pos) {
		switch (data[pos]) {
			case '&':
				buffer.append(XOR("&amp;"));
				break;
			case '\"':
				buffer.append(XOR("&quot;"));
				break;
			case '\'':
				buffer.append(XOR("&apos;"));
				break;
			case '<':
				buffer.append(XOR("&lt;"));
				break;
			case '>':
				buffer.append(XOR("&gt;"));
				break;
			default:
				buffer.append(&data[pos], 1);
				break;
		}
	}

	data.swap(buffer);
}

void Decode(std::string& data) {
	ReplaceString(data, XOR("&amp;"), XOR("&"));
	ReplaceString(data, XOR("&quot;"), XOR("\""));
	ReplaceString(data, XOR("&apos;"), XOR("\'"));
	ReplaceString(data, XOR("&lt;"), XOR("<"));
	ReplaceString(data, XOR("&gt;"), XOR(">"));
}

extern "C" {
	__declspec(dllexport) const char* CCCCCCCC(const char* Response) {
		std::string Value = Crypto::DecryptResponse(Response);

		char* Allocation = (char*)malloc(Value.length() + 1);
		memset(Allocation, 0, Value.length() + 1);
		strcpy(Allocation, Value.c_str());
		return Allocation;
	}

	__declspec(dllexport) const char* AAAAAAAA(const char* String) {
		std::string Yes(String);
		Encode(Yes);

		std::string Value = Crypto::Base64Encode(Crypto::EncryptAES(Crypto::Rotate(Yes)));

		int Length = Value.length();

		char* Allocation = (char*)malloc(Length + 1);
		memset(Allocation, 0, Length + 1);
		strcpy(Allocation, Value.c_str());

		return Allocation;
	}

	__declspec(dllexport) const char* DDDDDDDD(const char* Password) {
		/*std::string Yes(Password);
		Encode(Yes);

		uint8_t PasswordBuffer[64];

		Crypto::XECRYPT_SHA512_STATE State;
		Crypto::Sha512Init(&State);
		Crypto::Sha512Update(&State, (uint8_t*)Yes.data(), Yes.length());
		Crypto::Sha512Final(&State, PasswordBuffer, sizeof(PasswordBuffer));

		std::stringstream Str;
		Str.setf(std::ios_base::hex, std::ios::basefield);
		Str.setf(std::ios_base::uppercase);
		Str.fill('0');

		for (int i = 0; i < sizeof(PasswordBuffer); ++i) {
			Str << std::setw(2) << (unsigned short)PasswordBuffer[i];
		}

		return AAAAAAAA(Str.str().c_str());*/
		return AAAAAAAA(Password);
	}

	__declspec(dllexport) const char* BBBBBBBB(const char* Username) {
		std::string Value = Crypto::Rotate(Crypto::DecryptAES(Crypto::Base64Decode(Username), XOR("ahfgcbfhghghtydh")));
		Decode(Value);

		char* Allocation = (char*)malloc(Value.length() + 1);
		memset(Allocation, 0, Value.length() + 1);
		strcpy(Allocation, Value.c_str());
		return Allocation;
	}

	__declspec(dllexport) void EEEEEEEE(void* Data) {
		return free(Data);
	}

	__declspec(dllexport) bool FFFFFFFF(const char* File, const char* Process) {
		DWORD ProcessID = Injector::GetProcessIdByName(Process);
		if (ProcessID) {
			std::ifstream Input(File, std::ios::binary);
			if (Input.good()) {
				Input.seekg(0, Input.end);
				int Length = Input.tellg();
				Input.seekg(0, Input.beg);

				char* EncryptedBuffer = new char[Length];
				memset(EncryptedBuffer, 0, Length);
				Input.read(EncryptedBuffer, Length);

				std::ofstream Output(std::string(File) + XOR(".cache"), std::ios::binary);
				if (Output.good()) {
					Output.write((const char*)&EncryptedBuffer[2], Length - 2);
					Output.close();

					delete[] EncryptedBuffer;
					return Injector::NativeInject(ProcessID, std::string(File) + XOR(".cache"));
				}

				delete[] EncryptedBuffer;
			}
		}

		return false;
	}
}