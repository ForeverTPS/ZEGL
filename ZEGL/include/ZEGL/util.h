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

/**
* Collection of generic functions for commonly used tasks that do not belong
* in other classes.
*/
namespace Util
{
	/**
	* Converts a degree angle to radians.
	*
	* \param[in] deg Angle in degrees to be converted
	*
	* \return Angle in radians
	*/
	inline float ToRadian(const float deg) { return (deg * 0.01745329251994329576923690768f); }

	/**
	* Converts a radian angle to degrees.
	*
	* \param[in] rad Angle in radians to be converted
	*
	* \return Angle in degrees
	*/
	inline float ToDegree(const float rad) { return (rad * 57.2957795130823208767981548141f); }

	/**
	* Causes the application to pause for an amount of time.
	*
	* \param[in] milliseconds Amount of time to wait
	*/
	inline void	Sleep(int milliseconds)	{ SDL_Delay(milliseconds); }

	/**
	* Splits a string using a given character delimiter.
	*
	* \param[in] s The string to be split
	* \param[in] delim Character to use as a split delimiter
	*
	* \return The string components split in to a std::vector
	*/
	std::vector<std::string> SplitString(const std::string& s, char delim);

	/**
	* Test if a string is a numerical value.
	*
	* \param[in] s The string to test if it is a number
	*
	* \return True if the string is a number otherwise false
	*/
	bool IsNumber(const std::string& s);

	/**
	* Safely deletes instances by using nullptr checking
	*
	* \param[in] pVal Pointer to the object to be deleted
	*/
	template<typename T>
	inline void SafeDelete(T*& pVal)
	{
		delete pVal;
		pVal = NULL;
	}

	/**
	* Convert a string to a number.
	*
	* \param[in] s The string to be converted to a number
	*
	* \return A number of the typecast specified or 0 if not possible
	*/
	template <typename T>
	inline T StringToNumber(const std::string& s)
	{
		stringstream ss(s);
		T result;
		return ss >> result ? result : 0;
	}

	/**
	* Convert a number to a string
	*
	* \param[in] number The number to be converted to a string
	*
	* \return A string containing the number passed in
	*/
	template <typename T>
	inline std::string NumberToString(T number)
	{
		std::stringstream ss;
		ss << number;
		return ss.str();
	}

	/**
	* Takes a value and returns clamps it within a specified range.
	*
	* \param[in] a Value to clamp
	* \param[in] min Minimum value (floor)
	* \param[in] max Maximum value (ceil)
	*
	* \return The resulting value which will be either the original or the min/max
	* specified if lower/higher respectively
	*/
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
