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

#ifndef UTIL_H
#define UTIL_H

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef WIN32
	#include <unistd.h>
#endif

#ifdef WIN32
	#define SNPRINTF _snprintf_s
#else
	#define SNPRINTF snprintf
#endif

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define INVALID_VALUE 0xFFFFFFFF

#ifndef NDEBUG
	#define ASSERT(condition, message) \
			do { \
				if (! (condition)) { \
					std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
					<< " line " << __LINE__ << ": " << message << std::endl; \
					std::exit(EXIT_FAILURE); \
				} \
			} while (false)
#else
	#define ASSERT(condition, message) do { } while (false)
#endif

namespace Util
{
	void Sleep(int milliseconds)
	{
		SDL_Delay(milliseconds);
	}

	std::vector<std::string> Split(const std::string &s, char delim)
	{
		std::vector<std::string> elems;

		const char* cstr = s.c_str();
		unsigned int strLength = (unsigned int)s.length();
		unsigned int start = 0;
		unsigned int end = 0;

		while (end <= strLength)
		{
			while (end <= strLength)
			{
				if (cstr[end] == delim)
				{
					break;
				}
				end++;
			}

			elems.push_back(s.substr(start, end - start));
			start = end + 1;
			end = start;
		}

		return elems;
	}

	template<class T> void SafeDelete(T*& pVal)
	{
		delete pVal;
		pVal = NULL;
	}

	template<class T> void SafeDeleteArray(T*& pVal)
	{
		delete[] pVal;
		pVal = NULL;
	}
};

#endif
