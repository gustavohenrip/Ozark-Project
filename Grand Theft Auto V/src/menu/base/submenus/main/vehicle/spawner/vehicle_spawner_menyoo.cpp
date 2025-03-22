#include "vehicle_spawner_menyoo.hpp"
#include "menu/base/submenu_handler.hpp"
#include "../vehicle_spawner.hpp"
#include "utils/xml/pugixml.hpp"
#include "menu/base/util/notify.hpp"
#include "utils/va.hpp"
#include "menu/base/util/control_manager.hpp"
#include "utils/fibers.hpp"
#include "utils/fiber_pool.hpp"
#include "rage/engine.hpp"

using namespace VehicleSpawnerMenyooMenuVars;

namespace VehicleSpawnerMenyooMenuVars {
	Vars_ m_Vars;

	struct SpoonerAttachments {
		struct VehicleProperties {
			struct Colours {
				int Primary;
				int Secondary;
				int Pearl;
				int Rim;
				int Mod1_a;
				int Mod1_b;
				int Mod1_c;
				int Mod2_a;
				int Mod2_b;
				bool IsPrimaryColourCustom;
				bool IsSecondaryColourCustom;
				int Cust1_R;
				int Cust1_G;
				int Cust1_B;
				int Cust2_R;
				int Cust2_G;
				int Cust2_B;
				int TyreSmoke_R;
				int TyreSmoke_G;
				int TyreSmoke_B;
				int LrInterior;
				int LrDashboard;
			} Colours;

			struct Neons {
				bool Left;
				bool Right;
				bool Front;
				bool Back;
				int R;
				int G;
				int B;
			} Neons;

			struct DoorsOpen {
				bool BackLeftDoor;
				bool BackRightDoor;
				bool FrontLeftDoor;
				bool FrontRightDoor;
				bool Hood;
				bool Trunk;
				bool Trunk2;
			} DoorsOpen;

			struct DoorsBroken {
				bool BackLeftDoor;
				bool BackRightDoor;
				bool FrontLeftDoor;
				bool FrontRightDoor;
				bool Hood;
				bool Trunk;
				bool Trunk2;
			} DoorsBroken;

			struct TyresBursted {
				bool FrontLeft;
				bool FrontRight;
				bool _2;
				bool _3;
				bool BackLeft;
				bool BackRight;
				bool _6;
				bool _7;
				bool _8;
			} TyresBursted;

			struct Mods {
				std::string _0;
				std::string _1;
				std::string _2;
				std::string _3;
				std::string _4;
				std::string _5;
				std::string _6;
				std::string _7;
				std::string _8;
				std::string _9;
				std::string _10;
				std::string _11;
				std::string _12;
				std::string _13;
				std::string _14;
				std::string _15;
				std::string _16;
				bool _17;
				bool _18;
				bool _19;
				bool _20;
				bool _21;
				bool _22;
				std::string _23;
				std::string _24;
				std::string _25;
				std::string _26;
				std::string _27;
				std::string _28;
				std::string _29;
				std::string _30;
				std::string _31;
				std::string _32;
				std::string _33;
				std::string _34;
				std::string _35;
				std::string _36;
				std::string _37;
				std::string _38;
				std::string _39;
				std::string _40;
				std::string _41;
				std::string _42;
				std::string _43;
				std::string _44;
				std::string _45;
				std::string _46;
				std::string _47;
				std::string _48;
			} Mods;

			int Livery;
			std::string NumberPlateText;
			int NumberPlateIndex;
			int WheelType;
			bool WheensInvisible;
			std::string EngineSoundName;
			int WindowTint;
			bool BulletProofTyres;
			float DirtLevel;
			float PaintFade;
			int RoofState;
			bool SirenActive;
			bool EngineOn;
			int EngineHealth;
			bool LightsOn;
			int IsRadioLoud;
			int LockStatus;
			int RpmMultiplier;
			int TorqueMultiplier;
			int HeadlightIntensity;
		} VehicleProperties;

		struct PedProperties {
			struct PedProps {
				std::string _0;
				std::string _1;
				std::string _2;
				std::string _3;
				std::string _4;
				std::string _5;
				std::string _6;
				std::string _7;
				std::string _8;
				std::string _9;
			} PedProps;

			struct PedComps {
				std::string _0;
				std::string _1;
				std::string _2;
				std::string _3;
				std::string _4;
				std::string _5;
				std::string _6;
				std::string _7;
				std::string _8;
				std::string _9;
				std::string _10;
				std::string _11;
			} PedComps;

			bool IsStill;
			uint32_t CurrentWeapon;
			bool RelationshipGroupAltered;
			uint32_t RelationshipGroup;
			bool ScenarioActive;
			bool AnimActive;
			std::string AnimDict;
			std::string AnimName;
		} PedProperties;

		struct Attachment {
			bool IsAttached;
			int AttachedTo;
			int BoneIndex;
			Math::Vector3<float> Position;
			Math::Vector3<float> Rotation;
		} Attachment;

		struct OriginalPlacement {
			Math::Vector3<float> Position;
			Math::Vector3<float> Rotation;
		} Placement;

		int InitialHandle;
		int Handle;
		int Type;
		uint32_t ModelHash;
		bool Dynamic;
		bool FrozenPosition;
		int OpacityLevel;
		int LodDistance;
		bool IsVisible;
		int MaxHealth;
		int Health;
		bool HasGravity;
		bool IsOnFire;
		bool IsInvincible;
		bool IsBulletProof;
		bool IsCollisionProof;
		bool IsExplosionProof;
		bool IsFireProof;
		bool IsMeleeProof;
		bool IsOnlyDamagedByPlayer;
	};

