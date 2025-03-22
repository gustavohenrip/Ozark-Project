#include "translations.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../settings.hpp"
#include "utils/va.hpp"
#include "utils/json.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/fiber_pool.hpp"
#include "translations_selected.hpp"
#include "utils/log.hpp"

using namespace TranslationMenuVars;

namespace TranslationMenuVars {
	Vars_ m_Vars;

	void CreateTemplateTranslation() {
		try {
			std::ofstream Output(Utils::VA::VA("%sTemplate.json", Utils::GetConfig()->GetTranslationPath().c_str()));
			if (Output.good()) {
				nlohmann::json Json;

				for (auto& Submenu : Menu::GetSubmenuHandler()->GetSubmenus()) {
					if (Submenu) {
						std::string SubmenuName = Submenu->m_Name.GetOriginal();
						for (auto& Option : Submenu->m_Options) {
							if (Option->GetName().HasTranslation() && !Option->GetName().GetOriginal().empty()) {
								Json["Strings"][SubmenuName][Option->GetName().GetOriginal()] = Option->GetName().GetOriginal();
							}

							if (Option->GetTooltip().HasTranslation() && !Option->GetTooltip().GetOriginal().empty()) {
								Json["Strings"][SubmenuName][Option->GetTooltip().GetOriginal()] = Option->GetTooltip().GetOriginal();
							}

							Option->WriteTranslation(Json["Strings"][SubmenuName]);
						}

						if (Submenu->m_Name.HasTranslation() && !Submenu->m_Name.GetOriginal().empty()) {
							Json["Strings"]["Submenus"][Submenu->m_Name.GetOriginal()] = Submenu->m_Name.GetOriginal();
						}

						for (auto& String : Submenu->GetStrings()) {
							if (String.HasTranslation() && !String.GetOriginal().empty()) {
								Json["Strings"][SubmenuName][String.GetOriginal()] = String.GetOriginal();
							}
						}
					}

					for (auto& String : Global::Vars::g_TranslationTable) {
						Json["Strings"]["Misc"][String->GetOriginal()] = String->GetOriginal();
					}
				}

				Output << Json.dump(4);
				Output.close();
			}
		} catch (std::exception& E) {
			LOG_WARN("[JSON] %s", E.what());
		}

		// recache list to include template
		TranslationMenu::GetInstance()->UpdateOnce();
	}

	void Reset() {
		for (auto& Submenu : Menu::GetSubmenuHandler()->GetSubmenus()) {
			std::string SubmenuName = Submenu->m_Name.GetOriginal();
			for (auto& Option : Submenu->m_Options) {
				if (Option->GetName().HasTranslation()) Option->GetName().Reset();
				if (Option->GetTooltip().HasTranslation()) Option->GetTooltip().Reset();
			}

			if (Submenu->m_Name.HasTranslation()) Submenu->m_Name.Reset();

			for (auto& String : Submenu->GetStrings()) if (String.HasTranslation()) String.Reset();
		}

		for (auto& String : Global::Vars::g_TranslationTable) {
			String->Reset();
		}
	}

