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

#include "renderer.h"
#include "window.h"
#include <GL/glew.h>
#include <cassert>


Renderer::Renderer(const Window& window) :
	m_window(&window),
	m_tempTarget(window.GetWidth(), window.GetHeight(), 0, GL_TEXTURE_2D, GL_NEAREST, GL_RGBA, GL_RGBA, false, GL_COLOR_ATTACHMENT0),
	m_defaultShader("basic_shader")
{
	SetTexture("displayTexture", Texture(m_window->GetWidth(), m_window->GetHeight(), 0, GL_TEXTURE_2D, GL_LINEAR, GL_RGBA, GL_RGBA, true, GL_COLOR_ATTACHMENT0));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void Renderer::Render(const Entity& object)
{
	//GetTexture("displayTexture").BindAsRenderTarget();
	m_window->BindAsRenderTarget();

	glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//object.RenderAll(m_defaultShader, *this, *m_mainCamera);
}
