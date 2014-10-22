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

#ifndef ENTITY_H
#define ENTITY_H

#include "mymath.h"
#include "texture.h"
#include "textureatlas.h"
#include "util.h"

class Camera;
class Core;
class Input;
class Shader;

struct EntityData
{
	Vector3f	m_pos;			// 12 bytes
	float		m_rot;			// 4 bytes	- offset 12
	float		m_scale;		// 4 bytes	- offset 16
	Vector2f	m_texCoords[4];	// 32 bytes - offset 20|28|36|42
};

class Entity
{
public:
	Entity(const Vector3f& pos = Vector3f(0.0f, 0.0f, 0.0f), float rot = 0.0f, float scale = 1.0f)
	{
		m_data.m_pos		= pos;
		m_data.m_rot		= rot;
		m_data.m_scale		= scale;
	}
		
	virtual ~Entity() {}

	void ProcessInput(const Input& input, float delta) {}
	void Update(float delta) {}
	void Render() const {}

	inline Vector3f*		GetPos()				{ return &m_data.m_pos; }
	inline const Vector3f&	GetPos()		const	{ return m_data.m_pos; }
	inline float			GetRot()		const	{ return m_data.m_rot; }
	inline float			GetScale()		const	{ return m_data.m_scale; }

	inline void	SetPos(float x, float y, float z = 0.0f)	{ m_data.m_pos.SetX(x); m_data.m_pos.SetY(y); }
	inline void	SetPos(Vector3f& pos)						{ m_data.m_pos = pos; }
	inline void	SetRot(float rot)							{ m_data.m_rot = rot; }
	inline void	SetScale(float scale)						{ m_data.m_scale = scale; }

protected:
	EntityData	m_data;

private:
	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;
};

class RenderEntity : public Entity
{
public:
	RenderEntity(const Texture& texture, const Texture& normalMap, const TextureAtlas& textureAtlas,
		const Vector3f& pos = Vector3f(0.0f, 0.0f, 0.0f), float rot = 0.0f, float scale = 1.0f) :
		m_textureAtlas(textureAtlas),
		m_texture(texture),
		m_normalMap(normalMap),
		m_hasTextureAtlas(true),
		Entity(pos, rot, scale) 
	{
	}

	RenderEntity(const Texture& texture, const Texture& normalMap, const Vector2f textureCoords[4],
		const Vector3f& pos = Vector3f(0.0f, 0.0f, 0.0f), float rot = 0.0f, float scale = 1.0f) :
		m_texture(texture),
		m_normalMap(normalMap),
		m_hasTextureAtlas(false),
		Entity(pos, rot, scale) 
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			m_data.m_texCoords[i] = textureCoords[i];
		}
	}

	virtual ~RenderEntity() {}

	inline const Texture&	GetTexture()	const	{ return m_texture; }
	inline const Texture&	GetNormalMap()	const	{ return m_normalMap; }
	
protected:
	TextureAtlas	m_textureAtlas;
	Texture			m_texture;
	Texture			m_normalMap;

	bool			m_hasTextureAtlas;

private:
	RenderEntity(RenderEntity const&) = delete;
	RenderEntity& operator=(RenderEntity const&) = delete;
};

#endif