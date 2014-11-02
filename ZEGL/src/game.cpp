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
}

void Game::Init(const Window& window)
{
	m_window = &window;
	m_camera = new Camera(m_window);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

	tileMap = new TileMap();
}

void Game::InitShaders()
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

	glGenBuffers(1, &m_VAB[OCCLUDER_TILE_VB]);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB[OCCLUDER_TILE_VB]);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // size
	glEnableVertexAttribArray(2); // texCoords0
	glEnableVertexAttribArray(3); // texCoords1
	glEnableVertexAttribArray(4); // texCoords2
	glEnableVertexAttribArray(5); // texCoords3

	m_bytesAllocated[OCCLUDER_TILE_VB] = sizeof(EntityData) * tileMap->GetActiveOccluderTilesData().size();

	glBufferData(GL_ARRAY_BUFFER, m_bytesAllocated[OCCLUDER_TILE_VB], &tileMap->GetActiveOccluderTilesData()[0], GL_STREAM_DRAW);

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

	bytesNeeded = sizeof(EntityData)* tileMap->GetActiveOccluderTilesData().size();
	if (bytesNeeded > m_bytesAllocated[OCCLUDER_TILE_VB])
	{
		glBufferData(GL_ARRAY_BUFFER, bytesNeeded, &tileMap->GetActiveOccluderTilesData()[0], GL_STREAM_DRAW);
		m_bytesAllocated[OCCLUDER_TILE_VB] = bytesNeeded;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytesNeeded, &tileMap->GetActiveOccluderTilesData()[0]);
	}
}

void Game::Render()
{
	m_window->BindAsRenderTarget();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		m_activeLight = m_lights[i];

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
    
    m_activeLight = nullptr;
}
