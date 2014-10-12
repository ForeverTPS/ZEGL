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
class EntityComponent;
class Input;
class Renderer;
class Shader;

class Entity
{
public:
	Entity(const Vector3f& pos = Vector3f(0.0f, 0.0f, 0.0f), float rot = 0.0f, float scale = 1.0f) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {}
	virtual ~Entity() {}

	void ProcessInput(const Input& input, float delta) {}
	void Update(float delta) {}
	void Render(const Shader& shader, const Renderer& renderer, const Camera& camera) const {}

	inline Vector3f	GetPos()	const { return m_pos; }
	inline float	GetRot()	const { return m_rot; }
	inline float	GetScale()	const { return m_scale; }

	inline void	SetPos(float x, float y, float z = 0.0f)	{ m_pos.SetX(x); m_pos.SetY(y); }
	inline void	SetPos(Vector3f& pos)						{ m_pos = pos; }
	inline void	SetRot(float rot)							{ m_rot = rot; }
	inline void	SetScale(float scale)						{ m_scale = scale; }

protected:
private:
	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;

	Vector3f	m_pos;
	float		m_rot;
	float		m_scale;
};

#endif