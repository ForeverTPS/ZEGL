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

#include "referenceCounter.h"
#include <GL/glew.h>
#include <unordered_map>
#include <string>

namespace ZEGL
{
	class TextureData : public ReferenceCounter
	{
	public:
		TextureData(GLenum textureTarget, 
					int width, int height, 
					int numTextures, 
					unsigned char** data,	
					GLfloat* filters, 
					GLenum* internalFormat, GLenum* format, 
					bool clamp, 
					GLenum* attachments);

		virtual ~TextureData();

		void Bind(int textureNum) const;
		void BindAsRenderTarget() const;

		inline int GetWidth()  const { return m_width; }
		inline int GetHeight() const { return m_height; }

	protected:
	private:
		TextureData(TextureData const&) = delete;
		TextureData& operator=(TextureData const&) = delete;

		void InitTextures(unsigned char** data, GLfloat* filter, GLenum* internalFormat, GLenum* format, bool clamp);
		void InitRenderTargets(GLenum* attachments);

		GLuint* m_textureID;
		GLenum	m_textureTarget;
		GLuint	m_frameBuffer;
		GLuint	m_renderBuffer;
		int		m_numTextures;
		int		m_width;
		int		m_height;
	};

	class Texture
	{
	public:
		Texture(const std::string& fileName, 
				GLenum textureTarget = GL_TEXTURE_2D, 
				GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, 
				GLenum internalFormat = GL_RGBA, 
				GLenum format = GL_RGBA, 
				bool clamp = false, 
				GLenum attachment = GL_NONE);

		Texture(int width = 0, int height = 0, 
				unsigned char* data = nullptr, 
				GLenum textureTarget = GL_TEXTURE_2D, 
				GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, 
				GLenum internalFormat = GL_RGBA, 
				GLenum format = GL_RGBA, 
				bool clamp = false, 
				GLenum attachment = GL_NONE);		
		
		Texture(const Texture& texture);
		void operator=(Texture texture);
		virtual ~Texture();

		void Bind(unsigned int unit = 0) const;
		void BindAsRenderTarget() const;

		inline int GetWidth()  const { return m_textureData->GetWidth(); }
		inline int GetHeight() const { return m_textureData->GetHeight(); }

		bool operator==(const Texture& texture) const { return m_textureData == texture.m_textureData; }
		bool operator!=(const Texture& texture) const { return !operator==(texture); }

	protected:
	private:
		static std::unordered_map<std::string, TextureData*> s_resourceMap;

		TextureData*	m_textureData;
		std::string		m_fileName;
	};
}
