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

#include "mappedvalues.h"
#include "shader.h"
#include "glm/glm.hpp"

const float DEFAULT_ENTITY_SIZE = 64.0f;

class Input;

class Entity : public MappedValues
{
public:
	Entity(const glm::vec3& pos) : m_pos(pos) {}
	virtual ~Entity() {}

	virtual void Init() = 0;
	virtual void LoadResources() = 0;

	virtual void Update(float delta) = 0;
	virtual void ProcessInput(const Input& input, float delta) = 0;

	virtual void Draw(const Shader& shader) = 0;
	virtual void DrawOcclusion(const Shader& shader) = 0;

	inline glm::vec3 GetPos() const	{ return m_pos; }

	inline void	SetPos(float x, float y, float z = 0.0f)	{ m_pos.x = x; m_pos.y = y; m_pos.z = z; }
	inline void	SetPos(const glm::vec3& pos)				{ m_pos = pos; }

protected:
	glm::vec3 m_pos;

private:
	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;
};

#endif