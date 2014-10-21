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

#include "texture.h"

struct TextureRegion
{
	float x, y, w, h;
};

class TextureAtlasData : public ReferenceCounter
{
public:
	TextureAtlasData(const std::string& fileName);
	virtual ~TextureAtlasData();

protected:
private:
	TextureAtlasData(TextureAtlasData const&) = delete;
	TextureAtlasData& operator=(TextureAtlasData const&) = delete;

	std::map<std::string, TextureRegion> m_textureRegions;

	Texture* m_texture;
};

class TextureAtlas
{
public:
	TextureAtlas(const std::string& fileName);
	virtual ~TextureAtlas();

	const TextureRegion& GetRegion(const std::string& regionName) const;

protected:
private:
	TextureAtlas(TextureAtlas const&) = delete;
	TextureAtlas& operator=(TextureAtlas const&) = delete;

	static std::map<std::string, TextureAtlasData*> s_resourceMap;

	TextureAtlasData*	m_textureAtlasData;
	std::string			m_fileName;
};