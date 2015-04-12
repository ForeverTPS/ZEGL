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

#include "sprite.h"

using namespace ZEGL;

Sprite::Sprite(const Texture& texture,
			   const Texture& normalMap,
			   const TextureAtlas& textureAtlas,
			   const std::string& regionName,
			   const glm::vec3& pos,
			   float rot,
			   float xScale,
			   float yScale) :
	m_name(regionName),
	RenderEntity(texture, normalMap, textureAtlas, pos, rot, xScale, yScale)
{
	CalcTextureCoords(m_name);
}

Sprite::Sprite(const Texture& texture,
			   const TextureAtlas& textureAtlas,
			   const std::string& regionName,
			   const glm::vec3& pos,
			   float rot,
			   float xScale,
			   float yScale) :
	m_name(regionName),
	RenderEntity(texture, textureAtlas, pos, rot, xScale, yScale)
{
	CalcTextureCoords(m_name);
}

Sprite::Sprite(const Sprite& sprite) :
	m_name(sprite.m_name),
	RenderEntity(sprite)
{
}

void Sprite::Draw(SpriteBatch& batch)
{
	batch.Draw(*this);
}