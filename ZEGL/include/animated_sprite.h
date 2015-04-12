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

#pragma once

#include "sprite.h"
#include <SDL.h>

namespace ZEGL
{
	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(const Texture& texture,
					   const Texture& normalMap,
					   const TextureAtlas& textureAtlas,
					   const std::vector<std::string>& frameNames,
					   const glm::vec3& pos = glm::vec3(0.0f),
					   float rot = 0.0f,
					   float xScale = -1.0f,
					   float yScale = -1.0f);

		AnimatedSprite(const Texture& texture,
					   const TextureAtlas& textureAtlas,
					   const std::vector<std::string>& frameNames,
					   const glm::vec3& pos = glm::vec3(0.0f),
					   float rot = 0.0f,
					   float xScale = -1.0f,
					   float yScale = -1.0f);

		AnimatedSprite(AnimatedSprite const&);
		AnimatedSprite& operator=(AnimatedSprite const&) = delete;
		virtual ~AnimatedSprite() {}

		void CalcTextureCoords();

		void SetFrame(unsigned int frame);

		void Reset(bool restart = false);

		void Start(unsigned int frame = 0);

		inline int	Pause() { m_isAnimating = false; }
		
		inline void Resume() { m_isAnimating = true; }
		
		inline void Stop() { Reset(); }

		inline void SetLooping(bool loop) { m_loop = loop; }
		
		inline void SetSpeed(Uint32 speed) { m_speed = speed; }

		void Animate();

	protected:
	private:
		unsigned int									m_currentFrame;
		std::unordered_map<std::string, unsigned int>	m_frameNames;
		std::vector<glm::vec2[4]>						m_textureCoordinates;

		bool											m_isAnimating;
		bool											m_loop;

		double											m_lastAnimated;
		double											m_speed;
	};
}