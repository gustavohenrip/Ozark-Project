#include "hooks.hpp"
#include "utils/log.hpp"

void Menu::Hooks::UnbindAttachmentExtensionHook(uint64_t Target, uint8_t DL, uint8_t R8b, uint64_t Override) {
	/*if (Target) {
		uint64_t ParentEntity = *(uint64_t*)(Target);
		uint64_t ChildEntity = *(uint64_t*)(Target + 0x18);

		if (IsValidPtr(ParentEntity) && IsValidPtr(ChildEntity)) {
			if (!Override) {
				uint64_t ParentAttachmentInfo = *(uint64_t*)(ParentEntity + 0x50);
				if (!IsValidPtr(ParentAttachmentInfo)) {
					LOG_DEV("Invalid parent attachment info");
					return;
				}

				uint64_t ParentExtension = *(uint64_t*)(ParentAttachmentInfo + 0x48);
				if (!IsValidPtr(ParentExtension)) {
					LOG_DEV("Invalid parent extension");
					return;
				}
			}

			return OriginalUnbindAttachmentExtension(Target, DL, R8b, Override);
		} else if (!IsValidPtr(ParentEntity) && IsValidPtr(ChildEntity)) {
			OriginalUnbindAttachmentExtension(Target, DL, R8b, Override);
		}
	}*/

	if (!IsValidPtr(Target)) return;

	uint64_t Entity = *(uint64_t*)Target;
	if (!Override) {
		Entity = *(uint64_t*)(Entity + 0x50);
		if (IsValidPtr(Entity)) {
			Entity = *(uint64_t*)(Entity + 0x48);
		}
	}

	if (!IsValidPtr(Entity)) return;
	return OriginalUnbindAttachmentExtension(Target, DL, R8b, Override);
}