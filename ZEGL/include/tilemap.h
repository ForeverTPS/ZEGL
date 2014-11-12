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
	std::string textureName;
	std::string normalMapName;
	bool		occluder;
};

class Tile : public Sprite
{
public:
	Tile(const std::string& texture, const std::string& normalMap, const glm::vec3& pos = glm::vec3(0.0f), bool occluder = false) :
		Sprite(texture, normalMap, pos, occluder) {}

	virtual ~Tile() {}

	virtual void Draw(const Shader& shader);
	virtual void DrawOcclusion(const Shader& shader);

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

	void Draw(const Shader& shader);
	void DrawShadowLayer(const Shader& shader);

	inline const std::vector<Tile*>	GetActiveTiles() const { return m_activeTiles; }

protected:
private:
	TileMap(TileMap const&) = delete;
	TileMap& operator=(TileMap const&) = delete;

	void UpdateActiveTiles();

	std::vector<Tile*>		m_map;
	std::vector<Tile*>		m_activeTiles;
};

#endif