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

#include "reference_counter.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace ZEGL
{
	class Camera;
	class Game;

	/**
	* Uses vertex and fragment shader code in GLSL to create a shader program
	* for rendering the game.
	*/
	class Shader
	{
	public:
		/**
		* Create a shader with the given name by either loading from file or
		* if the shader has already been loaded, incrementing the reference count.
		*
		* \param[in] fileName fileName The name of the shader with no path or extension
		*/
		Shader(const std::string& fileName = "./res/shaders/basic_shader");
		Shader(const Shader& other);
		~Shader();

		Shader& operator=(Shader const&) = delete;

		/**
		* Set the shader active for use in rendering
		*/
		void Bind() const;

		/**
		* Set the shader as no longer in use for rendering
		*/
		void UnBind() const;

		//@{
		/**
		* Update the specified uniform with a new value.
		*
		* \param[in] uniformName Name of the uniform to update
		* \param[in] value Value to assign to the uniform
		*/
		void SetUniformi(const std::string& uniformName, int value) const;
		void SetUniformf(const std::string& uniformName, float value) const;
		void SetUniformVector2f(const std::string& uniformName, const glm::vec2& value) const;
		void SetUniformVector3f(const std::string& uniformName, const glm::vec3& value) const;
		void SetUniformVector4f(const std::string& uniformName, const glm::vec4& value) const;
		void SetUniformMatrix4f(const std::string& uniformName, const glm::mat4& value) const;
		//@}


		/**
		* Updates all known uniforms.
		*
		* Called every from Game::Update() to update all hard-coded uniform options.
		*/
		void UpdateUniforms() const;

	protected:
	private:
		class ShaderData : public ReferenceCounter
		{
		public:
			ShaderData(const std::string& fileName);
			ShaderData(ShaderData const&) = delete;
			ShaderData& operator=(ShaderData const&) = delete;
			~ShaderData();

			inline int GetProgram() const { return m_program; }
			inline const std::vector<std::string>& GetUniformNames() const { return m_uniformNames; }
			inline const std::vector<std::string>& GetUniformTypes() const { return m_uniformTypes; }
			inline const std::unordered_map<std::string, unsigned int>& GetUniformMap() const { return m_uniformMap; }

		protected:
		private:
			void AddProgram(const std::string& text, int type);

			void AddAllAttributes(const std::string& vertexShaderText);
			void AddAllUniforms(const std::string& shaderText);
			void AddUniform(const std::string& uniformName, const std::string& uniformType);

			void CompileShader() const;

			static int										s_supportedOpenGLLevel;
			static std::string								s_glslVersion;

			int												m_program;
			std::vector<int>								m_shaders;
			std::vector<std::string>						m_uniformNames;
			std::vector<std::string>						m_uniformTypes;
			std::unordered_map<std::string, unsigned int>	m_uniformMap;
		};

		static std::unordered_map<std::string, ShaderData*> s_resourceMap;

		ShaderData* m_shaderData;
		std::string	m_fileName;
	};
}
