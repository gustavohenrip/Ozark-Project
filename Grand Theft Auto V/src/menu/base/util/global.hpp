#pragma once
#include "stdafx.hpp"
#include "global/variables.hpp"

namespace Menu {
	class Global {
	private:
		void* m_handle;

	public:
		Global(int index) : m_handle(&::Global::Vars::g_GlobalCache[index >> 18 & 0x3F][index & 0x3FFFF]) {}
		Global(void* p) : m_handle(p) {}
		Global(const Global& copy) : m_handle(copy.m_handle) {}

		Global At(int index) const {
			return Global(reinterpret_cast<void**>(this->m_handle) + (index));
		}

		Global At(int index, int size) const {
			return this->At(1 + (index * size));
		}

		template <typename T>
		T* Get() {
			return reinterpret_cast<T*>(this->m_handle);
		}

		template <typename T>
		T& As() {
			return *this->Get<T>();
		}
	};


	class ScriptLocal {
	private:
		int m_Index;
		uint64_t m_Storage;

	public:
		ScriptLocal(uint64_t Storage, int Index)
			: m_Storage(Storage), m_Index(Index) {}

		ScriptLocal At(int Index) { m_Index += Index; return *this; }
		ScriptLocal At(int Index, int Size) { return At(1 + (Index * Size)); }

		template <typename T>
		T* Get() {
			return reinterpret_cast<T*>(Data());
		}

		template <typename T>
		T& As() {
			return *this->Get<T>();
		}

	private:
		void* Data() { return (void*)(m_Storage + (m_Index << 3)); }
	};
}