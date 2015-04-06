/*
* Copyright(c) 2014, Phil Sampson (http://www.zamma.co.uk)
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

#pragma once

#include <SDL.h>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>

#ifndef WIN32
	#include <unistd.h>
#endif

namespace ZEGL
{
	#if _MSC_VER
		#define snprintf _snprintf_s
	#endif

	#ifndef NDEBUG
		#define ASSERT(condition, message) \
			do { \
				if (!(condition)) {	\
					std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
					<< " line " << __LINE__ << ": " << message << std::endl; \
					LOG_CLOSE(); \
					std::exit(EXIT_FAILURE); \
				} \
			} while (false)
	#else
		#define ASSERT(condition, message) do { } while (false)
	#endif
}

namespace Util
{
	inline float ToRadian(const float deg) { return (deg * 0.01745329251994329576923690768f); }
	inline float ToDegree(const float rad) { return (rad * 57.2957795130823208767981548141f); }

	inline void	Sleep(int milliseconds)	{ SDL_Delay(milliseconds); }

	std::vector<std::string>& SplitString(const std::string& s, char delim, std::vector<std::string>& elems);
	std::vector<std::string> SplitString(const std::string& s, char delim);

	bool IsNumber(const std::string& s);

	template<typename T>
	inline void SafeDelete(T*& pVal)
	{
		delete pVal;
		pVal = NULL;
	}

	template <typename T>
	inline T StringToNumber(const std::string& s)
	{
		stringstream ss(s);
		T result;
		return ss >> result ? result : 0;
	}

	template <typename T>
	inline std::string NumberToString(T number)
	{
		std::stringstream ss;
		ss << number;
		return ss.str();
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
