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

#ifndef MAPPEDVALUES_H
#define MAPPEDVALUES_H

#include "texture.h"
#include "textureatlas.h"
#include "util.h"
#include "glm/glm.hpp"
#include <map>

class MappedValues
{
public:
	MappedValues() :
		m_textureAtlas(nullptr),
		m_defaultTexture(Texture("default.png")),
		m_defaultVec3(glm::vec3(0.0f)),
		m_defaultString("") {}

	virtual ~MappedValues()
	{
		Util::SafeDelete(m_textureAtlas);
	}

	inline void SetResourcePath(const std::string& name, const std::string& value)	{ m_resourcePathsMap[name] = value; }
	inline void SetVec3(const std::string& name, const glm::vec3& value)			{ m_vec3Map[name] = value; }
	inline void SetFloat(const std::string& name, float value)						{ m_floatMap[name] = value; }
	inline void SetTexture(const std::string& name, const Texture& value)			{ m_textureMap[name] = value; }
	inline void SetTextureAtlas(TextureAtlas* textureAtlas)							{ m_textureAtlas = textureAtlas; }

	inline const std::string& GetResourcePath(const std::string& name) const
	{
		std::map<std::string, std::string>::const_iterator it = m_resourcePathsMap.find(name);
		if (it != m_resourcePathsMap.end())
		{
			return it->second;
		}

		return m_defaultString;
	}
	
	inline const glm::vec3& GetVec3(const std::string& name) const
	{
		std::map<std::string, glm::vec3>::const_iterator it = m_vec3Map.find(name);
		if (it != m_vec3Map.end())
		{
			return it->second;
		}

		return m_defaultVec3;
	}

	inline float GetFloat(const std::string& name) const
	{
		std::map<std::string, float>::const_iterator it = m_floatMap.find(name);
		if (it != m_floatMap.end())
		{
			return it->second;
		}

		return 0.0f;
	}

	inline const Texture& GetTexture(const std::string& name) const
	{
		std::map<std::string, Texture>::const_iterator it = m_textureMap.find(name);
		if (it != m_textureMap.end())
		{
			return it->second;
		}

		return m_defaultTexture;
	}

	inline const TextureAtlas* GetTextureAtlas() const { return m_textureAtlas; }

protected:
private:
	//MappedValues(MappedValues const&) = delete;
	MappedValues& operator=(MappedValues const&) = delete;

	std::map<std::string, std::string>	m_resourcePathsMap;

	std::map<std::string, glm::vec3>	m_vec3Map;
	std::map<std::string, float>		m_floatMap;
	std::map<std::string, Texture>		m_textureMap;

	TextureAtlas*	m_textureAtlas;

	Texture			m_defaultTexture;
	glm::vec3		m_defaultVec3;
	std::string		m_defaultString;
};

#endif