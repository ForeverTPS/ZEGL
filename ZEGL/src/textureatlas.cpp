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

#include "textureatlas.h"
#include "util.h"

std::map<std::string, TextureAtlasData*> TextureAtlas::s_resourceMap;

TextureAtlasData::TextureAtlasData(const std::string& fileName)
{

}

TextureAtlasData::~TextureAtlasData()
{
	Util::SafeDelete(m_texture);
}

TextureAtlas::TextureAtlas(const std::string& fileName)
{
	m_fileName = fileName;

	std::map<std::string, TextureAtlasData*>::const_iterator it = s_resourceMap.find(fileName);
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

//const TextureRegion& TextureAtlas::GetRegion(const std::string& regionName) const
//{
//
//}