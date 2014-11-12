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

#include "glm/glm.hpp"
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

	inline void		Bind() { glUseProgram(m_program); }

	inline GLuint	GetProgram() const			{ return m_program; }
	inline void		SetProgram(GLuint program)	{ m_program = program; }

	inline void AddShader(GLuint shader)	
	{ 
		glAttachShader(m_program, shader); 
		m_shaders.push_back(shader); 
	}

	inline void CompileShader()
	{
		glLinkProgram(m_program);
		Shaders::CheckShader(m_program, GL_LINK_STATUS, true, "Error linking shader program");

		glValidateProgram(m_program);
		Shaders::CheckShader(m_program, GL_VALIDATE_STATUS, true, "Invalid shader program");
	}

	inline void BindVertices(GLuint buffer) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		GLuint location = glGetAttribLocation(m_program, "aVertex");
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	inline void BindTexture(const std::string& loc, GLuint active, const GLuint tex) const 
	{
		GLuint location = glGetUniformLocation(m_program, loc.c_str());
		glActiveTexture(GL_TEXTURE0 + active);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(location, active);
	}

	inline void BindValue(const std::string& loc, const float f) const
	{
		GLint ul = glGetUniformLocation(m_program, loc.c_str());
		glUniform1f(ul, f);
	}

	inline void BindValue(const std::string& loc, const bool b) const 
	{
		GLint ul = glGetUniformLocation(m_program, loc.c_str());
		glUniform1i(ul, b);
	}

	inline void BindValue(const std::string& loc, const glm::vec2& v) const 
	{
		GLint ul = glGetUniformLocation(m_program, loc.c_str());
		glUniform2f(ul, v.x, v.y);
	}

	inline void BindValue(const std::string& loc, const glm::vec3& v) const 
	{
		GLint ul = glGetUniformLocation(m_program, loc.c_str());
		glUniform3f(ul, v.x, v.y, v.z);
	}

	inline void BindValue(const std::string& loc, const glm::vec4& v) const
	{
		GLint ul = glGetUniformLocation(m_program, loc.c_str());
		glUniform4f(ul, v.r, v.g, v.b, v.a);
	}

	inline void BindValue(const std::string& loc, const glm::mat3& m) const
	{
		GLint ul = glGetUniformLocation(m_program, loc.c_str());
		glUniformMatrix3fv(ul, 1, GL_FALSE, &m[0][0]);
	}

	inline void BindValue(const std::string& loc, const glm::mat4& m) const 
	{
		GLint ul = glGetUniformLocation(m_program, loc.c_str());
		glUniformMatrix4fv(ul, 1, GL_FALSE, &m[0][0]);
	}

	inline void Draw(const size_t count) const 
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

protected:
private:
	GLuint				m_program;
	std::vector<GLuint> m_shaders;
};

#endif
