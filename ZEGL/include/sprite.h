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

#include "sprite_batch.h"

namespace ZEGL
{
	/**
	* Used for rendering a 2D object in the world. No animation is included
	* just a static texture.
	*
	* \see [RenderEntity]
	*/
	class Sprite : public RenderEntity
	{
	public:
		/**
		* Constructor using a TextureAtlas.
		*
		* Used for an entity which has a texture containing multiple areas
		* that can be used for rendering different things.
		*
		* \param[in] texture Texture to use for the entity
		* \param[in] normalMap Corresponding normal map for the texture
		* \param[in] textureAtlas A texture atlas which defines the texture
		* \param[in] regionName Name of the region in the texture atlas for this sprite
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the entity
		* \param[in] scale Initial scale of the entity
		*
		* \see [Texture][TextureAtlas]
		*/
		Sprite(const Texture& texture,
			   const Texture& normalMap,
			   const TextureAtlas& textureAtlas,
			   const std::string& regionName,
			   const glm::vec3& pos = glm::vec3(0.0f),
			   float rot = 0.0f,
			   float scale = 1.0f);
		
		/**
		* Constructor using a TextureAtlas but default normal map.
		*
		* Used for an entity which has a texture containing multiple areas
		* that can be used for rendering different things. This uses a default
		* normal map texture.
		*
		* \param[in] texture Texture to use for the entity
		* \param[in] textureAtlas A texture atlas which defines the texture
		* \param[in] regionName Name of the region in the texture atlas for this sprite
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the entity
		* \param[in] scale Initial scale of the entity
		*
		* \see [Texture][TextureAtlas]
		*/
		Sprite(const Texture& texture,
			const TextureAtlas& textureAtlas,
			const std::string& regionName,
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float scale = 1.0f);

		/**
		* Constructor using a pre-calculated texture coordinates.
		*
		* Used for an entity which has a texture coordinate data and no atlas.
		*
		* \param[in] texture Texture to use for the entity
		* \param[in] normalMap Corresponding normal map for the texture
		* \param[in] textureCoords Quad texture coordinates
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the entity
		* \param[in] scale Initial scale of the entity
		*
		* \see [Texture][TextureAtlas]
		*/
		Sprite(const Texture& texture,
			const Texture& normalMap,
			const glm::vec2 textureCoords[4],
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float scale = 1.0f);

		Sprite(Sprite const&);
		Sprite& operator=(Sprite const&) = delete;		
		virtual ~Sprite() {}

		void Draw(SpriteBatch& batch);

	protected:
	private:
		std::string		m_name;
	};
}