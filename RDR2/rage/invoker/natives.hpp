#pragma once
#include "native_invoker.hpp"
#include "rage/types/base_types.hpp"
#define _i rage::invoker::invoke

namespace native {
	// player
	static BOOL getPlayerInvincible(Player player) { return _i<BOOL>(0xCBBCB2CCFA7DC4E, player); } // 0xCBBCB2CCFA7DC4E
	static Ped getPlayerPed(int index) { return _i<Ped>(0x275F255ED201B937, index); } // 0x275F255ED201B937
	static Ped getPlayerPedScriptIndex(int index) { return _i<Ped>(0x5C880F9056D784C8, index); } // 0x5C880F9056D784C8
	static const char* getPlayerName(int index) { return _i<const char*>(0x7124FD9AC0E01BA0, index); } // 0x7124FD9AC0E01BA0
	static int playerId() { return _i<int>(0x217E9DC48139933D); } // 0x217E9DC48139933D
	static int playerPedId() { return _i<int>(0x096275889B8E0EE0); } // 0x096275889B8E0EE0
	static void setPlayerInvincible(int playerId, BOOL toggle) { _i<int>(0xFEBEEBC9CBDF4B12, playerId, toggle); } // 0xFEBEEBC9CBDF4B12
	static void setPlayerModel(Any p0, Any p1, Any p2) { _i<int>(0xED40380076A31506, p0, p1, p2); } // 0xED40380076A31506
	static BOOL isPlayerDead(Player index) { return _i<BOOL>(0x2E9C3FCB6798F397, index); } // 0x2E9C3FCB6798F397
	static BOOL canPlayerStartMission(Player index) { return _i<BOOL>(0x2DF170B1185AF777, index); } // 0x2DF170B1185AF777
	static BOOL isPlayerReadyForCutscene(Player index) { return _i<BOOL>(0xAA67BCB0097F2FA3, index); } // 0xAA67BCB0097F2FA3
	static BOOL isPlayerTargettingEntity(Player player, Entity entity, Any p2) { return _i<BOOL>(0x27F89FDC16688A7A, player, entity, p2); } // 0x27F89FDC16688A7A
	static BOOL getPlayerTargetEntity(Player player, Entity* entity) { return _i<BOOL>(0xAE663DDD99C8A670, player, entity); } // 0xAE663DDD99C8A670
	static BOOL isPlayerFreeAiming(Player index) { return _i<BOOL>(0x936F967D4BE1CE9D, index); } // 0x936F967D4BE1CE9D
	static BOOL	isPlayerFreeAimingAtEntity(Player index, Entity entity) { return _i<BOOL>(0x8C67C11C68713D25, index, entity); } // 0x8C67C11C68713D25
	static BOOL	getEntityPlayerIsfreeAimingAt(Player index, Entity* entity) { return _i<BOOL>(0xA6817C110B830EAD, index, entity); } // 0xA6817C110B830EAD
	static int getMaxWantedLevel() { return _i<int>(0xD04CFAD1E2B7984A); } // 0xD04CFAD1E2B7984A
	static BOOL	isPlayerTargettingAnything(Player index) { return _i<BOOL>(0x4605C66E0F935F83, index); } // 0x4605C66E0F935F83
	static void restorePlayerStamina(Player index, float p1) { _i<int>(0xC41F4B6E23FE6A4A, index, p1); } // 0xC41F4B6E23FE6A4A
	static int getPlayerGroup(Player index) { return _i<int>(0x9BAB31815159ABCF, index); } // 0x9BAB31815159ABCF
	static void setPlayerWeaponDamageMultiplier(Player player, float damage) { _i<int>(0x94D529F7B73D7A85, player, damage); } // 0x94D529F7B73D7A85
	static void setPlayerWeaponDefenseModifier(Player player, float defense) { _i<int>(0xD15CC2D493160BE3, player, defense); } // 0xD15CC2D493160BE3
	static void setPlayerMeleeDamageMultiplier(Player player, float damage) { _i<int>(0xE4CB5A3F18170381, player, damage); } // 0xE4CB5A3F18170381
	static void restoreSpecialAbility(Player index, Any p1, Any p2) { _i<int>(0x1D77B47AFA584E90, index, p1, p2); } // 0x1D77B47AFA584E90
	static void setPlayerInfiniteDeadEyeEagleEye(Player index, BOOL toggle) { _i<int>(0x28A13BF6B05C3D83, index, toggle); }
	static void setMaxWantedLevel(int max) { _i<int>(0x28A4BD2CEE236E19, max); } // 0x28A4BD2CEE236E19
	static void setSwimMultiplierForPlayer(Player player, float multiplier) { _i<int>(0xBFCEABDE34DA5085, player, multiplier); } // 0xBFCEABDE34DA5085
	static void setEveryoneIgnorePlayer(Player player, BOOL toggle) { _i<int>(0x34630A768925B852, player, toggle); } // 0x34630A768925B852
	static void setWantedIntensityForPlayer(Player player, int intensity) { _i<int>(0xA80FF73F772ACF6A, player, intensity); } // 0xA80FF73F772ACF6A
	static void setWantedLevelMultiplier(float multiplier) { _i<int>(0xD7FA719CB54866C2, multiplier); } // 0xD7FA719CB54866C2
	static void setBountyForPlayer(Player player, int amount) { _i<int>(0x093A9D1F72DF0D19, player, amount); } // 0x093A9D1F72DF0D19
	static void endPursuit() { _i<int>(0x55F37F5F3F2475E1); } // 0x55F37F5F3F2475E1
	static void setPlayerForcedAim(Player player, BOOL toggle, Ped ped, int p3, BOOL p4) { _i<int>(0xD5FCC166AEB2FD0F, player, toggle, ped, p3, p4); } // 0xD5FCC166AEB2FD0F
	static void setWeaponDegrationMultiplier(Player player, float multiplier) { _i<int>(0x11A7FF918EF6BC66, player, multiplier); }
	static BOOL isPlayerPlaying(Player player) { return _i<BOOL>(0xBFFB35986CAAE58C, player); }
	static void setPlayerCanBeHarassedByGangs(Player player, BOOL toggle) { _i<int>(0xC7FE774412046825, player, toggle); }
	static void setEveryoneIgnorsPlayer(Player player, BOOL toggle) { _i<int>(0x34630A768925B852, player, toggle); }