	SpoonerAttachments SpawnEntityFromXmlNode(pugi::xml_node node) {
		SpoonerAttachments data;
		data.Type = node.child("Type").text().as_int();
		data.FrozenPosition = node.child("FrozenPos").text().as_bool();
		data.ModelHash = node.child("ModelHash").text().as_uint();
		data.Dynamic = node.child("Dynamic").text().as_bool();
		data.InitialHandle = node.child("InitialHandle").text().as_int();
		data.OpacityLevel = node.child("OpacityLevel").text().as_int();
		data.LodDistance = node.child("LodDistance").text().as_int();
		data.IsVisible = node.child("IsVisible").text().as_bool();
		data.Health = node.child("Health").text().as_int();
		data.MaxHealth = node.child("MaxHealth").text().as_int();
		data.IsOnFire = node.child("IsOnFire").text().as_bool();
		data.IsInvincible = node.child("IsInvincible").text().as_bool();
		data.IsBulletProof = node.child("IsBulletProof").text().as_bool();
		data.IsCollisionProof = node.child("IsCollisionProof").text().as_bool();
		data.IsExplosionProof = node.child("IsExplosionProof").text().as_bool();
		data.IsFireProof = node.child("IsFireProof").text().as_bool();
		data.IsMeleeProof = node.child("IsMeleeProof").text().as_bool();
		data.IsOnlyDamagedByPlayer = node.child("IsOnlyDamagedByPlayer").text().as_bool();
		data.HasGravity = node.child("HasGravity").text().as_bool(true);

		data.Placement.Position = Math::Vector3<float>(node.child("PositionRotation").child("X").text().as_float(), node.child("PositionRotation").child("Y").text().as_float(), node.child("PositionRotation").child("Z").text().as_float());
		data.Placement.Rotation = Math::Vector3<float>(node.child("PositionRotation").child("Pitch").text().as_float(), node.child("PositionRotation").child("Roll").text().as_float(), node.child("PositionRotation").child("Yaw").text().as_float());

		data.Attachment.IsAttached = node.child("Attachment").attribute("isAttached").as_bool();
		data.Attachment.BoneIndex = node.child("Attachment").child("BoneIndex").text().as_int();
		data.Attachment.Position = Math::Vector3<float>(node.child("Attachment").child("X").text().as_float(), node.child("Attachment").child("Y").text().as_float(), node.child("Attachment").child("Z").text().as_float());
		data.Attachment.Rotation = Math::Vector3<float>(node.child("Attachment").child("Pitch").text().as_float(), node.child("Attachment").child("Roll").text().as_float(), node.child("Attachment").child("Yaw").text().as_float());
		
		if (node.child("Attachment").child("AttachedTo").text().as_string() == "VEHICLE") {
			data.Attachment.AttachedTo = m_Vars.m_VehicleHandle;
		} else {
			data.Attachment.AttachedTo = node.child("Attachment").child("AttachedTo").text().as_int();
		}

		Menu::GetControlManager()->SimpleRequestModel(data.ModelHash);

		switch (data.Type) {
			case 1: { // ped
				data.Handle = Native::CreatePed(21, data.ModelHash, data.Placement.Position.m_X, data.Placement.Position.m_Y, data.Placement.Position.m_Z, 1, 1, 0);
				if (Native::DoesEntityExist(data.Handle)) {
					Native::FreezeEntityPosition(data.Handle, data.FrozenPosition);

					Native::SetPedCanBeKnockedOffVehicle(data.Handle, false);
					Native::SetPedCanSwitchWeapon(data.Handle, false);

					Native::SetPedFleeAttributes(data.Handle, 0, 0);
					Native::SetPedCombatAttributes(data.Handle, 17, 1);

					if (data.OpacityLevel < 255) Native::SetEntityAlpha(data.Handle, data.OpacityLevel, 0);
					Native::SetEntityLodDist(data.Handle, data.LodDistance);
					Native::SetEntityVisible(data.Handle, data.IsVisible, false);

					if (data.MaxHealth) Native::SetEntityMaxHealth(data.Handle, data.MaxHealth);
					if (data.Health) Native::SetEntityHealth(data.Handle, data.Health);

					data.IsOnFire ? Native::StartEntityFire(data.Handle) : Native::StopEntityFire(data.Handle);

					Native::SetEntityInvincible(data.Handle, data.IsInvincible);
					Native::SetEntityProofs(data.Handle, data.IsBulletProof, data.IsFireProof, data.IsExplosionProof, data.IsCollisionProof, data.IsMeleeProof, 1, 1, 1);
					Native::SetEntityOnlyDamagedByPlayer(data.Handle, data.IsOnlyDamagedByPlayer);

					auto& pedProperties = node.child("PedProperties");

					if (pedProperties.child("IsStill").text().as_bool()) {
						Native::TaskStandStill(data.Handle, -1);
					}

					if (pedProperties.child("CanRagdoll")) Native::SetPedCanRagdoll(data.Handle, pedProperties.child("CanRagdoll").text().as_bool());

					if (pedProperties.child("HasShortHeight").text().as_bool()) {
						// *reinterpret_cast<byte*>(pPoolManager.GetEntityAddress(data.Handle) + 0x8A) = 100;
					}

					Native::SetPedArmour(data.Handle, pedProperties.child("Armour").text().as_int());

					if (pedProperties.child("CurrentWeapon")) {
						Native::GiveDelayedWeaponToPed(data.Handle, pedProperties.child("CurrentWeapon").text().as_uint(), 9999, 1);
					}

					if (pedProperties.child("PedComps")) {
						for (auto& nodeComps = pedProperties.child("PedComps").first_child(); nodeComps; nodeComps = nodeComps.next_sibling()) {
							int modType = std::stoi(std::string(nodeComps.name()).substr(1));
							std::string modValueStr = nodeComps.text().as_string();
							if (modValueStr.find(",") != std::string::npos) {
								Native::SetPedComponentVariation(data.Handle, modType, std::stoi(modValueStr.substr(0, modValueStr.find(","))), std::stoi(modValueStr.substr(modValueStr.find(",") + 1)), 0);
							}
						}
					}

					if (pedProperties.child("RelationshipGroupAltered").text().as_bool()) {
						Native::SetPedRelationshipGroupDefaultHash(data.Handle, pedProperties.child("RelationshipGroup").text().as_uint());
					}

					if (pedProperties.child("ScenarioActive").text().as_bool()) {
						Native::TaskStartScenarioInPlace(data.Handle, (char*)pedProperties.child("ScenarioName").text().as_string(), 0, 1);
					}

					if (pedProperties.child("AnimActive").text().as_bool()) {
						char* animDict = (char*)pedProperties.child("AnimDict").text().as_string();
						char* animName = (char*)pedProperties.child("AnimName").text().as_string();

						Native::RequestAnimDict(animDict);
						Native::TaskPlayAnim(data.Handle, animDict, animName, 8, 0, -1, 9, 0, 0, 0, 0);
					}
				}

				break;
			}

			case 2: { // vehicle
				data.Handle = Native::CreateVehicle(data.ModelHash, data.Placement.Position.m_X, data.Placement.Position.m_Y, data.Placement.Position.m_Z, 1, 1, data.Dynamic, 1);
				if (Native::DoesEntityExist(data.Handle)) {
					Native::FreezeEntityPosition(data.Handle, data.FrozenPosition);

					Native::SetVehicleModKit(data.Handle, 0);
					Native::SetEntityVisible(data.Handle, data.IsVisible, false);
					if (data.OpacityLevel < 255) Native::SetEntityAlpha(data.Handle, data.OpacityLevel, 0);
					Native::SetEntityLodDist(data.Handle, data.LodDistance);

					if (data.MaxHealth) Native::SetEntityMaxHealth(data.Handle, data.MaxHealth);
					if (data.Health) Native::SetEntityHealth(data.Handle, data.Health);

					Native::SetVehicleGravity(data.Handle, data.HasGravity);

					data.IsOnFire ? Native::StartEntityFire(data.Handle) : Native::StopEntityFire(data.Handle);

					Native::SetEntityInvincible(data.Handle, data.IsInvincible);
					Native::SetEntityProofs(data.Handle, data.IsBulletProof, data.IsFireProof, data.IsExplosionProof, data.IsCollisionProof, data.IsMeleeProof, 1, 1, 1);
					Native::SetEntityOnlyDamagedByPlayer(data.Handle, data.IsOnlyDamagedByPlayer);

					auto& vehicleProperties = node.child("VehicleProperties");

					Native::SetVehicleLivery(data.Handle, vehicleProperties.child("Livery").text().as_int());

					auto& vehicleColors = vehicleProperties.child("Colours");

					int currentPrimaryColor, currentSecondaryColor;
					Native::GetVehicleColours(data.Handle, &currentPrimaryColor, &currentSecondaryColor);
					Native::SetVehicleColours(data.Handle, vehicleColors.child("Primary").text().as_int(), currentSecondaryColor);
					Native::GetVehicleColours(data.Handle, &currentPrimaryColor, &currentSecondaryColor);
					Native::SetVehicleColours(data.Handle, currentPrimaryColor, vehicleColors.child("Secondary").text().as_int());

					int currentPearlescentColor, currentWheelColor;
					Native::GetVehicleExtraColours(data.Handle, &currentPearlescentColor, &currentWheelColor);
					Native::SetVehicleExtraColours(data.Handle, vehicleColors.child("Pearl").text().as_int(), currentWheelColor);
					Native::GetVehicleExtraColours(data.Handle, &currentPearlescentColor, &currentWheelColor);
					Native::SetVehicleExtraColours(data.Handle, currentPearlescentColor, vehicleColors.child("Rim").text().as_int());

					bool isPrimaryColourCustom = vehicleColors.child("IsPrimaryColourCustom").text().as_bool();
					bool isSecondaryColourCustom = vehicleColors.child("IsSecondaryColourCustom").text().as_bool();

					Color vehicleColor;

					if (isPrimaryColourCustom) {
						vehicleColor.R = vehicleColors.child("Cust1_R").text().as_int();
						vehicleColor.G = vehicleColors.child("Cust1_G").text().as_int();
						vehicleColor.B = vehicleColors.child("Cust1_B").text().as_int();
						Native::SetVehicleCustomPrimaryColour(data.Handle, vehicleColor.R, vehicleColor.G, vehicleColor.B);
					}

					if (isSecondaryColourCustom) {
						vehicleColor.R = vehicleColors.child("Cust2_R").text().as_int();
						vehicleColor.G = vehicleColors.child("Cust2_G").text().as_int();
						vehicleColor.B = vehicleColors.child("Cust2_B").text().as_int();
						Native::SetVehicleCustomSecondaryColour(data.Handle, vehicleColor.R, vehicleColor.G, vehicleColor.B);
					}

					vehicleColor.R = vehicleColors.child("tyreSmoke_R").text().as_int();
					vehicleColor.G = vehicleColors.child("tyreSmoke_G").text().as_int();
					vehicleColor.B = vehicleColors.child("tyreSmoke_B").text().as_int();
					Native::SetVehicleTyreSmokeColor(data.Handle, vehicleColor.R, vehicleColor.G, vehicleColor.B);

					int mod1a = vehicleColors.child("Mod1_a").text().as_int();
					int mod1b = vehicleColors.child("Mod1_b").text().as_int();
					int mod1c = vehicleColors.child("Mod1_c").text().as_int();
					Native::SetVehicleModColor1(data.Handle, mod1a, mod1b, mod1c);

					int mod2a = vehicleColors.child("Mod2_a").text().as_int();
					int mod2b = vehicleColors.child("Mod2_b").text().as_int();
					Native::SetVehicleModColor2(data.Handle, mod2a, mod2b);

					Native::SetVehicleNumberPlateText(data.Handle, (char*)vehicleProperties.child("NumberPlateText").text().as_string());
					Native::SetVehicleNumberPlateTextIndex(data.Handle, vehicleProperties.child("NumberPlateIndex").text().as_int());
					Native::SetVehicleWheelType(data.Handle, vehicleProperties.child("WheelType").text().as_int());

					std::string engineSoundName = vehicleProperties.child("EngineSoundName").text().as_string();
					//if (!engineSoundName.empty()) _FORCE_VEHICLE_ENGINE_AUDIO(data.Handle, (char*)engineSoundName.c_str());

					Native::SetVehicleWindowTint(data.Handle, vehicleProperties.child("WindowTint").text().as_int());
					Native::SetVehicleTyresCanBurst(data.Handle, !vehicleProperties.child("BulletProofTyres").text().as_bool());
					Native::SetVehicleDirtLevel(data.Handle, vehicleProperties.child("DirtLevel").text().as_float());
					Native::SetVehicleEnveffScale(data.Handle, vehicleProperties.child("PaintFade").text().as_float());

					switch (vehicleProperties.child("RoofState").text().as_int()) {
						case 0:
						case 3:
							Native::RaiseConvertibleRoof(data.Handle, false);
							break;
						case 2:
						case 1:
							Native::LowerConvertibleRoof(data.Handle, false);
							break;
					}

					Native::SetVehicleSiren(data.Handle, vehicleProperties.child("SirenActive").text().as_bool());

					if (vehicleProperties.child("EngineOn")) Native::SetVehicleEngineOn(data.Handle, vehicleProperties.child("EngineOn").text().as_bool(), true, 0);
					if (vehicleProperties.child("EngineHealth")) Native::SetVehicleEngineHealth(data.Handle, vehicleProperties.child("EngineHealth").text().as_int());

					if (vehicleProperties.child("LightsOn")) Native::SetVehicleLights(data.Handle, vehicleProperties.child("LightsOn").text().as_bool() ? 3 : 4);
					if (vehicleProperties.child("IsRadioLoud").text().as_int(0)) {
						Native::SetVehicleRadioLoud(data.Handle, vehicleProperties.child("IsRadioLoud").text().as_bool());
						Native::SetVehicleRadioEnabled(data.Handle, true);
					}

					Native::SetVehicleDoorsLocked(data.Handle, vehicleProperties.child("LockStatus").text().as_int());

					auto& vehicleNeons = vehicleProperties.child("Neons");

					Native::_SetVehicleNeonLightEnabled(data.Handle, 0, vehicleNeons.child("Left").text().as_bool());
					Native::_SetVehicleNeonLightEnabled(data.Handle, 1, vehicleNeons.child("Right").text().as_bool());
					Native::_SetVehicleNeonLightEnabled(data.Handle, 2, vehicleNeons.child("Front").text().as_bool());
					Native::_SetVehicleNeonLightEnabled(data.Handle, 3, vehicleNeons.child("Back").text().as_bool());
					Native::_SetVehicleNeonLightsColour(data.Handle, vehicleNeons.child("R").text().as_int(), vehicleNeons.child("G").text().as_int(), vehicleNeons.child("B").text().as_int());

					auto& vehicleModExtras = vehicleProperties.child("ModExtras");

					for (auto& nodeVehicleModExtrasObject = vehicleModExtras.first_child(); nodeVehicleModExtrasObject; nodeVehicleModExtrasObject = nodeVehicleModExtrasObject.next_sibling()) {
						Native::SetVehicleExtra(data.Handle, std::stoi(std::string(nodeVehicleModExtrasObject.name()).substr(1)), !nodeVehicleModExtrasObject.text().as_bool());
					}

					auto& vehicleMods = vehicleProperties.child("Mods");

					for (auto& nodeVehicleModsObject = vehicleMods.first_child(); nodeVehicleModsObject; nodeVehicleModsObject = nodeVehicleModsObject.next_sibling()) {
						int modType = std::stoi(std::string(nodeVehicleModsObject.name()).substr(1));
						std::string modValueStr = nodeVehicleModsObject.text().as_string();
						if (modValueStr.find(",") == std::string::npos) {
							Native::ToggleVehicleMod(data.Handle, modType, nodeVehicleModsObject.text().as_bool());
						} else {
							Native::SetVehicleMod(data.Handle, modType, stoi(modValueStr.substr(0, modValueStr.find(","))), stoi(modValueStr.substr(modValueStr.find(",") + 1)) ? true : false);
						}
					}

					auto& vehicleDoorsOpen = vehicleProperties.child("DoorsOpen");

					if (vehicleDoorsOpen) {
						if (vehicleDoorsOpen.child("BackLeftDoor").text().as_bool()) {
							Native::SetVehicleDoorOpen(data.Handle, 2, false, true);
							Native::PlayVehicleDoorOpenSound(data.Handle, 1);
						} else {
							Native::SetVehicleDoorShut(data.Handle, 2, true);
							Native::PlayVehicleDoorCloseSound(data.Handle, 1);
						}

						if (vehicleDoorsOpen.child("BackRightDoor").text().as_bool()) {
							Native::SetVehicleDoorOpen(data.Handle, 3, false, true);
							Native::PlayVehicleDoorOpenSound(data.Handle, 1);
						} else {
							Native::SetVehicleDoorShut(data.Handle, 3, true);
							Native::PlayVehicleDoorCloseSound(data.Handle, 1);
						}

						if (vehicleDoorsOpen.child("FrontLeftDoor").text().as_bool()) {
							Native::SetVehicleDoorOpen(data.Handle, 0, false, true);
							Native::PlayVehicleDoorOpenSound(data.Handle, 1);
						} else {
							Native::SetVehicleDoorShut(data.Handle, 0, true);
							Native::PlayVehicleDoorCloseSound(data.Handle, 1);
						}

						if (vehicleDoorsOpen.child("FrontRightDoor").text().as_bool()) {
							Native::SetVehicleDoorOpen(data.Handle, 1, false, true);
							Native::PlayVehicleDoorOpenSound(data.Handle, 1);
						} else {
							Native::SetVehicleDoorShut(data.Handle, 1, true);
							Native::PlayVehicleDoorCloseSound(data.Handle, 1);
						}

						if (vehicleDoorsOpen.child("Hood").text().as_bool()) {
							Native::SetVehicleDoorOpen(data.Handle, 4, false, true);
							Native::PlayVehicleDoorOpenSound(data.Handle, 1);
						} else {
							Native::SetVehicleDoorShut(data.Handle, 4, true);
							Native::PlayVehicleDoorCloseSound(data.Handle, 1);
						}

						if (vehicleDoorsOpen.child("Trunk").text().as_bool()) {
							Native::SetVehicleDoorOpen(data.Handle, 5, false, true);
							Native::PlayVehicleDoorOpenSound(data.Handle, 1);
						} else {
							Native::SetVehicleDoorShut(data.Handle, 5, true);
							Native::PlayVehicleDoorCloseSound(data.Handle, 1);
						}

						if (vehicleDoorsOpen.child("Trunk2").text().as_bool()) {
							Native::SetVehicleDoorOpen(data.Handle, 6, false, true);
							Native::PlayVehicleDoorOpenSound(data.Handle, 1);
						} else {
							Native::SetVehicleDoorShut(data.Handle, 6, true);
							Native::PlayVehicleDoorCloseSound(data.Handle, 1);
						}
					}

					auto& vehicleDoorsBroken = vehicleProperties.child("DoorsBroken");

					if (vehicleDoorsBroken) {
						if (vehicleDoorsBroken.child("BackLeftDoor").text().as_bool()) {
							Native::SetVehicleDoorBroken(data.Handle, 2, true);
						}

						if (vehicleDoorsBroken.child("BackRightDoor").text().as_bool()) {
							Native::SetVehicleDoorBroken(data.Handle, 3, true);
						}

						if (vehicleDoorsBroken.child("FrontLeftDoor").text().as_bool()) {
							Native::SetVehicleDoorBroken(data.Handle, 0, true);
						}

						if (vehicleDoorsBroken.child("FrontRightDoor").text().as_bool()) {
							Native::SetVehicleDoorBroken(data.Handle, 1, true);
						}

						if (vehicleDoorsBroken.child("Hood").text().as_bool()) {
							Native::SetVehicleDoorBroken(data.Handle, 4, true);
						}

						if (vehicleDoorsBroken.child("Trunk").text().as_bool()) {
							Native::SetVehicleDoorBroken(data.Handle, 5, true);
						}

						if (vehicleDoorsBroken.child("Trunk2").text().as_bool()) {
							Native::SetVehicleDoorBroken(data.Handle, 6, true);
						}
					}

					auto& vehicleTyresBursted = vehicleProperties.child("TyresBursted");

					if (vehicleTyresBursted) {
						if (vehicleTyresBursted.child("FrontLeft").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 0, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("FrontRight").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 1, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("_2").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 2, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("_3").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 3, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("BackLeft").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 4, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("BackRight").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 5, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("_6").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 6, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("_7").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 7, 1, 1000.f);
						}

						if (vehicleTyresBursted.child("_8").text().as_bool()) {
							if (!Native::GetVehicleTyresCanBurst(data.Handle)) Native::SetVehicleTyresCanBurst(data.Handle, true);
							Native::SetVehicleTyreBurst(data.Handle, 8, 1, 1000.f);
						}
					}

					auto& vehicleRpmMultiplier = vehicleProperties.child("RpmMultiplier");
					auto& vehicleTorqueMultiplier = vehicleProperties.child("TorqueMultiplier");
					auto& vehicleMaxSpeed = vehicleProperties.child("MaxSpeed");
					auto& vehicleHeadlightIntensity = vehicleProperties.child("HeadlightIntensity");

					if (vehicleRpmMultiplier) {
						Native::_SetVehicleEnginePowerMultiplier(data.Handle, vehicleRpmMultiplier.text().as_float());
					}

					if (vehicleTorqueMultiplier) {
						uint64_t Address = Rage::Engine::GetEntityAddress(data.Handle);
						if (Address) {
							*(float*)(Address + 0x1360) = vehicleTorqueMultiplier.text().as_float();
						}
					}

					if (vehicleMaxSpeed) {
						Native::SetEntityMaxSpeed(data.Handle, vehicleMaxSpeed.text().as_float());
					}

					if (vehicleHeadlightIntensity) {
						Native::SetVehicleLightMultiplier(data.Handle, vehicleHeadlightIntensity.text().as_float());
					}
				}

				break;
			}

			case 3: { // object
				data.Handle = Native::CreateObject(data.ModelHash, data.Placement.Position.m_X, data.Placement.Position.m_Y, data.Placement.Position.m_Z, 1, 1, data.Dynamic);
				if (Native::DoesEntityExist(data.Handle)) {
					Native::FreezeEntityPosition(data.Handle, data.FrozenPosition);

					if (data.OpacityLevel < 255) Native::SetEntityAlpha(data.Handle, data.OpacityLevel, 0);
					Native::SetEntityLodDist(data.Handle, data.LodDistance);
					Native::SetEntityVisible(data.Handle, data.IsVisible, 1);

					if (data.MaxHealth) Native::SetEntityMaxHealth(data.Handle, data.MaxHealth);
					if (data.Health) Native::SetEntityHealth(data.Handle, data.Health);

					data.IsOnFire ? Native::StartEntityFire(data.Handle) : Native::StopEntityFire(data.Handle);

					Native::SetEntityInvincible(data.Handle, data.IsInvincible);
					Native::SetEntityProofs(data.Handle, data.IsBulletProof, data.IsFireProof, data.IsExplosionProof, data.IsCollisionProof, data.IsMeleeProof, 1, 1, 1);

					Native::SetEntityOnlyDamagedByPlayer(data.Handle, data.IsOnlyDamagedByPlayer);
				}

				break;
			}
		}