	void LoadTranslation(std::string File) {
		try {
			std::ifstream Input(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetTranslationPath().c_str(), File.c_str()));
			if (Input.good()) {
				nlohmann::json Json = nlohmann::json::parse(Input);
				Input.close();

				for (auto& Submenu : Menu::GetSubmenuHandler()->GetSubmenus()) {
					std::string SubmenuName = Submenu->m_Name.GetOriginal();
					for (auto& Option : Submenu->m_Options) {
						if (Option->GetName().HasTranslation() && !Option->GetName().GetOriginal().empty()) {
							Option->GetName().SetMapped(JsonGet<std::string>(Json["Strings"][SubmenuName], Option->GetName().GetOriginal(), Option->GetName().GetOriginal()).c_str());
						}

						if (Option->GetTooltip().HasTranslation() && !Option->GetTooltip().GetOriginal().empty()) {
							Option->GetTooltip().SetMapped(JsonGet<std::string>(Json["Strings"][SubmenuName], Option->GetTooltip().GetOriginal(), Option->GetTooltip().GetOriginal()).c_str());
						}

						Option->ReadTranslation(Json["Strings"][SubmenuName]["Scrollables"]);
					}

					if (Submenu->m_Name.HasTranslation() && !Submenu->m_Name.GetOriginal().empty()) {
						if (!Json["Strings"]["Submenus"][SubmenuName].is_null()) {
							if (strcmp(Json["Strings"]["Submenus"][SubmenuName].get<std::string>().c_str(), "Ozark")) { // fuck niggers™
								Submenu->m_Name.SetMapped(Json["Strings"]["Submenus"][SubmenuName].get<std::string>().c_str());
							}
						}
					}

					for (auto& String : Submenu->GetStrings()) {
						if (String.HasTranslation() && !String.GetOriginal().empty()) {
							String.SetMapped(JsonGet<std::string>(Json["Strings"][SubmenuName], String.GetOriginal(), String.GetOriginal()).c_str());
						}
					}
				}

				for (auto& String : Global::Vars::g_TranslationTable) {
					String->SetMapped(JsonGet<std::string>(Json["Strings"]["Misc"], String->GetOriginal(), String->GetOriginal()).c_str());
				}
			} else {
				Menu::GetNotify()->NotifyStacked(TranslationMenu::GetInstance()->getString("Failed to open translation file!"), Global::UIVars::g_NotificationError);
			}
		} catch (nlohmann::json::parse_error& Error) {
			if (Error.id == 101) {
				// encoding is wack
				Menu::GetNotify()->NotifyStacked(TranslationMenu::GetInstance()->getString("Failed to verify translation encoding. Ensure encoding is UTF-8!"), Global::UIVars::g_NotificationError);
			} else {
				// failed
				Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s: %i", TranslationMenu::GetInstance()->getString("Failed to load translation").c_str(), Error.id), Global::UIVars::g_NotificationError);
			}
		} catch (std::exception& Exception) {
			// error
			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s: %s", TranslationMenu::GetInstance()->getString("Failed to load translation. Exception").c_str(), Exception.what()), Global::UIVars::g_NotificationError);
		}
	}

	void UpdateExistingTranslation(std::string File) {
		try {
			std::ifstream Input(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetTranslationPath().c_str(), File.c_str()));
			if (Input.good()) {
				nlohmann::json Json = nlohmann::json::parse(Input);
				Input.close();

				for (auto& Submenu : Menu::GetSubmenuHandler()->GetSubmenus()) {
					std::string SubmenuName = Submenu->m_Name.GetOriginal();
					for (auto& Option : Submenu->m_Options) {
						if (Option->GetName().HasTranslation() && !Option->GetName().GetOriginal().empty()) {
							if (Json["Strings"][SubmenuName][Option->GetName().GetOriginal()].is_null()) {
								Json["Strings"][SubmenuName][Option->GetName().GetOriginal()] = Option->GetName().GetOriginal();
							}
						}

						if (Option->GetTooltip().HasTranslation() && !Option->GetTooltip().GetOriginal().empty()) {
							if (Json["Strings"][SubmenuName][Option->GetTooltip().GetOriginal()].is_null()) {
								Json["Strings"][SubmenuName][Option->GetTooltip().GetOriginal()] = Option->GetTooltip().GetOriginal();
							}
						}

						Option->WriteTranslation(Json["Strings"][SubmenuName]["Scrollables"]);
					}

					if (Submenu->m_Name.HasTranslation() && !Submenu->m_Name.GetOriginal().empty()) {
						if (Json["Strings"]["Submenus"][SubmenuName].is_null()) {
							Json["Strings"]["Submenus"][SubmenuName] = SubmenuName;
						}
					}

					for (auto& String : Submenu->GetStrings()) {
						if (String.HasTranslation() && !String.GetOriginal().empty()) {
							if (Json["Strings"][SubmenuName][String.GetOriginal()].is_null()) {
								Json["Strings"][SubmenuName][String.GetOriginal()] = String.GetOriginal();
							}
						}
					}
				}

				for (auto& String : Global::Vars::g_TranslationTable) {
					if (Json["Strings"]["Misc"][String->GetOriginal()].is_null()) {
						Json["Strings"]["Misc"][String->GetOriginal()] = String->GetOriginal();
					}
				}

				std::ofstream Output(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetTranslationPath().c_str(), File.c_str()));
				if (Output.good()) {
					Output << Json.dump(4);
					Output.close();
				}
			} else {
				Menu::GetNotify()->NotifyStacked(TranslationMenu::GetInstance()->getString("Failed to open translation file!"), Global::UIVars::g_NotificationError);
			}
		} catch (nlohmann::json::parse_error& Error) {
			if (Error.id == 101) {
				// encoding is wack
				Menu::GetNotify()->NotifyStacked(TranslationMenu::GetInstance()->getString("Failed to verify translation encoding. Ensure encoding is UTF-8!"), Global::UIVars::g_NotificationError);
			} else {
				// failed
				Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s: %i", TranslationMenu::GetInstance()->getString("Failed to load translation").c_str(), Error.id), Global::UIVars::g_NotificationError);
			}
		} catch (std::exception& Exception) {
			// error
			Menu::GetNotify()->NotifyStacked(Utils::VA::VA("%s: %s", TranslationMenu::GetInstance()->getString("Failed to load translation. Exception").c_str(), Exception.what()), Global::UIVars::g_NotificationError);
		}

		TranslationMenu::GetInstance()->UpdateOnce();
	}
}

