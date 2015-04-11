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
#include <GL/glew.h>
#include <unordered_map>
#include <string>

namespace ZEGL
{
	/**
	* Base level class for holding image date
	*/
	class Texture
	{
	public:
		/**
		* Create a texture and load image data from the given file name.
		*
		* \param[in] fileName Name of the image file
		* \param[in] textureTarget The type of texture 2D etc
		* \param[in] filter Type of GL filtering to apply
		* \param[in] internalFormat Color format of the image
		* \param[in] format Color format of the image
		* \param[in] clamp Whether or not to apply screen clamping
		* \param[in] attachment Any attachments
		*/
		Texture(const std::string& fileName, 
				GLenum textureTarget = GL_TEXTURE_2D, 
				GLfloat filter = GL_LINEAR_MIPMAP_LINEAR, 
				GLenum internalFormat = GL_RGBA, 
				GLenum format = GL_RGBA, 
				bool clamp = false, 
				GLenum attachment = GL_NONE);

		/**
		* Create a texture of a given width and height with pre-loaded data
		*
		* \param[in] width Width to make the texture
		* \param[in] height Height to make the texture
		* \param[in] data Pointer to the pre-loaded image data
		* \param[in] textureTarget The type of texture 2D etc
		* \param[in] filter Type of GL filtering to apply
		* \param[in] internalFormat Color format of the image
		* \param[in] format Color format of the image
		* \param[in] clamp Whether or not to apply screen clamping
		* \param[in] attachment Any attachments
		*/
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

		/**
		* Binds the texture to OpenGL for use in rendering.
		*
		* \param[in] unit The texture number to use when binding multiple textures to
		* shader. By default uses 0 if only one texture is to be passed to the shader.
		*
		* \see [Shader]
		*/
		void Bind(unsigned int unit = 0) const;

		/**
		* Binds the texture to be used as the render target rather than rendering to the screen
		* buffer.
		*/
		void BindAsRenderTarget() const;

		/**
		* Get the texture width.
		*
		* \return The width of the texture
		*/
		inline int GetWidth() const { return m_textureData->GetWidth(); }

		/**
		* Get the texture height.
		*
		* \return The height of the texture
		*/
		inline int GetHeight() const { return m_textureData->GetHeight(); }

		bool operator==(const Texture& texture) const { return m_textureData == texture.m_textureData; }
		bool operator!=(const Texture& texture) const { return !operator==(texture); }

	protected:
	private:
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

			TextureData(TextureData const&) = delete;
			TextureData& operator=(TextureData const&) = delete;
			~TextureData();

			void Bind(int textureNum) const;
			void BindAsRenderTarget() const;

			inline int GetWidth() const { return m_width; }
			inline int GetHeight() const { return m_height; }

		protected:
		private:
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

		static std::unordered_map<std::string, TextureData*> s_resourceMap;

		TextureData*	m_textureData;
		std::string		m_fileName;
	};
}
