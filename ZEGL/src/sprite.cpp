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

#include "sprite.h"

Sprite::Sprite(const std::string& texture, const std::string& normalMap, const std::string& textureAtlas,
	const glm::vec3& pos, float rot, float scale, bool occluder) :
	Entity(pos, rot, scale, occluder) 
{
	SetResourcePath("uDiffuse", texture);
	SetResourcePath("uNormal", normalMap);
	SetResourcePath("textureAtlas", textureAtlas);
}

Sprite::Sprite(const std::string& texture, const std::string& normalMap, const glm::vec2 textureCoords[4],
	const glm::vec3& pos, float rot, float scale, bool occluder) :
	Entity(pos, rot, scale, occluder)
{
	SetResourcePath("uDiffuse", texture);
	SetResourcePath("uNormal", normalMap);

	for (unsigned int i = 0; i < 4; i++)
	{
		m_data.m_texCoords[i] = textureCoords[i];
	}
}

void Sprite::LoadResources()
{	
	SetTexture("uDiffuse", Texture(GetResourcePath("uDiffuse")));
	SetTexture("uNormal", Texture(GetResourcePath("uNormal")));

	std::string fileName = GetResourcePath("textureAtlas");
	if (fileName.length() > 0)
	{
		SetTextureAtlas(new TextureAtlas(fileName));
	}
}