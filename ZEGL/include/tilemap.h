/*  ____ ____ ____ ____
 * ||Z |||E |||G |||L ||
 * ||__|||__|||__|||__||
 * |/__\|/__\|/__\|/__\|
 *
 * Copyright(c) 2014, Phil Sampson
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

#ifndef TILEMAP_H
#define TILEMAP_H

#include "sprite.h"
#include <vector>

class Window;

struct TileDefinition
{
	std::string tilename;
	std::string textureName;
	std::string normalMapName;
	std::string textureAtlasName;
	bool		occluder;
};

class Tile : public Sprite
{
public:
	Tile(const std::string& texture, const std::string& normalMap, const std::string& textureAtlas,
		const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, bool occluder = false) :
		Sprite(texture, normalMap, textureAtlas, pos, rot, DEFAULT_ENTITY_SIZE, occluder) {}

	Tile(const std::string& texture, const std::string& normalMap, const glm::vec2 textureCoords[4],
		const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, bool occluder = false) :
		Sprite(texture, normalMap, textureCoords, pos, rot, DEFAULT_ENTITY_SIZE, occluder) {}

	virtual ~Tile() {}

protected:
private:
	Tile(Tile const&) = delete;
	Tile& operator=(Tile const&) = delete;
};

class TileMap
{
public:
	TileMap() {}
	virtual ~TileMap();

	void LoadResources(const std::string& fileName);

	void Update(float delta);

	inline const std::vector<Tile*>			GetActiveTiles()				const { return m_activeTiles; }
	inline const std::vector<EntityData>&	GetActiveTilesData()			const { return m_activeTilesData; }
	inline const std::vector<Tile*>			GetActiveOccluderTiles()		const { return m_activeOccluderTiles; }
	inline const std::vector<EntityData>&	GetActiveOccluderTilesData()	const { return m_activeOccluderTilesData; }

protected:
private:
	TileMap(TileMap const&) = delete;
	TileMap& operator=(TileMap const&) = delete;

	void UpdateActiveTiles();

	std::vector<Tile*>		m_map;
	std::vector<Tile*>		m_activeTiles;
	std::vector<EntityData>	m_activeTilesData;
	std::vector<Tile*>		m_activeOccluderTiles;
	std::vector<EntityData>	m_activeOccluderTilesData;
};

#endif