		if (Native::DoesEntityExist(data.Handle)) {
			Native::NetworkRegisterEntityAsNetworked(data.Handle);
		}

		m_Vars.m_SpawnedAttachments.push_back(data.Handle);
		return data;
	}

	void LoadMenyoo(const char* File) {
		Menu::GetNotify()->NotifyStacked("Started load process");
		Native::SetEntityVisible(Menu::GetLocalPlayer().m_Ped, true, true);

		if (m_Vars.m_DeleteCurrent) {
			for (auto& E : m_Vars.m_SpawnedVehicles) {
				Native::SetEntityAsMissionEntity(E, 1, 1);
				Native::DeleteEntity(&E);
			}

			for (auto& E : m_Vars.m_SpawnedAttachments) {
				Native::SetEntityAsMissionEntity(E, 1, 1);
				Native::DeleteEntity(&E);
			}
		}

		m_Vars.m_VehicleHandle = 0;
		m_Vars.m_InitialHandle = 0;

		pugi::xml_document Doc;
		if (Doc.load_file(Utils::VA::VA("%s/%s.xml", Utils::GetConfig()->GetMenyooPath().c_str(), File)).status != pugi::status_ok) {
			Menu::GetNotify()->NotifyStacked("Unable to load file");
			return;
		}

#pragma region Root
		auto& root = Doc.child("Vehicle");
		if (root) {
			uint32_t vehicleHash = root.child("ModelHash").text().as_uint();
			m_Vars.m_InitialHandle = root.child("InitialHandle").text().as_int();

			Menu::GetControlManager()->SimpleRequestModel(vehicleHash);

			float forward = 5.f;
			float playerHeading = Native::GetEntityHeading(Menu::GetLocalPlayer().m_Ped);

			float xVect = forward * sin((playerHeading * PI) / 180.f) * -1.0f;
			float yVect = forward * cos((playerHeading * PI) / 180.f);

			m_Vars.m_VehicleHandle = Native::CreateVehicle(vehicleHash, Menu::GetLocalPlayer().m_Coords.m_X + xVect, Menu::GetLocalPlayer().m_Coords.m_Y + yVect, Menu::GetLocalPlayer().m_Coords.m_Z, playerHeading, true, true, false);

			if (!Native::DoesEntityExist(m_Vars.m_VehicleHandle)) {
				Menu::GetNotify()->NotifyStacked("Create Vehicle Failed");
				return;
			}

			Menu::GetControlManager()->SimpleRequestControl(m_Vars.m_VehicleHandle);
			m_Vars.m_SpawnedVehicles.push_back(m_Vars.m_VehicleHandle);

			Native::DecorSetInt(m_Vars.m_VehicleHandle, "MPBitset", (1 << 10));
			Native::SetVehicleIsStolen(m_Vars.m_VehicleHandle, false);

			if (Menu::GetLocalPlayer().m_InVehicle) {
				Native::SetEntityAsMissionEntity(Menu::GetLocalPlayer().m_Vehicle, 1, 1);
				Native::DeleteEntity(&Menu::GetLocalPlayer().m_Vehicle);
			}

			Native::SetPedIntoVehicle(Menu::GetLocalPlayer().m_Ped, m_Vars.m_VehicleHandle, -1);
			Native::SetVehicleModKit(m_Vars.m_VehicleHandle, 0);

			Native::SetEntityVisible(m_Vars.m_VehicleHandle, root.child("IsVisible").text().as_bool(), false);
			Native::SetEntityVisible(Menu::GetLocalPlayer().m_Ped, root.child("IsDriverVisible").text().as_bool(), false);

			int opacityLevel = root.child("OpacityLevel").text().as_int();
			if (opacityLevel < 255) Native::SetEntityAlpha(m_Vars.m_VehicleHandle, opacityLevel, 0);
			Native::SetEntityLodDist(m_Vars.m_VehicleHandle, root.child("LodDistance").text().as_int());

			if (root.child("MaxHealth")) Native::SetEntityMaxHealth(m_Vars.m_VehicleHandle, root.child("MaxHealth").text().as_int());
			if (root.child("Health")) Native::SetEntityHealth(m_Vars.m_VehicleHandle, root.child("Health").text().as_int());

			Native::SetVehicleGravity(m_Vars.m_VehicleHandle, root.child("HasGravity").text().as_bool(true));

			root.child("IsOnFire").text().as_bool() ? Native::StartEntityFire(m_Vars.m_VehicleHandle) : Native::StopEntityFire(m_Vars.m_VehicleHandle);

			Native::SetEntityInvincible(m_Vars.m_VehicleHandle, root.child("IsInvincible").text().as_bool());
			Native::SetEntityProofs(m_Vars.m_VehicleHandle, root.child("IsBulletProof").text().as_bool(), root.child("IsFireProof").text().as_bool(), root.child("IsExplosionProof").text().as_bool(), root.child("IsCollisionProof").text().as_bool(), root.child("IsMeleeProof").text().as_bool(), 1, 1, 1);
			Native::SetEntityOnlyDamagedByPlayer(m_Vars.m_VehicleHandle, root.child("IsOnlyDamagedByPlayer").text().as_bool());

			auto& vehicleProperties = root.child("VehicleProperties");

			Native::SetVehicleLivery(m_Vars.m_VehicleHandle, vehicleProperties.child("Livery").text().as_int());

			auto& vehicleColors = vehicleProperties.child("Colours");

			int currentPrimaryColor, currentSecondaryColor;
			Native::GetVehicleColours(m_Vars.m_VehicleHandle, &currentPrimaryColor, &currentSecondaryColor);
			Native::SetVehicleColours(m_Vars.m_VehicleHandle, vehicleColors.child("Primary").text().as_int(), currentSecondaryColor);
			Native::GetVehicleColours(m_Vars.m_VehicleHandle, &currentPrimaryColor, &currentSecondaryColor);
			Native::SetVehicleColours(m_Vars.m_VehicleHandle, currentPrimaryColor, vehicleColors.child("Secondary").text().as_int());

			int currentPearlescentColor, currentWheelColor;
			Native::GetVehicleExtraColours(m_Vars.m_VehicleHandle, &currentPearlescentColor, &currentWheelColor);
			Native::SetVehicleExtraColours(m_Vars.m_VehicleHandle, vehicleColors.child("Pearl").text().as_int(), currentWheelColor);
			Native::GetVehicleExtraColours(m_Vars.m_VehicleHandle, &currentPearlescentColor, &currentWheelColor);
			Native::SetVehicleExtraColours(m_Vars.m_VehicleHandle, currentPearlescentColor, vehicleColors.child("Rim").text().as_int());

			bool isPrimaryColourCustom = vehicleColors.child("IsPrimaryColourCustom").text().as_bool();
			bool isSecondaryColourCustom = vehicleColors.child("IsSecondaryColourCustom").text().as_bool();

			Color vehicleColor;

			if (isPrimaryColourCustom) {
				vehicleColor.R = vehicleColors.child("Cust1_R").text().as_int();
				vehicleColor.G = vehicleColors.child("Cust1_G").text().as_int();
				vehicleColor.B = vehicleColors.child("Cust1_B").text().as_int();
				Native::SetVehicleCustomPrimaryColour(m_Vars.m_VehicleHandle, vehicleColor.R, vehicleColor.G, vehicleColor.B);
			}

			if (isSecondaryColourCustom) {
				vehicleColor.R = vehicleColors.child("Cust2_R").text().as_int();
				vehicleColor.G = vehicleColors.child("Cust2_G").text().as_int();
				vehicleColor.B = vehicleColors.child("Cust2_B").text().as_int();
				Native::SetVehicleCustomSecondaryColour(m_Vars.m_VehicleHandle, vehicleColor.R, vehicleColor.G, vehicleColor.B);
			}

			vehicleColor.R = vehicleColors.child("tyreSmoke_R").text().as_int();
			vehicleColor.G = vehicleColors.child("tyreSmoke_G").text().as_int();
			vehicleColor.B = vehicleColors.child("tyreSmoke_B").text().as_int();
			Native::SetVehicleTyreSmokeColor(m_Vars.m_VehicleHandle, vehicleColor.R, vehicleColor.G, vehicleColor.B);

			int mod1a = vehicleColors.child("Mod1_a").text().as_int();
			int mod1b = vehicleColors.child("Mod1_b").text().as_int();
			int mod1c = vehicleColors.child("Mod1_c").text().as_int();
			Native::SetVehicleModColor1(m_Vars.m_VehicleHandle, mod1a, mod1b, mod1c);

			int mod2a = vehicleColors.child("Mod2_a").text().as_int();
			int mod2b = vehicleColors.child("Mod2_b").text().as_int();
			Native::SetVehicleModColor2(m_Vars.m_VehicleHandle, mod2a, mod2b);

			Native::SetVehicleNumberPlateText(m_Vars.m_VehicleHandle, (char*)vehicleProperties.child("NumberPlateText").text().as_string());
			Native::SetVehicleNumberPlateTextIndex(m_Vars.m_VehicleHandle, vehicleProperties.child("NumberPlateIndex").text().as_int());
			Native::SetVehicleWheelType(m_Vars.m_VehicleHandle, vehicleProperties.child("WheelType").text().as_int());

			std::string engineSoundName = vehicleProperties.child("EngineSoundName").text().as_string();
			//if (!engineSoundName.empty()) _FORCE_VEHICLE_ENGINE_AUDIO(m_Vars.m_VehicleHandle, (char*)engineSoundName.c_str());

			Native::SetVehicleWindowTint(m_Vars.m_VehicleHandle, vehicleProperties.child("WindowTint").text().as_int());
			Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, !vehicleProperties.child("BulletProofTyres").text().as_bool());
			Native::SetVehicleDirtLevel(m_Vars.m_VehicleHandle, vehicleProperties.child("DirtLevel").text().as_float());
			Native::SetVehicleEnveffScale(m_Vars.m_VehicleHandle, vehicleProperties.child("PaintFade").text().as_float());

			switch (vehicleProperties.child("RoofState").text().as_int()) {
				case 0:
				case 3:
					Native::RaiseConvertibleRoof(m_Vars.m_VehicleHandle, false);
					break;
				case 2:
				case 1:
					Native::LowerConvertibleRoof(m_Vars.m_VehicleHandle, false);
					break;
			}

			Native::SetVehicleSiren(m_Vars.m_VehicleHandle, vehicleProperties.child("SirenActive").text().as_bool());

			if (vehicleProperties.child("EngineOn")) Native::SetVehicleEngineOn(m_Vars.m_VehicleHandle, vehicleProperties.child("EngineOn").text().as_bool(), true, 0);
			if (vehicleProperties.child("EngineHealth")) Native::SetVehicleEngineHealth(m_Vars.m_VehicleHandle, vehicleProperties.child("EngineHealth").text().as_int());

			if (vehicleProperties.child("LightsOn")) Native::SetVehicleLights(m_Vars.m_VehicleHandle, vehicleProperties.child("LightsOn").text().as_bool() ? 3 : 4);
			if (vehicleProperties.child("IsRadioLoud").text().as_int(0)) {
				Native::SetVehicleRadioLoud(m_Vars.m_VehicleHandle, vehicleProperties.child("IsRadioLoud").text().as_bool());
				Native::SetVehicleRadioEnabled(m_Vars.m_VehicleHandle, true);
			}

			Native::SetVehicleDoorsLocked(m_Vars.m_VehicleHandle, vehicleProperties.child("LockStatus").text().as_bool());

			auto& vehicleNeons = vehicleProperties.child("Neons");

			Native::_SetVehicleNeonLightEnabled(m_Vars.m_VehicleHandle, 0, vehicleNeons.child("Left").text().as_bool());
			Native::_SetVehicleNeonLightEnabled(m_Vars.m_VehicleHandle, 1, vehicleNeons.child("Right").text().as_bool());
			Native::_SetVehicleNeonLightEnabled(m_Vars.m_VehicleHandle, 2, vehicleNeons.child("Front").text().as_bool());
			Native::_SetVehicleNeonLightEnabled(m_Vars.m_VehicleHandle, 3, vehicleNeons.child("Back").text().as_bool());
			Native::_SetVehicleNeonLightsColour(m_Vars.m_VehicleHandle, vehicleNeons.child("R").text().as_int(), vehicleNeons.child("G").text().as_int(), vehicleNeons.child("B").text().as_int());

			auto& vehicleModExtras = vehicleProperties.child("ModExtras");

			for (auto& nodeVehicleModExtrasObject = vehicleModExtras.first_child(); nodeVehicleModExtrasObject; nodeVehicleModExtrasObject = nodeVehicleModExtrasObject.next_sibling()) {
				Native::SetVehicleExtra(m_Vars.m_VehicleHandle, std::stoi(std::string(nodeVehicleModExtrasObject.name()).substr(1)), !nodeVehicleModExtrasObject.text().as_bool());
			}

			auto& vehicleMods = vehicleProperties.child("Mods");

			for (auto& nodeVehicleModsObject = vehicleMods.first_child(); nodeVehicleModsObject; nodeVehicleModsObject = nodeVehicleModsObject.next_sibling()) {
				int modType = std::stoi(std::string(nodeVehicleModsObject.name()).substr(1));
				std::string modValueStr = nodeVehicleModsObject.text().as_string();
				if (modValueStr.find(",") == std::string::npos) {
					Native::ToggleVehicleMod(m_Vars.m_VehicleHandle, modType, nodeVehicleModsObject.text().as_bool());
				} else {
					Native::SetVehicleMod(m_Vars.m_VehicleHandle, modType, stoi(modValueStr.substr(0, modValueStr.find(","))) ? true : false, stoi(modValueStr.substr(modValueStr.find(",") + 1)) ? true : false);
				}
			}

			auto& vehicleDoorsOpen = vehicleProperties.child("DoorsOpen");

			if (vehicleDoorsOpen) {
				if (vehicleDoorsOpen.child("BackLeftDoor").text().as_bool()) {
					Native::SetVehicleDoorOpen(m_Vars.m_VehicleHandle, 2, false, true);
					Native::PlayVehicleDoorOpenSound(m_Vars.m_VehicleHandle, 1);
				} else {
					Native::SetVehicleDoorShut(m_Vars.m_VehicleHandle, 2, true);
					Native::PlayVehicleDoorCloseSound(m_Vars.m_VehicleHandle, 1);
				}

				if (vehicleDoorsOpen.child("BackRightDoor").text().as_bool()) {
					Native::SetVehicleDoorOpen(m_Vars.m_VehicleHandle, 3, false, true);
					Native::PlayVehicleDoorOpenSound(m_Vars.m_VehicleHandle, 1);
				} else {
					Native::SetVehicleDoorShut(m_Vars.m_VehicleHandle, 3, true);
					Native::PlayVehicleDoorCloseSound(m_Vars.m_VehicleHandle, 1);
				}

				if (vehicleDoorsOpen.child("FrontLeftDoor").text().as_bool()) {
					Native::SetVehicleDoorOpen(m_Vars.m_VehicleHandle, 0, false, true);
					Native::PlayVehicleDoorOpenSound(m_Vars.m_VehicleHandle, 1);
				} else {
					Native::SetVehicleDoorShut(m_Vars.m_VehicleHandle, 0, true);
					Native::PlayVehicleDoorCloseSound(m_Vars.m_VehicleHandle, 1);
				}

				if (vehicleDoorsOpen.child("FrontRightDoor").text().as_bool()) {
					Native::SetVehicleDoorOpen(m_Vars.m_VehicleHandle, 1, false, true);
					Native::PlayVehicleDoorOpenSound(m_Vars.m_VehicleHandle, 1);
				} else {
					Native::SetVehicleDoorShut(m_Vars.m_VehicleHandle, 1, true);
					Native::PlayVehicleDoorCloseSound(m_Vars.m_VehicleHandle, 1);
				}

				if (vehicleDoorsOpen.child("Hood").text().as_bool()) {
					Native::SetVehicleDoorOpen(m_Vars.m_VehicleHandle, 4, false, true);
					Native::PlayVehicleDoorOpenSound(m_Vars.m_VehicleHandle, 1);
				} else {
					Native::SetVehicleDoorShut(m_Vars.m_VehicleHandle, 4, true);
					Native::PlayVehicleDoorCloseSound(m_Vars.m_VehicleHandle, 1);
				}

				if (vehicleDoorsOpen.child("Trunk").text().as_bool()) {
					Native::SetVehicleDoorOpen(m_Vars.m_VehicleHandle, 5, false, true);
					Native::PlayVehicleDoorOpenSound(m_Vars.m_VehicleHandle, 1);
				} else {
					Native::SetVehicleDoorShut(m_Vars.m_VehicleHandle, 5, true);
					Native::PlayVehicleDoorCloseSound(m_Vars.m_VehicleHandle, 1);
				}

				if (vehicleDoorsOpen.child("Trunk2").text().as_bool()) {
					Native::SetVehicleDoorOpen(m_Vars.m_VehicleHandle, 6, false, true);
					Native::PlayVehicleDoorOpenSound(m_Vars.m_VehicleHandle, 1);
				} else {
					Native::SetVehicleDoorShut(m_Vars.m_VehicleHandle, 6, true);
					Native::PlayVehicleDoorCloseSound(m_Vars.m_VehicleHandle, 1);
				}
			}

			auto& vehicleDoorsBroken = vehicleProperties.child("DoorsBroken");

			if (vehicleDoorsBroken) {
				if (vehicleDoorsBroken.child("BackLeftDoor").text().as_bool()) {
					Native::SetVehicleDoorBroken(m_Vars.m_VehicleHandle, 2, true);
				}

				if (vehicleDoorsBroken.child("BackRightDoor").text().as_bool()) {
					Native::SetVehicleDoorBroken(m_Vars.m_VehicleHandle, 3, true);
				}

				if (vehicleDoorsBroken.child("FrontLeftDoor").text().as_bool()) {
					Native::SetVehicleDoorBroken(m_Vars.m_VehicleHandle, 0, true);
				}

				if (vehicleDoorsBroken.child("FrontRightDoor").text().as_bool()) {
					Native::SetVehicleDoorBroken(m_Vars.m_VehicleHandle, 1, true);
				}

				if (vehicleDoorsBroken.child("Hood").text().as_bool()) {
					Native::SetVehicleDoorBroken(m_Vars.m_VehicleHandle, 4, true);
				}

				if (vehicleDoorsBroken.child("Trunk").text().as_bool()) {
					Native::SetVehicleDoorBroken(m_Vars.m_VehicleHandle, 5, true);
				}

				if (vehicleDoorsBroken.child("Trunk2").text().as_bool()) {
					Native::SetVehicleDoorBroken(m_Vars.m_VehicleHandle, 6, true);
				}
			}

			auto& vehicleTyresBursted = vehicleProperties.child("TyresBursted");

			if (vehicleTyresBursted) {
				if (vehicleTyresBursted.child("FrontLeft").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 0, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("FrontRight").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 1, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("_2").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 2, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("_3").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 3, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("BackLeft").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 4, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("BackRight").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 5, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("_6").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 6, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("_7").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 7, 1, 1000.f);
				}

				if (vehicleTyresBursted.child("_8").text().as_bool()) {
					if (!Native::GetVehicleTyresCanBurst(m_Vars.m_VehicleHandle)) Native::SetVehicleTyresCanBurst(m_Vars.m_VehicleHandle, true);
					Native::SetVehicleTyreBurst(m_Vars.m_VehicleHandle, 8, 1, 1000.f);
				}
			}

			auto& vehicleRpmMultiplier = vehicleProperties.child("RpmMultiplier");
			auto& vehicleTorqueMultiplier = vehicleProperties.child("TorqueMultiplier");
			auto& vehicleMaxSpeed = vehicleProperties.child("MaxSpeed");
			auto& vehicleHeadlightIntensity = vehicleProperties.child("HeadlightIntensity");

			if (vehicleRpmMultiplier) {
				Native::_SetVehicleEnginePowerMultiplier(m_Vars.m_VehicleHandle, vehicleRpmMultiplier.text().as_float());
			}

			if (vehicleTorqueMultiplier) {
				uint64_t Address = Rage::Engine::GetEntityAddress(m_Vars.m_VehicleHandle);
				if (Address) {
					*(float*)(Address + 0x1360) = vehicleTorqueMultiplier.text().as_float();
				}
			}

			if (vehicleMaxSpeed) {
				Native::SetEntityMaxSpeed(m_Vars.m_VehicleHandle, vehicleMaxSpeed.text().as_float());
			}

			if (vehicleHeadlightIntensity) {
				Native::SetVehicleLightMultiplier(m_Vars.m_VehicleHandle, vehicleHeadlightIntensity.text().as_float());
			}
		} else {
			Menu::GetNotify()->NotifyStacked("File Formatting Error");
			return;
		}