void TranslationMenu::Init() {
	SetName("Translations");
	SetParentSubmenu<SettingsMenu>();

	addString("Failed to open translation file!");
	addString("Failed to verify translation encoding. Ensure encoding is UTF-8!");
	addString("Failed to load translation");
	addString("Failed to load translation. Exception");

	addOption(ButtonOption("Create Template Translation")
		.addTranslation()
		.addOnClick([] { Utils::GetFiberPool()->Push(CreateTemplateTranslation); }));

	addOption(ScrollOption<int>(SCROLLSELECT, "Update Existing Translation")
		.addTranslation()
		.addScroll(m_Vars.m_UpdateTranslation, 0, NUMOF(m_Vars.m_TranslationList), m_Vars.m_TranslationList)
		.addOnUpdate([] (ScrollOption<int>* Option) { Option->addScroll(m_Vars.m_UpdateTranslation, 0, m_Vars.m_ListSize, m_Vars.m_TranslationList); })
		.addOnClick([] { UpdateExistingTranslation(m_Vars.m_TranslationList[m_Vars.m_UpdateTranslation].m_Name.GetOriginal()); })
		.addRequirement([] { return m_Vars.m_ListSize > 0; }));

	addOption(ButtonOption("Reset to English")
		.addTranslation()
		.addOnClick([] { Utils::GetFiberPool()->Push(Reset); }));

	addOption(BreakOption("Translations")
		.addTranslation());

	addString("~m~None");

	TranslationSelectedMenu::GetInstance();
}

void TranslationMenu::Update() {}

/*Called once on submenu open*/
void TranslationMenu::UpdateOnce() {
	ClearOptionsOffset(4);

	m_Vars.m_CachedTranslations.clear();
	int Size = 0;

	std::vector<std::string> Translations = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetTranslationPath(), ".json");
	if (Translations.size()) {
		for (auto& Translation : Translations) {
			m_Vars.m_TranslationList[Size].m_Name = Translation;
			m_Vars.m_TranslationList[Size].m_Result = Size++;

			std::pair<std::string, struct stat> Info;
			Info.first = Translation;

			if (stat(Utils::VA::VA("%s%s.json", Utils::GetConfig()->GetTranslationPath().c_str(), Translation.c_str()), &Info.second) == 0) {
				std::size_t Index = m_Vars.m_CachedTranslations.size();
				if (Index < 0) Index = 0;
				m_Vars.m_CachedTranslations.push_back(Info);

				addOption(SubmenuOption(Translation)
					.addSubmenu<TranslationSelectedMenu>()
					.addOnClick([=] { TranslationSelectedMenuVars::m_Vars.m_Selected = Index; }));
			}
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}

	m_Vars.m_ListSize = Size;
}

/*Called always*/
void TranslationMenu::FeatureUpdate() {}

TranslationMenu* _instance;
TranslationMenu* TranslationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new TranslationMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

TranslationMenu::~TranslationMenu() { delete _instance; }