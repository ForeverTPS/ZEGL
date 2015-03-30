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

#pragma once

#include "referencecounter.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <unordered_map>

namespace ZEGL
{
	class Camera;
	class Game;

	class ShaderData : public ReferenceCounter
	{
	public:
		ShaderData(const std::string& fileName);
		virtual ~ShaderData();

		inline int GetProgram()														const { return m_program; }
		inline const std::vector<int>& GetShaders()									const { return m_shaders; }
		inline const std::vector<std::string>& GetUniformNames()					const { return m_uniformNames; }
		inline const std::vector<std::string>& GetUniformTypes()					const { return m_uniformTypes; }
		inline const std::unordered_map<std::string, unsigned int>& GetUniformMap() const { return m_uniformMap; }

	protected:
	private:
		ShaderData(ShaderData const&) = delete;
		ShaderData& operator=(ShaderData const&) = delete;

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

	class Shader
	{
	public:
		Shader(const std::string& fileName = "basic_shader");
		Shader(const Shader& other);
		virtual ~Shader();

		void Load(const std::string& fileName);

		void Bind() const;
		void UnBind() const;

		void SetUniformi(const std::string& uniformName, int value) const;
		void SetUniformf(const std::string& uniformName, float value) const;
		void SetUniformVector2f(const std::string& uniformName, const glm::vec2& value) const;
		void SetUniformVector3f(const std::string& uniformName, const glm::vec3& value) const;
		void SetUniformVector4f(const std::string& uniformName, const glm::vec4& value) const;
		void SetUniformMatrix4f(const std::string& uniformName, const glm::mat4& value) const;

		void UpdateUniforms(Game* game) const;

	protected:
	private:
		//Shader(const Shader& other) = delete;
		Shader& operator=(Shader const&) = delete;

		static std::unordered_map<std::string, ShaderData*> s_resourceMap;

		ShaderData* m_shaderData;
		std::string	m_fileName;
	};
}
