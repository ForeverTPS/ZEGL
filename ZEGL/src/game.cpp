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

#include <stdio.h>
#include <string.h>
#define FONTSTASH_IMPLEMENTATION
#include "fontstash/fontstash.h"
#include <GL/glew.h>
#define GLFONTSTASH_IMPLEMENTATION
#include "fontstash/gl3fontstash.h"

#include "game.h"
#include "camera.h"
#include "light.h"
#include "tilemap.h"
#include "util.h"
#include "window.h"
#include <iostream>

using namespace ZEGL;

GLuint gVAO;
GLuint gVAB;
Light* light;
TileMap* tileMap;
int testFont = FONS_INVALID;

Game::Game() :
	m_camera(nullptr),
    m_window(nullptr),
	m_ambientColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)),
    m_activeLight(nullptr),
	m_fontContext(nullptr)
{
}

Game::~Game()
{
	Util::SafeDelete(m_camera);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		Util::SafeDelete(m_lights[i]);
	}

	Util::SafeDelete(tileMap);

	gl3fonsDelete(m_fontContext);
}

void Game::Init(const Window* window)
{
	m_window = window;
	m_camera = new Camera(m_window);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

	m_fontContext = gl3fonsCreate(512, 512, FONS_ZERO_TOPLEFT);
	testFont = fonsAddFont(m_fontContext, "sans", "./res/fonts/DroidSerif-Regular.ttf");
    
	tileMap = new TileMap("test_level.ldf");

	light = new Light(Shader("point_light"));
	light->SetLightColor(glm::vec4(1.0f, 0.8f, 0.6f, 1.0f));
	light->SetAmbientColor(glm::vec4(0.6f, 0.6f, 1.0f, 0.2f));
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
	tileMap->UpdateActiveTiles(m_window, m_camera->GetPos());

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(EntityData)*tileMap->GetActiveTilesData().size(), &tileMap->GetActiveTilesData()[0]);
}

void Game::Render()
{
	m_window->BindAsRenderTarget();

	glClear(GL_COLOR_BUFFER_BIT);

	Texture texture = tileMap->GetActiveTiles()[0].GetTexture();
	Texture normalMap = tileMap->GetActiveTiles()[0].GetNormalMap();

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

		glDisable(GL_BLEND);
	}
    
    m_activeLight = nullptr;

	gl3fonsProjection(m_fontContext, (GLfloat*)&(m_camera->GetTransform(m_window)[0][0]), m_window->GetWidth(), m_window->GetHeight());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	fonsClearState(m_fontContext);
	fonsSetSize(m_fontContext, 16.0f);
	fonsSetFont(m_fontContext, testFont);
	fonsSetColor(m_fontContext, gl3fonsRGBA(255,255,255,255));
	fonsSetAlign(m_fontContext, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
	fonsDrawText(m_fontContext, 5, 5, m_fps, NULL);

	glDisable(GL_BLEND);
}

void Game::SetFPSDisplay(int frames)
{
	snprintf(m_fps, sizeof(m_fps), "FPS: %d", frames);
}
