#pragma once
#include "stdafx.hpp"
#include <queue>

namespace Utils {
	struct Thread {
		const char* m_Name;
		HANDLE m_Handle;
		bool m_Running;
		void(*m_Function)();
	};

	class ThreadPool {
	public:
		void AddJob(std::function<void()> Function);
		void Add(const char* Name, void(*Function)());
		void Remove(const char* Name);
		void Unload();

		std::vector<Thread*>& GetThreads() { return m_Threads; }
	private:
		std::vector<Thread*> m_Threads;
	};
	
	ThreadPool* GetThreadPool();
}