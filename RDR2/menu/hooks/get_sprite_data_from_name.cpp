#include "hooks.hpp"

uint64_t menu::hooks::getSpriteDataFromHashHook(uint32_t first, uint32_t second) {
    return originalGetSpriteDataFromHash(first, second);
}

uint64_t menu::hooks::getSpriteDataFromNameHook(const char* first, const char* second) {
    return originalGetSpriteDataFromName(first, second);
}