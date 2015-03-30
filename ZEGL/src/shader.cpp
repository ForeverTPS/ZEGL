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

#include "camera.h"
#include "game.h"
#include "light.h"
#include "logfile.h"
#include "util.h"
#include "window.h"
#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>

using namespace ZEGL;

static void			CheckShaderError(int shader, int flag, bool isProgram, const std::string& errorMessage);
static std::string	LoadShader(const std::string& fileName);

std::unordered_map<std::string, ShaderData*> Shader::s_resourceMap;

int			ShaderData::s_supportedOpenGLLevel = 0;
std::string	ShaderData::s_glslVersion = "";

static void CheckShaderError(int shader, int flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (!success)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "%s: '%s'", errorMessage.c_str(), error);
		LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
	}
}

static std::string LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open(("./res/shaders/" + fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
};

ShaderData::ShaderData(const std::string& fileName)
{
	m_program = glCreateProgram();

	if (m_program == 0)
	{
		snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "Error creating shader program - %s", fileName.c_str());
		LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
		LOG_CLEANUP();
		exit(1);
	}

	if (s_supportedOpenGLLevel == 0)
	{
		int majorVersion;
		int minorVersion;

		glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
		glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

		s_supportedOpenGLLevel = majorVersion * 100 + minorVersion * 10;

		if (s_supportedOpenGLLevel >= 330)
		{
			std::ostringstream convert;
			convert << s_supportedOpenGLLevel;

			s_glslVersion = convert.str();
		}
		else if (s_supportedOpenGLLevel >= 320)
		{
			s_glslVersion = "150";
		}
		else if (s_supportedOpenGLLevel >= 310)
		{
			s_glslVersion = "140";
		}
		else if (s_supportedOpenGLLevel >= 300)
		{
			s_glslVersion = "130";
		}
		else if (s_supportedOpenGLLevel >= 210)
		{
			s_glslVersion = "120";
		}
		else if (s_supportedOpenGLLevel >= 200)
		{
			s_glslVersion = "110";
		}
		else
		{
			snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "Error: OpenGL Version %d.%d does not support shaders.", majorVersion, minorVersion);
			LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
			LOG_CLEANUP();
			exit(1);
		}
	}

	std::string vertexShaderText = LoadShader(fileName + "_vs.glsl");
	std::string fragmentShaderText = LoadShader(fileName + "_fs.glsl");

	AddProgram(vertexShaderText, GL_VERTEX_SHADER);
	AddProgram(fragmentShaderText, GL_FRAGMENT_SHADER);

	AddAllAttributes(vertexShaderText);

	CompileShader();

	AddAllUniforms(vertexShaderText);
	AddAllUniforms(fragmentShaderText);
}

ShaderData::~ShaderData()
{
	for (std::vector<int>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		glDetachShader(m_program, *it);
		glDeleteShader(*it);
	}
	glDeleteProgram(m_program);
}

void ShaderData::AddProgram(const std::string& text, int type)
{
	int shader = glCreateShader(type);

	if (shader == 0)
	{
		snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "Error creating shader type %d", type);
		LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
		LOG_CLEANUP();
		exit(1);
	}

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = (int)text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar InfoLog[1024];

		glGetShaderInfoLog(shader, 1024, NULL, InfoLog);
		snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "Error compiling shader type %d: '%s'", shader, InfoLog);
		LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
		LOG_CLEANUP();
		exit(1);
	}

	glAttachShader(m_program, shader);
	m_shaders.push_back(shader);
}

void ShaderData::AddAllAttributes(const std::string& vertexShaderText)
{
	const std::string ATTRIBUTE_KEY = "attribute";
	int currentAttribLocation = 0;

	size_t attributeLocation = vertexShaderText.find(ATTRIBUTE_KEY);
	while (attributeLocation != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = vertexShaderText.rfind("\n", attributeLocation);

		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = vertexShaderText.substr(lastLineEnd, attributeLocation - lastLineEnd);
			isCommented = ((potentialCommentSection.find("//") != std::string::npos) || (potentialCommentSection.find("#") != std::string::npos));
		}

		if (!isCommented)
		{
			size_t begin = attributeLocation + ATTRIBUTE_KEY.length();
			size_t end = vertexShaderText.find(";", begin);

			std::string attributeLine = vertexShaderText.substr(begin + 1, end - begin - 1);

			begin = attributeLine.find(" ");
			std::string attributeName = attributeLine.substr(begin + 1);

			size_t arrayBrackets = attributeName.find("[");
			if (arrayBrackets != std::string::npos)
			{
				attributeName = attributeName.substr(0, arrayBrackets);
			}

			glBindAttribLocation(m_program, currentAttribLocation, attributeName.c_str());
			currentAttribLocation++;
		}
		attributeLocation = vertexShaderText.find(ATTRIBUTE_KEY, attributeLocation + ATTRIBUTE_KEY.length());
	}
}

void ShaderData::AddAllUniforms(const std::string& shaderText)
{
	static const std::string UNIFORM_KEY = "uniform";

	size_t uniformLocation = shaderText.find(UNIFORM_KEY);
	while (uniformLocation != std::string::npos)
	{
		bool isCommented = false;
		size_t lastLineEnd = shaderText.rfind("\n", uniformLocation);

		if (lastLineEnd != std::string::npos)
		{
			std::string potentialCommentSection = shaderText.substr(lastLineEnd, uniformLocation - lastLineEnd);
			isCommented = potentialCommentSection.find("//") != std::string::npos;
		}

		if (!isCommented)
		{
			size_t begin = uniformLocation + UNIFORM_KEY.length();
			size_t end = shaderText.find(";", begin);

			std::string uniformLine = shaderText.substr(begin + 1, end - begin - 1);

			begin = uniformLine.find(" ");
			std::string uniformName = uniformLine.substr(begin + 1);
			std::string uniformType = uniformLine.substr(0, begin);

			m_uniformNames.push_back(uniformName);
			m_uniformTypes.push_back(uniformType);
			AddUniform(uniformName, uniformType);
		}
		uniformLocation = shaderText.find(UNIFORM_KEY, uniformLocation + UNIFORM_KEY.length());
	}
}

