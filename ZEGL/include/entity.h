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

class Camera;
class Core;
class Input;
class Shader;
class Texture;

class Entity
{
public:
	Entity(const Vector3f& pos = Vector3f(0.0f, 0.0f, 0.0f), float rot = 0.0f, float scale = 1.0f) :
		m_texture(nullptr),
		m_normalMap(nullptr)
	{
		data.m_pos = pos;
		data.m_rot = rot;
		data.m_scale = scale;
	}
		
	virtual ~Entity() {}

	void ProcessInput(const Input& input, float delta) {}
	void Update(float delta) {}
	void Render(const Shader& shader, const Camera& camera) const {}

	inline Vector3f	GetPos()	const { return data.m_pos; }
	inline float	GetRot()	const { return data.m_rot; }
	inline float	GetScale()	const { return data.m_scale; }

	inline void	SetPos(float x, float y, float z = 0.0f)	{ data.m_pos.SetX(x); data.m_pos.SetY(y); }
	inline void	SetPos(Vector3f& pos)						{ data.m_pos = pos; }
	inline void	SetRot(float rot)							{ data.m_rot = rot; }
	inline void	SetScale(float scale)						{ data.m_scale = scale; }

	inline void	SetTextureRow(int row)						{ data.m_texRow = row; }
	inline void	SetTextureCol(int col)						{ data.m_texCol = col; }

protected:
	struct EntityData
	{
		Vector3f	m_pos;			// 12 bytes
		float		m_rot;			// 4 bytes	- offset 12
		float		m_scale;		// 4 bytes	- offset 16
		int			m_texRow;		// 4 bytes	- offset 20
		int			m_texCol;		// 4 bytes	- offset 24
	};

	EntityData	data;

	Texture*	m_texture;
	Texture*	m_normalMap;

private:
	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;
};

#endif