	// ped
	static void deletePed(Ped* ped) { _i<int>(0xCC0EF140F99365C5, ped); } // 0xCC0EF140F99365C5
	static void setPedInfiniteAmmoClip(Ped ped, BOOL toggle) { _i<int>(0xFBAA1E06B6BCA741, ped, toggle); }
	static void setPedInfiniteAmmo(Ped ped, BOOL toggle, Hash hash) { _i<int>(0x3EDCB0505123623B, ped, toggle, hash); }
	static BOOL isPedShooting(Ped ped) { return _i<BOOL>(0x34616828CD07F1A1, ped); } // 0x34616828CD07F1A1
	static Ped clonePed(Ped ped, float heading, BOOL isNetwork, BOOL thisScriptCheck) { return _i<Ped>(0xEF29A16337FACADB, ped, heading, isNetwork, thisScriptCheck); } // 0xEF29A16337FACADB
	static void clonePedToTarget(Ped ped, Ped targetPed) { _i<int>(0xE952D6431689AD9A, ped, targetPed); } // 0xE952D6431689AD9A
	static BOOL isPedAPlayer(Ped ped) { return _i<BOOL>(0x12534C348C6CB68B, ped); } // 0x12534C348C6CB68B
	static math::vector3<float> getPedBoneCoords(Ped ped, int boneId, float x, float y, float z) { return _i<math::vector3<float>>(0x17C07FC640E86B4E, ped, boneId, x, y, z); } // 0x17C07FC640E86B4E
	static int getPedBoneIndex(Ped ped, int boneId) { return _i<int>(0x3F428D08BE5AAE31, ped, boneId); } // 0x3F428D08BE5AAE31
	static void setPedCoreLevel(Ped ped, int coreType, int val) { _i<int>(0xC6258F41D86676E0, ped, coreType, val); } // 0xC6258F41D86676E0
	static void setPedMaxHealth(Ped ped, int val) { _i<int>(0xF5F6378C4F3419D3, ped, val); } // 0xF5F6378C4F3419D3
	static void setPedShootsAtCoord(Ped ped, float x, float y, float z, Any p4, Any p5, Any p6, float p7) { _i<int>(0x4C57F27D1554E6B0, ped, x, y, z, p4, p5, p6, p7); } // 0x4C57F27D1554E6B0
	static BOOL isPedOnMount(Ped ped) { return _i<BOOL>(0x460BC76A0E10655E, ped); } // 0x460BC76A0E10655E
	static Ped getMount(Ped ped) { return _i<Ped>(0xE7E11B8DCBED1058, ped); } // 0xE7E11B8DCBED1058
	static void restorePedStamina(Ped ped, float p1) { _i<int>(0x675680D089BFA21F, ped, p1); } // 0x675680D089BFA21F
	static void setPedCanRagdoll(Ped ped, BOOL toggle) { _i<int>(0xB128377056A54E2A, ped, toggle); } // 0xB128377056A54E2A
	static void setPedCanRagdollFromPlayerImpact(Ped ped, BOOL toggle) { _i<int>(0xDF993EE5E90ABA25, ped, toggle); } // 0xDF993EE5E90ABA25
	static void setPedCanRagdollOnCollision(Ped ped, BOOL toggle) { _i<int>(0xF0A4F1BBF4FA7497, ped, toggle); } // 0xF0A4F1BBF4FA7497
	static void setPedCanBeKnockedOffVehicle(Ped ped, int state) { _i<int>(0x7A6535691B477C48, ped, state); } // 0x7A6535691B477C48
	static void explodePedHead(Ped ped, Hash weaponHash) { _i<int>(0x2D05CED3A38D0F3A, ped, weaponHash); } // 0x2D05CED3A38D0F3A
	static BOOL getCurrentPedWeapon(Ped ped, Hash* weaponHash, BOOL p2, int p3, BOOL p4) { return _i<BOOL>(0x3A87E44BB9A01D54, ped, weaponHash, p2, p3, p4); } // 0x3A87E44BB9A01D54
	static BOOL isPedInAnyVehicle(Ped ped, BOOL atGetIn) { return _i<BOOL>(0x997ABD671D25CA0B, ped, atGetIn); } // 0x997ABD671D25CA0B
	static void clearPedBloodDamage(Ped ped) { _i<int>(0x8FE22675A5A45817, ped); } // 0x8FE22675A5A45817
	static void clearPedDamageDecalByZone(Ped ped, int p1, char* p2) { _i<int>(0x523C79AEEFCC4A2A, ped, p1, p2); }
	static void clearPedDirt(Ped ped) { _i<int>(0x6585D955A68452A5, ped); }
	static void clearPedWetness(Ped ped) { _i<int>(0x9C720776DAA43E7E, ped); }
	static void applyDamageToPed(Ped ped, int damage, bool unk, int unk2, int unk3) { _i<int>(0x697157CED63F18D4, ped, damage, unk, unk2, unk3); } // 0x697157CED63F18D4
	static void setPedConfigFlag(Ped ped, int flagId, BOOL value) { _i<int>(0x1913FE4CBF41C463, ped, flagId, value); } // 0x1913FE4CBF41C463
	static void setPedKeepsTask(Ped ped, BOOL toggle) { _i<int>(0x971D38760FBC02EF, ped, toggle); }
	static void setPedScale(Ped ped, float scale) { _i<int>(0x25ACFC650B65C538, ped, scale); } // 0x25ACFC650B65C538
	static Vehicle getVehiclePedIsIn(Ped ped, BOOL lastVehicle) { return _i<Vehicle>(0x9A9112A0FE9A4713, ped, lastVehicle); } // 0x9A9112A0FE9A4713
	static void clearPedDecorations(Ped ped) { _i<int>(0x0E5173C163976E38, ped); } // 0x0E5173C163976E38
	static void setPedDefaultOutfit(Ped ped, BOOL unk) { _i<int>(0x283978A15512B2FE, ped, unk); } // 0x283978A15512B2FE
	static BOOL isPedDeadOrDying(Ped ped, BOOL p1) { return _i<BOOL>(0x3317DEDB88C95038, ped, p1); } // 0x3317DEDB88C95038
	static void setPedRagdollForceFall(Ped ped) { _i<int>(0x01F6594B923B9251, ped); } // 0x01F6594B923B9251
	static void reviveInjuredPed(Ped ped) { _i<int>(0x8D8ACD8388CD99CE, ped); }  //0x8D8ACD8388CD99CE
	static void resurrectPed(Ped ped) { _i<int>(0x71BC8E838B9C6035, ped); }//0x71BC8E838B9C6035 
	static void clearPedTasksImmediately(Ped ped, BOOL p1, BOOL p2) { _i<int>(0xAAA34F8A7CB32098, ped, p1, p2); } // 0xAAA34F8A7CB32098
	static int getPedNearbyPeds(Ped ped, int* sizeAndRet, int ignore = -1, int unk = 0) { return _i<int>(0x23F8F5FC7E8C4A6B, ped, sizeAndRet, ignore, unk); } // 0x23F8F5FC7E8C4A6B
	static BOOL setPedToRagdoll(Ped ped, int time1, int time2, int ragdollType, BOOL p4, BOOL p5, BOOL p6) { return _i<BOOL>(0xAE99FB955581844A, ped, time1, time2, ragdollType, p4, p5, p6); } // 0xAE99FB955581844A
	static void setPedOnMount(Ped ped, Ped mount, int seatIndex, BOOL p3) { _i<int>(0x028F76B6E78246EB, ped, mount, seatIndex, p3); } // 0x028F76B6E78246EB
	static Ped getLastPedMount(Ped ped) { return _i<Ped>(0x4C8B59171957BCF7, ped); } // 0x4C8B59171957BCF7
	static Ped createPed(Hash modelHash, math::vector3<float> coords, float heading, BOOL p5, BOOL p6, BOOL p7, BOOL p8) { return _i<Ped>(0xD49F9B0955C367DE, modelHash, coords.x, coords.y, coords.z, heading, p5, p6, p7, p8); } // 0xD49F9B0955C367DE
	static void setPedAsGroupMember(Ped ped, int groupId) { _i<int>(0x9F3480FE65DB31B5, ped, groupId); } // 0x9F3480FE65DB31B5
	static void setPedCombatAbility(Ped ped, int ab) { _i<int>(0xC7622C0D36B2FDA8, ped, ab); } // 0xC7622C0D36B2FDA8
	static void setPedCombatRange(Ped ped, int range) { _i<int>(0x3C606747B23E497B, ped, range); } // 0xC7622C0D36B2FDA8
	static void setPedIntoVehicle(Ped ped, Vehicle vehicle, int seat) { _i<int>(0xF75B0D629E1C063D, ped, vehicle, seat); } // 0xF75B0D629E1C063D
	static BOOL isPedReloading(Ped ped) { return _i<BOOL>(0x24B100C68C645951, ped); } // 0x24B100C68C645951
	static void setPedFaceFeature(Ped ped, int index, float value) { _i<int>(0x5653AB26C82938CF, ped, index, value); } // 0x5653AB26C82938CF
	static float getPedFaceFeature(Ped ped, int index) { return _i<float>(0xFD1BA1EEF7985BB8, ped, index); } // 0xFD1BA1EEF7985BB8
	static void setPedComponentEnabled(Ped ped, Hash componentHash, BOOL immediately, BOOL p3, BOOL p4) { _i<int>(0xD3A7B003ED343FD9, ped, componentHash, immediately, p3, p4); } // 0xD3A7B003ED343FD9
	static uint32_t getComponentAtIndex(Ped ped, int index, BOOL p2, uint64_t* p3, uint64_t* p4) { return _i<uint32_t>(0x77BA37622E22023B, ped, index, p2, p3, p4); }
	static void setPedComponentVariation(Ped ped, Hash componentHash, Hash variationHash, BOOL updatePed, BOOL isMpComponent, Any p5) { _i<int>(0x66B957AAC2EAAEAB, ped, componentHash, variationHash, updatePed, isMpComponent, p5); } // 0x66B957AAC2EAAEAB
	static void clearPedComponentCategory(Ped ped, Hash category) { _i<int>(0xD710A5007C2AC539, ped, category, 0); } // 0xD710A5007C2AC539
	static void setPedCapsule(Ped ped, float value) { _i<int>(0x364DF566EC833DE2, ped, value); } // 0x364DF566EC833DE2
	static void setPedGravity(Ped ped, BOOL toggle) { _i<int>(0x9FF447B6B6AD960A, ped, toggle); } // 0x9FF447B6B6AD960A
	static void setPedRandomComponentVariation(Ped ped, int unk) { _i<int>(0xC8A9481A01E63C28, ped, unk); } // 0xC8A9481A01E63C28
	static Entity getPedSourceOfDeath(Ped ped) { return _i<Entity>(0x93C8B64DEB84728C, ped); } // 0x93C8B64DEB84728C
	static BOOL isPedMale(Ped ped) { return _i<BOOL>(0x6D9F5FAA7488BA46, ped); }

