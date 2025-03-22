#pragma once
#include "stdafx.hpp"

namespace Auth::Crypto {
#define XECRYPT_SHA512_DIGEST_SIZE 64
#define XECRYPT_SHA512_BLOCK_SIZE 128

	typedef struct _XECRYPT_SHA512_STATE {
		uint32_t m_Count;
		uint64_t m_State[8];
		uint8_t m_Buffer[128];
	} XECRYPT_SHA512_STATE, * PXECRYPT_SHA512_STATE;

	void RC4(uint8_t* pbKey, uint32_t cbKey, uint8_t* pbInpOut, uint32_t cbInpOut, uint32_t startOffset = 0);
	void RC4Modified(uint8_t* pbKey, uint32_t cbKey, uint8_t* pbInpOut, uint32_t cbInpOut, uint32_t startOffset = 0);
	void Sha512Init(XECRYPT_SHA512_STATE* pShaState);
	void Sha512Update(XECRYPT_SHA512_STATE* pShaState, uint8_t* pbInp, uint32_t cbInp);
	void Sha512Final(XECRYPT_SHA512_STATE* pShaState, uint8_t* pbOut, uint32_t cbOut);

	std::string Base64Decode(std::string source);
	std::string DecryptAES(std::string message, std::string iv, int length);
	std::string Rotate(std::string input);
}