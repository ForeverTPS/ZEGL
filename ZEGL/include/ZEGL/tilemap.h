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

#include "tile.h"
#include <vector>

namespace ZEGL
{
	class Game;
	class Window;

	/**
	* A collection of tiles making up a level/map
	*
	* \see [Tile]
	*/
	class TileMap
	{
	public:
		/**
		* Create and load a tile map from the specified file.
		*
		* \param[in] fileName Name of the tile map file
		*/
		TileMap(const std::string& fileName);
		TileMap(TileMap const&) = delete;
		TileMap& operator=(TileMap const&) = delete;
		~TileMap();
		
		/**
		* Performs all update actions for the tile map.
		*
		* Removes any tiles not visible and adds any tiles now on screen to the
		* active tile list
		*
		* \param[in] delta Delta frame time in milliseconds
		*/
		void Update(float delta);

		/**
		* Renders all of the active tiles to screen using the shader for
		* the current active light.
		*
		* \param[in] game The game which is rendering
		*
		* \see [Game][Shader][Tile]
		*/
		void Render();

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
		inline const std::vector<EntityData>& GetActiveTilesData() const { return m_activeTilesData; }

	protected:
	private:
		void Load(const std::string& fileName);

		std::vector<Tile>		m_map;
		std::vector<Tile>		m_activeTiles;
		std::vector<EntityData>	m_activeTilesData;

		GLuint					m_VAO;
		GLuint					m_VAB;
		size_t					m_bytesAllocated;
	};
}