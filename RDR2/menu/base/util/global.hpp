#pragma once
#include "global/variables.hpp"

namespace menu {
	class globals {
	public:
		globals(int index)
			: _handle(&global::vars::g_globalPointer[index >> 18 & 0x3F][index & 0x3FFFF])
		{ }

		globals(void* p)
			: _handle(p)
		{ }

		globals(const globals& copy)
			: _handle(copy._handle)
		{ }

		globals at(int index) {
			return globals(reinterpret_cast<void**>(this->_handle) + (index));
		}

		globals at(int index, int size) {
			return this->at(1 + (index * size));
		}

		template <typename T>
		T* get() {
			return reinterpret_cast<T*>(this->_handle);
		}

		template <typename T>
		T& as() {
			return *this->get<T>();
		}
	private:
		void* _handle;
	};
}