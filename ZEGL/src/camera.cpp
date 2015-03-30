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

#include "camera.h"
#include "window.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GL/glew.h>

using namespace ZEGL;

Camera::Camera(const Window* window) :
	m_zoom(1.0f),
	m_pos(glm::vec3(0.0f)),
	m_rot(0.0f)
{
	m_transform.Update(m_pos, m_rot, m_zoom);

	RecreateTransform(window);
}

const glm::mat4& Camera::GetTransform(const Window* window)
{
	if (m_transform.m_lastPos != m_pos ||
		m_transform.m_lastRot != m_rot ||
		m_transform.m_lastZoom != m_zoom)
	{
		RecreateTransform(window);
	}

	return m_transform.m_matrix;
}

void Camera::RecreateTransform(const Window* window)
{
	m_transform.Update(m_pos, m_rot, m_zoom);

	glm::mat4 reverseTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_pos.x, -m_pos.y, 0.0f));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoom, m_zoom, 1.0f));
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f),	m_rot, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 ortho(1.0f);
	if (window)
	{
		ortho = glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -1.0f, 1.0f);
	}

	m_transform.m_matrix = ortho * reverseTransMat * rotMat * scaleMat;
}