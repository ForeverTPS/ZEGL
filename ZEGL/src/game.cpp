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
#include "shader.h"
#include "texture.h"
#include "window.h"
#include <GL/glew.h>
#include <iostream>

enum { POSITION_VB, TEXCOORD_VB, INDEX_VB };
GLuint gVAO;
GLuint gVAB[3];
Shader shader;

float DEFAULT_LIGHT_Z = 0.075f;
float AMBIENT_INTENSITY = 0.2f;
float LIGHT_INTENSITY = 1.0f;
Vector3f LIGHT_POS(0.5f, 0.5f, DEFAULT_LIGHT_Z);
Vector3f LIGHT_COLOR(1.0f, 0.0f, 0.0f);
Vector3f AMBIENT_COLOR(0.5f, 0.5f, 0.5f);
Vector3f FALLOFF(0.4f, 3.0f, 20.0f);
Texture* rock;
Texture* rock_n;
Vector2f mouse_x;
float mouse_y;

void Game::Init(const Window& window)
{
	rock = new Texture("rock.png");
	rock_n = new Texture("rock_n.png");

	shader.Load("basic_shader");

	shader.Bind();
	shader.SetUniformi("u_diffuse", 0);
	shader.SetUniformi("u_normals", 1);
	shader.SetUniformVector2f("Resolution", Vector2f(800.0f, 600.0f));
	shader.SetUniformVector3f("LightPos", LIGHT_POS);
	shader.SetUniformVector4f("LightColor", Vector4f(LIGHT_COLOR.GetX(), LIGHT_COLOR.GetY(), LIGHT_COLOR.GetZ(), LIGHT_INTENSITY));
	shader.SetUniformVector4f("AmbientColor", Vector4f(AMBIENT_COLOR.GetX(), AMBIENT_COLOR.GetY(), AMBIENT_COLOR.GetZ(), AMBIENT_INTENSITY));
	shader.SetUniformVector3f("Falloff", FALLOFF);
	shader.UnBind();

	GLfloat vertexData[] =
	{
		-0.5f, -0.5f,	// bottom left
		0.5f, -0.5f,	// bottom right
		0.5f, 0.5f,		// top right
		-0.5f, 0.5f		// top left
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

	m_camera = new Camera();
}

void Game::ProcessInput(const Input& input, float delta)
{
	Vector2f mousePos = input.GetMousePosition();
	LIGHT_POS.SetX(mousePos.GetX()/800.0f);
	LIGHT_POS.SetY(1.0f - mousePos.GetY()/600.0f);
}

void Game::Update(float delta)
{
	shader.Bind();
	shader.UpdateUniforms(*m_camera);
	shader.SetUniformVector3f("LightPos", LIGHT_POS);
	shader.UnBind();
}

void Game::Render(Renderer* renderer)
{
	glClear(GL_COLOR_BUFFER_BIT);

	shader.Bind();

	rock->Bind(0);
	rock_n->Bind(1);

	glBindVertexArray(gVAO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);


	shader.UnBind();
}
