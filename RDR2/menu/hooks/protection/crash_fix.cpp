#include "../hooks.hpp"
#include "menu/base/submenus/main/world/weather.hpp"
#include "menu/base/util/fiber_pool.hpp"

void menu::hooks::weatherCrashHook(uint64_t rcx, uint32_t edx, uint32_t r8d, float xmm0) {
	if (edx >= 0xFFFF) {
		util::getFiberPool()->run([] {
			weatherMenuVars::setWeather(native::getHashKey("highpressure"), false);
		});

		return;
	}

	return originalWeatherCrash(rcx, edx, r8d, xmm0);
}

uint64_t menu::hooks::getPoolIndexHook(entityPool* decryptedPool, int index) {
	if (decryptedPool) {
		if (decryptedPool->unk2 != -1) {
			return originalGetPoolIndex(decryptedPool, index);
		}
	}

	return 0;
}

bool menu::hooks::gatlingAttachCrashHook(uint64_t rcx) {
	uint64_t entity = *(uint64_t*)(rcx + 0xA8);
	if (entity) {
		uint64_t entityDrawHandler = *(uint64_t*)(entity + 0x50);
		if (entityDrawHandler) {
			if ((*(uint32_t*)(entityDrawHandler + 0x5C) & 0xF) >= 2) {
				uint64_t info = *(uint64_t*)entityDrawHandler;
				if (info) {
					if (*(uint32_t*)(info + 0x9C) > 0xFF) {
						return false;
					}
				}
			}
		}
	}
	return originalGatlingAttachCrash(rcx);
}

void menu::hooks::gatlingDetachmentCrashHook(uint64_t trailer, uint16_t flag, uint64_t r8) {
	uint64_t entity = *(uint64_t*)(trailer + 0xA8);
	if (entity) {
		uint64_t entityDrawHandler = *(uint64_t*)(entity + 0x50);
		if (entityDrawHandler) {
			if ((*(uint32_t*)(entityDrawHandler + 0x5C) & 0xF) >= 2) {
				uint64_t info = *(uint64_t*)entityDrawHandler;
				if (info) {
					if (*(uint16_t*)(info + 0xC18) > 0) {
						if (!isValidPtr(*(uint64_t*)(info + 0xC10))) {
							*(uint16_t*)(info + 0xC18) = 0;
							return;
						}
					}
				}
			}
		}
	}
	return originalGatlingDetachmentCrash(trailer, flag, r8);
}

void menu::hooks::unbindAttachmentExtensionHook(uint64_t target, uint8_t dl, uint8_t r8b, uint64_t Override) {
	if (target) {
		uint64_t parentEntity = *(uint64_t*)(target);
		uint64_t childEntity = *(uint64_t*)(target + 0x18);
		if (parentEntity && childEntity) {
			if (!Override) {
				uint64_t parentAttachmentInfo = *(uint64_t*)(parentEntity + 0xA8);
				if (!parentAttachmentInfo) {
					return;
				}

				uint64_t parentExtension = *(uint64_t*)(parentAttachmentInfo + 0x50);
				if (!parentExtension) {
					return;
				}
			}

			return originalUnbindAttachmentExtension(target, dl, r8b, Override);
		}
		else if (!parentEntity && childEntity) {
			originalUnbindAttachmentExtension(target, dl, r8b, Override);
		}
	}
}