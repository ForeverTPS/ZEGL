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

#ifndef SPRITE_H
#define SPRITE_H

#include "entity.h"
#include "shader.h"

class Sprite : public Entity
{
public:
	Sprite(const std::string& texture, const std::string& normalMap, const glm::vec3& pos = glm::vec3(0.0f), bool occluder = false);
	virtual ~Sprite();

	virtual void Init() {}
	virtual void LoadResources();

	virtual void Update(float delta) {}
	virtual void ProcessInput(const Input& input, float delta) {}

	virtual void Draw(const Shader& shader);
	virtual void DrawOcclusion(const Shader& shader);

	inline bool	IsOccluder() const					{ return m_occluder; }
	inline void SetOcculder(bool occludes = true)	{ m_occluder = occludes; }

protected:
	static const glm::vec3 s_vertices[6];

	GLuint		m_VAB;
	bool		m_occluder;

private:
	Sprite(Sprite const&) = delete;
	Sprite& operator=(Sprite const&) = delete;
};

#endif