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
#include "camera.h"
#include "game.h"
#include "logfile.h"
#include "tinyxml2.h"
#include "window.h"
#include <fstream>
#include <iostream>

using namespace tinyxml2;

TileMap::~TileMap()
{
	for (unsigned int i = 0; i < m_map.size(); i++)
	{
		Util::SafeDelete(m_map[i]);
	}
}

void TileMap::LoadResources(const std::string& fileName)
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
			std::map<std::string, TileDefinition> m_tileDefs;

			while (tileElement != nullptr)
			{
				std::string tileId;
				TileDefinition tileDef;

				tileId = tileElement->Attribute("id");
				tileDef.tilename = tileElement->Attribute("name");
				tileDef.textureName = tileElement->Attribute("texture");
				tileDef.normalMapName = tileElement->Attribute("normalMap");
				tileDef.textureAtlasName = tileElement->Attribute("textureAtlas");
				tileElement->QueryBoolAttribute("occluder", &tileDef.occluder);

				m_tileDefs.insert(std::pair<std::string, TileDefinition>(tileId, tileDef));

				tileElement = tileElement->NextSiblingElement();
			}

			std::ifstream file;
			file.open(("./res/levels/" + fileName).c_str());

			std::string line;

			if (file.is_open())
			{
				float x = DEFAULT_ENTITY_SIZE / 2;
				float y = x;

				while (file.good())
				{
					getline(file, line);
				
					std::vector<std::string> tiles = Util::Split(line, ',');
					for (unsigned int i = 0; i < tiles.size(); i++)
					{
						std::map<std::string, TileDefinition>::const_iterator it = m_tileDefs.find(tiles[i]);
						if (it != m_tileDefs.end())
						{
							Tile* t = new Tile(it->second.textureName, it->second.normalMapName, it->second.textureAtlasName, glm::vec3(x, y, 0.0f), 0.0f, it->second.occluder);
							t->LoadResources();
							m_map.push_back(t);

							x += DEFAULT_ENTITY_SIZE;
						}
						else
						{
							snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "No tile definition found for: %s", tiles[i]);
							LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
							LOG_CLEANUP();
							exit(1);
						}
					}

					x = DEFAULT_ENTITY_SIZE / 2;
					y += DEFAULT_ENTITY_SIZE;
				}

				for (unsigned int i = 0; i < m_map.size(); i++)
				{
					m_map[i]->CalcTextureCoords("rock");
					if (m_map[i]->IsOccluder())
					{
						m_activeOccluderTiles.push_back(m_map[i]);
						m_activeOccluderTilesData.push_back(m_map[i]->GetData());
					}
					else
					{
						m_activeTiles.push_back(m_map[i]);
						m_activeTilesData.push_back(m_map[i]->GetData());
					}
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
		snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "%s", error.c_str());
		LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
		LOG_CLEANUP();
		exit(1);
	}
}

void TileMap::Update(float delta)
{
	UpdateActiveTiles();
}

void TileMap::UpdateActiveTiles()
{
	Game* game = Game::GetInstance();
	glm::vec3 cameraPos = game->GetCamera().GetPos();
	const Window* window = game->GetWindow();

	m_activeTiles.clear();
	m_activeTilesData.clear();
	m_activeOccluderTiles.clear();
	m_activeOccluderTilesData.clear();
	
	for (unsigned int i = 0; i < m_map.size(); i++)
	{
		glm::vec3 pos = m_map[i]->GetPos();

		if (pos.x > cameraPos.x - DEFAULT_ENTITY_SIZE && pos.x < cameraPos.x + (float)window->GetWidth() + DEFAULT_ENTITY_SIZE &&
			pos.y > cameraPos.y - DEFAULT_ENTITY_SIZE && pos.y < cameraPos.y + (float)window->GetHeight() + DEFAULT_ENTITY_SIZE)
		{
			if (m_map[i]->IsOccluder())
			{
				m_activeOccluderTiles.push_back(m_map[i]);
				m_activeOccluderTilesData.push_back(m_map[i]->GetData());
			}
			else
			{
				m_activeTiles.push_back(m_map[i]);
				m_activeTilesData.push_back(m_map[i]->GetData());
			}
		}
	}
}