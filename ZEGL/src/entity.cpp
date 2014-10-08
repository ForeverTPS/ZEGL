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

//#include "entity.h"
#include "entitycomponent.h"
#include "util.h"

Entity::~Entity()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		Util::SafeDelete(m_components[i]);
	}

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		Util::SafeDelete(m_children[i]);
	}
}

Entity* Entity::AddChild(Entity* child)
{
	m_children.push_back(child);
	child->GetTransform()->SetParent(&m_transform);
	child->SetCore(m_core);

	return this;
}

Entity* Entity::AddComponent(EntityComponent* component)
{
	m_components.push_back(component);
	component->SetParent(this);

	return this;
}

void Entity::ProcessInputAll(const Input& input, float delta)
{
	ProcessInput(input, delta);

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->ProcessInputAll(input, delta);
	}
}

void Entity::UpdateAll(float delta)
{
	Update(delta);

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateAll(delta);
	}
}

void Entity::RenderAll(const Shader& shader, const Renderer& renderer, const Camera& camera) const
{
	Render(shader, renderer, camera);

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->RenderAll(shader, renderer, camera);
	}
}

void Entity::ProcessInput(const Input& input, float delta)
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->ProcessInput(input, delta);
	}
}

void Entity::Update(float delta)
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Update(delta);
	}
}

void Entity::Render(const Shader& shader, const Renderer& renderer, const Camera& camera) const
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Render(shader, renderer, camera);
	}
}

void Entity::SetCore(Core* core)
{
	if (m_core != core)
	{
		m_core = core;

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			m_components[i]->AddToEngine(core);
		}

		for (unsigned int i = 0; i < m_children.size(); i++)
		{
			m_children[i]->SetCore(core);
		}
	}
}

std::vector<Entity*> Entity::GetAllAttached()
{
	std::vector<Entity*> result;

	for (unsigned int i = 0; i < m_children.size(); i++)
	{
		std::vector<Entity*> childObjects = m_children[i]->GetAllAttached();
		result.insert(result.end(), childObjects.begin(), childObjects.end());
	}

	result.push_back(this);
	return result;
}