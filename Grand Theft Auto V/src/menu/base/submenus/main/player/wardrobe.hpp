#pragma once
#include "menu/base/submenu.hpp"

class WardrobeMenu : public Menu::Submenu {
public:
	static WardrobeMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WardrobeMenu()
		: Menu::Submenu() {}

	~WardrobeMenu();
};

namespace WardrobeMenuVars {
	struct Vars_ {
		bool m_PreviewCamera;
		Cam m_PreviewCam;

		int m_Clothing;
		int m_ClothingModel;
		int m_ClothingVariation;

		int m_Accessory;
		int m_AccessoryModel;
		int m_AccessoryVariation;
		int m_AccessoryClear;
	};

	void RemoveCamera();

	extern ScrollStruct<int> Components[12];
	extern ScrollStruct<int> Accessories[5];
	extern Vars_ m_Vars;
}