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

#include "example.h"
#include "camera.h"
#include "light.h"
#include "util.h"
#include "window.h"

MyGame::MyGame() :
	m_gVAO(0),
	m_gVAB(0),
	m_light(nullptr),
	m_tileMap(nullptr)
{
}

MyGame::~MyGame()
{
	Util::SafeDelete(m_tileMap);
}

void MyGame::Init(Window* window)
{
	Game::Init(window);

	m_tileMap = new TileMap("./res/levels/test_level.ldf");

	m_light = new Light(Shader("point_light"));
	m_light->SetLightColor(glm::vec4(1.0f, 0.8f, 0.6f, 1.0f));
	m_light->SetAmbientColor(glm::vec4(0.6f, 0.6f, 1.0f, 0.2f));
	m_lights.push_back(m_light);

	glGenVertexArrays(1, &m_gVAO);
	glBindVertexArray(m_gVAO);

	glGenBuffers(1, &m_gVAB);
	glBindBuffer(GL_ARRAY_BUFFER, m_gVAB);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // size
	glEnableVertexAttribArray(2); // texCoords0
	glEnableVertexAttribArray(3); // texCoords1
	glEnableVertexAttribArray(4); // texCoords2
	glEnableVertexAttribArray(5); // texCoords3

	glBufferData(GL_ARRAY_BUFFER, sizeof(EntityData)*m_tileMap->GetActiveTilesData().size(), &m_tileMap->GetActiveTilesData()[0], GL_STREAM_DRAW);

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

void MyGame::Update(float delta)
{
	Game::Update(delta);

	glm::vec2 mousePos = m_input.GetMousePosition();
	m_light->SetPos((mousePos.x / m_window->GetWidth()), (1.0f - mousePos.y / m_window->GetHeight()), m_light->GetPos().z);

	m_tileMap->UpdateActiveTiles(m_window, m_camera->GetPos());

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(EntityData)*m_tileMap->GetActiveTilesData().size(), &m_tileMap->GetActiveTilesData()[0]);
}

void MyGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Texture texture = m_tileMap->GetActiveTiles()[0].GetTexture();
	Texture normalMap = m_tileMap->GetActiveTiles()[0].GetNormalMap();

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

		glBindVertexArray(m_gVAO);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_tileMap->GetActiveTilesData().size());

		shader.UnBind();

		glDisable(GL_BLEND);
	}

	m_activeLight = nullptr;

	Game::Render();
}