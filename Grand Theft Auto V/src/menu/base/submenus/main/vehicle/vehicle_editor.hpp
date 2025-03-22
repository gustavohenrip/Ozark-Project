#pragma once
#include "menu/base/submenu.hpp"

class VehicleEditorMenu : public Menu::Submenu {
public:
	static VehicleEditorMenu* GetInstance();

	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VehicleEditorMenu()
		: Menu::Submenu() {}

	~VehicleEditorMenu();
};

namespace VehicleEditorMenuVars {
	struct VehicleStats {
		float m_EstimatedMaxSpeed;
		float m_Acceleration;
		float m_Braking;
		float m_Traction;
		bool m_Success;

		// Mods
		int m_Armor;
		int m_Brakes;
		int m_FrontBumper;
		int m_RearBumper;
		int m_Engine;
		int m_Exhaust;
		int m_Hood;
		int m_Horn;
		int m_Lights;
		int m_Skirts;
		int m_Suspension;
		int m_Transmission;
		int m_Spoiler;
		int m_Grille;
		int m_LeftFender;
		int m_RightFender;
		int m_Roof;
		int m_Chassis;
		int m_Turbo;
		int m_WindowTint;
		int m_XenonColor;
		int m_PlateIndex;
		bool m_Neon[4] = { false, false, false, false };
		Color m_NeonColor = { 0, 0, 0, 0 };

		// Bennys
		int m_BennysPlateholder;
		int m_BennysVanityPlate;
		int m_BennysTrimDesign;
		int m_BennysOrnaments;
		int m_BennysInteriorDesign;
		int m_BennysDials;
		int m_BennysDoors;
		int m_BennysSeats;
		int m_BennysSteeringWheel;
		int m_BennysShifter;
		int m_BennysPlaques;
		int m_BennysTrunk;
		int m_BennysAudioInstall;
		int m_BennysHydraulics;
		int m_BennysEngineBlock;
		int m_BennysAirFilters;
		int m_BennysStuntBrace;
		int m_BennysArchCovers;
		int m_BennysAerials;
		int m_BennysRearStripeColor;
		int m_BennysTank;
		int m_BennysWindowMod;
		int m_BennysLivery;
		int m_BennysSpecialFrontRims;
		int m_BennysSpecialRearRims;

		VehicleStats()
			: m_EstimatedMaxSpeed(0.f),
			m_Acceleration(0.f),
			m_Braking(0.f),
			m_Traction(0.f),
			m_Success(false),
			m_WindowTint(0),
			m_Spoiler(0),
			m_Grille(0),
			m_LeftFender(0),
			m_RightFender(0),
			m_PlateIndex(0),
			m_XenonColor(0),
			m_Roof(0),
			m_Chassis(0),
			m_Armor(0),
			m_Brakes(0),
			m_FrontBumper(0),
			m_RearBumper(0),
			m_Engine(0),
			m_Exhaust(0),
			m_Hood(0),
			m_Horn(0),
			m_Lights(0),
			m_Skirts(0),
			m_Suspension(0),
			m_Transmission(0),
			m_Turbo(0),

			m_BennysPlateholder(0),
			m_BennysVanityPlate(0),
			m_BennysTrimDesign(0),
			m_BennysOrnaments(0),
			m_BennysInteriorDesign(0),
			m_BennysDials(0),
			m_BennysDoors(0),
			m_BennysSeats(0),
			m_BennysSteeringWheel(0),
			m_BennysShifter(0),
			m_BennysPlaques(0),
			m_BennysTrunk(0),
			m_BennysAudioInstall(0),
			m_BennysHydraulics(0),
			m_BennysEngineBlock(0),
			m_BennysAirFilters(0),
			m_BennysStuntBrace(0),
			m_BennysArchCovers(0),
			m_BennysAerials(0),
			m_BennysRearStripeColor(0),
			m_BennysTank(0),
			m_BennysWindowMod(0),
			m_BennysLivery(0),
			m_BennysSpecialFrontRims(0),
			m_BennysSpecialRearRims(0)
		{}
	};

	struct Vars_ {
		bool m_LSC = true;
		int m_GlobalStats[2] = { 0, 0 };
		float m_Values[4][4];
		VehicleStats m_CachedStats;
		VehicleStats m_NewStats;
		bool m_UpdateCache = false;

		// Wheel cache
		std::vector<std::pair<const char*, int>> m_WheelSport;
		std::vector<std::pair<const char*, int>> m_WheelMuscle;
		std::vector<std::pair<const char*, int>> m_WheelLowrider;
		std::vector<std::pair<const char*, int>> m_WheelSUV;
		std::vector<std::pair<const char*, int>> m_WheelOffroad;
		std::vector<std::pair<const char*, int>> m_WheelTuner;
		std::vector<std::pair<const char*, int>> m_WheelBike;
		std::vector<std::pair<const char*, int>> m_WheelHighEnd;
	};

	void UpdateCaches();
	void CacheVehicleStats();

	namespace Scripts {
		const char* GetVehicleHornLabel(int uParam0);
		void DrawVehicleStats(Vehicle Veh);
		void PopulateGlobals(int* uParam0);
	};

	std::string GetVehicleModName(eVehicleModTypes Type, std::string Default);
	extern Vars_ m_Vars;
}
