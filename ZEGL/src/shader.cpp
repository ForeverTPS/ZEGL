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
#include "logfile.h"
#include "util.h"
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

namespace Shaders
{
	GLuint CreateShader(const std::string& fileName, GLenum type)
	{
		GLuint shader = glCreateShader(type);

		if (shader == 0)
		{
			snprintf(LogFile::s_errorMsg, sizeof(LogFile::s_errorMsg), "Error creating shader type %d", type);
			LOG_ENTRY(LogFile::s_errorMsg, LogFile::LOG_ERROR);
			LOG_CLEANUP();
			exit(1);
		}

		std::string shaderSource = LoadShader(fileName);

		const GLchar* p[1];
		p[0] = shaderSource.c_str();
		GLint lengths[1];
		lengths[0] = (int)shaderSource.length();

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

		return shader;
	}

	std::string LoadShader(const std::string& fileName)
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
	}

	void CompileShader(GLuint program) 
	{
		glLinkProgram(program);
		CheckShader(program, GL_LINK_STATUS, true, "Error linking shader program");

		glValidateProgram(program);
		CheckShader(program, GL_VALIDATE_STATUS, true, "Invalid shader program");
	}

	static void CheckShader(int shader, int flag, bool isProgram, const std::string& errorMessage)
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
}