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

Entity::Entity(const Vector3f& pos, float rot, float scale)
{
	m_data.m_pos = pos;
	m_data.m_rot = rot;
	m_data.m_scale = scale;
}

RenderEntity::RenderEntity(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
	const Vector3f& pos, float rot, float scale) :
	m_textureAtlas(textureAtlas),
	m_texture(texture),
	m_normalMap(normalMap),
	m_hasTextureAtlas(true),
	Entity(pos, rot, scale) {}

RenderEntity::RenderEntity(const Texture& texture, const Texture& normalMap, const Vector2f textureCoords[4],
	const Vector3f& pos, float rot, float scale) :
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

	m_data.m_texCoords[0] = Vector2f(x, y);
	m_data.m_texCoords[1] = Vector2f(x, y);
	m_data.m_texCoords[2] = Vector2f(x, y);
	m_data.m_texCoords[3] = Vector2f(x, y);

	return true;
}