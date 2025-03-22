#include "misc_model_swap.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include "swaps/misc_model_swap_add.hpp"
#include "utils/memory/memory.hpp"
#include "menu/base/util/control_manager.hpp"

using namespace MiscModelSwapMenuVars;

namespace MiscModelSwapMenuVars {
	Vars_ m_Vars;

	struct HashNode {
		uint32_t m_ModelHash;
		uint16_t m_Data;
		uint16_t m_Padding;
		HashNode* m_Next;
	};

	HashNode* FindModelInMap(uint32_t Model) {
		uint16_t HashTableCount = *(uint16_t*)Memory::GetAddressFromInstruction(Global::Vars::g_GetModelInfo);
		uint64_t HashTable = *(uint64_t*)Memory::GetAddressFromInstruction(Global::Vars::g_GetModelInfo + 0x7E);
		HashNode** HashMap = *(HashNode***)Memory::GetAddressFromInstruction(Global::Vars::g_GetModelInfo + 0x21);

		for (uint16_t i = 0; i < HashTableCount; i++) {
			for (HashNode* Current = HashMap[i]; Current; Current = Current->m_Next) {
				if (Current->m_ModelHash == Model) {
					return Current;
				}
			}
		}

		return nullptr;
	}

	void SwapExisting(ModelSwap& Instance) {
		if (Instance.m_Toggled) {
			HashNode* Node = FindModelInMap(Instance.m_OriginalModel);
			if (Node) {
				HashNode* NewNode = FindModelInMap(Instance.m_NewModel);
				if (NewNode) {
					Node->m_Data = NewNode->m_Data;
				}
			}
		} else {
			HashNode* Node = FindModelInMap(Instance.m_OriginalModel);
			if (Node) {
				Node->m_Data = Instance.m_OriginalOffset;
			}
		}
	}

	void AddSwap(const char* Original, const char* New) {
		if (!strcmp(Original, New)) {
			return;
		}

		for (auto& Swap : m_Vars.m_Swaps) {
			if (!strcmp(Swap.m_OriginalName, Original)) {
				// Already exists, remove existing swap and re-swap.
				RemoveSwap(Swap.m_OriginalModel);
				break;
			}
		}

		uint32_t OriginalModel = 0;
		uint32_t NewModel = 0;

		if (Original[0] == '0' && (Original[1] == 'x' || Original[1] == 'X')) {
			OriginalModel = (uint32_t)_strtoui64(Original, 0, 0);
		} else {
			std::string StringName(Original);
			if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
				OriginalModel = (uint32_t)atoi(Original);
			} else {
				OriginalModel = Native::GetHashKey(Original);
			}
		}

		if (New[0] == '0' && (New[1] == 'x' || New[1] == 'X')) {
			NewModel = (uint32_t)_strtoui64(New, 0, 0);
		} else {
			std::string StringName(New);
			if (std::find_if(StringName.begin(), StringName.end(), [] (unsigned char c) { return !std::isdigit(c); }) == StringName.end()) {
				NewModel = (uint32_t)atoi(New);
			} else {
				NewModel = Native::GetHashKey(New);
			}
		}

		Menu::GetControlManager()->SimpleRequestModel(OriginalModel);
		Menu::GetControlManager()->SimpleRequestModel(NewModel);

		if (!Native::IsModelValid(OriginalModel) || !Native::IsModelValid(NewModel)
			|| !Native::IsModelInCdimage(OriginalModel) || !Native::IsModelInCdimage(NewModel)) {
			return;
		}

		HashNode* Node = FindModelInMap(OriginalModel);
		if (Node) {
			HashNode* NewNode = FindModelInMap(NewModel);
			if (NewNode) {
				ModelSwap Swap;
				Swap.m_OriginalModel = OriginalModel;
				Swap.m_NewModel = NewModel;
				Swap.m_OriginalName = Original;
				Swap.m_NewName = New;
				Swap.m_OriginalOffset = Node->m_Data;

				m_Vars.m_Swaps.push_back(Swap);

				Node->m_Data = NewNode->m_Data;
			}
		}
	}

	void RemoveSwap(uint32_t OriginalHash) {
		auto Vit = std::find_if(m_Vars.m_Swaps.begin(), m_Vars.m_Swaps.end(), [=] (ModelSwap& Element) { return Element.m_OriginalModel == OriginalHash; });
		if (Vit != m_Vars.m_Swaps.end()) {
			HashNode* Node = FindModelInMap(OriginalHash);
			if (Node) {
				Node->m_Data = Vit->m_OriginalOffset;
			}

			m_Vars.m_Swaps.erase(Vit);
		}
	}
}

void MiscModelSwapMenu::Init() {
	SetName("Model Swap");
	SetParentSubmenu<MiscMenu>();

	addOption(SubmenuOption("+ Add Swap")
		.addTranslation().addHotkey()
		.addSubmenu<MiscModelSwapAddMenu>());

	addOption(BreakOption("Swaps")
		.addTranslation());

	addString("~m~None");
}

void MiscModelSwapMenu::Update() {}

/*Called once on submenu open*/
void MiscModelSwapMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	for (auto& Swap : m_Vars.m_Swaps) {
		addOption(ToggleOption(Swap.m_OriginalName)
			.addToggle(Swap.m_Toggled)
			.addOnClick([&] { SwapExisting(Swap); }));
	}

	if (m_Vars.m_Swaps.size() == 0) {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void MiscModelSwapMenu::FeatureUpdate() {}

MiscModelSwapMenu* _instance;
MiscModelSwapMenu* MiscModelSwapMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscModelSwapMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscModelSwapMenu::~MiscModelSwapMenu() { delete _instance; }