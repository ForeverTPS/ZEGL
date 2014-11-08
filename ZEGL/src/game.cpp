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
#include "light.h"
#include "texture.h"
#include "tilemap.h"
#include "util.h"
#include "window.h"
#include <iostream>

#include "glm/gtc/matrix_transform.hpp"

TileMap* tileMap;

Game* Game::s_game = nullptr;

Game::Game() :
	m_camera(nullptr),
    m_window(nullptr),
	m_activeLight(nullptr) 
{
	Game::SetInstance(this);
}

Game::~Game()
{
	Util::SafeDelete(m_camera);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		Util::SafeDelete(m_lights[i]);
	}

	Util::SafeDelete(tileMap);

	glDeleteBuffers(NUM_BUFFERS, m_VAB);
	glDeleteVertexArrays(1, &m_VAO);
}

void Game::Init(const Window& window)
{
	m_window = &window;
	m_camera = new Camera(m_window);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	InitShaders();
	InitFrameBuffers();

	tileMap = new TileMap();
	
	Light* light = new Light(glm::vec3(200.0f, 200.0f, 10.0f), glm::vec3(1.0f, 0.9f, 0.9f), 800.0f, glm::vec3(0.4f, 3.0f, 20.0f));
	m_lights.push_back(light);
}

void Game::InitShaders()
{
	GLuint ambient_vs = Shaders::CreateShader("ambient_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint ambient_fs = Shaders::CreateShader("ambient_shader_fs.glsl", GL_FRAGMENT_SHADER);
	m_ambientShader.SetProgram(glCreateProgram());
	m_ambientShader.AddShader(ambient_vs);
	m_ambientShader.AddShader(ambient_fs);
	m_ambientShader.CompileShader();

	m_ambientShader.Bind();
	m_ambientShader.BindValue("uMVP", m_camera->GetTransform(m_window));
}

void Game::InitFrameBuffers() 
{
	
}

void Game::LoadResources()
{
	tileMap->LoadResources("test_level.ldf");

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VAB[STD_TILE_VB]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB[STD_TILE_VB]);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // size
	glEnableVertexAttribArray(2); // texCoords0
	glEnableVertexAttribArray(3); // texCoords1
	glEnableVertexAttribArray(4); // texCoords2
	glEnableVertexAttribArray(5); // texCoords3

	m_bytesAllocated[STD_TILE_VB] = sizeof(EntityData) * tileMap->GetActiveTilesData().size();

	glBufferData(GL_ARRAY_BUFFER, m_bytesAllocated[STD_TILE_VB], &tileMap->GetActiveTilesData()[0], GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)20);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)28);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)36);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)42);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
}

void Game::ProcessInput(const Input& input, float delta)
{
	glm::vec2 mousePos = input.GetMousePosition();
	m_lights[0]->SetPos(glm::vec3(mousePos, 0));
}

void Game::Update(float delta)
{
	tileMap->Update(delta);

	size_t bytesNeeded;

	bytesNeeded = sizeof(EntityData) * tileMap->GetActiveTilesData().size();
	if (bytesNeeded > m_bytesAllocated[STD_TILE_VB])
	{
		glBufferData(GL_ARRAY_BUFFER, bytesNeeded, &tileMap->GetActiveTilesData()[0], GL_STREAM_DRAW);
		m_bytesAllocated[STD_TILE_VB] = bytesNeeded;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytesNeeded, &tileMap->GetActiveTilesData()[0]);
	}
}

void Game::Render()
{
	Texture texture = tileMap->GetActiveTiles()[0]->GetTexture("uDiffuse");

	m_window->BindAsRenderTarget();

	glClear(GL_COLOR_BUFFER_BIT);

	m_ambientShader.Bind();
	m_ambientShader.BindTexture("uDiffuse", 0, texture.GetTextureID(0));
	glBindVertexArray(m_VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileMap->GetActiveTilesData().size());

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		m_activeLight = m_lights[i];
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_activeLight = nullptr;
}
