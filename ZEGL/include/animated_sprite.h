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
			float rot = 0.0f);

		AnimatedSprite(AnimatedSprite const&) = delete;
		AnimatedSprite& operator=(AnimatedSprite const&) = delete;
		~AnimatedSprite();

		void SetFrame(int frame);

	protected:
	private:
		std::unordered_map<std::string, int>	m_frames;
		std::vector<glm::vec4>					m_textureCoordinates;
	};
}