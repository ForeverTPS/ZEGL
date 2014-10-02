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
#include "mymath.h"
#include <map>

class MappedValues
{
public:
	MappedValues() :
		m_defaultTexture(Texture("defaultTexture.png")),
		m_defaultVector3f(Vector3f(0, 0, 0)) {}

	inline void SetVector3f(const std::string& name, const Vector3f& value) { m_vector3fMap[name] = value; }
	inline void SetFloat(const std::string& name, float value)              { m_floatMap[name] = value; }
	inline void SetTexture(const std::string& name, const Texture& value)   { m_textureMap[name] = value; }

	inline const Vector3f& GetVector3f(const std::string& name) const
	{
		std::map<std::string, Vector3f>::const_iterator it = m_vector3fMap.find(name);
		if (it != m_vector3fMap.end())
		{
			return it->second;
		}

		return m_defaultVector3f;
	}

	inline float GetFloat(const std::string& name) const
	{
		std::map<std::string, float>::const_iterator it = m_floatMap.find(name);
		if (it != m_floatMap.end())
		{
			return it->second;
		}

		return 0;
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

protected:
private:
	//MappedValues(MappedValues const&) = delete;
	MappedValues& operator=(MappedValues const&) = delete;

	std::map<std::string, Vector3f> m_vector3fMap;
	std::map<std::string, float>	m_floatMap;
	std::map<std::string, Texture>	m_textureMap;

	Texture		m_defaultTexture;
	Vector3f	m_defaultVector3f;
};

#endif
