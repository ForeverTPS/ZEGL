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
#include <sstream>
#include <vector>

#ifndef WIN32
	#include <unistd.h>
#endif

#if _MSC_VER
	#define WIN32_LEAN_AND_MEAN
	#define fopen		fopen_s
	#define snprintf	_snprintf_s
	#define strcpy		strcpy_s
	#define vsprintf	vsprintf_s
	#define vsnprintf	vsnprintf_s
	#define	getenv		_dupenv_s
#endif

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define INVALID_VALUE 0xFFFFFFFF

#ifndef NDEBUG
	#define ASSERT(condition, message) \
			do { \
				if (! (condition)) { \
					std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
					<< " line " << __LINE__ << ": " << message << std::endl; \
					LOG_CLEANUP(); \
					std::exit(EXIT_FAILURE); \
				} \
			} while (false)
#else
	#define ASSERT(condition, message) do { } while (false)
#endif

#define MATH_PI 3.1415926535897932384626433832795
#define ToRadians(x) (float)(((x) * MATH_PI / 180.0f))
#define ToDegrees(x) (float)(((x) * 180.0f / MATH_PI))

namespace Util
{
	void Sleep(int milliseconds);

	std::vector<std::string> Split(const std::string &s, char delim);

	template<class T>
	void SafeDelete(T*& pVal)
	{
		delete pVal;
		pVal = NULL;
	}

	inline int StringToNumber(const char* str)
	{
		int val = 0;
		while (*str)
		{
			val = val * 10 + (*str++ - '0');
		}
		return val;
	}
	
	template <class T>
	inline bool StringToNumber(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
	{
		std::istringstream iss(s);
		return !(iss >> f >> t).fail();
	}

	template <typename T>
	inline std::string NumberToString(T num)
	{
		std::ostringstream ostr;
		ostr << num;

		return ostr.str();
	}

	template<typename T>
	inline T Clamp(const T &a, const T &min, const T &max)
	{
		if (a < min)
		{
			return min;
		}
		else if (a > max)
		{
			return max;
		}
		else
		{
			return a;
		}
	}
};

#endif
