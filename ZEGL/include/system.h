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

/**
* Using a namespace rather than a static class or singleton etc to provide
* generic engine system operations
*/
namespace System
{
	bool Init();
	void Quit();

	void LogSDLInfo();
	void LogSystemInfo();
	void LogSubSystemInfo(SDL_Window* window);
	void LogOSInfo();

	SDL_Window* CreateAndLogWindow(const char* title, int w, int h, Uint32 flags);
	SDL_Window* CreateAndLogWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
}