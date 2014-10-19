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

enum { POSITION_VB, TEXCOORD_VB, INDEX_VB };
GLuint gVAO;
GLuint gVAB[3];
Light* light;
Texture* rock;
Texture* rock_n;
Vector2f mouse_x;
float mouse_y;

Game::Game() :
	m_camera(nullptr),
	m_defaultShader(nullptr),
	m_ambientColor(Vector3f(0.2f, 0.2f, 0.2f)),
	m_ambientIntensity(1.0f)
{
	LOG_INIT("ZEGL");
}

Game::~Game()
{
	Util::SafeDelete(m_camera);
	Util::SafeDelete(m_defaultShader);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		Util::SafeDelete(m_lights[i]);
	}

	Util::SafeDelete(rock);
	Util::SafeDelete(rock_n);
	
	LOG_CLEANUP();
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
    
	m_defaultShader = new Shader("ambient_shader");

	m_defaultShader->Bind();
	m_defaultShader->SetUniformVector3f("L_AmbientColor", Vector3f(0.2f, 0.2f, 0.2f));
	m_defaultShader->UnBind();
    
    rock = new Texture("rock.png");
    rock_n = new Texture("rock_n.png");

	light = new Light(Shader("point_light"));
	m_lights.push_back(light);

	Shader shader = light->GetShader();
	shader.Bind();
	shader.SetUniformi("u_diffuse", 0);
	shader.SetUniformi("u_normals", 1);
	shader.UnBind();
    
    GLfloat vertexData[] =
    {
        0.0f, 0.0f,		// bottom left
        100.0f, 0.0f,	// bottom right
        100.0f, 100.0f,	// top right
        0.0f, 100.0f	// top left
    };
    
    GLfloat texData[] =
    {
        0.0f, 1.0f,		// bottom left
        1.0f, 1.0f,		// bottom right
        1.0f, 0.0f,		// top right
        0.0f, 0.0f		// top left
    };
    
    GLuint indexData[] = { 3, 2, 1, 0 };
    
    glGenVertexArrays(1, &gVAO);
    glBindVertexArray(gVAO);
    
    glGenBuffers(1, &gVAB[POSITION_VB]);
    glBindBuffer(GL_ARRAY_BUFFER, gVAB[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &gVAB[TEXCOORD_VB]);
    glBindBuffer(GL_ARRAY_BUFFER, gVAB[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), texData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    glGenBuffers(1, &gVAB[INDEX_VB]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVAB[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
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

	m_defaultShader->Bind();
	m_defaultShader->UpdateUniforms(*m_camera, this);
    rock->Bind(0);
    rock_n->Bind(1);
    glBindVertexArray(gVAO);
    glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
	m_defaultShader->UnBind();

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_EQUAL);

		m_activeLight = m_lights[i];

		Shader shader = m_activeLight->GetShader();
        
		shader.Bind();
		shader.UpdateUniforms(*m_camera, this);
        
        rock->Bind(0);
        rock_n->Bind(1);
        
        glBindVertexArray(gVAO);
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);
        
		shader.UnBind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_BLEND);
	}
}
