#pragma once
#include "stdafx.hpp"
#include <queue>

namespace utils {
	struct thread {
		const char* name;
		HANDLE handle;
		bool running;
		void(*func)();
	};

	class threadPool {
	public:
		void addJob(std::function<void()> func);
		void add(const char* name, void(*func)());
		void remove(const char* name);
		std::vector<thread*>& getThreads() { return threads; }
	private:
		std::vector<thread*> threads;
	}; threadPool* getThreadPool();
}