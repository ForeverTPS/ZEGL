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

#include "system.h"
#include "logger.h"
#include "random.h"
#include "util.h"

bool ZEGL::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LOG_ERROR("SDL could not initialize! SDL Error: " << SDL_GetError());
		return false;
	}

	Random::Init();

	return true;
}

void ZEGL::Quit()
{
	SDL_Quit();
}

void ZEGL::LogSDLInfo()
{

}

void ZEGL::LogSystemInfo()
{

}

void ZEGL::LogSubSystemInfo(SDL_Window* window)
{

}

void ZEGL::LogOSInfo()
{

}

SDL_Window* ZEGL::CreateAndLogWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
{
	return 0;
}