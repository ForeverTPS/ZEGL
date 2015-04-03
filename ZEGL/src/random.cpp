/**
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

#include "random.h"
#include <random>

static std::mt19937 generator;

void Random::Init()
{
	generator.seed(std::random_device()());
}

int Random::RandInt(int max)
{
	std::uniform_int_distribution<std::mt19937::result_type> range(0, max);
	return range(generator);
}

int Random::RandInt(int min, int max)
{
	std::uniform_int_distribution<std::mt19937::result_type> range(min, max);
	return range(generator);
}

float Random::RandFloat(float max)
{
	std::uniform_real_distribution<float> range(0.0f, max);
	return range(generator);
}

float Random::RandFloat(float min, float max)
{
	std::uniform_real_distribution<float> range(min, max);
	return range(generator);
}

bool Random::RandBool()
{
	return (generator() % 2) == 0;
}