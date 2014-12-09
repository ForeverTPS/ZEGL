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

#include "stdafx.h"

#include "game.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "tilemap.h"
#include "util.h"
#include "window.h"

const float lightSize = 300.0f;

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

	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);

	glDeleteTextures(1, &m_occlusionFBO_tex);
	glDeleteTextures(1, &m_shadowMapFBO_tex);

	glDeleteFramebuffers(1, &m_occlusionFBO);
	glDeleteFramebuffers(1, &m_shadowMapFBO);
}

void Game::Init(const Window& window)
{
	srand(clock());

	m_window = &window;
	m_camera = new Camera(m_window);
    
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Make fullscreen quad vbo.
	glm::vec3 verts[6] = {
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(1, 1, 0),
		glm::vec3(1, 0, 0)
	};

	m_VBO_size = sizeof(verts);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_VBO_size, &verts[0], GL_STATIC_DRAW);

	InitShaders();
	InitFrameBuffers();

	tileMap = new TileMap();
	
	AddLight(new Light(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 1.0f), 800.0f, glm::vec3(0.3f, 3.0f, 20.0f)));
}

void Game::LoadResources()
{
	tileMap->LoadResources("test_level.ldf");
}

void Game::InitShaders()
{
	GLuint sprite_vs = Shaders::CreateShader("sprite_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint sprite_fs = Shaders::CreateShader("sprite_shader_fs.glsl", GL_FRAGMENT_SHADER);
	m_spriteShader.SetProgram(glCreateProgram());
	m_spriteShader.AddShader(sprite_vs);
	m_spriteShader.AddShader(sprite_fs);
	m_spriteShader.CompileShader();

	m_spriteShader.Bind();
	m_spriteShader.BindValue("uAmbient", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

	GLuint shadowMap_vs = Shaders::CreateShader("sprite_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint shadowMap_fs = Shaders::CreateShader("shadowmap_shader_fs.glsl", GL_FRAGMENT_SHADER);
	m_shadowMapShader.SetProgram(glCreateProgram());
	m_shadowMapShader.AddShader(shadowMap_vs);
	m_shadowMapShader.AddShader(shadowMap_fs);
	m_shadowMapShader.CompileShader();

	GLuint shadowRender_vs = Shaders::CreateShader("sprite_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint shadowRender_fs = Shaders::CreateShader("shadowrender_shader_fs.glsl", GL_FRAGMENT_SHADER);
	m_shadowRenderShader.SetProgram(glCreateProgram());
	m_shadowRenderShader.AddShader(shadowRender_vs);
	m_shadowRenderShader.AddShader(shadowRender_fs);
	m_shadowRenderShader.CompileShader();
}

void Game::InitFrameBuffers() 
{
	glGenFramebuffers(1, &m_occlusionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_occlusionFBO);

	glGenTextures(1, &m_occlusionFBO_tex);
	glBindTexture(GL_TEXTURE_2D, m_occlusionFBO_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)lightSize, (int)lightSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_occlusionFBO_tex, 0);

	GLenum occlusionDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, occlusionDrawBuffers);

	glGenFramebuffers(1, &m_shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);

	glGenTextures(1, &m_shadowMapFBO_tex);
	glBindTexture(GL_TEXTURE_2D, m_shadowMapFBO_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)lightSize, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_shadowMapFBO_tex, 0);
	
	GLenum shadowMapDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, shadowMapDrawBuffers);
}

void Game::ProcessInput(const Input& input, float delta)
{
	glm::vec2 mousePos = input.GetMousePosition();
	m_lights[m_lights.size()-1]->SetPos(glm::vec3(mousePos, 0.0f));

	if (input.GetMouseDown(Input::MOUSE_LEFT_BUTTON))
	{
		AddLight(new Light(glm::vec3(mousePos, 0.0f), glm::vec3(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX), 800.0f, glm::vec3(0.3f, 3.0f, 20.0f)));
	}
}

void Game::Update(float delta)
{
	tileMap->Update(delta);
}

