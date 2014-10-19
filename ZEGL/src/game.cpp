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
#include "util.h"
#include "window.h"
#include <GL/glew.h>
#include <iostream>

GLuint gVAO;
GLuint gVAB;
Light* light;
Texture* rock;
Texture* rock_n;
Vector2f mouse_x;
float mouse_y;

Game::Game() :
	m_camera(nullptr),
	m_defaultShader(Shader("ambient_shader")),
	m_ambientColor(Vector3f(0.2f, 0.2f, 0.2f)),
	m_ambientIntensity(1.0f) {}

Game::~Game()
{
	Util::SafeDelete(m_camera);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		Util::SafeDelete(m_lights[i]);
	}

	Util::SafeDelete(rock);
	Util::SafeDelete(rock_n);
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
    
    rock = new Texture("rock.png");
    rock_n = new Texture("rock_n.png");

	light = new Light(Shader("point_light"));
	m_lights.push_back(light);
    
	EntityData entity;
	entity.m_pos	= Vector3f(100.0f, 100.0f, 0.0f);
	entity.m_rot	= 0.0f;
	entity.m_scale	= 100.0f;
	entity.m_texCoords[0] = Vector2f(0.0f, 0.0f);
	entity.m_texCoords[1] = Vector2f(0.0f, 1.0f);
	entity.m_texCoords[2] = Vector2f(1.0f, 0.0f);
	entity.m_texCoords[3] = Vector2f(1.0f, 1.0f);
    
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

	glBufferData(GL_ARRAY_BUFFER, sizeof(EntityData), &entity, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (GLvoid*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)20);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)28);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)36);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)42);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
}

void Game::ProcessInput(const Input& input, float delta)
{
    Vector2f mousePos = input.GetMousePosition();
    light->SetPos((mousePos.GetX() / m_window->GetWidth()), (1.0f - mousePos.GetY() / m_window->GetHeight()));
}

void Game::Update(float delta)
{

}

void Game::Render()
{
	m_window->BindAsRenderTarget();

	glClear(GL_COLOR_BUFFER_BIT);

	m_defaultShader.Bind();
	m_defaultShader.UpdateUniforms(this);
    rock->Bind(0);
    rock_n->Bind(1);
    glBindVertexArray(gVAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
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
        
        rock->Bind(0);
        rock_n->Bind(1);
        
        glBindVertexArray(gVAO);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
        
		shader.UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
}
