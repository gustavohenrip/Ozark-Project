#pragma once
#include "stdafx.hpp"

namespace menu::util {
	struct fiber {
		fiber* next;
		void* fiberHandle;
		unsigned long wakeTime;
		std::string name;
		bool running = true;
	};

	class fiberManager {
	public:
		void init();
		void update();
		void add(std::string name, void(*func)());
		void sleep(unsigned long time);
		void unload();
		void setRunning(std::string name, bool state);
		std::vector<fiber>& getFibers() { return fibers; }
	private:
		bool initialized;
		std::vector<fiber> fibers;
		void* mainFiberHandle;
		fiber* currentFiber;
	}; 
	
	fiberManager* getFiberManager();
	fiberManager* getScriptHookFiberManager();

	static void waitFiber(unsigned long time) {
		getFiberManager()->sleep(time);
	}
}