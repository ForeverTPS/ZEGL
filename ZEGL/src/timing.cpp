#include "timing.h"

#include <ctime>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WIN64)
	#define OS_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
    #define OS_MACOSX
#elif defined(__linux__)
	#define OS_LINUX
#else
	#define OS_OTHER
#endif

#ifdef OS_WINDOWS
	#include <Windows.h>
	#include <iostream>
	static double g_freq;
	static bool g_timerInitialized = false;
#else
    #include <SDL2/SDL.h>
#endif

double Time::GetTime()
{
	#ifdef OS_WINDOWS
		if (!g_timerInitialized)
		{
			LARGE_INTEGER li;
			if (!QueryPerformanceFrequency(&li))
			{
				std::cerr << "QueryPerformanceFrequency failed in timer initialization" << std::endl;
			}
			
			g_freq = double(li.QuadPart);
			g_timerInitialized = true;
		}
	
		LARGE_INTEGER li;
		if (!QueryPerformanceCounter(&li))
		{
			std::cerr << "QueryPerformanceCounter failed in get time!" << std::endl;
		}
		
		return double(li.QuadPart)/g_freq;
	#else
        return (double)SDL_GetTicks()/1000.0;
	#endif
}
