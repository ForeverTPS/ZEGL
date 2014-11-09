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

#include "glm/gtc/matrix_transform.hpp"

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

	glDeleteBuffers(NUM_BUFFERS, m_VAB);
	glDeleteVertexArrays(1, &m_VAO);

	glDeleteBuffers(1, &m_quadVAB);
	glDeleteVertexArrays(1, &m_quadVAO);

	glDeleteTextures(1, &m_spriteFBO_color);
	glDeleteTextures(1, &m_spriteFBO_normal);
	glDeleteTextures(1, &m_lightAccumFBO_tex);
	glDeleteTextures(1, &m_lightPassFBO_tex);
	glDeleteTextures(1, &m_lightPassMaskFBO_tex);
	glDeleteFramebuffers(1, &m_spriteFBO);
	glDeleteFramebuffers(1, &m_lightAccumFBO);
	glDeleteFramebuffers(1, &m_lightPassFBO);
	glDeleteFramebuffers(1, &m_lightPassMaskFBO);
	glDeleteRenderbuffers(1, &m_spriteFBO_render);
}

void Game::Init(const Window& window)
{
	m_window = &window;
	m_camera = new Camera(m_window);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glGenVertexArrays(1, &m_quadVAO);
	glBindVertexArray(m_quadVAO);

	//Make fullscreen quad vbo.
	glm::vec3 verts[6] = {
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(-1, 1, 1),
		glm::vec3(1, 1, 1)
	};

	size_t m_quadVBO_size = sizeof(verts);

	glGenBuffers(1, &m_quadVAB);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVAB);
	glBufferData(GL_ARRAY_BUFFER, m_quadVBO_size, &verts[0], GL_STATIC_DRAW);

	InitShaders();
	InitFrameBuffers();

	tileMap = new TileMap();
	
	Light* light = new Light(glm::vec3(200.0f, 200.0f, 10.0f), glm::vec3(1.0f, 0.9f, 0.9f), 800.0f, glm::vec3(0.4f, 3.0f, 20.0f));
	m_lights.push_back(light);
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
	m_spriteShader.BindValue("uMVP", m_camera->GetTransform(m_window));

	GLuint lightPass_vs = Shaders::CreateShader("lightgen_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint lightPass_fs = Shaders::CreateShader("lightgen_shader_fs.glsl", GL_FRAGMENT_SHADER);
	m_lightPassShader.SetProgram(glCreateProgram());
	m_lightPassShader.AddShader(lightPass_vs);
	m_lightPassShader.AddShader(lightPass_fs);
	m_lightPassShader.CompileShader();

	GLuint shadow_vs = Shaders::CreateShader("sprite_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint shadow_fs = Shaders::CreateShader("shadowgen_shader_fs.glsl", GL_FRAGMENT_SHADER);
	GLuint shadow_gs = Shaders::CreateShader("shadowgen_shader_gs.glsl", GL_GEOMETRY_SHADER);
	m_shadowShader.SetProgram(glCreateProgram());
	m_shadowShader.AddShader(shadow_vs);
	m_shadowShader.AddShader(shadow_fs);
	m_shadowShader.AddShader(shadow_gs);
	m_shadowShader.CompileShader();

	m_shadowShader.Bind();
	m_shadowShader.BindValue("uMVP", m_camera->GetTransform(m_window));

	GLuint lightAccum_vs = Shaders::CreateShader("texturedquad_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint lightAccum_fs = Shaders::CreateShader("combineshadow_shader_fs.glsl", GL_FRAGMENT_SHADER);
	m_lightAccumShader.SetProgram(glCreateProgram());
	m_lightAccumShader.AddShader(lightAccum_vs);
	m_lightAccumShader.AddShader(lightAccum_fs);
	m_lightAccumShader.CompileShader();

	GLuint blend_vs = Shaders::CreateShader("texturedquad_shader_vs.glsl", GL_VERTEX_SHADER);
	GLuint blend_fs = Shaders::CreateShader("finalblend_shader_fs.glsl", GL_FRAGMENT_SHADER);
	m_blendShader.SetProgram(glCreateProgram());
	m_blendShader.AddShader(blend_vs);
	m_blendShader.AddShader(blend_fs);
	m_blendShader.CompileShader();
}

void Game::InitFrameBuffers() 
{
	int width = m_window->GetWidth();
	int height = m_window->GetHeight();

	//FBO for normal draw pass
	glGenFramebuffers(1, &m_spriteFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_spriteFBO);

	glGenTextures(1, &m_spriteFBO_color);
	glBindTexture(GL_TEXTURE_2D, m_spriteFBO_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_spriteFBO_color, 0);

	glGenTextures(1, &m_spriteFBO_normal);
	glBindTexture(GL_TEXTURE_2D, m_spriteFBO_normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenRenderbuffers(1, &m_spriteFBO_render);
	glBindRenderbuffer(GL_RENDERBUFFER, m_spriteFBO_render);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_spriteFBO_render);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, m_spriteFBO_normal, 0);

	GLenum spriteDrawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, spriteDrawBuffers);

	//FBO for light occlusion pass (the one lights are drawn to)
	glGenFramebuffers(1, &m_lightPassFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightPassFBO);

	glGenTextures(1, &m_lightPassFBO_tex);
	glBindTexture(GL_TEXTURE_2D, m_lightPassFBO_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_lightPassFBO_tex, 0);

	GLenum lightDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightDrawBuffers);

	glGenFramebuffers(1, &m_lightPassMaskFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightPassMaskFBO);

	glGenTextures(1, &m_lightPassMaskFBO_tex);
	glBindTexture(GL_TEXTURE_2D, m_lightPassMaskFBO_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_lightPassMaskFBO_tex, 0);
	GLenum lightMaskDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightMaskDrawBuffers);

	// FBO for lights (used to recombine our stuff into)
	glGenFramebuffers(1, &m_lightAccumFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_lightAccumFBO);

	glGenTextures(1, &m_lightAccumFBO_tex);
	glBindTexture(GL_TEXTURE_2D, m_lightAccumFBO_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_lightAccumFBO_tex, 0);
	GLenum lightAccumDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightAccumDrawBuffers);
}

