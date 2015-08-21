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

static const GLfloat s_screenQuad[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
};

MyGame::MyGame() :
	m_light(nullptr),
	m_tileMap(nullptr),
	m_testSprite(nullptr),
	m_testAnimSprite(nullptr)
{
}

MyGame::~MyGame()
{
	Util::SafeDelete(m_tileMap);
	Util::SafeDelete(m_testSprite);
	Util::SafeDelete(m_testAnimSprite);

	if (m_screenVBO)
	{
		glDeleteBuffers(1, &m_screenVBO);
	}
	if (m_screenVAO)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_screenVAO);
	}
}

void MyGame::Init(Window* window)
{
	Game::Init(window);

	glGenVertexArrays(1, &m_screenVAO);
	glBindVertexArray(m_screenVAO);
	glGenBuffers(1, &m_screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_screenQuad), s_screenQuad, GL_STATIC_DRAW);

	m_tileMap = new TileMap("./res/levels/test_level.ldf");

	m_light = new Light(Shader("./res/shaders/point_light"));
	m_light->SetLightColor(glm::vec4(1.0f, 0.8f, 0.6f, 1.0f));
	m_light->SetAmbientColor(glm::vec4(0.6f, 0.6f, 1.0f, 0.2f));
	m_lights.push_back(m_light);

	m_spriteBatch.Init();

	Texture rock("./res/textures/rock.png");
	TextureAtlas atlas("./res/textures/test_atlas.atl");
	m_testSprite = new Sprite(rock, atlas, "rock", glm::vec3(300.0f, 300.0f, 0.0f));

	Texture gb("./res/textures/gb.png");
	TextureAtlas gbAtlas("./res/textures/gb.atl");
	std::vector<std::string> frames;
	frames.push_back("gb_walk-0-0");
	frames.push_back("gb_walk-1-0");
	frames.push_back("gb_walk-2-0");
	frames.push_back("gb_walk-3-0");
	frames.push_back("gb_walk-4-0");
	frames.push_back("gb_walk-5-0");
	m_testAnimSprite = new AnimatedSprite(gb, gbAtlas, frames, glm::vec3(600.0f, 400.0f, 0.0f));
	m_testAnimSprite->SetLooping(true);
	m_testAnimSprite->SetSpeed(75);
	m_testAnimSprite->Scale(0.5f);
	m_testAnimSprite->Start();

	Audio::AddMusic("./res/sound/music/MI_Intro.mp3", "Intro");
	Audio::PlayMusic("Intro");
}

void MyGame::Update(float delta)
{
	Game::Update(delta);

	//m_camera->SetPos(m_camera->GetPos().x - 1.0f, m_camera->GetPos().y);

	glm::vec2 mousePos = m_input.GetMousePosition();
	m_light->SetPos((mousePos.x / m_window->GetWidth()), (1.0f - mousePos.y / m_window->GetHeight()), m_light->GetPos().z);

	m_tileMap->Update(delta);

	m_testSprite->SetRot(m_testSprite->GetRot() + 0.5f * delta);
	m_testAnimSprite->Update(delta);
}

void MyGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_EQUAL);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		m_activeLight = m_lights[i];
		m_tileMap->Render();
	}

	m_activeLight = nullptr;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_spriteBatch.Begin();
	m_testSprite->Draw(m_spriteBatch);
	m_testAnimSprite->Draw(m_spriteBatch);
	m_spriteBatch.End();

	glDisable(GL_BLEND);

	Game::Render();
}