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
		m_textureCoordinates[i][0] = sprite.m_textureCoordinates[i][0];
		m_textureCoordinates[i][1] = sprite.m_textureCoordinates[i][1];
		m_textureCoordinates[i][2] = sprite.m_textureCoordinates[i][2];
		m_textureCoordinates[i][3] = sprite.m_textureCoordinates[i][3];
	}
}

void AnimatedSprite::CalcTextureCoords()
{

}

void AnimatedSprite::SetFrame(unsigned int frame)
{
	if (frame >= m_frameNames.size())
	{
		return;
	}

	m_currentFrame = frame;

	for (int i = 0; i < 4; i++)
	{
		m_data.m_texCoords[i] = m_textureCoordinates[m_currentFrame][i];
	}
}

void AnimatedSprite::Reset(bool restart)
{
	SetFrame(0);
	m_isAnimating = restart;
}

void AnimatedSprite::Start(unsigned int frame)
{
	SetFrame(frame);
	m_isAnimating = true;
}

void AnimatedSprite::Animate()
{
	if (m_isAnimating)
	{
		double time = Time::GetTime();
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