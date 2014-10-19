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

#include "entity.h"

#define DEFAULT_TILE_SIZE 32

class Tile : public RenderEntity
{
public:
	Tile(const Texture& texture, const Texture& normalMap, const Vector2f texCoords[4],
		const Vector3f& pos = Vector3f(0.0f, 0.0f, 0.0f), float rot = 0.0f, float scale = 1.0f) :
		m_tileSize(DEFAULT_TILE_SIZE),
		RenderEntity(texture, normalMap, texCoords, pos, rot, scale) {}

	virtual ~Tile();

protected:
private:
	Tile(Tile const&) = delete;
	Tile& operator=(Tile const&) = delete;

	int m_tileSize;
};

class TileMap
{
public:
	TileMap();
	virtual ~TileMap();

	void Update(float delta);

protected:
private:
	TileMap(TileMap const&) = delete;
	TileMap& operator=(TileMap const&) = delete;
};