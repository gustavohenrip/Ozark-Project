#include "misc_texture_swap.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../misc.hpp"
#include <filesystem>
#include "utils/json.hpp"
#include "utils/fibers.hpp"
#include "utils/fiber_pool.hpp"
#include "menu/base/util/notify.hpp"
#include "menu/base/util/control_manager.hpp"
#include "rage/engine.hpp"
#include "utils/memory/memory.hpp"
#include "utils/log.hpp"

using namespace MiscTextureSwapMenuVars;

namespace MiscTextureSwapMenuVars {
	Vars_ m_Vars;

	Rage::Types::DictionaryItem* FindItem(std::string Dict, std::string Name) {
		uint32_t Hash = Native::GetHashKey(Dict.c_str());
		uint32_t TextureHash = Native::GetHashKey(("platform:/textures/" + Dict).c_str());

		Rage::Types::TxdStore* Txd = Global::Vars::g_TextureStore;
		if (IsValidPtr(Txd) && IsValidPtr(Txd->m_Pool) && IsValidPtr(Txd->m_Mask)) {
			Rage::Types::DictionaryPool* Pool = Txd->m_Pool;
			for (uint32_t i = 0; i < Txd->m_Count; i++) {
				if (~(Txd->m_Mask[i] >> 7) & 1) {
					Rage::Types::DictionaryPool Element = Pool[i];
					if (IsValidPtr(Element.m_Pool)) {
						if (Element.m_DictionaryHash == Hash || Element.m_DictionaryHash == TextureHash) {
							for (short j = 0; j < Element.m_Pool->m_Count; j++) {
								Rage::Types::DictionaryItem* Item = Element.m_Pool->m_Items[j];
								if (IsValidPtr(Item)) {
									if (IsValidPtr(Item->m_Name)) {
										if (!strcmp(Item->m_Name, Name.c_str())) {
											return Item;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return nullptr;
	}

	void Handle(MiscTextureSwapMenu* This, TextureSwap& Swap) {
		for (auto& Element : Swap.m_Contexts) {
			std::string FilePath = std::string(Utils::GetConfig()->GetTextureSwapFolderPath()) + Element.m_Target.first + ".ytd";
			std::string AssetName = "OTS" + Element.m_Target.first;

			if (!Swap.m_Initialized) {
				if (std::filesystem::exists(FilePath)) {
					Rage::Engine::RegisterTexture(FilePath, AssetName + ".ytd");

					Menu::GetControlManager()->SimpleRequestDict(AssetName.c_str());
					Menu::GetControlManager()->SimpleRequestDict(Element.m_Original.first.c_str());

					Rage::Types::DictionaryItem* Item = FindItem(Element.m_Original.first, Element.m_Original.second);
					if (Item) {
						Swap.m_Initialized = true;
						memcpy(&Element.m_OriginalItem, Item, sizeof(Rage::Types::DictionaryItem));
						goto Start;
					}
				} else {
					Menu::GetNotify()->NotifyStacked(This->getString("Failed to find dictionary") + " " + Element.m_Target.first + ".ytd", Global::UIVars::g_NotificationError);
				}

				continue;
			}

		Start:
			// do the hax
			Rage::Types::DictionaryItem* Item = FindItem(Element.m_Original.first, Element.m_Original.second);
			if (Item) {
				if (Swap.m_Enabled) {
					Memory::WriteMemcpy(Item, (Rage::Types::DictionaryItem*)Rage::Engine::GetSpriteTexture(AssetName.c_str(), Element.m_Target.second.c_str()), sizeof(Rage::Types::DictionaryItem));
				} else {
					Memory::WriteMemcpy(Item, &Element.m_OriginalItem, sizeof(Rage::Types::DictionaryItem));
				}
			}
		}
	}

	void Load(MiscTextureSwapMenu* This) {
		try {
			std::ifstream Input(Utils::GetConfig()->GetTextureSwapPath());
			if (Input.good()) {
				nlohmann::json Json;
				Input >> Json;
				Input.close();

				if (Json.size() > 0) {
					Json = Json["textures"];

					for (auto& it = Json.begin(); it != Json.end(); ++it) {
						nlohmann::json& Element = *it;

						if (std::find_if(begin(m_Vars.m_Swaps), end(m_Vars.m_Swaps), [=](TextureSwap& Swap) { return !Swap.m_Name.compare(it.key()); }) == end(m_Vars.m_Swaps)) {
							TextureSwap Swap;
							Swap.m_Name = it.key();
							Swap.m_Initialized = false;

							for (auto& oit = Element.begin(); oit != Element.end(); ++oit) {
								nlohmann::json& ElementObject = *oit;

								TextureSwapContext Context;
								Context.m_Original = { ElementObject["original"][0].get<std::string>(), ElementObject["original"][1].get<std::string>() };
								Context.m_Target = { ElementObject["target"][0].get<std::string>(), ElementObject["target"][1].get<std::string>() };
								Swap.m_Contexts.push_back(Context);
							}

							m_Vars.m_Swaps.push_back(Swap);
						}
					}
				}
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}

		This->ClearOptionsOffset(0);
		if (m_Vars.m_Swaps.size()) {
			for (auto& S : m_Vars.m_Swaps) {
				This->addOption(ToggleOption(S.m_Name)
					.addToggle(S.m_Enabled)
					.addOnClick([&, This] { Handle(This, S); }));
			}
		} else {
			This->addOption(ButtonOption(This->getString("~m~None")));
		}
	}
}

void MiscTextureSwapMenu::Init() {
	SetName("Texture Swap");
	SetParentSubmenu<MiscMenu>();

	addString("Failed to load dictionary");
	addString("Failed to find dictionary");
	addString("~m~None");

	if (!std::filesystem::exists(Utils::GetConfig()->GetTextureSwapPath())) {
		try {
			std::ofstream Output(Utils::GetConfig()->GetTextureSwapPath());
			if (Output.good()) {
				nlohmann::json Json;

				Json["textures"]["example"] = nlohmann::json::array();

				nlohmann::json Object;
				Object["original"] = { "slamvan", "slamvan_lights_glass" };
				Object["target"] = { "CustomYTD", "slamvan_lights_glass" };

				Json["textures"]["example"].push_back(Object);

				Object["original"] = { "skydome", "moon-new" };
				Object["target"] = { "CustomYTD", "super_cool_moon" };

				Json["textures"]["example"].push_back(Object);

				Output << Json.dump(4);
				Output.close();
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}
	}

	Load(this);
}

void MiscTextureSwapMenu::Update() {}

/*Called once on submenu open*/
void MiscTextureSwapMenu::UpdateOnce() {
	Load(this);
}

/*Called always*/
void MiscTextureSwapMenu::FeatureUpdate() {}

MiscTextureSwapMenu* _instance;
MiscTextureSwapMenu* MiscTextureSwapMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MiscTextureSwapMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

MiscTextureSwapMenu::~MiscTextureSwapMenu() { delete _instance; }