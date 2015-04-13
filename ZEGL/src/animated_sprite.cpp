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

#include "animated_sprite.h"
#include "timing.h"

using namespace ZEGL;

AnimatedSprite::AnimatedSprite(const Texture& texture,
							   const Texture& normalMap,
							   const TextureAtlas& textureAtlas,
							   const std::vector<std::string>& frameNames,
							   const glm::vec3& pos,
							   float rot,
							   float xScale,
							   float yScale) :
	m_currentFrame(0),
	m_isAnimating(false),
	m_loop(false),
	m_lastAnimated(0),
	m_speed(1000),
	Sprite(texture, normalMap, textureAtlas, frameNames[0], pos, rot, xScale, yScale)
{
	for (unsigned int i = 0; i < frameNames.size(); i++)
	{
		m_frameNames.insert(std::pair<std::string, int>(frameNames[i], i));
	}

	CalcTextureCoords();
}

AnimatedSprite::AnimatedSprite(const Texture& texture,
							   const TextureAtlas& textureAtlas,
							   const std::vector<std::string>& frameNames,
							   const glm::vec3& pos,
							   float rot,
							   float xScale,
							   float yScale) :
	m_currentFrame(0),
	m_isAnimating(false),
	m_loop(false),
	m_lastAnimated(0),
	m_speed(1000),
	Sprite(texture, textureAtlas, frameNames[0], pos, rot, xScale, yScale)
{
	for (unsigned int i = 0; i < frameNames.size(); i++)
	{
		m_frameNames.insert(std::pair<std::string, int>(frameNames[i], i));
	}

	CalcTextureCoords();
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite& sprite) :
	m_currentFrame(sprite.m_currentFrame),
	m_isAnimating(sprite.m_isAnimating),
	m_loop(sprite.m_loop),
	m_lastAnimated(sprite.m_lastAnimated),
	m_speed(sprite.m_speed),
	m_frameNames(sprite.m_frameNames),
	Sprite(sprite)
{
	for (unsigned int i = 0; i < sprite.m_textureCoordinates.size(); i++)
	{
		m_textureCoordinates[i] = sprite.m_textureCoordinates[i];
	}
}

void AnimatedSprite::CalcTextureCoords()
{
	unsigned int i = 0;
	for (std::unordered_map<std::string, unsigned int>::const_iterator it = m_frameNames.begin(); it != m_frameNames.end(); it++)
	{
		TextureRegion region = m_textureAtlas.GetRegion(it->first);
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

			QuadTexCoords texCoords;

			texCoords.topLeft = glm::vec2(x, y + h);
			texCoords.topRight = glm::vec2(x + w, y + h);
			texCoords.bottomLeft = glm::vec2(x, y);
			texCoords.bottomRight = glm::vec2(x + w, y);

			m_textureCoordinates.push_back(texCoords);

			i++;
		}
	}

	SetFrame(0);
}

void AnimatedSprite::SetFrame(unsigned int frame)
{
	if (frame >= m_frameNames.size())
	{
		return;
	}

	m_currentFrame = frame;

	m_data.m_texCoords[0] = m_textureCoordinates[m_currentFrame].topLeft;
	m_data.m_texCoords[1] = m_textureCoordinates[m_currentFrame].topRight;
	m_data.m_texCoords[2] = m_textureCoordinates[m_currentFrame].bottomLeft;
	m_data.m_texCoords[3] = m_textureCoordinates[m_currentFrame].bottomRight;
}

void AnimatedSprite::Reset(bool restart)
{
	m_isAnimating = restart;
	if (restart)
	{
		Start(0);
	}
	else
	{
		m_isAnimating = false;
		SetFrame(0);
	}
}

void AnimatedSprite::Start(unsigned int frame)
{
	SetFrame(frame);
	m_isAnimating = true;
	m_lastAnimated = 0;
}

void AnimatedSprite::Animate()
{
	if (m_isAnimating)
	{
		Uint32 time = SDL_GetTicks();
		if ((time - m_lastAnimated) > m_speed)
		{
			m_lastAnimated = time;
			if ((m_currentFrame + 1) >= m_frameNames.size())
			{
				if (m_loop)
				{
					SetFrame(0);
				}
				else
				{
					m_isAnimating = false;
				}
			}
			else
			{
				SetFrame(m_currentFrame + 1);
			}
		}
	}
}