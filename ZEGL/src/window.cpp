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

#include "window.h"
#include "logger.h"
#include "system.h"
#include "util.h"
#include <GL/glew.h>

using namespace ZEGL;

Window::Window(int width, int height, const std::string& title) :
	m_width(width),
	m_height(height),
	m_title(title),
	m_isCloseRequested(false)
{
	m_window = System::CreateAndLogWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	m_glContext = SDL_GL_CreateContext(m_window);
	if (m_glContext == nullptr)
	{
		LOG_ERROR("Error creating GL Context! SDL Error: " << SDL_GetError());
		LOG_CLOSE();
		exit(1);
	}

	glewExperimental = GL_TRUE;

	GLenum error = GL_NO_ERROR;
	error = glewInit();
	if (error != GLEW_OK)
	{
		LOG_ERROR("Error: " << glewGetErrorString(error));
	}

	glViewport(0, 0, width, height);
}

Window::~Window()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
}

void Window::Update()
{
	m_input.ResetAllMouseButtonDown();
	m_input.ResetAllMouseButtonUp();
	
	m_input.ResetAllKeyDown();
	m_input.ResetAllKeyUp();
	
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			m_isCloseRequested = true;
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			m_input.SetMouseX(e.motion.x);
			m_input.SetMouseY(e.motion.y);
		}

		if (e.type == SDL_KEYDOWN)
		{
			int value = e.key.keysym.scancode;

			m_input.SetKey(value, true);
			m_input.SetKeyDown(value, true);
		}
		if (e.type == SDL_KEYUP)
		{
			int value = e.key.keysym.scancode;

			m_input.SetKey(value, false);
			m_input.SetKeyUp(value, true);
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			int value = e.button.button;

			m_input.SetMouse(value, true);
			m_input.SetMouseDown(value, true);
		}
		if (e.type == SDL_MOUSEBUTTONUP)
		{
			int value = e.button.button;

			m_input.SetMouse(value, false);
			m_input.SetMouseUp(value, true);
		}
	}
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void Window::BindAsRenderTarget() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GetWidth(), GetHeight());
}

void Window::SetFullScreen(bool value)
{
	int mode = value ? SDL_WINDOW_FULLSCREEN : 0;
	SDL_SetWindowFullscreen(m_window, mode);
}

