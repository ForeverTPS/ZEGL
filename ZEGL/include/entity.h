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

#ifndef ENTITY_H
#define ENTITY_H

#include "mappedvalues.h"
#include "glm/glm.hpp"

const float DEFAULT_ENTITY_SIZE = 64.0f;

class Input;

struct EntityData
{
	glm::vec3	m_pos;			// 12 bytes
	float		m_rot;			// 4 bytes	- offset 12
	float		m_scale;		// 4 bytes	- offset 16
	glm::vec2	m_texCoords[4];	// 32 bytes - offset 20|28|36|42
};

class Entity : public MappedValues
{
public:
	Entity(const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = DEFAULT_ENTITY_SIZE, bool occluder = false)
	{
		m_data.m_pos = pos;
		m_data.m_rot = rot;
		m_data.m_scale = scale;
		m_occluder = occluder;
	}

	virtual ~Entity() {}

	virtual void Init() = 0;
	virtual void LoadResources() = 0;

	virtual void Update(float delta) = 0;
	virtual void ProcessInput(const Input& input, float delta) = 0;

	inline bool CalcTextureCoords(const std::string& regionName)
	{
		const TextureAtlas* textureAtlas = GetTextureAtlas();
		if (textureAtlas == nullptr)
		{
			return false;
		}

		TextureRegion region = textureAtlas->GetRegion(regionName);
		if (region.w == 0)
		{
			return false;
		}

		Texture t = GetTexture("uDiffuse");

		int textureWidth = t.GetWidth();
		int textureHeight = t.GetHeight();

		float x = region.x / textureWidth;
		float y = region.y / textureHeight;
		float w = region.w / textureWidth;
		float h = region.h / textureHeight;

		m_data.m_texCoords[0] = glm::vec2(x, y);
		m_data.m_texCoords[1] = glm::vec2(x, y + h);
		m_data.m_texCoords[2] = glm::vec2(x + w, y);
		m_data.m_texCoords[3] = glm::vec2(x + w, y + h);

		return true;
	}

	inline const EntityData& GetData() const { return m_data; }

	inline glm::vec3	GetPos()	const	{ return m_data.m_pos; }
	inline float		GetRot()	const	{ return m_data.m_rot; }
	inline float		GetScale()	const	{ return m_data.m_scale; }

	inline void	SetPos(float x, float y, float z = 0.0f)	{ m_data.m_pos.x = x; m_data.m_pos.y = y; m_data.m_pos.z = z; }
	inline void	SetPos(const glm::vec3& pos)				{ m_data.m_pos = pos; }
	inline void	SetRot(float rot)							{ m_data.m_rot = rot; }
	inline void	SetScale(float scale)						{ m_data.m_scale = scale; }

	inline bool	IsOccluder() const { return m_occluder; }
	inline void SetOcculder(bool occludes = true) { m_occluder = occludes; }

protected:
	EntityData	m_data;

	bool m_occluder;

private:
	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;
};

#endif