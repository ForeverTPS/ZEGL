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

#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "referencecounter.h"
#include <string>
#include <map>

struct TextureRegion
{
	float x, y, w, h;
};

class TextureAtlasData : public ReferenceCounter
{
public:
	TextureAtlasData(const std::string& fileName);
	virtual ~TextureAtlasData() {}

	inline const std::map<std::string, TextureRegion>& GetRegions() const { return m_textureRegions; }

protected:
private:
	TextureAtlasData(TextureAtlasData const&) = delete;
	TextureAtlasData& operator=(TextureAtlasData const&) = delete;

	void ParseTextureAtlas(const std::string& fileName);

	std::map<std::string, TextureRegion> m_textureRegions;
};

class TextureAtlas
{
public:
	TextureAtlas(const std::string& fileName);
	TextureAtlas(TextureAtlas const&);
	virtual ~TextureAtlas();

	TextureRegion GetRegion(const std::string& regionName) const;

protected:
private:
	//TextureAtlas(TextureAtlas const&) = delete;
	TextureAtlas& operator=(TextureAtlas const&) = delete;

	static std::map<std::string, TextureAtlasData*> s_resourceMap;

	TextureAtlasData*	m_textureAtlasData;
	std::string			m_fileName;
};

#endif