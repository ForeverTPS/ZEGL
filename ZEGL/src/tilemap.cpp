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
#include "light.h"
#include "logger.h"
#include "shader.h"
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

TileMap::TileMap(const std::string& fileName) :
	m_VAO(0),
	m_VAB(0),
	m_bytesAllocated(0)
{
	Load(fileName);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VAB);
	glBindBuffer(GL_ARRAY_BUFFER, m_VAB);

	glEnableVertexAttribArray(0); // pos
	glEnableVertexAttribArray(1); // size
	glEnableVertexAttribArray(2); // texCoords0
	glEnableVertexAttribArray(3); // texCoords1
	glEnableVertexAttribArray(4); // texCoords2
	glEnableVertexAttribArray(5); // texCoords3

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)16);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)20);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)28);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)36);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(EntityData), (GLvoid*)44);

	glVertexAttribDivisor(0, 1);
	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

TileMap::~TileMap()
{
	glDeleteBuffers(1, &m_VAB);
	glDeleteVertexArrays(1, &m_VAO);
}

void TileMap::Load(const std::string& fileName)
{
	bool success = true;
	std::string error;

	XMLDocument doc;
	if (doc.LoadFile((fileName + ".tdef").c_str()) != XML_NO_ERROR)
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
			file.open((fileName).c_str());

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

void TileMap::Update(const Window* window, const glm::vec3& cameraPos)
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

	glBindBuffer(GL_ARRAY_BUFFER, m_VAB);
	size_t bytesNeeded = sizeof(EntityData) * m_activeTilesData.size();
	if (bytesNeeded > m_bytesAllocated) {
		glBufferData(GL_ARRAY_BUFFER, bytesNeeded, &m_activeTilesData[0], GL_STREAM_DRAW);
		m_bytesAllocated = bytesNeeded;
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, 0, bytesNeeded, &m_activeTilesData[0]);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TileMap::Render(Game* game)
{
	Texture texture = m_activeTiles[0].GetTexture();
	Texture normalMap = m_activeTiles[0].GetNormalMap();

	Shader shader = game->GetActiveLight()->GetShader();

	shader.Bind();
	shader.UpdateUniforms(game);

	texture.Bind(0);
	normalMap.Bind(1);

	glBindVertexArray(m_VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 16);
	glBindVertexArray(0);

	shader.UnBind();
}