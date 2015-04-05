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

#include "renderentity.h"
#include <vector>

namespace ZEGL
{
	const float DEFAULT_TILE_SIZE = 64.0f;

	class Window;

	struct TileDefinition
	{
		std::string tilename;
		std::string textureName;
		std::string normalMapName;
		std::string textureAtlasName;
	};

	class Tile : public RenderEntity
	{
	public:
		Tile(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
			const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = (float)DEFAULT_TILE_SIZE);

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
		TileMap(const std::string& fileName);
		virtual ~TileMap() {}

		void Update(float delta) {}
		void UpdateActiveTiles(const Window* window, const glm::vec3& cameraPos);

		inline const std::vector<Tile>&			GetActiveTiles()		const { return m_activeTiles; }
		inline const std::vector<EntityData>&	GetActiveTilesData()	const { return m_activeTilesData; }

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