#pragma once
#include "stdafx.h"

namespace auth::crypto {
#define XECRYPT_SHA512_DIGEST_SIZE 64
#define XECRYPT_SHA512_BLOCK_SIZE 128

	typedef struct _XECRYPT_SHA512_STATE {
		uint32_t m_count;
		uint64_t m_state[8];
		uint8_t m_buffer[128];
	} XECRYPT_SHA512_STATE, * PXECRYPT_SHA512_STATE;

	void rc4(uint8_t* pbKey, uint32_t cbKey, uint8_t* pbInpOut, uint32_t cbInpOut, uint32_t startOffset = 0);
	void rc4_modified(uint8_t* pbKey, uint32_t cbKey, uint8_t* pbInpOut, uint32_t cbInpOut, uint32_t startOffset = 0);
	void sha_init(XECRYPT_SHA512_STATE* pShaState);
	void sha_update(XECRYPT_SHA512_STATE* pShaState, uint8_t* pbInp, uint32_t cbInp);
	void sha_final(XECRYPT_SHA512_STATE* pShaState, uint8_t* pbOut, uint32_t cbOut);

	std::string base64_decode(std::string source);
	std::string aes_decrypt(std::string message, std::string iv, int length);
	std::string rotate(std::string input);
	std::string rotate_number(std::string input);
	std::string rotate_number_reverse(std::string input);
	std::string base64_bytes(uint8_t* payload, uint32_t size);
}