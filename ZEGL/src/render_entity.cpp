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

#include "render_entity.h"

using namespace ZEGL;

RenderEntity::RenderEntity(const Texture& texture, 
						   const Texture& normalMap, 
						   const TextureAtlas& textureAtlas,
						   const glm::vec3& pos, 
						   float rot, 
						   float xScale,
						   float yScale) :
	m_textureAtlas(textureAtlas),
	m_texture(texture),
	m_normalMap(normalMap),
	Entity(pos, rot, xScale, yScale) 
{
}

RenderEntity::RenderEntity(const Texture& texture,
						   const TextureAtlas& textureAtlas,
						   const glm::vec3& pos,
						   float rot,
						   float xScale,
						   float yScale) :
	m_textureAtlas(textureAtlas),
	m_texture(texture),
	m_normalMap(Texture("./res/textures/default_n.png")),
	Entity(pos, rot, xScale, yScale)
{
}

RenderEntity::RenderEntity(const RenderEntity& renderEntity) :
	m_textureAtlas(renderEntity.m_textureAtlas),
	m_texture(renderEntity.m_texture),
	m_normalMap(renderEntity.m_normalMap),
	Entity(renderEntity)
{
}

void RenderEntity::CalcTextureCoords(const std::string regionName)
{
	TextureRegion region = m_textureAtlas.GetRegion(regionName);
	if (region.w != 0)
	{
		int textureWidth = m_texture.GetWidth();
		int textureHeight = m_texture.GetHeight();

		float x = region.x / textureWidth;
		float y = region.y / textureHeight;
		float w = region.w / textureWidth;
		float h = region.h / textureHeight;

		if (GetXScale() == -1.0f || GetYScale() == -1.0f)
		{
			SetXScale(region.w);
			SetYScale(region.h);
		}

		m_data.m_texCoords[0] = glm::vec2(x, y + h);
		m_data.m_texCoords[1] = glm::vec2(x + w, y + h);
		m_data.m_texCoords[2] = glm::vec2(x, y);
		m_data.m_texCoords[3] = glm::vec2(x + w, y);
	}
}