	// object
	static Object createObject(Hash modelHash, math::vector3<float> coords, BOOL isNetwork, BOOL thisScriptCheck, BOOL dynamic, BOOL p7, BOOL p8) { return _i<Object>(0x509D5878EB39E842, modelHash, coords.x, coords.y, coords.z, isNetwork, thisScriptCheck, dynamic, p7, p8); } // 0x509D5878EB39E842
	static Pickup createPickup(Hash pickupHash, float x, float y, float z, int flags, int p5, BOOL p6, Hash modelHash, int p8, float p9, Any p10) { return _i<Pickup>(0xFBA08C503DD5FA58, pickupHash, x, y, z, flags, p5, p6, modelHash, p8, p9, p10); } // 0xFBA08C503DD5FA58
	static Pickup createAmbientPickup2(Hash pickupHash, float x, float y, float z, int flags, int value, Hash modelHash, BOOL p7, BOOL p8) { return _i<Object>(0x673966A0C0FD7171, pickupHash, x, y, z, flags, value, modelHash, p7, p8); } // 0x673966A0C0FD7171
	static Pickup createAmbientPickup(Hash pickupHash, math::vector3<float> coords, int flags, int value, Hash modelHash, BOOL p7, BOOL p8, int p9, float p10) { return _i<Object>(0x673966A0C0FD7171, pickupHash, coords.x, coords.y, coords.z, flags, value, modelHash, p7, p8, p9, p10); }
	static Pickup createPickupRotate(Hash hash, math::vector3<float> coords, math::vector3<float> rot, int bitset, int unk1, int unk2, BOOL unk3, Hash objectHash, int unk4, float unk5, int unk6) { return _i<Pickup>(0x891804727E0A98B7, hash, coords.x, coords.y, coords.z, rot.x, rot.y, rot.z, bitset, unk1, unk2, unk3, objectHash, unk4, unk5, unk6); }
	static BOOL doesPickupExist(Pickup pickup) { return _i<BOOL>(0xAFC1CA75AD4074D1, pickup); } // 0xAFC1CA75AD4074D1
	static Object getPickupObject(Pickup pickup) { return _i<Object>(0x5099BC55630B25AE, pickup); }

	// entity
	static math::vector3<float> getOffsetFromEntityGivenWorldCoords(Entity entity, math::vector3<float> offset) { return _i<math::vector3<float>>(0x497C6B1A2C9AE69C, entity, offset.x, offset.y, offset.z); } // 0x497C6B1A2C9AE69C
	static math::vector3<float> getOffsetFromEntityInWorldCoords(Entity entity, math::vector3<float> offset) { return _i<math::vector3<float>>(0x1899F328B0E12848, entity, offset.x, offset.y, offset.z); } // 0x1899F328B0E12848
	static void setEntityCoords(Entity entity, math::vector3<float> pos, BOOL xAxis = FALSE, BOOL yAxis = FALSE, BOOL zAxis = FALSE, BOOL clearArea = FALSE) { _i<int>(0x06843DA7060A026B, entity, pos.x, pos.y, pos.z, xAxis, yAxis, zAxis, clearArea); } // 0x06843DA7060A026B
	static void setEntityCoordsNoOffset(Entity entity, float xPos, float yPos, float zPos, BOOL xAxis, BOOL yAxis, BOOL zAxis) { _i<int>(0x239A3351AC1DA385, entity, xPos, yPos, zPos, xAxis, yAxis, zAxis); } // 0x239A3351AC1DA385
	static math::vector3<float> getEntityCoords(Entity entity) { return _i<math::vector3<float>>(0xA86D5F069399F44D, entity, 1, 0, 0); } // 0xA86D5F069399F44D
	static math::vector3<float> getEntityRotation(Entity entity, int order) { return _i<math::vector3<float>>(0xE09CAF86C32CB48F, entity, order); } // 0xE09CAF86C32CB48F
	static float getEntitySpeed(Entity entity) { return _i<float>(0xFB6BA510A533DF81, entity); } // 0xFB6BA510A533DF81
	static BOOL doesEntityExist(Entity entity) { return _i<BOOL>(0xD42BD6EB2E0F1677, entity); } // 0xD42BD6EB2E0F1677
	static Hash getEntityModel(Entity entity) { return _i<Hash>(0xDA76A9F39210D365, entity); } // 0xDA76A9F39210D365
	static math::vector3<float> getWorldPositionOfEntityBone(Entity entity, int boneIndex) { return _i<math::vector3<float>>(0x82CFA50E34681CA5, entity, boneIndex); } // 0x82CFA50E34681CA5
	static BOOL hasEntityClearLosToEntity(Entity entity1, Entity entity2, int traceType) { return _i<BOOL>(0xFCDFF7B72D23A1AC, entity1, entity1, traceType); } // 0xFCDFF7B72D23A1AC
	static BOOL hasEntityClearLosToEntityInFront(Entity entity1, Entity entity2, Any p2) { return _i<BOOL>(0xE88F19660651D566, entity1, entity1, p2); } // 0xE88F19660651D566
	static void setEntityVelocity(Entity entity, math::vector3<float> velocity) { _i<int>(0x1C99BB7B6E96D16F, entity, velocity.x, velocity.y, velocity.z); } // 0x1C99BB7B6E96D16F
	static math::vector3<float> getEntityVelocity(Entity entity, Any unk) { return _i<math::vector3<float>>(0x4805D2B1D8CF94A9, entity, unk); } // 0x4805D2B1D8CF94A9
	static void applyForceToEntityCenterOfMass(Entity entity, int forceType, float x, float y, float z, BOOL p5, BOOL isDirectionRel, BOOL isForceRel, BOOL p8) { _i<int>(0x31DA7CEC5334DB37, entity, forceType, x, y, z, p5, isDirectionRel, isForceRel, p8); } // 0x31DA7CEC5334DB37
	static void applyForceToEntity(Entity entity, int forceType, float x, float y, float z, float xRot, float yRot, float zRot, int boneIndex, BOOL isRel, BOOL p10, BOOL highForce, BOOL p12, BOOL p13) { _i<int>(0xF15E8F5D333F09C4, entity, forceType, x, y, z, xRot, yRot, zRot, boneIndex, isRel, p10, highForce, p12, p13); } // 0xF15E8F5D333F09C4
	static void deleteEntity(Entity* entity) { _i<int>(0x4CD38C78BD19A497, entity); } // 0x4CD38C78BD19A497
	static void deleteEntity2(Entity* entity) { _i<int>(0x5E94EA09E7207C16, entity); } // 0x5E94EA09E7207C16
	static void deleteVehicle(Entity* entity) { _i<int>(0xE20A909D8C4A70F8, entity); } // 0xE20A909D8C4A70F8
	static void	detachEntity(Entity entity, BOOL p1, BOOL collision) { _i<int>(0x64CDE9D6BF8ECAD3, entity, p1, collision); } // 0x64CDE9D6BF8ECAD3
	static void freezeEntityPosition(Entity entity, BOOL toggle) { _i<int>(0x7D9EFB7AD6B19754, entity, toggle); } // 0x7D9EFB7AD6B19754
	static void setEntityAsMissionEntity(Entity entity, BOOL p1, BOOL p2) { _i<int>(0xDC19C288082E586E, entity, p1, p2); } // 0xDC19C288082E586E
	static void setEntityAsNoLongerNeeded(Entity* entity) { _i<int>(0x4971D2F8162B9674, entity); } // 0x4971D2F8162B9674
	static void setEntityInvincible(Entity entity, BOOL toggle) { _i<int>(0xA5C38736C426FCB8, entity, toggle); } // 0xA5C38736C426FCB8
	static BOOL isEntityAPed(Entity entity) { return _i<BOOL>(0xCF8176912DDA4EA5, entity); } // 0xCF8176912DDA4EA5
	static int getEntityHealth(Entity entity) { return _i<int>(0x82368787EA73C0F7, entity); } // 0x82368787EA73C0F7
	static int getEntityMaxHealth(Entity entity, BOOL p1) { return _i<int>(0x15D757606D170C3C, entity, p1); } // 0x15D757606D170C3C
	static void	setEntityHealth(Entity entity, int health, Any p2) { _i<int>(0xAC2767ED8BDFAB15, entity, health, p2); } // 0xAC2767ED8BDFAB15
	static int getEntityType(Entity entity) { return _i<int>(0x97F696ACA466B4E0, entity); } // 0x97F696ACA466B4E0
	static void setEntityVisible(Entity entity, BOOL toggle) { _i<int>(0x1794B4FCC84D812F, entity, toggle); } // 0x1794B4FCC84D812F
	static float getEntityHeading(Entity entity) { return _i<float>(0xC230DD956E2F5507, entity); } // 0xC230DD956E2F5507
	static BOOL isEntityDead(Entity entity) { return _i<BOOL>(0x7D5B1F88E7504BBA, entity); } // 0x7D5B1F88E7504BBA
	static BOOL isEntityVisible(Entity entity) { return _i<BOOL>(0xFFC96ECB7FA404CA, entity); } // 0xFFC96ECB7FA404CA
	static BOOL isEntityOnScreen(Entity entity) { return _i<BOOL>(0x613C15D5D8DB781F, entity); } // 0x613C15D5D8DB781F
	static Player getNearestPlayerToEntity(Any p0, Any p1, Any p2) { return _i <Player>(0x990E294FC387FB88, p0, p1, p2); } // 0x990E294FC387FB88
	static void setEntityAlpha(Entity entity, int alphaLevel) { _i<int>(0x0DF7692B1D9E7BA7, entity, alphaLevel, 0); } // 0x0DF7692B1D9E7BA7 
	static math::vector3<float> getEntityForwardVector(Entity entity) { return _i<math::vector3<float>>(0x2412D9C05BB09B97, entity); } // 0x2412D9C05BB09B97
	static BOOL isEntityAttached(Entity entity) { return _i<BOOL>(0xEE6AD63ABF59C0B7, entity); } // 0xEE6AD63ABF59C0B7
	static BOOL isEntityAttachedToEntity(Entity entity1, Entity entity2) { return _i<BOOL>(0x154A3C529497053E, entity1, entity2); } // 0x154A3C529497053E
	static void attachEntityToEntity(Entity entity1, Entity entity2, int boneIndex, math::vector3<float> position, math::vector3<float> rotation, BOOL p9, BOOL useSoftPinning, BOOL collision, BOOL isPed, int vertexIndex, BOOL fixedRot, BOOL p15, BOOL p16) { _i<int>(0x6B9BBD38AB0796DF, entity1, entity2, boneIndex, position.x, position.y, position.z, rotation.x, rotation.y, rotation.z, p9, useSoftPinning, collision, isPed, vertexIndex, fixedRot, p15, p16); } // 0x6B9BBD38AB0796DF
	static float getEntityHeightAboveGround(Entity entity) { return _i<float>(0x0D3B5BAEA08F63E9, entity); } // 0x0D3B5BAEA08F63E9
	static void setEntityCollision(Entity entity, BOOL toggle, BOOL keepPhysics) { _i<int>(0xF66F820909453B8C, entity, toggle, keepPhysics); } // 0xF66F820909453B8C
	static void setEntityRotation(Entity entity, math::vector3<float> rot, int rotationOrder, BOOL p5) { _i<int>(0x9CC8314DFEDE441E, entity, rot.x, rot.y, rot.z, rotationOrder, p5); } // 0x9CC8314DFEDE441E
	static void setEntityGravity(Entity entity, BOOL toggle) { _i<int>(0x0CEDB728A1083FA7, entity, toggle); } // 0x0CEDB728A1083FA7
	static void setEntityScorched(Entity entity, BOOL toggle) { _i<int>(0x85B8A7534E44BC23, entity, toggle); }
	static void setEntityLightingColor(Entity entity, int r, int g, int b) { _i<int>(0x6EC2A67962296F49, entity, r, g, b); }
	static BOOL isEntityPlayingAnim(Entity entity, const char* animDict, const char* animName, int taskFlag) { return _i<BOOL>(0xDEE49D5CA6C49148, entity, animDict, animName, taskFlag); }
	static int getEntityBoneIndexByName(Entity entity, const char* name) { return _i<int>(0xBACA8FE9C76C124E, entity, name); } // 0xBACA8FE9C76C124E
	static BOOL isEntityUpright(Entity entity, float angle) { return _i<BOOL>(0xF6F6AFD8D4FB2658, entity, angle); } // 0xF6F6AFD8D4FB2658
	
