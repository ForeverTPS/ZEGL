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

#include "sprite.h"

const glm::vec3 Sprite::s_vertices[6] = {
	glm::vec3(0, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(1, 0, 0),
	glm::vec3(0, 1, 0),
	glm::vec3(1, 1, 0),
	glm::vec3(1, 0, 0)
};

Sprite::Sprite(const std::string& texture, const std::string& normalMap, const glm::vec3& pos, bool occluder) :
	m_occluder(occluder),
	Entity(pos)
{
	SetResourcePath("uDiffuse", texture);
	SetResourcePath("uNormal", normalMap);
}

Sprite::~Sprite()
{
	glDeleteBuffers(1, &m_VAB);
}

void Sprite::LoadResources()
{	
	SetTexture("uDiffuse", Texture(GetResourcePath("uDiffuse")));
	SetTexture("uNormal", Texture(GetResourcePath("uNormal")));

	glGenBuffers(1, &m_VAB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
}

void Sprite::Draw(const Shader& shader) 
{
	shader.BindVertices(m_VAB);
	shader.BindValue("uPos", m_pos);

	shader.BindTexture("uDiffuse", 0, GetTexture("uDiffuse").GetTextureID());
	shader.BindTexture("uNormal", 1, GetTexture("uNormal").GetTextureID());

	shader.Draw(6);
}

void Sprite::DrawOcclusion(const Shader& shader) 
{
	if (IsOccluder())
	{
		shader.BindVertices(m_VAB);
		shader.BindValue("uPos", m_pos);

		shader.BindTexture("uDiffuse", 0, GetTexture("uDiffuse").GetTextureID());
		shader.BindTexture("uNormal", 1, GetTexture("uNormal").GetTextureID());

		shader.Draw(6);
	}
}