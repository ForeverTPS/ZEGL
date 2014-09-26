#include "game.h"
#include "camera.h"
#include "input.h"
#include "window.h"
#include <GL/glew.h>
#include <iostream>

#include "shader.h"

GLuint gVAO = 0;
GLuint gVAB = 0;
GLuint gIBO = 0;
Shader shader;

void Game::Init(const Window& window)
{
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	shader.Load("basic_shader");

	GLfloat vertexData[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	GLuint indexData[] = { 3, 2, 1, 0 };

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	glGenBuffers(1, &gVAB);
	glBindBuffer(GL_ARRAY_BUFFER, gVAB);
	glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &gIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

	m_camera = new Camera();
}

void Game::ProcessInput(const Input& input, float delta)
{
}

void Game::Update(float delta)
{
	shader.Bind();
	shader.UpdateUniforms(*m_camera);
	shader.UnBind();
}

void Game::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	shader.Bind();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gVAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

	glDisableVertexAttribArray(0);

	shader.UnBind();
}
