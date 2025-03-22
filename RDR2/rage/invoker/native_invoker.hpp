#pragma once
#include "utils/math.hpp"
#include "global/variables.hpp"

namespace rage::invoker {
	class nativeContext {
	protected:
		void* returnData;
		uint32_t argumentCount;
		void* argumentData;
		uint32_t dataCount;
		math::vector3<float>* scriptVectors[4];
		math::vector4<float> resultVectors[4];
	public:
		void reset() {
			argumentCount = 0;
			dataCount = 0;
		}

		template<typename T>
		T getArgument(int index) {
			uint64_t* _ = (uint64_t*)argumentData;
			return *(T*)&_[index];
		}

		template<typename T>
		void setArgument(int index, T value) {
			uint64_t* _ = (uint64_t*)argumentData;
			*(T*)&_[index] = value;
		}

		template<typename T>
		T getReturnResult(int index) {
			uint64_t* _ = (uint64_t*)returnData;
			return *(T*)&_[index];
		}

		template<typename T>
		void setReturn(int index, T value) {
			uint64_t* _ = (uint64_t*)returnData;
			*(T*)&_[index] = value;
		}

		void* getReturnData() {
			return returnData;
		}

		void updateResultVectors() {
			for (uint32_t i = 0; i < dataCount; i++) {
				scriptVectors[i]->x = resultVectors[i].x;
				scriptVectors[i]->y = resultVectors[i].y;
				scriptVectors[i]->z = resultVectors[i].z;
			}
			
			dataCount = 0;
		}
	};

	class nativeSetup : public nativeContext {
	private:
		uint8_t tempBuffer[256];
	public:
		inline nativeSetup() {
			argumentData = &tempBuffer;
			returnData = &tempBuffer;

			argumentCount = 0;
			dataCount = 0;

			memset(tempBuffer, 0x0, sizeof(tempBuffer));
		}

		void dump() {
			for (uint32_t i = 0; i < argumentCount; i++) {
				printf("%llx ", *(uint64_t*)(&tempBuffer[i * 8]));
			} printf("\n");
		}

		uint32_t getArgumentCount() {
			return argumentCount;
		}

		template<typename T>
		void push(T value) {
			memset(&tempBuffer[8 * argumentCount], 0, 8);
			*(T*)&tempBuffer[8 * argumentCount] = value;
			argumentCount++;
		}

		template<typename T>
		T getReturn() {
			return *(T*)tempBuffer;
		}

		uint64_t* getReturnPtr() {
			return (uint64_t*)tempBuffer;
		}
	};

	typedef void(*nativeHandler)(nativeContext*);

	static std::unordered_map<uint64_t, nativeHandler> cachedNatives;
	static nativeHandler getNativeHandler(uint64_t hash) {
		auto handler = cachedNatives[hash];
		if (!handler) {
			auto rdx = *(uint64_t*)global::vars::g_nativeTableEncryption;
			auto r9 = hash;
			r9 = ~r9;
			rdx = math::ror8(rdx, 5);
			rdx = math::rol8(rdx, 0x20);
			r9 = math::rol8(r9, 0x20);
			auto ecx = (uint32_t)rdx;
			ecx = ecx & 0x1F;
			ecx++;
			r9 = math::ror8(r9, (uint8_t)ecx);

			uint64_t finalHash = (rdx ^ r9);

			auto table = global::vars::g_nativeRegistration[finalHash & 0xFF];
			for (; table; table = table->nextRegistration) {
				for (uint32_t i = 0; i < table->count; i++) {
					if (finalHash == table->hashes[i]) {
						cachedNatives[hash] = (nativeHandler)table->functions[i];
						return cachedNatives[hash];
					}
				}
			}
		}

		return handler;
	}

	static void callNative(uint64_t hash, nativeSetup* setup) {
		nativeHandler handler = getNativeHandler(hash);
		if (handler) {
			__try {
				handler(setup);
			} __except (true) {}
		}
	}

	template<typename R, typename... meme>
	static R invoke(uint64_t hash, meme&&... args) {
		nativeSetup setup;

		int dummy[] = {
			0,
			((void)setup.push(std::forward<meme>(args)),
			0) ... };

		callNative(hash, &setup);
		setup.updateResultVectors();
		return setup.getReturn<R>();
	}
}