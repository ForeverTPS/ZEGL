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

#include "shader.h"
#include "camera.h"
#include "logfile.h"
#include "util.h"
#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>

static void			CheckShaderError(int shader, int flag, bool isProgram, const std::string& errorMessage);
static std::string	LoadShader(const std::string& fileName);

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

		fprintf(stderr, "%s: '%s'\n", errorMessage.c_str(), error);
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

Shader::Shader(const std::string& fileName)
{
	if (fileName != "")
	{
		Load(fileName);
	}
}

Shader::~Shader()
{
	for (std::vector<int>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
	{
		glDetachShader(m_program, *it);
		glDeleteShader(*it);
	}
	glDeleteProgram(m_program);
}

void Shader::Load(const std::string& fileName)
{
	m_fileName = fileName;

	m_program = glCreateProgram();

	if (m_program == 0)
	{
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
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

void Shader::AddProgram(const std::string& text, int type)
{
	int shader = glCreateShader(type);

	if (shader == 0)
	{
		fprintf(stderr, "Error creating shader type %d\n", type);
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
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", shader, InfoLog);

		exit(1);
	}

	glAttachShader(m_program, shader);
	m_shaders.push_back(shader);
}

void Shader::AddAllAttributes(const std::string& vertexShaderText)
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

			glBindAttribLocation(m_program, currentAttribLocation, attributeName.c_str());
			currentAttribLocation++;
		}
		attributeLocation = vertexShaderText.find(ATTRIBUTE_KEY, attributeLocation + ATTRIBUTE_KEY.length());
	}
}

void Shader::AddAllUniforms(const std::string& shaderText)
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

void Shader::AddUniform(const std::string& uniformName, const std::string& uniformType)
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

void Shader::CompileShader() const
{
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Invalid shader program");
}

void Shader::Bind() const
{
	glUseProgram(m_program);
}

void Shader::UnBind() const
{
	glUseProgram(0);
}

void Shader::UpdateUniforms(Camera& camera) const
{
	for (unsigned int i = 0; i < m_uniformNames.size(); i++)
	{
		std::string uniformName = m_uniformNames[i];
		std::string uniformType = m_uniformTypes[i];

		if (uniformName == "MVP")
		{
			SetUniformMatrix4f(uniformName, camera.GetTransform());
		}	
		else
		{
			//throw "Invalid Uniform: " + uniformName;
		}
	}
}

void Shader::SetUniformi(const std::string& uniformName, int value) const
{
	glUniform1i(m_uniformMap.at(uniformName), value);
}

void Shader::SetUniformf(const std::string& uniformName, float value) const
{
	glUniform1f(m_uniformMap.at(uniformName), value);
}

void Shader::SetUniformVector2f(const std::string& uniformName, const Vector2f& value) const
{
	glUniform2f(m_uniformMap.at(uniformName), value.GetX(), value.GetY());
}

void Shader::SetUniformVector3f(const std::string& uniformName, const Vector3f& value) const
{
	glUniform3f(m_uniformMap.at(uniformName), value.GetX(), value.GetY(), value.GetZ());
}

void Shader::SetUniformVector4f(const std::string& uniformName, const Vector4f& value) const
{
	glUniform4f(m_uniformMap.at(uniformName), value.GetX(), value.GetY(), value.GetZ(), value.GetW());
}

void Shader::SetUniformMatrix4f(const std::string& uniformName, const Matrix4f& value) const
{
	glUniformMatrix4fv(m_uniformMap.at(uniformName), 1, GL_FALSE, &(value[0][0]));
}