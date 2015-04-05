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

/**
* Using a namespace rather than a static class or singleton etc to provide
* random number generation using std::mt19937 as the generator and std::random_device
* as a seeder
*/
namespace Random
{
	/**
	* Initialize the random number system by seeding std::mt19937 with std::random_device
	*/
	void Init();

	/**
	* Generate a random integer
	*
	* \param[in] max Maximum value for the random number
	*
	* \return A random integer in the range 0 - max
	*/
	int	RandInt(int max);

	/**
	* Generate a random integer
	*
	* \param[in] min Minimum value for the random number
	* \param[in] max Maximum value for the random number
	*
	* \return A random integer in the range min - max
	*/
	int	RandInt(int min, int max);

	/**
	* Generate a random float
	*
	* \param[in] max Maximum value for the random number
	*
	* \return A random integer in the range 0.0f - max
	*/
	float RandFloat(float max);

	/**
	* Generate a random float
	*
	* \param[in] min Minimum value for the random number
	* \param[in] max Maximum value for the random number
	*
	* \return A random float in the range min - max
	*/
	float RandFloat(float min, float max);

	/**
	* Generate a random true or false value
	*
	* \return A random bool of either true or false
	*/
	bool RandBool();
}