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

#include <stdio.h>
#include <string.h>
#define FONTSTASH_IMPLEMENTATION
#include "fontstash/fontstash.h"
#include <GL/glew.h>
#define GLFONTSTASH_IMPLEMENTATION
#include "fontstash/gl3fontstash.h"

#include "game.h"
#include "audio.h"
#include "camera.h"
#include "light.h"
#include "tilemap.h"
#include "util.h"
#include "window.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace ZEGL;

Game* Game::s_gameInstance = nullptr;

Game::Game() :
	m_camera(nullptr),
	m_window(nullptr),
	m_ambientColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)),
	m_activeLight(nullptr),
	m_fontContext(nullptr)
{
	Game::s_gameInstance = this;
}

Game::~Game()
{
	Util::SafeDelete(m_camera);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		Util::SafeDelete(m_lights[i]);
	}

	gl3fonsDelete(m_fontContext);

	Audio::Clear();
}

void Game::Init(Window* window)
{
	m_window = window;
	m_camera = new Camera();

	Audio::Init();
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	m_fontContext = gl3fonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	m_debugFont = fonsAddFont(m_fontContext, "sans", "./res/fonts/DroidSerif-Regular.ttf");
}

void Game::Update(float delta)
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
			m_window->Close();
		}

		if (e.type == SDL_MOUSEMOTION)
		{
			m_input.SetMouseX(e.motion.x);
			m_input.SetMouseY(e.motion.y);
		}

		if (e.type == SDL_KEYDOWN)
		{
			int value = e.key.keysym.sym;

			m_input.SetKey(value, true);
			m_input.SetKeyDown(value, true);
		}
		if (e.type == SDL_KEYUP)
		{
			int value = e.key.keysym.sym;

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

void Game::Render()
{
	glm::mat4 ortho = glm::ortho(0.0f, (float)m_window->GetWidth(), (float)m_window->GetHeight(), 0.0f, -1.0f, 1.0f);
	gl3fonsProjection(m_fontContext, (GLfloat*)&(ortho[0][0]), m_window->GetWidth(), m_window->GetHeight());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fonsClearState(m_fontContext);
	fonsSetSize(m_fontContext, 16.0f);
	fonsSetFont(m_fontContext, m_debugFont);
	fonsSetColor(m_fontContext, gl3fonsRGBA(255,255,255,255));
	fonsSetAlign(m_fontContext, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
	fonsDrawText(m_fontContext, 5, 5, m_fps, NULL);

	glDisable(GL_BLEND);
}

void Game::SetFPSDisplay(int frames)
{
	snprintf(m_fps, sizeof(m_fps), "FPS: %d", frames);
}