void Game::Draw()
{
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	m_window->BindAsRenderTarget();
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 1 - Render sprites in full color
	m_spriteShader.Bind();
	m_spriteShader.BindValue("uMVP", m_camera->GetTransform());
	m_spriteShader.BindValue("uSize", glm::vec2(64.0f, 64.0f));

	tileMap->Draw(m_spriteShader);

	//////////////////////////////////////////////////////////////////////////
	m_spriteShader.Bind();
	m_spriteShader.BindVertices(m_VBO);
	m_spriteShader.BindValue("uMVP", m_camera->GetTransform(true));
	m_spriteShader.BindValue("uPos", glm::vec3(m_window->GetWidth() - lightSize, 0.0f, 0.0f));
	m_spriteShader.BindValue("uSize", glm::vec2(lightSize, lightSize));
	m_spriteShader.BindTexture("uDiffuse", 0, m_occlusionFBO_tex);
	m_spriteShader.Draw(6);
	m_spriteShader.BindValue("uMVP", m_camera->GetTransform(true));
	m_spriteShader.BindValue("uPos", glm::vec3(m_window->GetWidth() - lightSize, lightSize + 5.0f, 0.0f));
	m_spriteShader.BindValue("uSize", glm::vec2(lightSize, 5.0f));
	m_spriteShader.BindTexture("uDiffuse", 0, m_shadowMapFBO_tex);
	m_spriteShader.Draw(6);
	//////////////////////////////////////////////////////////////////////////

	glBindVertexArray(m_VAO);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		m_activeLight = m_lights[i];
		DrawLight();
	}

	m_activeLight = nullptr;
}

void Game::DrawLight()
{
	float mx = m_activeLight->GetPos().x;
	float my = (float)m_window->GetHeight() - m_activeLight->GetPos().y;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 2 - Render light region to occlusion FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_occlusionFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	m_camera->SetPos(glm::vec3(mx - lightSize / 2.0f, my - lightSize / 2.0f, 0.0f));
	//m_camera->SetOrtho(0.0f, 256.0f, 0.0f, 256.0f);

	m_spriteShader.Bind();
	m_spriteShader.BindValue("uMVP", m_camera->GetTransform(true));
	m_spriteShader.BindValue("uSize", glm::vec2(64.0f, 64.0f));

	tileMap->DrawShadowLayer(m_spriteShader);
	
	//m_camera->SetOrthoToWindow();
	m_camera->RevertToLast();

	// 3 - Build a 1D shadow map from occlusion FBO
	glBindFramebuffer(GL_FRAMEBUFFER, m_shadowMapFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	//m_camera->SetOrtho(0.0f, 256.0f, 0.0f, 1.0f);

	m_shadowMapShader.Bind();
	m_shadowMapShader.BindVertices(m_VBO);
	m_shadowMapShader.BindValue("uMVP", m_camera->GetTransform(true));
	m_shadowMapShader.BindValue("uPos", glm::vec3(0.0f, 0.0f, 0.0f));
	m_shadowMapShader.BindValue("uSize", glm::vec2(lightSize, 1.0f));
	m_shadowMapShader.BindValue("uResolution", glm::vec2(lightSize, lightSize));
	m_shadowMapShader.BindTexture("uDiffuse", 0, m_occlusionFBO_tex);
	m_shadowMapShader.Draw(6);

	m_camera->SetOrthoToWindow();

	// 4 - Render the blurred shadows
	m_window->BindAsRenderTarget();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_shadowRenderShader.Bind();
	m_shadowRenderShader.BindVertices(m_VBO);
	m_shadowRenderShader.BindValue("uMVP", m_camera->GetTransform(true));
	m_shadowRenderShader.BindValue("uPos", glm::vec3(mx - lightSize / 2.0f, my - lightSize / 2.0f, 0.0f));
	m_shadowRenderShader.BindValue("uSize", glm::vec2(lightSize, lightSize));
	m_shadowRenderShader.BindValue("uResolution", glm::vec2(lightSize, lightSize));
	m_shadowRenderShader.BindValue("uSoftShadows", 1.0f);
	m_shadowRenderShader.BindValue("uColor", glm::vec4(m_activeLight->GetColor(), 0.7f));
	m_shadowRenderShader.BindTexture("uDiffuse", 0, m_shadowMapFBO_tex);
	m_shadowRenderShader.Draw(6);
}
