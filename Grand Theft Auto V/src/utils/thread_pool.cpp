#include "thread_pool.hpp"
#include "global/variables.hpp"

namespace Utils {
	void ThreadPool::AddJob(std::function<void()> Function) {
		struct Data {
			std::function<void()> m_F;
		};

		Data* D = new Data();
		D->m_F = Function;

		CreateThread(0, 0, [] (void* lpParam) -> DWORD {
			Data* T = (Data*)lpParam;
			T->m_F();
			delete T;
			return 0;
		}, D, 0, 0);
	}

	void ThreadPool::Add(const char* Name, void(*Function)()) {
		// return;
		Thread* Instance = new Thread();
		Instance->m_Name = Name;
		Instance->m_Running = true;
		Instance->m_Function = Function;

		Instance->m_Handle = CreateThread(0, 0, [] (void* lpParam) -> DWORD {
			Thread* T = (Thread*)lpParam;

			while (T && T->m_Running && !Global::Vars::g_Unloading) {
				if (T->m_Function) T->m_Function();
			}

			delete T;
			return 0;
		}, Instance, 0, 0);

		m_Threads.push_back(Instance);
	}

	void ThreadPool::Unload() {
		for (auto& Obj : m_Threads) {
			if (Obj) {
				Obj->m_Running = false;
				TerminateThread(Obj->m_Handle, 0);
			}
		}
	}

	void ThreadPool::Remove(const char* Name) {
		for (auto& Obj : m_Threads) {
			if (Obj) {
				if (Obj->m_Name) {
					if (!strcmp(Obj->m_Name, Name)) {
						Obj->m_Running = false;
						return;
					}
				}
			}
		}
	}

	ThreadPool* GetThreadPool() {
		static ThreadPool Instance;
		return &Instance;
	}
}