void ShaderData::AddUniform(const std::string& uniformName, const std::string& uniformType)
{
	unsigned int location = glGetUniformLocation(m_program, uniformName.c_str());

	if (location == 0xFFFFFFFF)
	{
		snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "Invalid uniform location -  %s", uniformName.c_str());
		LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
		ASSERT(0 != 0, LogFile::s_errorMsg);
	}

	m_uniformMap.insert(std::pair<std::string, unsigned int>(uniformName, location));
}

void ShaderData::CompileShader() const
{
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Invalid shader program");
}

Shader::Shader(const std::string& fileName)
{
	m_fileName = fileName;

	std::unordered_map<std::string, ShaderData*>::const_iterator it = s_resourceMap.find(fileName);
	if (it != s_resourceMap.end())
	{
		m_shaderData = it->second;
		m_shaderData->AddReference();
	}
	else
	{
		m_shaderData = new ShaderData(fileName);
		s_resourceMap.insert(std::pair<std::string, ShaderData*>(fileName, m_shaderData));
	}
}

Shader::Shader(const Shader& other) :
	m_shaderData(other.m_shaderData),
	m_fileName(other.m_fileName)
{
	m_shaderData->AddReference();
}

Shader::~Shader()
{
	if (m_shaderData && m_shaderData->RemoveReference())
	{
		if (m_fileName.length() > 0)
		{
			s_resourceMap.erase(m_fileName);
		}

		Util::SafeDelete(m_shaderData);
	}
}

void Shader::Bind() const
{
	glUseProgram(m_shaderData->GetProgram());
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::UpdateUniforms(Game* game) const
{
	Camera camera = game->GetCamera();
	const Light* light = game->GetActiveLight();

	for (unsigned int i = 0; i < m_shaderData->GetUniformNames().size(); i++)
	{
		std::string uniformName = m_shaderData->GetUniformNames()[i];
		std::string uniformType = m_shaderData->GetUniformTypes()[i];

		if (uniformName == "u_diffuse")
		{
			SetUniformi("u_diffuse", 0);
		}
		else if (uniformName == "u_normals")
		{
			SetUniformi("u_normals", 1);
		}
		else if (uniformName.substr(0, 2) == "L_")
		{
			std::string unprefixedName = uniformName.substr(2, uniformName.length());	

			if (unprefixedName == "AmbientColor")
			{
				glm::vec3 color;
				float intensity;
				if (light)
				{
					color = light->GetAmbientColor();
					intensity = light->GetAmbientIntensity();
				}
				else
				{
					color = game->GetAmbientColor();
					intensity = game->GetAmbientIntensity();
				}
				
				SetUniformVector4f(uniformName, glm::vec4(color, intensity));
			}
			else if (unprefixedName == "Resolution")
			{
				SetUniformVector2f(uniformName, glm::vec2((float)game->GetWindow()->GetWidth(), (float)game->GetWindow()->GetHeight()));
			}
			else if (unprefixedName == "LightPos")
			{
				if (light)
				{
					SetUniformVector3f(uniformName, light->GetPos());
				}
				else
				{
					throw "No active light";
				}
			}
			else if (unprefixedName == "LightColor")
			{
				if (light)
				{
					glm::vec3 color = light->GetLightColor();
					float intensity = light->GetLightIntensity();
					SetUniformVector4f(uniformName, glm::vec4(color, intensity));
				}
				else
				{
					throw "No active light";
				}
			}
			else if (unprefixedName == "Falloff")
			{
				if (light)
				{
					SetUniformVector3f(uniformName, light->GetFalloff());
				}
				else
				{
					throw "No active light";
				}
			}
			else
			{
				throw "Invalid Uniform: " + uniformName;
			}
		}
		else if (uniformName == "MVP")
		{
			SetUniformMatrix4f(uniformName, camera.GetTransform(game->GetWindow()));
		}
		else
		{
			throw "Invalid Uniform: " + uniformName;
		}
	}
}

void Shader::SetUniformi(const std::string& uniformName, int value) const
{
	glUniform1i(m_shaderData->GetUniformMap().at(uniformName), value);
}

void Shader::SetUniformf(const std::string& uniformName, float value) const
{
	glUniform1f(m_shaderData->GetUniformMap().at(uniformName), value);
}

void Shader::SetUniformVector2f(const std::string& uniformName, const glm::vec2& value) const
{
	glUniform2f(m_shaderData->GetUniformMap().at(uniformName), value.x, value.y);
}

void Shader::SetUniformVector3f(const std::string& uniformName, const glm::vec3& value) const
{
	glUniform3f(m_shaderData->GetUniformMap().at(uniformName), value.x, value.y, value.z);
}

void Shader::SetUniformVector4f(const std::string& uniformName, const glm::vec4& value) const
{
	glUniform4f(m_shaderData->GetUniformMap().at(uniformName), value.x, value.y, value.z, value.w);
}

void Shader::SetUniformMatrix4f(const std::string& uniformName, const glm::mat4& value) const
{
	glUniformMatrix4fv(m_shaderData->GetUniformMap().at(uniformName), 1, GL_FALSE, &(value[0][0]));
}