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

#include "transform.h"
#include <vector>

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
		m_transform(pos, rot, scale),
		m_core(nullptr) {}

	virtual ~Entity();

	Entity* AddChild(Entity* child);
	Entity* AddComponent(EntityComponent* component);

	void ProcessInputAll(const Input& input, float delta);
	void UpdateAll(float delta);
	void RenderAll(const Shader& shader, const Renderer& renderer, const Camera& camera) const;

	std::vector<Entity*> GetAllAttached();

	inline Transform* GetTransform() { return &m_transform; }

	void SetCore(Core* engine);

protected:
private:
	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;

	void ProcessInput(const Input& input, float delta);
	void Update(float delta);
	void Render(const Shader& shader, const Renderer& renderer, const Camera& camera) const;

	Core*		m_core;
	Transform	m_transform;

	std::vector<Entity*>			m_children;
	std::vector<EntityComponent*>	m_components;
};

#endif