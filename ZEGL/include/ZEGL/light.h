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
	/**
	* Lights are entities used to illuminate the game world and are used by
	* the shaders to achieve this. They can have different colors, intensities
	* and be configured to fade out from the center differently. 
	*/
	__declspec(align(16)) class Light : public Entity
	{
	public:
		/**
		* Simple constructor which only takes the minimum required element that is
		* a shader. All other members are set to defaults.
		*
		* \param[in] shader The shader associated with this light
		*
		* \see [Entity][Shader]
		*/
		Light(const Shader& shader) :
			m_lightColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_ambientColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.2f)),
			m_falloff(glm::vec3(0.4f, 3.0f, 20.0f)),
			m_shader(shader),
			Entity(glm::vec3(0.0f, 0.0f, 0.075f)) {}

		/**
		* Full constructor which requires values for all members.
		*
		* \param[in] shader The shader associated with this light
		* \param[in] pos Position of the light
		* \param[in] lightCol Color of the main light emitted (alpha is used for intensity)
		* \param[in] ambientCol Color of the ambient light emitted (alpha is used for intensity)
		* \param[in] falloff Values to use (x,y,z) in calculating attenuation using 
		* [Constant Linear Quadratic Falloff](https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff)
		*
		* \see [Entity][Shader]
		*/
		Light(const Shader& shader, const glm::vec3& pos, const glm::vec4& lightCol, const glm::vec4& ambientCol, const glm::vec3& falloff) :
			m_lightColor(lightCol),
			m_ambientColor(ambientCol),
			m_falloff(falloff),
			m_shader(shader),
			Entity(pos) {}

		virtual ~Light() {}

		/**
		* Virtual update function to be implemented by any inherited classes
		*
		* Description in full
		*
		* \param[in] delta Delta frame time in milliseconds
		*/
		virtual void Update(float delta) {}

		/**
		* Get the main color of the light.
		*
		* \return The main light color (alpha is used for intensity)
		*/
		inline const glm::vec4& GetLightColor() const { return m_lightColor; }


		/**
		* Set the main color of the light.
		*
		* \param[in] lightCol New color value for the light (alpha is used for intensity)
		*/
		inline void SetLightColor(const glm::vec4& lightCol) { m_lightColor = lightCol; }
		

		/**
		* Get the intensity of the main light color.
		*
		* \return The intensity of the main light color
		*/
		inline const float GetLightIntensity() const { return m_lightColor.a; }
		

		/**
		* Set the intensity of the main light color.
		*
		* \param[in] lightIntensity New intensity value for the main light color
		*/
		inline void SetLightIntensity(float lightIntensity) { m_lightColor.a = lightIntensity; }
		
		/**
		* Get the ambient color of the light.
		*
		* \return The ambient light color (alpha is used for intensity)
		*/
		inline const glm::vec4& GetAmbientColor() const { return m_ambientColor; }
		
		/**
		* Set the ambient color of the light.
		*
		* \param[in] ambientCol New color value for the ambientCol light (alpha is used for intensity)
		*/
		inline void SetAmbientColor(const glm::vec4& ambientCol) { m_ambientColor = ambientCol; }
		
		/**
		* Get the intensity of the ambient light color.
		*
		* \return The intensity of the ambient light color
		*/
		inline const float GetAmbientIntensity() const { return m_ambientColor.a; }
		
		/**
		* Set the intensity of the ambient light color.
		*
		* \param[in] ambientIntensity New ambient value for the main light color
		*/
		inline void SetAmbientIntensity(float ambientIntensity) { m_ambientColor.a = ambientIntensity; }
		
		/**
		* Get the fall off values for the light to be used for attenuation calculation.
		*
		* \return The fall off values in a glm::vec3
		*/
		inline const glm::vec3& GetFalloff() const { return m_falloff; }
		
		/**
		* Set new fall off values attenuation calculation.
		*
		* \param[in] falloff New fall off values in a glm::vec3
		*/
		inline void SetFalloff(const glm::vec3& falloff) { m_falloff = falloff; }
		
		/**
		* Get the shader associated with this light.
		*
		* Each light must use a shader for rendering and so each light stores
		* a shader. Shaders use shared resources so the overhead is minimal regardless
		* of the number of lights.
		*
		* \return The shader associated with this light
		*
		* \see [Shader]
		*/
		inline const Shader& GetShader() const { return m_shader; }
	
		void* operator new(size_t i)	{ return _mm_malloc(i, 16); }
		void  operator delete(void* p)	{ _mm_free(p); }

	private:
		glm::vec4	m_lightColor;
		glm::vec4	m_ambientColor;
		glm::vec3	m_falloff;

		Shader		m_shader;
	};
}