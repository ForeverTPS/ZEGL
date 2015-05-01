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

#include "render_entity.h"
#include "shader.h"

namespace ZEGL
{
	class SpriteBatch
	{
	public:
		SpriteBatch();
		SpriteBatch(SpriteBatch const&) = delete;
		SpriteBatch& operator=(SpriteBatch const&) = delete;
		~SpriteBatch();

		/**
		* Initialize the sprite batch including creating the
		* vertex array and vertex buffer.
		*/
		void Init();

		/**
		* Must be called before drawing any sprites. This clears the previously
		* registered sprites and binds shaders ready for rendering.
		*
		* \param[in] game The game class
		*
		* \see [Game]
		*/
		void Begin();

		/**
		* Call when all sprite drawing is finished. This will push all registered
		* sprites to the GPU to be displayed.
		*/
		void End();
	
		/**
		* Register an entity to be drawn by the sprite batch.
		*
		* If a change in texture is required then the currently registered sprites
		* will be pushed to GPU so that texture swapping can be done in the shader.
		* Because of this, ideally you will want to draw entities sharing textures
		* as much as possible for the best performance.
		*
		* \param[in] renderEntity The entity to draw
		*
		* \see [RenderEntity][Sprite][AnimatedSprite]
		*/
		void Draw(RenderEntity& renderEntity);

		/**
		* Push all registered sprites to the GPU for rendering and then
		* clear the list of registered sprites.
		*/
		void Flush();

		/**
		* Set a custom shader to use instead of the default pass through.
		*
		* \param[in] shader Set a custom shader to use
		*
		* \see [Shader]
		*/
		void SetShader(Shader* shader);

	protected:
	private:
		void SetTexture(Texture* texture, Texture* normalMap = nullptr);

		std::vector<EntityData>	m_spriteData;

		Texture*				m_lastTexture;
		Texture*				m_lastNormalMap;

		Shader					m_defaultShader;
		Shader*					m_customShader;

		GLuint					m_VAO;
		GLuint					m_VAB;
		size_t					m_bytesAllocated;

		bool					m_isDrawing;
	};
}