	// clock
	static void setClockTime(int hour, int minute, int second) { _i<int>(0x3A52C59FFB2DEED8, hour, minute, second); } // 0x3A52C59FFB2DEED8
	static void getLocaltime(int* year, int* month, int* day, int* hour, int* minute, int* second) { _i<int>(0x90338AD4A784E455, year, month, day, hour, minute, second); }  // 0x90338AD4A784E455

	// weapon
	static BOOL getPedLastWeaponImpactCoord(Ped ped, math::vector3<float>* coords) { return _i<BOOL>(0x6C4D0409BA1A2BC2, ped, coords); } // 0x6C4D0409BA1A2BC2
	static void giveDelayedWeaponToPed(Ped ped, Hash weaponHash, Any p2, BOOL p3, Any p4) { _i<int>(0xB282DC6EBD803C75, ped, weaponHash, p2, p3, p4); } // 0xB282DC6EBD803C75
	static void setCurrentPedWeapon(Ped ped, Hash weaponHash, BOOL equipNow, int p3, BOOL p4, BOOL p5) { _i<int>(0xADF692B254977C0C, ped, weaponHash, equipNow, p3, p4, p5); } // 0xADF692B254977C0C
	static BOOL getMaxAmmo(Ped ped, Any* ammo, Hash weaponHash) { return _i<BOOL>(0xDC16122C7A20C933, ped, ammo, weaponHash); } // 0xDC16122C7A20C933
	static int getMaxAmmoInClip(Ped ped, Hash weaponHash, BOOL p2) { return _i<int>(0xA38DCFFCEA8962FA, ped, weaponHash, p2); } // 0xA38DCFFCEA8962FA
	static BOOL setAmmoInClip(Ped ped, Hash weaponHash, int ammo) { return _i<BOOL>(0xDCD2A934D65CB497, ped, weaponHash, ammo); } // 0xDCD2A934D65CB497
	static Any giveWeaponToPed(int ped, Hash weapName, int ammoCount, BOOL equipNow, BOOL p4, int group, BOOL p6, float p7, float p8, BOOL leftHanded, BOOL p10, float p11, BOOL p12) { return _i<Any>(0x5E3BDDBCB83F3D84, ped, weapName, ammoCount, equipNow, p4, group, p6, p7, p8, leftHanded, p10, p11, p12); } // 0x5E3BDDBCB83F3D84
	static BOOL isWeaponValid(Hash weaponHash) { return _i<BOOL>(0x937C71165CF334B3, weaponHash); } // 0x937C71165CF334B3
	static Hash getWeaponTypeGroup(Hash weaponHash) { return _i<Hash>(0xEDCA14CA5199FF25, weaponHash); } // 0xEDCA14CA5199FF25
	static void setPedAmmo(Ped ped, Hash weaponHash, int ammo) { _i<int>(0x14E56BC5B5DB6A19, ped, weaponHash, ammo); } // 0x14E56BC5B5DB6A19
	static void removeAllPedWeapons(Ped ped, BOOL p1, BOOL p2) { _i<int>(0xF25DF915FA38C5F3, ped, p2, p2); } // 0xF25DF915FA38C5F3
	static Entity getCurrentPedWeaponEntityIndex(Ped ped, int type = 0) { return _i<Entity>(0x3B390A939AF0B5FC, ped, type); } // 0x3B390A939AF0B5FC
	static void shootSingleBulletBetweenCoords(math::vector3<float> start, math::vector3<float> direction, int damage, BOOL p7, Hash weaponHash, Ped ownerPed, BOOL isAudible, BOOL isInvisible, float speed, BOOL p13) { _i<int>(0x867654CBC7606F2C, start.x, start.y, start.z, direction.x, direction.y, direction.z, damage, p7, weaponHash, ownerPed, isAudible, isInvisible, speed, p13); } // 0x867654CBC7606F2C
	static void giveAmmoOfTypeToPed(Ped ped, Hash ammo, int count) { _i<int>(0x106A811C6D3035F3, ped, ammo, count, 752097756); } // 0x106A811C6D3035F3
	static void giveWeaponComponentToEntity(Entity entity, Hash hash, Hash weaponHash, BOOL p3) { _i<int>(0x74C9090FDD1BB48E, entity, hash, weaponHash, p3); } // 0x74C9090FDD1BB48E
	static const char* getWeaponName(Hash hash) { return _i<const char*>(0x89CF5FF3D363311E, hash); } // 0x89CF5FF3D363311E
	static int getAmmoInPedWeapon(Ped ped, Hash hash) { return _i<int>(0x015A522136D7F951, ped, hash); } // 0x015A522136D7F951
	static void setPlayerTargetingMode(int targetMode) { _i<int>(0xD66A941F401E7302, targetMode); } // 0xD66A941F401E7302

