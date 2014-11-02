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

class Sprite : public Entity
{
public:
	Sprite(const std::string& texture, const std::string& normalMap, const std::string& textureAtlas,
		const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = DEFAULT_ENTITY_SIZE, bool occluder = false);

	Sprite(const std::string& texture, const std::string& normalMap, const glm::vec2 textureCoords[4],
		const glm::vec3& pos = glm::vec3(0.0f), float rot = 0.0f, float scale = DEFAULT_ENTITY_SIZE, bool occluder = false);

	virtual ~Sprite() {}

	virtual void Init() {}
	virtual void LoadResources();

	virtual void Update(float delta) {}
	virtual void ProcessInput(const Input& input, float delta) {}

protected:

private:
	Sprite(Sprite const&) = delete;
	Sprite& operator=(Sprite const&) = delete;
};

#endif