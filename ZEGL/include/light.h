/*
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

#pragma once

#include "entity.h"
#include "shader.h"

namespace ZEGL
{
	__declspec(align(16)) class Light : public Entity
	{
	public:
		Light(const Shader& shader) :
			m_lightColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_ambientColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)),
			m_falloff(glm::vec3(0.4f, 3.0f, 20.0f)),
			m_shader(shader),
			Entity(glm::vec3(0.0f, 0.0f, 0.075f)) {}

		Light(const Shader& shader, const glm::vec3& pos, const glm::vec4& lightCol, const glm::vec4& ambientCol, const glm::vec3& falloff) :
			m_lightColor(lightCol),
			m_ambientColor(ambientCol),
			m_falloff(falloff),
			m_shader(shader),
			Entity(pos) {}

		virtual ~Light() {}

		inline const glm::vec4& GetLightColor()		const { return m_lightColor; }
		inline const float GetLightIntensity()		const { return m_lightColor.a; }
		inline const glm::vec4& GetAmbientColor()	const { return m_ambientColor; }
		inline const float GetAmbientIntensity()	const { return m_ambientColor.a; }
		inline const glm::vec3& GetFalloff()		const { return m_falloff; }
		inline const Shader& GetShader()			const { return m_shader; }

		inline void SetLightColor(const glm::vec4& lightCol)		{ m_lightColor = lightCol; }
		inline void SetLightIntensity(float lightIntensity)			{ m_lightColor.a = lightIntensity; }
		inline void SetAmbientColor(const glm::vec4& ambientCol)	{ m_ambientColor = ambientCol; }
		inline void SetAmbientIntensity(float ambientIntensity)		{ m_ambientColor.a = ambientIntensity; }
		inline void SetFalloff(const glm::vec3& falloff)			{ m_falloff = falloff; }

		void* operator new(size_t i)	{ return _mm_malloc(i, 16); }
		void  operator delete(void* p)	{ _mm_free(p); }

	private:
		glm::vec4	m_lightColor;
		glm::vec4	m_ambientColor;
		glm::vec3	m_falloff;

		Shader		m_shader;
	};
}