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

#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include "camera.h"
#include "mappedvalues.h"
#include "shader.h"
#include "window.h"
#include <map>

class Entity;

class Renderer : public MappedValues
{
public:
	Renderer(const Window& window);
	virtual ~Renderer() {}

	void Render(const Entity& object);

	inline void SetMainCamera(const Camera& camera) { m_mainCamera = &camera; }
	inline unsigned int GetSamplerSlot(const std::string& samplerName) const { return m_samplerMap.find(samplerName)->second; }

protected:
	inline void SetSamplerSlot(const std::string& name, unsigned int value) { m_samplerMap[name] = value; }

private:
	Renderer(const Renderer& other);
	Renderer& operator=(Renderer const&) = delete;

	const Window*                       m_window;
	Texture								m_tempTarget;

	Camera                              m_altCamera;
	const Camera*                       m_mainCamera;

	Shader                              m_defaultShader;

	std::map<std::string, unsigned int> m_samplerMap;
};

#endif
