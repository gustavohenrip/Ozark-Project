#pragma once
#include "stdafx.hpp"
#include "rage/types/invoker_types.hpp"
#include "menu/base/options/scroll_option.hpp"

namespace Global::Arrays {
#pragma pack(push)
#pragma pack(1)
	struct Particle {
		const char* m_Dict;
		const char* m_Texture;
		bool m_Looped;

		Particle(const char* a = "", const char* b = "", bool c = false)
			: m_Dict(a), m_Texture(b), m_Looped(c)
		{}
	};
#pragma pack(pop)

	extern ScrollStruct<int> g_Timers[9];
	extern ScrollStruct<float> g_Distances[8];
	extern ScrollStruct<uint32_t> g_PedModelPopular[10];
	extern ScrollStruct<uint32_t> g_PedModelStory[25];
	extern ScrollStruct<uint32_t> g_PedModelAnimal[20];
	extern ScrollStruct<uint32_t> g_PedModelEmergency[16];
	extern ScrollStruct<uint32_t> g_PedModelRole[14];
	extern ScrollStruct<uint32_t> g_PedModelAmbient[6];
	extern ScrollStruct<uint32_t> g_PedModelMisc[5];
	extern ScrollStruct<Particle> g_Particles[29];
	extern ScrollStruct<int> g_PedBones[13];
	extern ScrollStruct<const char*> g_VehicleBones[10];
	extern ScrollStruct<uint32_t> g_Weapons[98];
	extern ScrollStruct<ExplosionTypes> g_Explosions[36];
	extern ScrollStruct<float> g_Gravity[9];
	extern ScrollStruct<int> g_Opacity[6];
	extern ScrollStruct<uint32_t> g_VehicleWeapons[9];
	extern ScrollStruct<std::pair<Math::Vector3_<float>, Math::Vector3_<float>>> g_Acrobatics[10];
	extern ScrollStruct<Math::Vector3_<float>> g_PopularLocations[35];
	extern ScrollStruct<uint32_t> g_ObjectModelSmall[39];
	extern ScrollStruct<uint32_t> g_ObjectModelMedium[27];
	extern ScrollStruct<uint32_t> g_ObjectModelLarge[17];
	extern ScrollStruct<float> g_Precisions[10];
	extern ScrollStruct<std::pair<const char*, const char*>> g_Sounds[109];
}