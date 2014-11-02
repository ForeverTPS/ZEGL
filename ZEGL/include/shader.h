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

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <string>
#include <vector>

namespace Shaders
{
	GLuint		CreateShader(const std::string& fileName, GLenum type);

	std::string	LoadShader(const std::string& fileName);
	void		CheckShader(int shader, int flag, bool isProgram, const std::string& errorMessage);
}

class Shader
{
public:
	Shader() : m_program(-1) {}
	virtual ~Shader()
	{
		for (std::vector<GLuint>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
		{
			glDetachShader(m_program, *it);
			glDeleteShader(*it);
		}
		glDeleteProgram(m_program);
	}

	inline void SetProgram(GLuint program)	{ m_program = program; }
	inline void AddShader(GLuint shader)	{ m_shaders.push_back(shader); }

protected:
private:
	GLuint				m_program;
	std::vector<GLuint> m_shaders;
};

#endif