	// fire
	static void addOwnedExplosion(Ped ped, math::vector3<float> coords, int explosionType, float radius, BOOL isAudible, BOOL isInvisible, float cameraShake) { _i<int>(0xD84A917A64D4D016, ped, coords.x, coords.y, coords.z, explosionType, radius, isAudible, isInvisible, cameraShake); } // 0xD84A917A64D4D016
	static void addExplosion(math::vector3<float> coords, int explosionType, float radius, BOOL isAudible, BOOL isInvisible, float cameraShake) { _i<int>(0x7D6F58F69DA92530, coords.x, coords.y, coords.z, explosionType, radius, isAudible, isInvisible, cameraShake); } // 0x7D6F58F69DA92530
	static int startScriptFire(math::vector3<float> pos, int p3, float p4, BOOL p5, const char* soundsetName, float p7, int p8) { return _i<int>(0x6B83617E04503888, pos.x, pos.y, pos.z, p3, p4, p5, soundsetName, p7, p8); } // 0x6B83617E04503888
	static void removeScriptFire(int id) { _i<int>(0x790125C36E194069, id); } // 0x790125C36E194069

	// graphics
	static void drawMarker(uint64_t type, math::vector3<float> pos, math::vector3<float> dir, math::vector3<float> rot, math::vector3<float> scale, int r, int g, int b, int a, BOOL bobUpAndDown, BOOL faceCamera, int p19, BOOL rotate, const char* textureDict, const char* textureName, BOOL drawOnEnts) { _i<int>(0x2A32FAA57B937173, type, pos.x, pos.y, pos.z, dir.x, dir.y, dir.z, rot.x, rot.y, rot.z, scale.x, scale.y, scale.z, r, g, b, a, bobUpAndDown, faceCamera, p19, rotate, textureDict, textureName, drawOnEnts); }
	static BOOL getScreenCoordFromWorldCoord(math::vector3<float> coord, float* X, float* Y) { return _i<BOOL>(0xCB50D7AFCC8B0EC6, coord.x, coord.y, coord.z, X, Y); } // 0xCB50D7AFCC8B0EC6
	static void drawRect(float x, float y, float w, float h, int r, int g, int b, int a, BOOL p8 = FALSE, BOOL p9 = FALSE) { _i<int>(0x405224591DF02025, x, y, w, h, r, g, b, a, p8, p9); } // 0x405224591DF02025
	static BOOL hasStreamedTextureDictLoaded(const char* dict) { return _i<BOOL>(0x54D6900929CCF162, dict); } // 0x54D6900929CCF162
	static void requestStreamedTextureDict(const char* dict, BOOL p1 = FALSE) { _i<int>(0xC1BA29DF5631B0F8, dict, p1); } // 0xC1BA29DF5631B0F8
	static void drawSprite(const char* dict, const char* name, float x, float y, float w, float h, float heading, int r, int g, int b, int a, BOOL p11) { _i<int>(0xC9884ECADE94CB34, dict, name, x, y, w, h, heading, r, g, b, a, p11); } // 0xC9884ECADE94CB34
	static void setScriptGFxDrawBehindPauseMenu(BOOL toggle) { _i<int>(0x906B86E6D7896B9E, toggle); } // 0x906B86E6D7896B9E
	static int startParticleFXLoopedAtCoord(const char* effect, math::vector3<float> coord, math::vector3<float> rotation, float scale, BOOL xAxis = 0, BOOL yAxis = 0, BOOL zAxis = 0, BOOL p11 = 0) { return _i<int>(0xBA32867E86125D3A, effect, coord.x, coord.y, coord.z, rotation.x, rotation.y, rotation.z, scale, xAxis, yAxis, zAxis, p11); }; // 0xBA32867E86125D3A
	static int startParticleFXNonLoopedAtCoord(const char* effect, math::vector3<float> coord, math::vector3<float> rotation, float scale, BOOL xAxis = 0, BOOL yAxis = 0, BOOL zAxis = 0) { return _i<int>(0x2E80BF72EF7C87AC, effect, coord.x, coord.y, coord.z, rotation.x, rotation.y, rotation.z, scale, xAxis, yAxis, zAxis); }; // 0x2E80BF72EF7C87AC
	static int startNetworkedParticleFXNonLoopedAtCoord(const char* effect, math::vector3<float> coord, math::vector3<float> rotation, float scale, BOOL xAxis = 0, BOOL yAxis = 0, BOOL zAxis = 0) { return _i<int>(0xFB97618457994A62, effect, coord.x, coord.y, coord.z, rotation.x, rotation.y, rotation.z, scale, xAxis, yAxis, zAxis); }; // 0xFB97618457994A62
	static void stopParticleFxLooped(int handle, BOOL p1) { _i<int>(0x22970F3A088B133B, handle, p1); } // 0x22970F3A088B133B
	static BOOL startParticleFxNonLoopedOnEntity(const char* effectName, Entity entity, math::vector3<float> offset, math::vector3<float> rotation, float scale, BOOL axisX = 0, BOOL axisY = 0, BOOL axisZ = 0) { return _i<BOOL>(0xFF4C64C513388C12, effectName, entity, offset.x, offset.y, offset.z, rotation.x, rotation.y, rotation.z, scale, axisX, axisY, axisZ); } // 0xFF4C64C513388C12
	static int startNetworkedParticleFxLoopedOnEntity(const char* effectName, Entity entity, math::vector3<float> offset, math::vector3<float> rotation, float scale, BOOL xAxis = 0, BOOL yAxis = 0, BOOL zAxis = 0) { return _i<int>(0x8F90AB32E1944BDE, effectName, entity, offset.x, offset.y, offset.z, rotation.x, rotation.y, rotation.z, scale, xAxis, yAxis, zAxis); } // 0x8F90AB32E1944BDE
	static int startParticleFxLoopedOnEntityBone(const char* effectName, Entity entity, math::vector3<float> offset, math::vector3<float> rotation, int boneIndex, float scale, BOOL xAxis = 0, BOOL yAxis = 0, BOOL zAxis = 0) { return _i<int>(0xD3BA6EC7F2FBD5E9, effectName, entity, offset.x, offset.y, offset.z, rotation.x, rotation.y, rotation.z,boneIndex, scale, xAxis, yAxis, zAxis); }
	static void setSnowCoverageType(int type) { _i<int>(0xF02A9C330BBFC5C7, type); }  //0xF02A9C330BBFC5C7 
	static int createPrivateSession(int unk, int unk1, int unk2, char* sessionID) { return _i<int>(0x39a8ef7af29a192c, unk, unk1, unk2, sessionID); } // 0x39a8ef7af29a192c
	static int createSoloSession(int unk, int unk1, int unk2, char* sessionID) { return _i<int>(0x2989e131fde37e97, unk, unk1, unk2, sessionID); } // 0x2989e131fde37e97
	static int getTransitionStage() { return _i<int>(0xFAED234C7F53ABEB); }
	static int setTransitionStage(int stage) { return _i<int>(0x4CABE596D632E4B0, stage); }
	static void setParticleFxLoopedEvolution(int ptfxHandle, const char* propertyName, float amount, BOOL noNetwork) { _i<int>(0x3674F389B0FACD80, ptfxHandle, propertyName, amount, noNetwork); }
	static BOOL networkIsSessionPrivate() { return _i<BOOL>(0xCEF70AA5B3F89BA1); }