#pragma endregion

#pragma region Attachments

		Utils::WaitFiber(100);

		auto& attachments = root.child("SpoonerAttachments");
		if (attachments) {
			std::vector<uint32_t> usedModels;
			std::vector<SpoonerAttachments> attachmentData;

			for (auto& nodeAttachment = attachments.first_child(); nodeAttachment; nodeAttachment = nodeAttachment.next_sibling()) {
				Menu::GetControlManager()->SimpleRequestModel(nodeAttachment.child("ModelHash").text().as_uint());
				if (nodeAttachment.child("Type").text().as_int() == 1) { // ped
					if (nodeAttachment.child("PedProperties").child("AnimActive").text().as_bool()) {
						Native::RequestAnimDict((char*)nodeAttachment.child("PedProperties").child("AnimDict").text().as_string());
					}
				}

				usedModels.push_back(nodeAttachment.child("ModelHash").text().as_uint());
			}

			for (auto& nodeAttachment = attachments.first_child(); nodeAttachment; nodeAttachment = nodeAttachment.next_sibling()) {
				Utils::WaitFiber(10);
				SpoonerAttachments e = SpawnEntityFromXmlNode(nodeAttachment);
				attachmentData.push_back(e);
			}

			if (!m_Vars.m_InitialHandle) {
				for (auto& e : attachmentData) {
					Native::AttachEntityToEntity(e.Handle, m_Vars.m_VehicleHandle, e.Attachment.BoneIndex, e.Attachment.Position.m_X, e.Attachment.Position.m_Y, e.Attachment.Position.m_Z, e.Attachment.Rotation.m_X, e.Attachment.Rotation.m_Y, e.Attachment.Rotation.m_Z, 1, 0, m_Vars.m_UseCollision, 1, 2, 1);
				}
			} else {
				SpoonerAttachments temp;
				temp.Handle = m_Vars.m_VehicleHandle;
				temp.InitialHandle = m_Vars.m_InitialHandle;
				attachmentData.push_back(temp);

				for (auto& e : attachmentData) {
					if (e.Attachment.IsAttached) {
						for (auto& b : attachmentData) {
							if (e.Handle != b.Handle && e.Attachment.AttachedTo == b.InitialHandle) {
								Native::AttachEntityToEntity(e.Handle, b.Handle, e.Attachment.BoneIndex, e.Attachment.Position.m_X, e.Attachment.Position.m_Y, e.Attachment.Position.m_Z, e.Attachment.Rotation.m_X, e.Attachment.Rotation.m_Y, e.Attachment.Rotation.m_Z, 1, 0, m_Vars.m_UseCollision, 1, 2, 1);
								break;
							}
						}
					}
				}
			}

			for (auto& e : usedModels) {
				Native::SetModelAsNoLongerNeeded(e);
			}

			for (auto& e : attachmentData) {
				Native::SetEntityAsNoLongerNeeded(&e.Handle);
			}
		}

