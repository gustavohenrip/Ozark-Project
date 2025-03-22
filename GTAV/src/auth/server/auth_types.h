#pragma once
#include "stdafx.h"

namespace auth {
    enum eCheats : uint8_t {
        CHEAT_GRAND_THEFT_AUTO_V = 1,
        CHEAT_RED_DEAD_REDEMPTION_2,
        CHEAT_END
    };

    enum ePackets : uint8_t {
        PACKET_PING = 1,
        PACKET_HEARTBEAT,
        PACKET_GET_SIGNATURES,
        PACKET_GET_TIME,
        PACKET_WELCOME,
        PACKET_METRIC,
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

    namespace request {
#pragma pack(push)
#pragma pack(1)
        struct auth_base_signature {
            char m_name[10];
            int m_offset;
            bool m_important;
            uint8_t m_signature_length;
            uint8_t m_signature;
        };

        struct auth_encryption_keys {
            uint8_t xor_key; // Applied to whole buffer
            uint8_t m_token_key[0x10]; // m_AuthToken RC4
            uint8_t m_hardware_key[0x10]; // m_HardwareID RC4
            uint8_t m_request_key[0x10]; // Stays the same in req->resp
        };

        struct auth_cheat_header {
            uint8_t m_cheat;
            uint32_t m_cheat_version;
            bool m_tester;
        };

        struct auth_header {
            uint32_t m_magic;
            uint16_t m_size;
            uint8_t m_packet;
            auth_cheat_header m_cheat;
            uint8_t m_auth_token[0x20]; // Used in encryption method
            uint8_t m_hardware_id[0x40]; // SHA-512
            auth_encryption_keys m_encryption_keys;
        };

        struct auth_packet_ping : auth_header {};

        struct auth_packet_welcome : auth_header {
            char m_username[100];
            char m_pc_name[32]; // PC Username
            uint8_t m_password_hash[0x40]; // SHA-512
        };

        struct auth_packet_get_time : auth_header {};

        struct auth_packet_metric : auth_header {
            uint8_t m_metric_id;
            char m_metric_info[200];
        };

        struct auth_packet_get_signatures : auth_header {};

        struct auth_packet_heartbeat : auth_header {
            char m_game_username[32];
            char m_game_build[32];
            uint32_t m_rockstar_id;
        };

#pragma pack(pop)
    }

    namespace response {
#pragma pack(push)
#pragma pack(1)
        struct auth_encryption_keys {
            uint8_t xor_key; // Applied to whole buffer
            uint8_t m_final_key[0x10]; // RC4'd at the end
            uint8_t m_first_key[0x10];
            uint8_t m_second_key[0x10];
            uint32_t m_hash;
        };

        struct auth_packet_ping : auth_encryption_keys {
            uint8_t m_ping_test; // Should be 69
        };

        struct auth_packet_welcome : auth_encryption_keys {
            eResponseStatus m_response_status;
            uint8_t m_auth_token[0x20];
        };

        struct auth_packet_get_time : auth_encryption_keys {
            eResponseStatus m_response_status;
            uint32_t m_seconds_left;
        };

        struct auth_packet_get_signatures_length : auth_encryption_keys {
            eResponseStatus m_response_status;
            uint32_t m_length;
        };

        struct auth_packet_get_signatures : auth_encryption_keys {
            eResponseStatus m_response_status;
        };

        struct auth_packet_heartbeat : auth_encryption_keys {
            eResponseStatus m_response_status;
            uint32_t m_seconds_left;
            bool m_update_available;
        };

        struct auth_packet_metric : auth_encryption_keys {
            bool m_processed;
            bool m_banned;
        };
#pragma pack(pop)
    }
}