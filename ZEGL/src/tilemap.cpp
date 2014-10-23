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

#include "tilemap.h"

Tile::Tile(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
	const Vector3f& pos, float rot, float scale) :
	m_tileSize(scale),
	RenderEntity(texture, normalMap, textureAtlas, pos, rot, scale)
{
}

Tile::Tile(const Texture& texture, const Texture& normalMap, const Vector2f textureCoords[4],
	const Vector3f& pos, float rot, float scale) :
	m_tileSize(scale),
	RenderEntity(texture, normalMap, textureCoords, pos, rot, scale)
{
}

Tile::Tile(const Tile& tile) :
	m_tileSize(tile.m_tileSize),
	RenderEntity(tile) {}

TileMap::TileMap(const std::string& fileName)
{
	if (fileName != "")
	{
		Load(fileName);
	}

	Texture texture("rock.png");
	Texture textureNormal("rock_n.png");
	TextureAtlas textureAtlas("test_atlas.xml");

	m_map.push_back(Tile(texture, textureNormal, textureAtlas, Vector3f(100.0f, 100.0f, 100.0f), 0.0f, 100.0f));
	m_map.push_back(Tile(texture, textureNormal, textureAtlas, Vector3f(300.0f, 100.0f, 100.0f), 0.0f, 100.0f));
	m_map.push_back(Tile(texture, textureNormal, textureAtlas, Vector3f(500.0f, 100.0f, 100.0f), 0.0f, 100.0f));

	for (unsigned int i = 0; i < m_map.size(); i++)
	{
		m_map[i].CalcTextureCoords("rock");
		m_activeTiles.push_back(m_map[i]);
		m_activeTilesData.push_back(m_map[i].GetData());
	}
}

void TileMap::Load(const std::string& fileName)
{

}