void Game::LoadResources()
{
	tileMap->LoadResources("test_level.ldf");

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(2, m_VAB);

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

	glBindBuffer(GL_ARRAY_BUFFER, m_VAB[OCCLUDER_TILE_VB]);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // size
	glEnableVertexAttribArray(2); // texCoords0
	glEnableVertexAttribArray(3); // texCoords1
	glEnableVertexAttribArray(4); // texCoords2
	glEnableVertexAttribArray(5); // texCoords3

	m_bytesAllocated[OCCLUDER_TILE_VB] = sizeof(EntityData)* tileMap->GetActiveOccluderTilesData().size();

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
	m_lights[0]->SetPos(glm::vec3(mousePos, 0));
}

void Game::Update(float delta)
{
	tileMap->Update(delta);
}

void Game::Render()
{
	glBindVertexArray(m_quadVAO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(m_VAO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_lightAccumFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_spriteFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_spriteShader.Bind();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	Texture texture = tileMap->GetActiveTiles()[0]->GetTexture("uDiffuse");
	Texture normal = tileMap->GetActiveTiles()[0]->GetTexture("uNormal");

	m_spriteShader.BindTexture("uDiffuse", 0, texture.GetTextureID(0));
	m_spriteShader.BindTexture("uNormal", 1, texture.GetTextureID(0));

	BindTileData();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileMap->GetActiveTilesData().size());
	BindOccluderTileData();
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileMap->GetActiveOccluderTilesData().size());
	
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (unsigned int i = 0; i < m_lights.size(); i++)
	{
		m_activeLight = m_lights[i];

		glBindVertexArray(m_quadVAO);	
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightPassFBO);
		glClear(GL_COLOR_BUFFER_BIT);

		// SET LIGHT DETAILS AND DRAW
		m_lightPassShader.Bind();
		m_lightPassShader.BindVertices(m_quadVAB);
		m_lightPassShader.BindValue("uLightColor", m_activeLight->GetColor());
		m_lightPassShader.BindValue("uLightPos", m_activeLight->GetPos());
		m_lightPassShader.BindValue("uConstant", m_activeLight->GetConstant());
		m_lightPassShader.BindValue("uLinear", m_activeLight->GetLinear());
		m_lightPassShader.BindValue("uQuadratic", m_activeLight->GetQuadratic());
		m_lightPassShader.BindValue("uRadius", m_activeLight->GetRadius());
		//m_lightPassShader.BindValue("uCameraPos", m_camera->GetPos());
		m_lightPassShader.BindValue("uCameraPos", glm::vec2(0, 0));
		m_lightPassShader.BindTexture("uNormal", 0, m_spriteFBO_normal);

		m_lightPassShader.Draw(6);

		glBindVertexArray(m_VAO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightPassMaskFBO);
		glClear(GL_COLOR_BUFFER_BIT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// DRAW SHADOWING
		m_shadowShader.Bind();
		BindOccluderTileData();
		m_shadowShader.BindValue("uLightPos", m_activeLight->GetPos());
		m_shadowShader.BindValue("uUnmask", false);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileMap->GetActiveOccluderTilesData().size());
		m_shadowShader.BindValue("uUnmask", true);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, tileMap->GetActiveOccluderTilesData().size());

		// DO BLURRING

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBindVertexArray(m_quadVAO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_lightAccumFBO);

		m_lightAccumShader.Bind();
		m_lightAccumShader.BindVertices(m_quadVAB);
		m_lightAccumShader.BindTexture("uIntensity", 0, m_lightPassFBO_tex);
		m_lightAccumShader.BindTexture("uMask", 1, m_lightPassMaskFBO_tex);
		m_lightAccumShader.Draw(6);
	}

	// DRAW FULLSCREEN QUAD
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	m_blendShader.Bind();
	m_blendShader.BindVertices(m_quadVAB);
	m_blendShader.BindTexture("uColor", 0, m_spriteFBO_color);
	m_blendShader.BindTexture("uIntensity", 1, m_lightAccumFBO_tex);
	
	m_blendShader.Draw(6);

	m_activeLight = nullptr;
}

void Game::BindTileData()
{
	size_t bytesNeeded;

	bytesNeeded = sizeof(EntityData)* tileMap->GetActiveTilesData().size();
	if (bytesNeeded > m_bytesAllocated[STD_TILE_VB])
	{
		glBufferData(GL_ARRAY_BUFFER, bytesNeeded, &tileMap->GetActiveTilesData()[0], GL_STREAM_DRAW);
		m_bytesAllocated[STD_TILE_VB] = bytesNeeded;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytesNeeded, &tileMap->GetActiveTilesData()[0]);
	}
}

void Game::BindOccluderTileData()
{
	size_t bytesNeeded;

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
