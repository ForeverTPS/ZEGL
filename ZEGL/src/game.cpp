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
#include "shader.h"
#include "texture.h"
#include "tilemap.h"
#include "util.h"
#include "window.h"
#include <GL/glew.h>
#include <iostream>

GLuint gVAO;
GLuint gVAB;
Light* light;
TileMap* tileMap;

Game::Game() :
	m_camera(nullptr),
    m_window(nullptr),
	m_defaultShader(Shader("ambient_shader")),
	m_ambientColor(glm::vec3(0.1f)),
	m_ambientIntensity(1.0f),
    m_activeLight(nullptr) {}

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
    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    
	tileMap = new TileMap("test_level.ldf");

	light = new Light(Shader("point_light"));
	m_lights.push_back(light);
    
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    glGenBuffers(1, &gVAB);
    glBindBuffer(GL_ARRAY_BUFFER, gVAB);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // size
	glEnableVertexAttribArray(2); // texCoords0
	glEnableVertexAttribArray(3); // texCoords1
	glEnableVertexAttribArray(4); // texCoords2
	glEnableVertexAttribArray(5); // texCoords3

	glBufferData(GL_ARRAY_BUFFER, sizeof(EntityData)*tileMap->GetActiveTilesData().size(), &tileMap->GetActiveTilesData()[0], GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)20);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)28);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)36);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)44);

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
	light->SetPos((mousePos.x / m_window->GetWidth()), (1.0f - mousePos.y / m_window->GetHeight()), light->GetPos().z);
}

void Game::Update(float delta)
{
	tileMap->UpdateActiveTiles(m_camera->GetPos());

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(EntityData)*tileMap->GetActiveTilesData().size(), &tileMap->GetActiveTilesData()[0]);
}

void Game::Render()
{
	m_window->BindAsRenderTarget();

	glClear(GL_COLOR_BUFFER_BIT);

	Texture texture = tileMap->GetActiveTiles()[0].GetTexture();
	Texture normalMap = tileMap->GetActiveTiles()[0].GetNormalMap();

	m_defaultShader.Bind();
	m_defaultShader.UpdateUniforms(this);
	texture.Bind(0);
	normalMap.Bind(1);
    glBindVertexArray(gVAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileMap->GetActiveTilesData().size());
	m_defaultShader.UnBind();

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		m_activeLight = m_lights[i];

		Shader shader = m_activeLight->GetShader();
        
		shader.Bind();
		shader.UpdateUniforms(this);
        
		texture.Bind(0);
		normalMap.Bind(1);
        
        glBindVertexArray(gVAO);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileMap->GetActiveTilesData().size());
        
		shader.UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
    
    m_activeLight = nullptr;
}
