#pragma once
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <sys/time.h>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <inttypes.h>

#define ARRAYSIZE(a) (int)((sizeof(a) / sizeof(*(a))) / static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#define NEW_SERVER

#ifdef NEW_SERVER
#define PORT 6968
#else
#define PORT 6969
#endif