	// misc
	static Hash getHashKey(const char* str) { return _i<Hash>(0xFD340785ADF8CFB7, str); } // 0xFD340785ADF8CFB7
	static const char* createVarString(int p0, const char* type, const char* str) { return _i<const char*>(0xFA925AC00EB830B9, p0, type, str); } // 0xFA925AC00EB830B9
	static void setSuperJumpThisFrame(Player index) { _i<int>(0xB3E9BE963F10C445, index); } // 0xB3E9BE963F10C445
	static void displayOnscreenKeyboard(BOOL p0, const char* title, const char* p2, const char* defaultText, const char* defaultConcat1, const char* defaultConcat2, const char* defaultConcat3, int maxLen) { _i<int>(0x044131118D8DB3CD, p0, title, p2, defaultText, defaultConcat1, defaultConcat2, defaultConcat3, maxLen); }; // 0x044131118D8DB3CD
	static int updateOnscreenKeyboard() { return _i<int>(0x37DF360F235A3893); } // 0x37DF360F235A3893
	static const char* getOnscreenKeyboardResult() { return _i<const char*>(0xAFB4CF58A4A292B1); } // 0xAFB4CF58A4A292B1
	static void setTimeScale(float timeScale) { _i<int>(0x9682AF6050854856, timeScale); } // 0x9682AF6050854856
	static int getGameTimer() { return _i<int>(0x4F67E8ECA7D3F667); } // 0x4F67E8ECA7D3F667
	static BOOL getGroundZFor3dCoord(float x, float y, float z, float* groundZ, BOOL p4) { return _i<BOOL>(0x24FA4267BB8D2431, x, y, z, groundZ, p4); }
	static void setWeatherType(Hash weatherHash, BOOL p1, BOOL p2, BOOL overrideNetwork, float transitionTime, BOOL p5) { _i<int>(0x59174F1AFE095B5A, weatherHash, p1, p2, overrideNetwork, transitionTime, p5); } // 0x59174F1AFE095B5A
	static int getRandomIntInRange(int startRange, int endRange) { return _i<int>(0xD53343AA4FB7DD28, startRange, endRange); }
	static float getRandomFloatInRange(float startRange, float endRange) { return _i<float>(0xE29F927A961F8AAA, startRange, endRange); }
	static void getModelDimensions(Hash modelHash, math::vector3<float>* minimum, math::vector3<float>* maximum) { _i<int>(0xDCB8DDD5D054A7E7, modelHash, minimum, maximum); }
	static void setBit(int* address, int offset) { _i<int>(0xF73FBE4845C43B5B, address, offset); } // 0xF73FBE4845C43B5B
	static float getDistanceBetweenCoords(math::vector3<float> from, math::vector3<float> to, BOOL useZ) { return _i<float>(0x0BE7F4E3CDBAFB28, from.x, from.y, from.z, to.x, to.y, to.z, useZ); };
	static void startLightningStrikeAtCoords(math::vector3<float> coords) { _i<int>(0x67943537D179597C, coords.x, coords.y, coords.z); }
	static int requestComposite(Hash compositeHash) { return _i<int>(0x73F0D0327BFA0812, compositeHash); }
	static BOOL hasCompositeLoaded(Hash compositeHash) { return _i<BOOL>(0x5E5D96BE25E9DF68, compositeHash); }
	static Object createComposite(Hash compositeHash, math::vector3<float> coords, float heading, int unk, int* unk2, int unk3) { return _i<int>(0x5B4BBE80AD5972DC, compositeHash, coords.x, coords.y, coords.z, heading, unk, unk2, unk3); }

	// hud
	static void setTextScale(float scale, float size) { _i<int>(0x4170B650590B3B00, scale, size); } // 0x4170B650590B3B00
	static void setTextCentre(BOOL align) { _i<int>(0xBE5261939FBECB8C, align); } // 0xBE5261939FBECB8C
	static void setTextColor(int r, int g, int b, int a) { _i<int>(0x50A41AD966910F03, r, g, b, a); } // 0x50A41AD966910F03
	static void drawText(const char* str, float xPos, float yPos) { _i<int>(0xD79334A4BB99BAD1, str, xPos, yPos); } // 0xD79334A4BB99BAD1
	static BOOL isPauseMenuActive() { return _i<BOOL>(0x535384D6067BA42E); } // 0x535384D6067BA42E
	static const char* getLabelText(const char* label) { return _i<const char*>(0xCFEDCCAD3C5BA90D, label); } // 0xCFEDCCAD3C5BA90D
	static void displayHud(BOOL toggle) { _i<int>(0xD63FE3AF9FB3D53F, toggle); } // 0xD63FE3AF9FB3D53F
	static void disableFrontendThisFrame() { _i<int>(0x56CE42A528156A67); } // 0x56CE42A528156A67
	static void setMinimapHideFlow(BOOL toggle) { _i<int>(0x4B8F743A4A6D2FF8, toggle); } //0x4B8F743A4A6D2FF8 
	static void clearAllHelpMessages() { _i<int>(0x916ED8321F087059); }
	static int promptRegisterBegin() { return _i<int>(0x04F97DE45A519419); }
	static void promptRegisterEnd(int prompt) { _i<int>(0xF7AA2696A22AD8B9, prompt); }
	static void promptSetControlAction(int prompt, Hash control) { _i<int>(0xB5352B7494A08258, prompt, control); }
	static void promptSetText(int prompt, char* text) { _i<int>(0x5DD02A8318420DD7, prompt, text); }
	static void promptSetStandardMode(int prompt, BOOL unk) { _i<int>(0xCC6656799977741B, prompt, unk); }
	static BOOL promptIsValid(int prompt) { return _i<int>(0x347469FBDD1589A9, prompt); }
	static void promptSetVisible(int prompt, BOOL toggle) { _i<int>(0x71215ACCFDE075EE, prompt, toggle); }
	static void promptSetEnabled(int prompt, BOOL toggle) { _i<int>(0x8A0FB4D03A630D21, prompt, toggle); }

	// pad  
	static BOOL isDisabledControlJustReleased(int padIndex, Hash control) { return _i<BOOL>(0x305C8DCD79DA8B0F, padIndex, control); } // 0x305C8DCD79DA8B0F
	static BOOL isDisabledControlJustPressed(int padIndex, Hash control) { return _i<BOOL>(0x91AEF906BCA88877, padIndex, control); } // 0x91AEF906BCA88877
	static BOOL isDisabledControlPressed(int padIndex, Hash control) { return _i<BOOL>(0xE2587F8CBBD87B1D, padIndex, control); } // 0xE2587F8CBBD87B1D
	static BOOL isControlPressed(int padIndex, Hash control) { return _i<BOOL>(0xF3A21BCD95725A4A, padIndex, control); } // 0xF3A21BCD95725A4A
	static void setInputExclusive(int padIndex, Hash control) { _i<int>(0xEDE476E5EE29EDB1, padIndex, control); } // 0xEDE476E5EE29EDB1
	static void disableControlAction(int padIndex, Hash control, BOOL disable) { _i<int>(0xFE99B66D079CF6BC, padIndex, control, disable); }
	static float getControlNormal(int padIndex, Hash control) { return _i<float>(0xEC3C9B8D5327B563, padIndex, control); }

	// streaming
	static void requestModel(Hash model) { _i<int>(0xFA28FE3A6246FC30, model); } // 0xFA28FE3A6246FC30
	static BOOL hasModelLoaded(Hash model) { return _i<BOOL>(0x1283B8B89DD5D1B6, model); } // 0x1283B8B89DD5D1B6
	static BOOL isModelInCdImage(Hash model) { return _i<BOOL>(0xD6F3B6D7716CFF8E, model); } // 0xD6F3B6D7716CFF8E
	static BOOL isModelValid(Hash model) { return _i<BOOL>(0x392C8D8E07B70EFC, model); } // 0x392C8D8E07B70EFC
	static void setModelAsNoLongerNeeded(Hash model) { _i<int>(0x4AD96EF928BD4F9A, model); } // 0x4AD96EF928BD4F9A
	static void requestNamedPtfxAsset(Hash name) { _i<int>(0xF2B2353BBC0D4E8F, name); } // 0xF2B2353BBC0D4E8F
	static void useParticleFXAsset(const char* name) { _i<int>(0xA10DB07FC234DD12, name); } // 0xA10DB07FC234DD12
	static void requestCollisionAtCoords(float x, float y, float z) { _i<int>(0x0A3720F162A033C9, x, y, z); }
	static void setFocusPosAndLevel(math::vector3<float> coords, math::vector3<float> offset) { _i<int>(0x25F6EF88664540E2, coords.x, coords.y, coords.z, offset.x, offset.y, offset.z); } // 0x25F6EF88664540E2 
	static void clearFocus() { _i<int>(0x86CCAF7CE493EFBE); } // 0x86CCAF7CE493EFBE
	static BOOL isModelAVehicle(Hash model) { return _i<Hash>(0x354F62672DE7DB0A, model); } // 0x354F62672DE7DB0A