#pragma endregion

		Menu::GetNotify()->NotifyStacked("Vehicle successfully loaded");
	}
}

void VehicleSpawnerMenyooMenu::Init() {
	SetName("Menyoo Vehicles");
	SetParentSubmenu<VehicleSpawnerMenu>();

	addOption(ToggleOption("Delete Current")
		.addTranslation()
		.addToggle(m_Vars.m_DeleteCurrent));

	addOption(ToggleOption("Use Collision")
		.addTranslation()
		.addToggle(m_Vars.m_UseCollision));

	addOption(BreakOption("Vehicles")
		.addTranslation());

	addString("~m~None");
}

void VehicleSpawnerMenyooMenu::Update() {}

/*Called once on submenu open*/
void VehicleSpawnerMenyooMenu::UpdateOnce() {
	ClearOptionsOffset(3);

	auto Files = Utils::GetConfig()->GetFilesInDirectory(Utils::GetConfig()->GetMenyooPath(), ".xml");
	if (Files.size()) {
		for (auto& Item : Files) {
			addOption(ButtonOption(Item)
				.addOnClick([=] { LoadMenyoo(Item.c_str()); }));
		}
	} else {
		addOption(ButtonOption(getString("~m~None")));
	}
}

/*Called always*/
void VehicleSpawnerMenyooMenu::FeatureUpdate() {}

VehicleSpawnerMenyooMenu* _instance;
VehicleSpawnerMenyooMenu* VehicleSpawnerMenyooMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VehicleSpawnerMenyooMenu();
		_instance->Init();
		Menu::GetSubmenuHandler()->AddSubmenu(_instance);
	}

	return _instance;
}

VehicleSpawnerMenyooMenu::~VehicleSpawnerMenyooMenu() { delete _instance; }