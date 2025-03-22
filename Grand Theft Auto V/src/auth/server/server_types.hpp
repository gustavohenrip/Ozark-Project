#pragma once
#include "stdafx.hpp"

namespace Auth {
    enum eCheats : uint8_t {
        CHEAT_GRAND_THEFT_AUTO_V = 1,
        CHEAT_RED_DEAD_REDEMPTION_2,
        CHEAT_WORLD_WAR_Z,
        CHEAT_SEA_OF_THIEVES,
        CHEAT_BLACK_OPS_1,
        CHEAT_MODERN_WARFARE_3,
        CHEAT_BLACK_OPS_2,
        CHEAT_BLACK_OPS_3,
        CHEAT_INFINITE_WARFARE,
        CHEAT_MODERN_WARFARE_REMASTERED,
        CHEAT_WORLD_WAR_2,
        CHEAT_MODERN_WARFARE,
        CHEAT_COLD_WAR,
        CHEAT_BATTLEFIELD_3,
        CHEAT_BATTLEFIELD_4,
        CHEAT_BATTLEFIELD_V,
        CHEAT_END
    };

    enum ePackets : uint8_t {
        PACKET_PING = 1,
        PACKET_WELCOME,
        PACKET_GET_TIME,
        PACKET_METRIC,
        PACKET_GET_SIGNATURES,
        PACKET_HEARTBEAT,
        PACKET_END
    };

    enum eSocketErrorCodes : uint8_t {
        ERROR_CANT_READ_MAGIC,
        ERROR_INVALID_MAGIC,
        ERROR_CANT_READ_REQUIRED_HEADER,
        ERROR_INVALID_SIZE,
        ERROR_INVALID_PACKET,
        ERROR_INVALID_CHEAT,
        ERROR_CANT_READ_HEADER_DATA,
        ERROR_INVALID_PACKET_SIZE,
        ERROR_AUTH_TOKEN_NOT_FOUND,
        ERROR_ALLOCATING_MEMORY
    };

    enum eResponseStatus {
        RESPONSE_ERROR,
        RESPONSE_SUCCESS,

        // welcome
        RESPONSE_WELCOME_NO_ACCOUNT,
        RESPONSE_WELCOME_DISABLED_ACCOUNT,
        RESPONSE_WELCOME_BANNED_ACCOUNT,
        RESPONSE_WELCOME_OUTDATED,
        RESPONSE_WELCOME_PTB_DISABLED,
        RESPONSE_WELCOME_DISABLED,
        RESPONSE_WELCOME_HWID_MISMATCH,
    };

    namespace Request {
#pragma pack(push)
#pragma pack(1)
        struct BaseSignature {
            char m_Name[10];
            int m_Offset;
            bool m_Important;
            uint8_t m_SignatureLength;
            uint8_t m_Signature;
        };

        struct EncryptionKeys {
            uint8_t m_XorKey; // Applied to whole buffer
            uint8_t m_TokenKey[0x10]; // m_AuthToken RC4
            uint8_t m_HardwareKey[0x10]; // m_HardwareID RC4
            uint8_t m_RequestKey[0x10]; // Stays the same in req->resp
        };

        struct CheatHeader {
            uint8_t m_Cheat;
            uint32_t m_CheatVersion;
            bool m_Tester;
        };

        struct Header {
            uint32_t m_Magic; // 'DOSB'
            uint16_t m_Size;
            uint8_t m_Packet;
            CheatHeader m_Cheat;
            uint8_t m_AuthToken[0x20]; // Used in encryption method
            uint8_t m_HardwareID[0x40]; // SHA-512
            EncryptionKeys m_EncryptionKeys;
        };

        struct PacketPing : Header {};

        struct PacketWelcome : Header {
            char m_Username[100];
            char m_PCName[32]; // PC Username
            uint8_t m_PasswordHash[0x40]; // SHA-512
        };

        struct PacketWelcomeLauncher : Header {
            char m_Username[100];
            uint8_t m_PasswordHash[0x40]; // SHA-512
            bool m_SizeOnly;
        };

        struct PacketGetTime : Header {};

        struct PacketMetric : Header {
            uint8_t m_MetricID;
            char m_MetricInfo[200];
        };

        struct PacketGetSignatures : Header {
            bool m_SizeOnly;
        };

        struct PacketHeartbeat : Header {
            char m_GameUsername[32];
            char m_GameBuild[32];
            uint32_t m_RockstarID;
            uint16_t m_RDRTreasureSpawned; // RDR
        };

#pragma pack(pop)
    }

    namespace Response {
#pragma pack(push)
#pragma pack(1)
        struct EncryptionKeys {
            uint8_t m_XorKey; // Applied to whole buffer
            uint8_t m_FinalKey[0x10]; // RC4'd at the end
            uint8_t m_FirstKey[0x10];
            uint8_t m_SecondKey[0x10];
            uint32_t m_Hash;
        };

        struct PacketPing : EncryptionKeys {
            uint8_t m_PingTest; // Should be 69
        };

        struct PacketWelcome : EncryptionKeys {
            eResponseStatus m_ResponseStatus;
            uint8_t m_AuthToken[0x20];
        };

        struct PacketGetTime : EncryptionKeys {
            eResponseStatus m_ResponseStatus;
            uint32_t m_SecondsLeft;
        };

        struct PacketGetSignaturesLength : EncryptionKeys {
            eResponseStatus m_ResponseStatus;
            uint32_t m_Length;
        };

        struct PacketGetSignatures : EncryptionKeys {
            eResponseStatus m_ResponseStatus;
        };

        struct PacketHeartbeat : EncryptionKeys {
            eResponseStatus m_ResponseStatus;
            uint32_t m_SecondsLeft;
            bool m_UpdateAvailable;
        };

        struct PacketWelcomeLauncher : EncryptionKeys {
            eResponseStatus m_ResponseStatus;
            uint32_t m_Length; // Length to download
        };

        struct PacketMetric : EncryptionKeys {
            bool m_Processed;
            bool m_Banned;
        };
#pragma pack(pop)
    }
}