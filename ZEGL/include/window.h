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
#include <glm/glm.hpp>
#include <string>

namespace ZEGL
{
	/**
	* The window is used for displaying the game to screen. It acts as a wrapper for the
	* SDL_Window with some extra functionality.
	*/
	class Window
	{
	public:
		/**
		* Creates an SDL_Window using an SDL_GLContext with the given width and height.
		*
		* \param[in] width The width to make the window
		* \param[in] height The height to make the window
		* \param[in] title The name to give the window (appears in menu bar etc)
		*/
		Window(int width, int height, const std::string& title);
		~Window();

		/**
		* Swaps the GL buffers round so that the last rendered buffer is now displayed 
		* on screen.
		*/
		void SwapBuffers();

		/**
		* Gets the window width in pixels.
		*
		* \return The window width in pixels
		*/
		inline int GetWidth() const { return m_width; }

		/**
		* Gets the window height in pixels.
		*
		* \return The window height in pixels
		*/
		inline int GetHeight() const { return m_height; }

		/**
		* Gets the window aspect ratio.
		*
		* \return The window aspect ratio
		*/
		inline float GetAspect() const { return (float)m_width / (float)m_height; }

		/**
		* Gets the coordinates of the center point in the window.
		*
		* \return The center point of the window as a glm::vec2
		*/
		inline glm::vec2 GetCenter() const { return glm::vec2((float)m_width / 2.0f, (float)m_height / 2.0f); }

		/**
		* Gets the SDL_Window.
		*
		* \return The SDL_Window which is encapsulated by this class
		*/
		inline SDL_Window* GetSDLWindow() { return m_window; }

		/**
		* Tells the window to close on the next frame.
		*/
		inline void Close() { m_isCloseRequested = true; }

		/**
		* Check if the window is closing.
		*
		* \return Whether the window is flagged to close
		*/
		inline bool IsCloseRequested() const { return m_isCloseRequested; }

		/**
		* Make the window full screen or windowed
		*
		* \param[in] value Whether or not to make the window full screen true/false
		*/
		void SetFullScreen(bool value);

	protected:
	private:
		Window(const Window& other) = delete;
		void operator=(const Window& other) = delete;

		static bool   s_glewInitialized;

		int           m_width;
		int           m_height;
		std::string   m_title;				
		SDL_Window*   m_window;
		SDL_GLContext m_glContext;
		bool          m_isCloseRequested;
	};
}