/**
* Copyright(c) 2014, Phil Sampson (http://www.zamma.co.uk)
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
#include "game.h"
#include "Window.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

using namespace ZEGL;

Camera::Camera() :
	m_zoom(1.0f),
	m_pos(glm::vec3(0.0f)),
	m_rot(0.0f)
{
	const Window* window = Game::GetInstance()->GetWindow();
	m_origin = window ? glm::vec3(window->GetWidth() / 2.0f, window->GetHeight() / 2.0f, 0.0f) : glm::vec3(0.0f);
	m_transform.Update(m_origin, m_pos, m_rot, m_zoom);

	RecreateTransform();
}

const glm::mat4& Camera::GetTransform()
{
	if (m_transform.m_lastPos != m_pos ||
		m_transform.m_lastRot != m_rot ||
		m_transform.m_lastZoom != m_zoom)
	{
		RecreateTransform();
	}

	return m_transform.m_matrix;
}

void Camera::RecreateTransform()
{
	const Window* window = Game::GetInstance()->GetWindow();

	m_transform.Update(m_origin, m_pos, m_rot, m_zoom);

	glm::mat4 ortho(1.0f);
	if (window)
	{
		ortho = glm::ortho(0.0f, (float)window->GetWidth(), (float)window->GetHeight(), 0.0f, -1.0f, 1.0f);
	}

	m_transform.m_matrix = glm::translate(ortho, glm::vec3(m_origin.x, m_origin.y, 0.0f));
	m_transform.m_matrix = glm::scale(m_transform.m_matrix, glm::vec3(m_zoom, m_zoom, 1.0f));
	m_transform.m_matrix = glm::rotate(m_transform.m_matrix, m_rot, glm::vec3(0.0f, 0.0f, 1.0f));
	m_transform.m_matrix = glm::translate(m_transform.m_matrix, glm::vec3(-m_origin.x, -m_origin.y, 0.0f));
}