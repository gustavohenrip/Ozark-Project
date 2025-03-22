#include "object_pool.hpp"
#include "global/variables.hpp"
#include "rage/engine.hpp"
#include "rage/invoker/natives.hpp"
#include "menu/base/util/player_manager.hpp"

namespace Menu::Pools {
	void ObjectPool::Update() {
		std::vector<Object> Objects;

		Rage::Types::GenericPool* Pool = Global::Vars::g_ObjectPool;
		if (IsValidPtr(Pool)) {
			if (Pool->m_Count > 4000) return;
			if (!Pool->m_Address) return;

			for (uint32_t i = 0; i < Pool->m_Count; i++) {
				if (Global::Vars::g_EntityPool) {
					if (Global::Vars::g_EntityPool->IsFull()) {
						break;
					}
				}

				uint64_t Address = Pool->GetEntity(i);
				if (Address) {
					Entity Handle = Rage::Engine::GetEntityHandleFromAddress(Address);
					if (Handle > 0) {
						Objects.push_back(Handle);
					}
				}
			}
		}

		for (std::size_t i = 0; i < Objects.size(); i++) {
			if (std::find(m_Storage.begin(), m_Storage.end(), Objects[i]) == m_Storage.end()) {
				m_Storage.push_back(Objects[i]);
			}
		}

		for (std::size_t i = 0; i < m_Storage.size(); i++) {
			if (!Native::DoesEntityExist(m_Storage[i])) {
				m_Storage.erase(m_Storage.begin() + i);
			}
		}
	}

	void ObjectPool::Run(std::function<void(Object)> Callback) {
		Update();

		for (std::size_t i = 0; i < m_Storage.size(); i++) {
			if (m_Storage[i]) {
				Callback(m_Storage[i]);
			}
		}
	}

	ObjectPool* GetObjectPool() {
		static ObjectPool Instance;
		return &Instance;
	}
}