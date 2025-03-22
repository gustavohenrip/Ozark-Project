#pragma once
#include "stdafx.hpp"
#include "../../Dependencies/cryptopp/include/cryptlib.h"
#include "../../Dependencies/cryptopp/include/hex.h"
#include "../../Dependencies/cryptopp/include/base64.h"
#include "../../Dependencies/cryptopp/include/filters.h"
#include "../../Dependencies/cryptopp/include/files.h"
#include "../../Dependencies/cryptopp/include/aes.h"
#include "../../Dependencies/cryptopp/include/osrng.h"
#include "../../Dependencies/cryptopp/include/modes.h"
#include "../../Dependencies/cryptopp/include/secblock.h"

using CryptoPP::SecByteBlock;
using CryptoPP::CBC_Mode;
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::Exception;
using CryptoPP::Base64Decoder;
using CryptoPP::Base64Encoder;

namespace Crypto {
	std::string Rotate(std::string input);
	std::string DecryptAES(std::string message, std::string iv = "", int length = 0);
	std::string EncryptAES(std::string message);
	std::string Base64Decode(std::string source);
	std::string Base64Encode(std::string source);
	std::string DecryptResponse(std::string message);
	void DecryptArrayAES(uint8_t* encBuffer, int encSize, uint8_t* outBuffer, int outSize, std::string fourKey);

#define XECRYPT_SHA512_DIGEST_SIZE 64
#define XECRYPT_SHA512_BLOCK_SIZE 128

	typedef struct _XECRYPT_SHA512_STATE {
		DWORD m_Count;
		uint64_t m_State[8];
		BYTE m_Buffer[128];
	} XECRYPT_SHA512_STATE, * PXECRYPT_SHA512_STATE;

	void Sha512Init(XECRYPT_SHA512_STATE* pShaState);
	void Sha512Update(XECRYPT_SHA512_STATE* pShaState, const BYTE* pbInp, DWORD cbInp);
	void Sha512Final(XECRYPT_SHA512_STATE* pShaState, BYTE* pbOut, DWORD cbOut);
}