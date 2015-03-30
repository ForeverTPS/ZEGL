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

#include "texture.h"
#include "textureatlas.h"
#include "util.h"
#include "glm.hpp"

namespace ZEGL
{
	class Camera;
	class Core;
	class Input;
	class Shader;

	struct EntityData
	{
		glm::vec3	m_pos;			// 12 bytes
		float		m_rot;			// 4 bytes	- offset 12
		float		m_scale;		// 4 bytes	- offset 16
		glm::vec2	m_texCoords[4];	// 32 bytes - offset 20|28|36|44
	};

	class Entity
	{
	public:
		Entity(const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = 1.0f);
		Entity(const Entity& entity);
		void operator=(Entity entity);
		virtual ~Entity() {}

		void ProcessInput(const Input& input, float delta) {}
		void Update(float delta) {}
		void Render() const {}

		inline glm::vec3	GetPos()	const	{ return m_data.m_pos; }
		inline float		GetRot()	const	{ return m_data.m_rot; }
		inline float		GetScale()	const	{ return m_data.m_scale; }

		inline void	SetPos(float x, float y, float z = 0.0f)	{ m_data.m_pos.x = x; m_data.m_pos.y = y; m_data.m_pos.z = z; }
		inline void	SetPos(const glm::vec3& pos)				{ m_data.m_pos = pos; }
		inline void	SetRot(float rot)							{ m_data.m_rot = rot; }
		inline void	SetScale(float scale)						{ m_data.m_scale = scale; }

		inline const EntityData& GetData() const { return m_data; }

	protected:
		EntityData	m_data;

	private:
		//Entity(Entity const&) = delete;
		//Entity& operator=(Entity const&) = delete;
	};

	class RenderEntity : public Entity
	{
	public:
		RenderEntity(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
			const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = 1.0f);

		RenderEntity(const Texture& texture, const Texture& normalMap, const glm::vec2 textureCoords[4],
			const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = 1.0f);

		RenderEntity(const RenderEntity& renderEntity);
		void operator=(RenderEntity renderEntity);

		virtual ~RenderEntity() {}

		bool CalcTextureCoords(const std::string regionName);

		inline const Texture&	GetTexture()	const	{ return m_texture; }
		inline const Texture&	GetNormalMap()	const	{ return m_normalMap; }

	protected:
		TextureAtlas	m_textureAtlas;
		Texture			m_texture;
		Texture			m_normalMap;

		bool			m_hasTextureAtlas;

	private:
		//RenderEntity(RenderEntity const&) = delete;
		//RenderEntity& operator=(RenderEntity const&) = delete;
	};
}