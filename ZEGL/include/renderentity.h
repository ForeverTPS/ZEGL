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

#pragma once;

#include "entity.h"

namespace ZEGL
{
	class RenderEntity : public Entity
	{
	public:
		RenderEntity(const Texture& texture,
			const Texture& normalMap,
			const TextureAtlas& textureAtlas,
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float scale = 1.0f);

		RenderEntity(const Texture& texture,
			const Texture& normalMap,
			const glm::vec2 textureCoords[4],
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float scale = 1.0f);

		RenderEntity(const RenderEntity& renderEntity);
		void operator=(RenderEntity renderEntity);
		virtual ~RenderEntity() {}

		bool					CalcTextureCoords(const std::string regionName);

		inline const Texture&	GetTexture()	const { return m_texture; }
		inline const Texture&	GetNormalMap()	const { return m_normalMap; }

	protected:
		TextureAtlas			m_textureAtlas;
		Texture					m_texture;
		Texture					m_normalMap;

		bool					m_hasTextureAtlas;
	};
}