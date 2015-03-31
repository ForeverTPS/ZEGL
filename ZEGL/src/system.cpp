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
#include "platform.h"
#include "random.h"
#include "util.h"
#include <SDL_syswm.h>

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
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);

	LOG_INFO("Compiled SDL: " << (int)compiled.major << "." << (int)compiled.minor << "." << (int)compiled.patch);
	LOG_INFO("Linked SDL: " << (int)linked.major << "." << (int)linked.minor << "." << (int)linked.patch);
	LOG_NEWLINE();
}

void ZEGL::LogSystemInfo()
{
	LOG_INFO("Logical CPU Cores: " << SDL_GetCPUCount());
	LOG_INFO("RAM: " << SDL_GetSystemRAM() << " MB");
	LOG_NEWLINE();
}

void ZEGL::LogSubSystemInfo(SDL_Window* window)
{
	SDL_SysWMinfo info;

	SDL_VERSION(&info.version);
	if (SDL_GetWindowWMInfo(window, &info))
	{
		const char* subsystem = "Unknown System";
		switch (info.subsystem)
		{
		case SDL_SYSWM_UNKNOWN:   break;
		case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows";      break;
		case SDL_SYSWM_X11:       subsystem = "X Window System";        break;
		case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;
		case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";             break;
		case SDL_SYSWM_UIKIT:     subsystem = "UIKit";                  break;
		case SDL_SYSWM_WAYLAND:	  subsystem = "Wayland";				break;
		case SDL_SYSWM_MIR:		  subsystem = "Mir";					break;
		case SDL_SYSWM_WINRT:	  subsystem = "Win RT";					break;
		}
		LOG_INFO("Subsystem: " << subsystem);
	}
	else
	{
		LOG_WARNING("Couldn't get used Subsystem : " << SDL_GetError());
	}

	LOG_NEWLINE();
}

void ZEGL::LogOSInfo()
{
#if defined(OS_WINDOWS_32)
	LOG_INFO("Windows 32-bit");
#elif defined(OS_WINDOWS_64)
	LOG_INFO("Windows 64-bit");
#elif defined(OS_UNIX)
	LOG_INFO("Unix");
#elif defined(OS_APPLE)
	LOG_INFO("Mac OSX / iOS");
#elif defined(OS_LINUX)
	LOG_INFO("Linux");
#else
	LOG_INFO("Other OS");
#endif

	LOG_NEWLINE();
}

SDL_Window* ZEGL::CreateAndLogWindow(const char* title, int w, int h, Uint32 flags)
{
	return CreateAndLogWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
}

SDL_Window* ZEGL::CreateAndLogWindow(const char* title, int x, int y, int w, int h, Uint32 flags)
{
	LogSDLInfo();
	LogSystemInfo();
	LogOSInfo();

	SDL_Window* window = SDL_CreateWindow(title, x, y, w, h, flags);
	if (window == nullptr)
	{
		LOG_ERROR("Error creating Window! Error: " << SDL_GetError());
		return nullptr;
	}

	LogSubSystemInfo(window);

	return window;
}