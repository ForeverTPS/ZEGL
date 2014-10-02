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

#ifndef MATERIAL_H
#define MATERIAL_H

#include "mappedValues.h"
#include "mymath.h"
#include "logfile.h"
#include "texture.h"
#include "util.h"
#include <cassert>
#include <map>

class MaterialData : public ReferenceCounter, public MappedValues
{
public:
protected:
private:
};

class Material
{
public:
	Material(const std::string& materialName = "") :
		m_materialName(materialName)
	{
		if (materialName.length() > 0)
		{
			std::map<std::string, MaterialData*>::const_iterator it = s_resourceMap.find(materialName);

			if (it == s_resourceMap.end())
			{
				LogFile::AddEntry("Error: Material " + materialName + " has not been initialized!", LogFile::LOG_ERROR);
				assert(0 != 0);
			}

			m_materialData = it->second;
			m_materialData->AddReference();
		}
	}

	Material(const Material& other) :
		m_materialData(other.m_materialData),
		m_materialName(other.m_materialName)
	{
		m_materialData->AddReference();
	}

	Material(const std::string& materialName, const Texture& diffuse, float specularIntensity, float specularPower,
		const Texture& normalMap = Texture("default_normal.jpg"),
		const Texture& dispMap = Texture("default_disp.png"), float dispMapScale = 0.0f, float dispMapOffset = 0.0f) :
		m_materialName(materialName)
	{
		m_materialData = new MaterialData();
		s_resourceMap[m_materialName] = m_materialData;

		m_materialData->SetTexture("diffuse", diffuse);
		m_materialData->SetFloat("specularIntensity", specularIntensity);
		m_materialData->SetFloat("specularPower", specularPower);
		m_materialData->SetTexture("normalMap", normalMap);
		m_materialData->SetTexture("dispMap", dispMap);

		float baseBias = dispMapScale / 2.0f;
		m_materialData->SetFloat("dispMapScale", dispMapScale);
		m_materialData->SetFloat("dispMapBias", -baseBias + baseBias * dispMapOffset);
	}

	virtual ~Material()
	{
		if (m_materialData && m_materialData->RemoveReference())
		{
			if (m_materialName.length() > 0)
			{
				s_resourceMap.erase(m_materialName);
			}

			Util::SafeDelete(m_materialData);
		}
	}

	inline void SetVector3f(const std::string& name, const Vector3f& value) { m_materialData->SetVector3f(name, value); }
	inline void SetFloat(const std::string& name, float value)              { m_materialData->SetFloat(name, value); }
	inline void SetTexture(const std::string& name, const Texture& value)   { m_materialData->SetTexture(name, value); }

	inline const Vector3f& GetVector3f(const std::string& name) const { return m_materialData->GetVector3f(name); }
	inline float GetFloat(const std::string& name)              const { return m_materialData->GetFloat(name); }
	inline const Texture& GetTexture(const std::string& name)   const { return m_materialData->GetTexture(name); }

protected:
private:
	Material& operator=(Material const&) = delete;

	static std::map<std::string, MaterialData*> s_resourceMap;

	MaterialData* m_materialData;
	std::string   m_materialName;
};

#endif
