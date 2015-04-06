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
	/**
	* Initialize the core of the system
	*
	* This performs all low level initialization for things such as SDL, OpenGL and
	* and utilities such as random number generation.
	*
	* \return Whether the initialization was successful or not
	*/
	bool Init();

	/**
	* Performs any 3rd party system cleanup e.g. SDL_Quit()
	*/
	void Quit();

	/**
	* Logs the compiled and linked version of SDL to the log file.
	*/
	void LogSDLInfo();

	/**
	* Logs basic information about the client system (Logical CPUs, RAM etc)
	*/
	void LogSystemInfo();

	/**
	* Logs basic OS information (only type etc)
	*
	* \param[in] window The main display window (the SDL_Window member of Window)
	*
	* \see [Window]
	*/
	void LogSubSystemInfo(SDL_Window* window);

	/**
	* Extension of LogSubSystemInfo which logs more about the OS e.g. 32bit/64bit
	*/
	void LogOSInfo();

	/**
	* Creates a new SDL_Window in the center of the screen
	*
	* \param[in] title Title for the window
	* \param[in] w Width to make the window
	* \param[in] h Height to make the window
	* \param[in] flags Creation flags for the window (See SDL documentation)
	*
	* \return An SDL_Window pointer to be assigned to a Window
	*/
	SDL_Window* CreateAndLogWindow(const char* title, int w, int h, Uint32 flags);

	/**
	* Creates a new SDL_Window at the given coordinates
	*
	* \param[in] title Title for the window
	* \param[in] x X coordinate to make the window
	* \param[in] y X coordinate to make the window
	* \param[in] w Width to make the window
	* \param[in] h Height to make the window
	* \param[in] flags Creation flags for the window (See SDL documentation)
	*
	* \return An SDL_Window pointer to be assigned to a Window
	*/
	SDL_Window* CreateAndLogWindow(const char* title, int x, int y, int w, int h, Uint32 flags);
}