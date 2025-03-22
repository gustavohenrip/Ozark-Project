#pragma once
#include "menu/base/submenu.hpp"

class VehicleHandlingBoatMenu : public Menu::Submenu {
public:
	static VehicleHandlingBoatMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleHandlingBoatMenu()
		: Menu::Submenu() {}

	~VehicleHandlingBoatMenu();
};

namespace VehicleHandlingBoatMenuVars {
	struct PhysicalRestore {
		float m_BoxFrontMult;
		float m_BoxRearMult;
		float m_BoxSideMult;
		float m_SampleTop;
		float m_SampleBottom;
	};

	struct AquaplaneRestore {
		float m_AquaplaneForce;
		float m_AquaplanePushWaterMult;
		float m_AquaplanePushWaterCap;
		float m_AquaplanePushWaterApply;
		float m_KeelSphereSize;
		float m_PropRadius;
		float m_ImpellerOffset;
		float m_ImpellerForceMult;
		float m_ProwRaiseMult;
	};

	struct RudderRestore {
		float m_RudderForce;
		float m_RudderOffsetSubmerge;
		float m_RudderOffsetForce;
		float m_RudderOffsetForceZMult;
	};

	struct ResistanceRestore {
		float m_DragCoefficient;
		Math::Vector3_<float> m_VecMoveResistance;
		Math::Vector3_<float> m_VecTurnResistance;
	};

	struct MiscRestore {
		float m_WaveAudioMult;
		float m_Look_L_R_CamHeight;
		float m_LowLodAngOffset;
		float m_LowLodDraughtOffset;
		float m_DinghySphereBuoyConst;
	};

	struct Vars_ {
		int m_Type;

		std::unordered_map<uint32_t, PhysicalRestore> m_PhysicalRestore;
		std::unordered_map<uint32_t, AquaplaneRestore> m_AquaplaneRestore;
		std::unordered_map<uint32_t, RudderRestore> m_RudderRestore;
		std::unordered_map<uint32_t, ResistanceRestore> m_ResistanceRestore;
		std::unordered_map<uint32_t, MiscRestore> m_MiscRestore;
	};

	void CachePhysical(uint32_t Model);
	void CacheAquaplane(uint32_t Model);
	void CacheRudder(uint32_t Model);
	void CacheResistance(uint32_t Model);
	void CacheMisc(uint32_t Model);
	extern Vars_ m_Vars;
}