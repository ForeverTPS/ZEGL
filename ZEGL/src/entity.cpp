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

#include "entity.h"

using namespace ZEGL;

Entity::Entity(const glm::vec3& pos, float rot, float scale)
{
	m_data.m_pos = pos;
	m_data.m_rot = rot;
	m_data.m_scale = scale;
}

Entity::Entity(const Entity& entity) :
	m_data(entity.m_data) {}

void Entity::operator=(Entity entity)
{
	char* temp[sizeof(Entity) / sizeof(char)];
	memcpy(temp, (void*)this, sizeof(Entity));
	memcpy((void*)this, (void*)&entity, sizeof(Entity));
	memcpy((void*)&entity, temp, sizeof(Entity));
}

RenderEntity::RenderEntity(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
	const glm::vec3& pos, float rot, float scale) :
	m_textureAtlas(textureAtlas),
	m_texture(texture),
	m_normalMap(normalMap),
	m_hasTextureAtlas(true),
	Entity(pos, rot, scale) {}

RenderEntity::RenderEntity(const Texture& texture, const Texture& normalMap, const glm::vec2 textureCoords[4],
	const glm::vec3& pos, float rot, float scale) :
	m_texture(texture),
	m_normalMap(normalMap),
	m_hasTextureAtlas(false),
	Entity(pos, rot, scale)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		m_data.m_texCoords[i] = textureCoords[i];
	}
}

RenderEntity::RenderEntity(const RenderEntity& renderEntity) :
	m_textureAtlas(renderEntity.m_textureAtlas),
	m_texture(renderEntity.m_texture),
	m_normalMap(renderEntity.m_normalMap),
	m_hasTextureAtlas(renderEntity.m_hasTextureAtlas),
	Entity(renderEntity) {}

void RenderEntity::operator=(RenderEntity renderEntity)
{
	char* temp[sizeof(RenderEntity) / sizeof(char)];
	memcpy(temp, (void*)this, sizeof(RenderEntity));
	memcpy((void*)this, (void*)&renderEntity, sizeof(RenderEntity));
	memcpy((void*)&renderEntity, temp, sizeof(RenderEntity));
}

bool RenderEntity::CalcTextureCoords(const std::string regionName)
{
	if (!m_hasTextureAtlas)
	{
		return false;
	}

	TextureRegion region = m_textureAtlas.GetRegion(regionName);
	if (region.w == 0)
	{
		return false;
	}

	int textureWidth = m_texture.GetWidth();
	int textureHeight = m_texture.GetHeight();

	float x = region.x / textureWidth;
	float y = region.y / textureHeight;
	float w = region.w / textureWidth;
	float h = region.h / textureHeight;

	m_data.m_texCoords[0] = glm::vec2(x, y + h);
	m_data.m_texCoords[1] = glm::vec2(x, y);
	m_data.m_texCoords[2] = glm::vec2(x + w, y + h);
	m_data.m_texCoords[3] = glm::vec2(x + w, y);

	return true;
}