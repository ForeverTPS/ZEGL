/*  ____ ____ ____ ____
 * ||Z |||E |||G |||L ||
 * ||__|||__|||__|||__||
 * |/__\|/__\|/__\|/__\|
 *
 * Copyright(c) 2014, Phil Sampson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
