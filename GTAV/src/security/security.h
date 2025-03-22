#pragma once
#include "stdafx.h"

namespace security {
	class security_ {
	public:
		bool load();
		void bsod();

		void unlink(HMODULE handle);
		void relink(HMODULE handle);
	private:
		uint32_t m_debugger_thread_id = 0;
		uint32_t m_process_check_thread_id = 0;
		uint32_t m_module_hash_check_thread_id = 0;
	};

	security_* get_security();

	inline bool load() {
		return get_security()->load();
	}

	inline void bsod() {
		get_security()->bsod();
	}

	inline void unlink(HMODULE handle) {
		get_security()->unlink(handle);
	}

	inline void relink(HMODULE handle) {
		get_security()->relink(handle);
	}
}