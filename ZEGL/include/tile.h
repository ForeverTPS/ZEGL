/*
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

#include "render_entity.h"
#include <vector>

namespace ZEGL
{
#define DEFAULT_TILE_SIZE 64

	class Game;
	class Window;

	/**
	* Holds the attributes which define a tile when loaded from a (tile definition) .tdef file
	*/
	struct TileDefinition
	{
		std::string tilename;			/*!< Name of the tile */
		std::string textureName;		/*!< Name of the texture used by the tile */
		std::string normalMapName;		/*!< Name of the normal map texture used by the tile */
		std::string textureAtlasName;	/*!< Name of the atlas map which defines the tile */
	};

	/**
	* An individual tile in a map/level
	*
	* All tiles for a map/level are stored within a TileMap. They are extensions
	* of the RenderEntity class which allows rendering. The main extension is the
	* tile size member.
	*
	* \see [RenderEntity][Texture][TextureAtlas][TileMap]
	*/
	class Tile : public RenderEntity
	{
	public:
		/**
		* Constructor using a TextureAtlas.
		*
		* Used for a tile which has a texture containing multiple areas
		* that can be used for rendering different things.
		*
		* \param[in] texture Texture to use for the tile
		* \param[in] normalMap Corresponding normal map for the texture
		* \param[in] textureAtlas A texture atlas which defines the texture
		* \param[in] pos Initial position of the entity
		* \param[in] rot Initial rotation angle (in radians) of the tile
		* \param[in] scale Initial scale of the tile
		*
		* \see [Texture][TextureAtlas]
		*/
		Tile(const Texture& texture,
			const Texture& normalMap,
			const TextureAtlas& textureAtlas,
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float scale = (float)DEFAULT_TILE_SIZE) :
			m_tileSize(scale),
			RenderEntity(texture, normalMap, textureAtlas, pos, rot, scale)
		{
		}

		/**
		* Constructor using a pre-calculated texture coordinates.
		*
		* Used for a tile which has a texture coordinate data and no atlas.
		*
		* \param[in] texture Texture to use for the tile
		* \param[in] normalMap Corresponding normal map for the texture
		* \param[in] textureCoords Quad texture coordinates
		* \param[in] pos Initial position of the tile
		* \param[in] rot Initial rotation angle (in radians) of the tile
		* \param[in] scale Initial scale of the tile
		*
		* \see [Texture][TextureAtlas]
		*/
		Tile(const Texture& texture,
			const Texture& normalMap,
			const glm::vec2 textureCoords[4],
			const glm::vec3& pos = glm::vec3(0.0f),
			float rot = 0.0f,
			float scale = (float)DEFAULT_TILE_SIZE) :
			m_tileSize(scale),
			RenderEntity(texture, normalMap, textureCoords, pos, rot, scale)
		{
		}

		Tile(const Tile& tile) :
			m_tileSize(tile.m_tileSize),
			RenderEntity(tile)
		{
		}

		Tile& operator=(Tile const&) = delete;

		virtual ~Tile() {}

	protected:
	private:
		float m_tileSize;
	};
}