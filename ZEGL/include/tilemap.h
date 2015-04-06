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

#include "renderentity.h"
#include <vector>

namespace ZEGL
{
#define DEFAULT_TILE_SIZE 64

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
		Tile(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
			const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = (float)DEFAULT_TILE_SIZE);

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
		Tile(const Texture& texture, const Texture& normalMap, const glm::vec2 textureCoords[4],
			const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = (float)DEFAULT_TILE_SIZE);

		Tile(const Tile& tile);

		virtual ~Tile() {}

	protected:
	private:
		Tile& operator=(Tile const&) = delete;

		float m_tileSize;
	};

	class TileMap
	{
	public:
		/**
		* Create and load a tile map from the specified file.
		*
		* \param[in] fileName Name of the tile map file
		*
		* \warning The tile map file must have a tile definition file with it that 
		* must have the same full name (including extension) but also an extra 
		* extension of .tdef for example tilemap.ldf and tilemap.ldf.tdef
		*/
		TileMap(const std::string& fileName);

		/**
		* Refresh the list of active tiles.
		*
		* Removes any tiles not visible and adds any tiles now on screen to the
		* active tile list
		*
		* \param[in] window The display window
		* \param[in] cameraPos The current camera position.
		*
		* \see [Window]
		*/
		void UpdateActiveTiles(const Window* window, const glm::vec3& cameraPos);

		/**
		* Get all the active tiles.
		*
		* \return All of the currently active tiles in the map
		*
		* \see [Tile]
		*/
		inline const std::vector<Tile>& GetActiveTiles() const { return m_activeTiles; }

		/**
		* Get all the active tiles data for passing to a shader.
		*
		* \return All of the currently active tile data
		*
		* \see [EntityData]
		*/
		inline const std::vector<EntityData>& GetActiveTilesData()	const { return m_activeTilesData; }

	protected:
	private:
		TileMap(TileMap const&) = delete;
		TileMap& operator=(TileMap const&) = delete;

		void Load(const std::string& fileName);

		std::vector<Tile>		m_map;
		std::vector<Tile>		m_activeTiles;
		std::vector<EntityData>	m_activeTilesData;
	};
}