	// camera
	static math::vector3<float> getGameplayCamRotation(int rotationOrder) { return _i<math::vector3<float>>(0x0252D2B5582957A6, rotationOrder); } // 0x0252D2B5582957A6
	static math::vector3<float> getGameplayCamCoord() { return _i<math::vector3<float>>(0x595320200B98596E); } // 0x595320200B98596E
	static BOOL doesCamExist(Cam cam) { return _i<BOOL>(0x153AD457764FD704, cam); } // 0x153AD457764FD704
	static Cam createCam(const char* name, BOOL p1) { return _i<Cam>(0xE72CDBA7F0A02DD6, name, p1); } // 0xE72CDBA7F0A02DD6
	static void setCamActive(Cam cam, BOOL active) { _i<int>(0x87295BCA613800C8, cam, active); } // 0x87295BCA613800C8
	static void renderScriptCams(BOOL render, BOOL ease, int easeTime, BOOL p3, BOOL p4, BOOL p5) { _i<int>(0x33281167E4942E4F, render, ease, easeTime, p3, p4, p5); } // 0x33281167E4942E4F
	static void setCamCoord(Cam cam, float x, float y, float z) { _i<int>(0xF9EE7D419EE49DE6, cam, x, y, z); } // 0xF9EE7D419EE49DE6
	static math::vector3<float> getCamCoord(Cam cam) { return _i<math::vector3<float>>(0x6B12F11C2A9F0344, cam); } // 0x6B12F11C2A9F0344
	static void setCamRotation(Cam cam, float x, float y, float z, int order) { _i<int>(0x63DFA6810AD78719, cam, x, y, z, order); } // 0x63DFA6810AD78719
	static void attachCamToPedBone(Cam cam, Ped ped, int boneIndex, float x, float y, float z, float heading) { _i<int>(0xDFC1E4A44C0324CA, cam, ped, boneIndex, x, y, z, heading); } // 0xDFC1E4A44C0324CA
	static void destroyCam(Cam cam, BOOL thisScriptCheck) { _i<int>(0x4E67E0B6D7FD5145, cam, thisScriptCheck); } // 0x4E67E0B6D7FD5145
	static void animateGameplayCamZoom(float p0, float distance) { _i<int>(0x2126C740A4AC370B, p0, distance); } // 0x2126C740A4AC370B
	static void setCinematicButtonActive(BOOL p0) { _i<int>(0xB90411F480457A6C, p0); } // 0xB90411F480457A6C
	static void setGameplayRelativeHeading(float heading, float unk) { _i<int>(0x5D1EB123EAC5D071, heading, unk); } // 0x5D1EB123EAC5D071
	static void setGameplayRelativePitch(float pitch, float unk) { _i<int>(0xFB760AF4F537B8BF, pitch, unk); }
	static float getGameplayRelativePitch() { return _i<float>(0x99AADEBBA803F827); }
	static void setGameplayEntityHint(Entity entity, math::vector3<float> offset, BOOL p4, int p5, int p6, int p7, Any p8) { _i<int>(0xD1F7F32640ADFD12, entity, offset.x, offset.y, offset.z, p4, p5, p6, p7, p8); }
	static void setGameplayCoordHint(math::vector3<float> coords, int duration, int blendOutDuration, int blendInDuration, int unk) { _i<int>(0xFA33B8C69A4A6A0F, coords.x, coords.y, coords.z, duration, blendOutDuration, blendInDuration, unk); }
	static void stopGameplayHint(BOOL p0) { _i<int>(0x1BCEC33D54CFCA8A, p0); }
	static void setCameraHeadingBounds(float min, float max) { _i<int>(0x05BD5E4088B30A66, min, max); }
	static float getHeadingFromVector2d(math::vector2<float> coords) { return _i<float>(0x38D5202FF9271C62, coords.x, coords.y); }

	// shapetest
	static int startShapeTestRay(math::vector3<float> start, math::vector3<float> end, int flags, Entity entity, int p8) { return _i<int>(0x377906D8A31E5586, start.x, start.y, start.z, end.x, end.y, end.z, flags, entity, p8); } // 0x377906D8A31E5586
	static int getShapeTestResult(int rayHandle, BOOL* hit, math::vector3<float>* endCoords, math::vector3<float>* surfaceNormal, Entity* entityHit) { return _i<int>(0xEDE8AC7C5108FB1D, rayHandle, hit, endCoords, surfaceNormal, entityHit); } // 0xEDE8AC7C5108FB1D

	// builtin
	static float vDist(math::vector3<float> coord1, math::vector3<float> coord2) { return _i<float>(0x2A488C176D52CCA5, coord1.x, coord1.y, coord1.z, coord2.x, coord2.y, coord2.z); } // 0x2A488C176D52CCA5

	// audio
	static void playSoundFrontend(const char* name, const char* ref, BOOL p2, BOOL p3) { _i<int>(0x67C540AA08E4A6F5, name, ref, p2, p3); } // 0x67C540AA08E4A6F5

	// map
	static Blip addBlipForEntity(Hash blipHash, Entity entity) { return _i<Blip>(0x23F74C2FDA6E7C61, blipHash, entity); } // 0x23F74C2FDA6E7C61
	static void setBlipNameToPlayerName(Blip blip, Player index) { _i<int>(0x093DD5A31BC2B459, blip, index); } // 0x093DD5A31BC2B459
	static Blip getBlipFromEntity(Entity entity) { return _i<Blip>(0x6D2C41A8BD6D6FD0, entity); } // 0x6D2C41A8BD6D6FD0
	static math::vector3<float> getWaypointCoords() { return _i<math::vector3<float>>(0x29B30D07C3F7873B); } // 0x29B30D07C3F7873B
	static BOOL isWaypointActive() { return _i<BOOL>(0x202B1BBFC6AB5EE4); } // 0x202B1BBFC6AB5EE4
	static void setBlipSprite(Blip blip, Hash hash, BOOL p2) { _i<int>(0x74F74D3207ED525C, blip, hash, p2); } // 0x74F74D3207ED525C
	static void displayRadar(BOOL toggle) { _i<int>(0x1B3DA717B9AFF828, toggle); } // 0x1B3DA717B9AFF828
	static void removeBlip(Blip* blip) { _i<int>(0xF2C3C9DA47AAA54A, blip); } // 0xF2C3C9DA47AAA54A
	static void setBlipToPlayername(Blip blip, Player player) { _i<int>(0x093DD5A31BC2B459, blip, player); } // 0x093DD5A31BC2B459

	// network
	static BOOL networkIsPlayerConnected(Player index) { return _i<BOOL>(0x93DC1BE4E1ABE9D1, index); } // 0x93DC1BE4E1ABE9D1
	static BOOL networkIsPlayerActive(Player index) { return _i<BOOL>(0xB8DFD30D6973E135, index); } // 0xB8DFD30D6973E135
	static BOOL networkIsInSession() { return _i<BOOL>(0xCA97246103B63917); } // 0xCA97246103B63917
	static void networkRequestControlOfEntity(Entity entity) { _i<int>(0xB69317BF5E782347, entity); } // 0xB69317BF5E782347
	static BOOL networkHasControlOfEntity(Entity entity) { return _i<BOOL>(0x01BF60A500E28887, entity); } // 0x01BF60A500E28887
	static Player networkGetPlayerIndexFromPed(Ped ped) { return _i<Player>(0x6C0E2E0125610278, ped); } // 0x6C0E2E0125610278
	static void networkClockTimeOverride(int hour, int minute, int second, int transitionTime, BOOL p4) { _i<int>(0x669E223E64B1903C, hour, minute, second, transitionTime, p4); } // 0x669E223E64B1903C
	static BOOL networkGetClockTimeOverride(int* hour, int* minute, int* second) { return _i<BOOL>(0x11A7ADCD629E170F, hour, minute, second); } // 0x11A7ADCD629E170F
	static void networkHandleFromPlayer(Player player, int* netHandle, int size) { _i<int>(0x388EB2B86C73B6B3, player, netHandle, size); } // 0x388EB2B86C73B6B3
	static BOOL networkIsFriend(int* netHandle) { return _i<BOOL>(0x1A24A179F9B31654, netHandle); } // 0x1A24A179F9B31654
	static BOOL networkIsHandleValid(int* netHandle, int size) { return _i<BOOL>(0x6F79B93B0A8E4133, netHandle, size); } // 0x6F79B93B0A8E4133
	static Player networkGetHostOfThisScript() { return _i<Player>(0xC7B4D79B01FA7A5C); } // 0xC7B4D79B01FA7A5C
	static BOOL networkAddFriend(int* netHandle, const char * msg) { return _i<BOOL>(0x8E02D73914064223, netHandle, msg); }
	static void networkRegisterEntityAsNetworked(Entity entity) { _i<int>(0x06FAACD625D80CAA, entity); }
	static void setNetworkIdExistsOnAllMachines(int netId, BOOL toggle) { _i<int>(0xE05E81A888FA63C8, netId, toggle); }
	static int networkGetNetworkIdFromEntity(Entity entity) { return _i<int>(0xA11700682F3AD45C, entity); }
	static int networkGetNumOfConnectedPlayers() { return _i<int>(0xA4A79DD2D9600654); }
	static void networkReportPlayer(Player index, int reportType, const char* description, const char* horsename) { _i<int>(0xA197C35F73AC0F12, index, reportType, description, horsename); } // 0xA197C35F73AC0F12
	static void networkSetInSpectatorMode(BOOL toggle, Ped playerPed) { _i<int>(0x423DE3854BB50894, toggle, playerPed); } //0x423DE3854BB50894
	static BOOL networkIsSessionActive() { return _i<BOOL>(0xD83C2B94E7508980); } // 0xD83C2B94E7508980
	static void networkSetEntityVisibleToNetwork(Entity entity, BOOL toggle) { _i<int>(0xF1CA12B18AEF5298, entity, toggle); } // 0xF1CA12B18AEF5298
	static std::size_t getFriendCount() { return (std::size_t)_i<int>(0xDB7ABDD203FA3704); } // 0xDB7ABDD203FA3704
	static void getNetHandleFromFriendIndex(int index, int* netHandle) { _i<int>(0xD45CB817D7E177D2, index, netHandle); } // 0xD45CB817D7E177D2
	static const char* getFriendNameFromNetHandle(int* netHandle) { return _i<const char*>(0x5659D87BE674AB17, netHandle); }
	static void triggerScriptEvent(int eventGroup, int* args, int argCount, int bit, Any* p4) { _i<int>(0x5AE99C571D5BBE5D, eventGroup, args, argCount, bit, p4); }

