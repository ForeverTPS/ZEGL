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

#pragma once

#include <SDL2/SDL.h>
#include <cctype>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#ifndef WIN32
	#include <unistd.h>
#endif

namespace ZEGL
{
	#if _MSC_VER
		#define WIN32_LEAN_AND_MEAN
		#define snprintf	_snprintf_s
		#define vsprintf	vsprintf_s
		#define vsnprintf	vsnprintf_s
		#define	getenv		_dupenv_s
	#endif

	#ifndef NDEBUG
		#define ASSERT(condition, message) \
			do { \
				if (!(condition)) {	\
					std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
					<< " line " << __LINE__ << ": " << message << std::endl; \
					LOG_CLEANUP(); \
					std::exit(EXIT_FAILURE); \
				} \
			} while (false)
	#else
		#define ASSERT(condition, message) do { } while (false)
	#endif
}

namespace Util
{
	#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
	#define INVALID_VALUE 0xFFFFFFFF

	const float MATH_PI = 3.1415926535897932384626433832795f;
	const float DEG2RAD = 0.01745329251994329576923690768f;
	const float RAD2DEG = 57.2957795130823208767981548141f;

	inline float ToRadian(const float Degree) { return (Degree * DEG2RAD); }
	inline float ToDegree(const float Radian) { return (Radian * RAD2DEG); }

	inline void Sleep(int milliseconds) { SDL_Delay(milliseconds); }

	std::vector<std::string>&	SplitString(const std::string& s, char delim, std::vector<std::string>& elems);
	std::vector<std::string>	SplitString(const std::string& s, char delim);

	template<typename T>
	void SafeDelete(T*& pVal)
	{
		delete pVal;
		pVal = NULL;
	}

	template <typename T>
	T StringToNumber(const std::string& s)
	{
		stringstream ss(s);
		T result;
		return ss >> result ? result : 0;
	}

	template <typename T>
	std::string NumberToString(T number)
	{
		std::stringstream ss;
		ss << number;
		return ss.str();
	}

	inline bool IsNumber(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}

	inline bool ContainsFlag(int flags, int flag)
	{
		return ((flags & flag) == flag);
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

	template<class T>
	class MySingleton
	{
	public:
		static T* GetInstance()
		{
			static T s_instance;
			return &s_instance;
		}

	private:
		MySingleton();
		~MySingleton();
		MySingleton(MySingleton const&);
		MySingleton& operator=(MySingleton const&);
	};
};
