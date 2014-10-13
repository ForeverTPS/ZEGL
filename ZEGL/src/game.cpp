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

#include "game.h"
#include "camera.h"
#include "lighting.h"
#include "logfile.h"
#include "shader.h"
#include "texture.h"
#include "util.h"
#include "window.h"
#include <GL/glew.h>
#include <iostream>

Game::Game() :
	m_camera(new Camera()),
	m_defaultShader(nullptr)
{
	LOG_INIT("ZEGL");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

Game::~Game()
{
	Util::SafeDelete(m_camera);
	Util::SafeDelete(m_defaultShader);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		Util::SafeDelete(m_lights[i]);
	}
	
	LOG_CLEANUP();
}

void Game::Init(const Window& window)
{
	m_window = &window;
	m_defaultShader = new Shader("ambient_shader");

	m_defaultShader->Bind();
	m_defaultShader->SetUniformVector3f("AmbientColor", Vector3f(0.2f, 0.2f, 0.2f));
	m_defaultShader->UnBind();
}

void Game::ProcessInput(const Input& input, float delta)
{
}

void Game::Update(float delta)
{
	m_defaultShader->Bind();
	m_defaultShader->UpdateUniforms(*m_camera);
	m_defaultShader->UnBind();
}

void Game::Render()
{
	m_window->BindAsRenderTarget();

	glClear(GL_COLOR_BUFFER_BIT);

	m_defaultShader->Bind();

	m_defaultShader->UnBind();

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
}