	// uilog
	static void setCachedObjective(const char* ob) { _i<int>(0xFA233F8FE190514C, ob); }; // 0xFA233F8FE190514C
	static void printCachedObjective() { _i<int>(0xE9990552DEC71600); } // 0xE9990552DEC71600
	static void clearCachedObjective() { _i<int>(0xDFF0D417277B41F8); } // 0xDFF0D417277B41F8

	// task
	static void taskStandStill(Ped ped, int time) { _i<int>(0x919BE13EED931959, ped, time); } // 0x919BE13EED931959
	static void taskGoStraightToCoord(Ped ped, math::vector3<float> coords, float speed, int timeout, float targetHeading, float distanceToSlide) { _i<int>(0xD76B57B44F1E6F8B, ped, coords.x, coords.y, coords.z, speed, timeout, targetHeading, distanceToSlide); } // 0xD76B57B44F1E6F8B
	static void taskJump(Ped ped, BOOL p1) { _i<int>(0x0AE4086104E067B1, ped, p1); } // 0x0AE4086104E067B1
	static void taskHandsUp(Ped ped, int duration, Ped facingPed, int p3, BOOL p4) { _i<int>(0xF2EAB31979A7F910, ped, duration, facingPed, p3, p4); } // 0xF2EAB31979A7F910
	static void taskCombatHatedTargetsAroundPed(Ped ped, float radius, int flags, Ped target) { _i<int>(0x7BF835BB9E2698C8, ped, radius, flags, target); } // 0x7BF835BB9E2698C8
	static void taskCombatPed(Ped ped, Ped targetPed, int p2, int p3) { _i<int>(0xF166E48407BAC484, ped, targetPed, p2, p3); } // 0xF166E48407BAC484
	static void taskPlayAnim(Ped ped, const char* animDict, const char* animName, float speed, float speedMultiplier, int duration, int flags, float playbackRate, BOOL p8, Any p9, BOOL p10, const char* p11, BOOL p12) { _i<int>(0xEA47FE3719165B94, ped, animDict, animName, speed, speedMultiplier, duration, flags, playbackRate, p8, p9, p10, p11, p12); } // 0xEA47FE3719165B94
	static void stopAnimTask(Ped ped, const char* animDictionary, const char* animationName, float p3) { _i<int>(0x97FF36A1D40EA00A, ped, animDictionary, animationName, p3); }
	static void requestAnimDict(const char* dict) { _i<int>(0xA862A2AD321F94B4, dict); }
	static void taskLookAtCoord(Ped ped, math::vector3<float> coords, int duration, int unk, int unk2, int unk3) { _i<int>(ped, coords.x, coords.y, coords.y, duration, unk, unk2, unk3); }

	// vehicle
	static Vehicle createVehicle(Hash modelHash, math::vector3<float> coords, float heading, BOOL p5, BOOL p6, BOOL p7, BOOL p8) { return _i<Vehicle>(0xAF35D0D2583051B0, modelHash, coords.x, coords.y, coords.z, heading, p5, p6, p7, p8); } // 0xAF35D0D2583051B0
	static Vehicle createVehicle2(Hash modelHash, math::vector3<float> coords, float heading, BOOL p5, BOOL p6, BOOL p7,Hash unkhash, BOOL p8) { return _i<Vehicle>(0x214651FB1DFEBA89, modelHash, coords.x, coords.y, coords.z, heading, p5, p6, p7, unkhash, p8); } // 0xAF35D0D2583051B0
	static Ped getPedInVehicleSeat(Vehicle vehicle, int seat) { return _i<Ped>(0xBB40DD2270B65366, vehicle, seat); } // 0xBB40DD2270B65366
	static BOOL areAnyVehicleSeatsFree(Vehicle vehicle) { return _i<BOOL>(0xA0A424505A1B6429, vehicle); } // 0xA0A424505A1B6429
	static void setVehicleEngineOn(Vehicle vehicle, BOOL value, BOOL instantly) { _i<int>(0xB64CFA14CB9A2E78, vehicle, value, instantly); } // 0xB64CFA14CB9A2E78
	static void setVehicleForwardSpeed(Vehicle vehicle, float speed) { _i<int>(0xF9F92AF49F12F6E7, vehicle, speed); } // 0xF9F92AF49F12F6E7
	static void setVehicleOnGroundProperly(Vehicle vehicle) { _i<int>(0x7263332501E07F52, vehicle); } // 0x7263332501E07F52
	static BOOL isThisModelATrain(Hash model) { return _i<BOOL>(0xFC08C8F8C1EDF174, model); } // 0xFC08C8F8C1EDF174
	static void setTrainSpeed(Vehicle train, float speed) { _i<int>(0xDFBA6BBFF7CCAFBB, train, speed); } // 0xDFBA6BBFF7CCAFBB
	static void deleteMissionTrain(Vehicle* train) { _i<int>(0x0D3630FB07E8B570, train); } // 0x0D3630FB07E8B570
	static void setVehicleFixed(Vehicle vehicle) { _i<int>(0x79811282A9D1AE56, vehicle); } // 0x79811282A9D1AE56
	static void explodeVehicle(Vehicle vehicle, BOOL isAudible, BOOL isInvisible, Any p3) { _i<int>(0x75DCED9EEC5769D7, vehicle, isAudible, isInvisible, p3); }
	static void setVehicleDirtLevel(Vehicle vehicle, float level) { _i<int>(0x758C3460EE915D0A, vehicle, level); } // 0x758C3460EE915D0A
	static BOOL isHornActive(Vehicle vehicle) { return _i<BOOL>(0x9D6BFC12B05C6121, vehicle); } // 0x9D6BFC12B05C6121
	static void setVehicleDoorOpen(Vehicle vehicle, int doorIndex, BOOL loose, BOOL openInstantly) { _i<int>(0x550CE392A4672412, vehicle, doorIndex, loose, openInstantly); } // 0x550CE392A4672412
	static void setVehicleDoorShut(Vehicle vehicle, int doorIndex, BOOL closeInstantly) { _i<int>(0x6A3C24B91FD0EA09, vehicle, doorIndex, closeInstantly); } // 0x6A3C24B91FD0EA09
	static void setVehicleDoorBroken(Vehicle vehicle, int doorIndex, BOOL deleteDoor) { _i<int>(0x9666CF20A1C6D780, vehicle, doorIndex, deleteDoor); } // 0x9666CF20A1C6D780
	static void setVehicleDoorsLocked(Vehicle vehicle, int doorLockStatus) { _i<int>(0x96F78A6A075D55D9, vehicle, doorLockStatus); } // 0x96F78A6A075D55D9

	// decorator
	static BOOL decorSetBool(Entity entity, const char* name, BOOL value) { return _i<BOOL>(0xFE26E4609B1C3772, entity, name, value); } // 0xFE26E4609B1C3772
	static BOOL decorSetFloat(Entity entity, const char* name, float value) { return _i<BOOL>(0x238F8B0C1C7FE834, entity, name, value); } // 0x238F8B0C1C7FE834
	static BOOL decorSetInt(Entity entity, const char* name, int value) { return _i<BOOL>(0xE88F4D7F52A6090F, entity, name, value); } // 0xE88F4D7F52A6090F
	static BOOL decorIsRegisteredAsType(const char* propertyName, int type) { return _i<BOOL>(0x72355278C069F272, propertyName, type); }
	static int decorGetInt(Entity entity, const char* propertyName) { return _i<int>(0x44DB62727762FD9B, entity, propertyName); }
	static void decorRegister(const char* propertyName, int type) { _i<int>(0x0B253D644E3C36B3, propertyName, type); }

	// scripts
	static Hash getHashOfThisScriptName() { return _i<Hash>(0xBC2C927F5C264960); } // 0xBC2C927F5C264960
}