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

#include "sprite_batch.h"
#include "logger.h"

using namespace ZEGL;

SpriteBatch::SpriteBatch() :
	m_lastTexture(nullptr),
	m_lastNormalMap(nullptr),
	m_customShader(nullptr),
	m_VAO(0),
	m_VAB(0),
	m_bytesAllocated(0),
	m_isDrawing(false)
{
}

SpriteBatch::~SpriteBatch()
{
	if (m_VAB)
	{
		glDeleteBuffers(1, &m_VAB);
	}
	if (m_VAO)
	{
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_VAO);
	}
}

void SpriteBatch::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VAB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // rot
	glEnableVertexAttribArray(2); // x size
	glEnableVertexAttribArray(3); // y size
	glEnableVertexAttribArray(4); // texCoords0
	glEnableVertexAttribArray(5); // texCoords1
	glEnableVertexAttribArray(6); // texCoords2
	glEnableVertexAttribArray(7); // texCoords3

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)16);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)20);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)24);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)32);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)40);
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)48);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteBatch::Begin(Game* game)
{
	if (m_isDrawing)
	{
		LOG_ERROR("Error: Must call SpriteBatch::End() before SpriteBatch::Begin()");
	}

	if (m_customShader != nullptr)
	{
		m_customShader->Bind();
	}
	else
	{
		m_defaultShader.Bind();
	}

	m_spriteData.clear();
	m_defaultShader.UpdateUniforms(game);

	m_isDrawing = true;
}

void SpriteBatch::End()
{
	if (!m_isDrawing)
	{
		LOG_ERROR("Error: Must call SpriteBatch::Begin() before SpriteBatch::End()");
	}

	if (m_spriteData.size() > 0)
	{
		Flush();
	}

	m_lastTexture = nullptr;
	m_lastNormalMap = nullptr;
	m_isDrawing = false;

	if (m_customShader != nullptr)
	{
		m_customShader->UnBind();
	}
	else
	{
		m_defaultShader.UnBind();
	}
}

void SpriteBatch::Draw(RenderEntity& renderEntity)
{
	if (!m_isDrawing)
	{
		LOG_ERROR("Error: Must call SpriteBatch::Begin() before SpriteBatch::End()");
	}

	Texture* texture = renderEntity.GetTexture();
	if (m_lastTexture != texture)
	{
		SetTexture(texture, renderEntity.GetNormalMap());
	}

	m_spriteData.push_back(renderEntity.GetData());
}

void SpriteBatch::Flush()
{
	if (m_spriteData.size() == 0 || m_lastTexture == nullptr)
	{
		return;
	}

	m_lastTexture->Bind(0);
	if (m_customShader != nullptr && m_lastNormalMap != nullptr)
	{
		m_lastNormalMap->Bind(1);
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB);

	size_t bytesNeeded = sizeof(EntityData)* m_spriteData.size();
	if (bytesNeeded > m_bytesAllocated) 
	{
		glBufferData(GL_ARRAY_BUFFER, bytesNeeded, &m_spriteData[0], GL_STREAM_DRAW);
		m_bytesAllocated = bytesNeeded;
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytesNeeded, &m_spriteData[0]);
	}

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_spriteData.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteBatch::SetShader(Shader* shader)
{
	if (m_isDrawing) 
	{
		Flush();
		if (m_customShader != nullptr)
		{
			m_customShader->UnBind();
		}
		else
		{
			m_defaultShader.UnBind();
		}
	}

	m_customShader = shader;
	
	if (m_isDrawing) 
	{
		if (m_customShader != nullptr)
		{
			m_customShader->Bind();
		}
		else
		{
			m_defaultShader.Bind();
		}
	}
}

void SpriteBatch::SetTexture(Texture* texture, Texture* normalMap)
{
	Flush();

	m_lastTexture = texture;
	m_lastNormalMap = normalMap;
}