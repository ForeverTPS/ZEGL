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

#include "tilemap.h"
#include "game.h"
#include "logger.h"
#include "tinyxml2.h"
#include "util.h"
#include "window.h"
#include <fstream>
#include <iostream>

using namespace tinyxml2;
using namespace ZEGL;

Tile::Tile(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
	const glm::vec3& pos, float rot, float scale) :
	m_tileSize(scale),
	RenderEntity(texture, normalMap, textureAtlas, pos, rot, scale)
{
}

Tile::Tile(const Texture& texture, const Texture& normalMap, const glm::vec2 textureCoords[4],
	const glm::vec3& pos, float rot, float scale) :
	m_tileSize(scale),
	RenderEntity(texture, normalMap, textureCoords, pos, rot, scale)
{
}

Tile::Tile(const Tile& tile) :
	m_tileSize(tile.m_tileSize),
	RenderEntity(tile) {}

TileMap::TileMap(const std::string& fileName)
{
	Load(fileName);
}

void TileMap::Load(const std::string& fileName)
{
	bool success = true;
	std::string error;

	XMLDocument doc;
	if (doc.LoadFile(("./res/levels/" + fileName + ".tdef").c_str()) != XML_NO_ERROR)
	{
		success = false;
		error = "Failed loading tile definitions: " + fileName + ".tdef";
	}
	else
	{
		XMLElement* tileElement = doc.FirstChildElement("Tile");
		if (tileElement == nullptr)
		{
			success = false;
			error = "No Tiles defined.";
		}
		else
		{
			std::unordered_map<std::string, TileDefinition> m_tileDefs;

			while (tileElement != nullptr)
			{
				std::string tileId;
				TileDefinition tileDef;

				tileId = tileElement->Attribute("id");
				tileDef.tilename = tileElement->Attribute("name");
				tileDef.textureName = tileElement->Attribute("texture");
				tileDef.normalMapName = tileElement->Attribute("normalMap");
				tileDef.textureAtlasName = tileElement->Attribute("textureAtlas");

				m_tileDefs.insert(std::pair<std::string, TileDefinition>(tileId, tileDef));

				tileElement = tileElement->NextSiblingElement();
			}

			std::ifstream file;
			file.open(("./res/levels/" + fileName).c_str());

			std::string line;

			if (file.is_open())
			{
				float x = DEFAULT_TILE_SIZE / 2;
				float y = x;

				while (file.good())
				{
					getline(file, line);
				
					std::vector<std::string> tiles = Util::SplitString(line, ',');
					for (unsigned int i = 0; i < tiles.size(); i++)
					{
						std::unordered_map<std::string, TileDefinition>::const_iterator it = m_tileDefs.find(tiles[i]);
						if (it != m_tileDefs.end())
						{
							Texture texture(it->second.textureName);
							Texture textureNormal(it->second.normalMapName);
							TextureAtlas textureAtlas(it->second.textureAtlasName);

							m_map.push_back(Tile(texture, textureNormal, textureAtlas, glm::vec3(x, y, 0.0f), 0.0f, DEFAULT_TILE_SIZE));
							m_map[m_map.size() - 1].CalcTextureCoords(tiles[i]);

							x += DEFAULT_TILE_SIZE;
						}
						else
						{
							//TODO: Handle error
						}
					}

					x = DEFAULT_TILE_SIZE / 2;
					y += DEFAULT_TILE_SIZE;
				}

				for (unsigned int i = 0; i < m_map.size(); i++)
				{
					m_map[i].CalcTextureCoords("rock");
					m_activeTiles.push_back(m_map[i]);
					m_activeTilesData.push_back(m_map[i].GetData());
				}
			}
			else
			{
				std::cerr << "Unable to load tile map: " << fileName << std::endl;
			}
		}
	}

	if (!success)
	{
		LOG_ERROR(error.c_str())
		LOG_CLOSE();
		exit(1);
	}
}

void TileMap::UpdateActiveTiles(const Window* window, const glm::vec3& cameraPos)
{
	m_activeTiles.clear();
	m_activeTilesData.clear();
	
	for (unsigned int i = 0; i < m_map.size(); i++)
	{
		glm::vec3 pos = m_map[i].GetPos();

		if (pos.x > cameraPos.x - DEFAULT_TILE_SIZE && pos.x < cameraPos.x + (float)window->GetWidth() + DEFAULT_TILE_SIZE &&
			pos.y > cameraPos.y - DEFAULT_TILE_SIZE && pos.y < cameraPos.y + (float)window->GetHeight() + DEFAULT_TILE_SIZE)
		{
			m_activeTiles.push_back(m_map[i]);
			m_activeTilesData.push_back(m_map[i].GetData());
		}
	}
}