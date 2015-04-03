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

#include "textureatlas.h"
#include "logger.h"
#include "tinyxml2.h"
#include "util.h"

using namespace tinyxml2;
using namespace ZEGL;

std::unordered_map<std::string, TextureAtlasData*> TextureAtlas::s_resourceMap;

TextureAtlasData::TextureAtlasData(const std::string& fileName)
{
	ParseTextureAtlas(fileName);
}

void TextureAtlasData::ParseTextureAtlas(const std::string& fileName)
{
	bool success = true;
	std::string error;

	XMLDocument doc;
	if (doc.LoadFile(("./res/textures/" + fileName).c_str()) != XML_NO_ERROR)
	{
		success = false;
		error = "Failed loading TextureAtlas: " + fileName;
	}
	else
	{
		XMLElement* region = doc.FirstChildElement("Region");
		if (region == nullptr)
		{
			success = false;
			error = "No REGION nodes defined.";
		}
		else
		{
			while (region != nullptr)
			{
				std::string regionName;
				TextureRegion textureRegion;

				regionName = region->Attribute("name");
				region->QueryFloatAttribute("x", &textureRegion.x);
				region->QueryFloatAttribute("y", &textureRegion.y);
				region->QueryFloatAttribute("w", &textureRegion.w);
				region->QueryFloatAttribute("h", &textureRegion.h);

				m_textureRegions.insert(std::pair<std::string, TextureRegion>(regionName, textureRegion));

				region = region->NextSiblingElement();
			}
		}
	}

	if (!success)
	{
		LOG_ERROR(error.c_str());
		LOG_CLOSE();
		exit(1);
	}
}

TextureAtlas::TextureAtlas(const std::string& fileName)
{
	m_fileName = fileName;

	std::unordered_map<std::string, TextureAtlasData*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end())
	{
		m_textureAtlasData = it->second;
		m_textureAtlasData->AddReference();
	}
	else
	{
		m_textureAtlasData = new TextureAtlasData(m_fileName);

		s_resourceMap.insert(std::pair<std::string, TextureAtlasData*>(fileName, m_textureAtlasData));
	}
}

TextureAtlas::TextureAtlas(const TextureAtlas& textureAtlas) :
	m_textureAtlasData(textureAtlas.m_textureAtlasData),
	m_fileName(textureAtlas.m_fileName)
{
	m_textureAtlasData->AddReference();
}

TextureAtlas::~TextureAtlas()
{
	if (m_textureAtlasData && m_textureAtlasData->RemoveReference())
	{
		if (m_fileName.length() > 0)
		{
			s_resourceMap.erase(m_fileName);
		}

		Util::SafeDelete(m_textureAtlasData);
	}
}

TextureRegion TextureAtlas::GetRegion(const std::string& regionName) const
{
	TextureRegion result = { 0.0f, 0.0f, 0.0f, 0.0f };
	std::unordered_map<std::string, TextureAtlasData*>::const_iterator it = s_resourceMap.find(m_fileName);
	if (it != s_resourceMap.end())
	{
		std::unordered_map<std::string, TextureRegion>::const_iterator it2 = it->second->GetRegions().find(regionName);
		if (it2 != it->second->GetRegions().end())
		{
			result = it2->second;
		}
	}

	return result;
}