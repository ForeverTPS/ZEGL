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

#include "stdafx.h"

#include "camera.h"
#include "window.h"

Camera::Camera(const Window* window) :
	m_zoom(1.0f),
	m_pos(glm::vec3(0.0f)),
	m_rot(0.0f),
	m_ortho(glm::mat4(1.0f)),
	m_window(window)
{
	m_transform.Update(m_pos, m_rot, m_zoom);
	SetOrthoToWindow();
}

void Camera::RevertToLast()
{
	m_pos = m_transform.m_lastPos;
	m_rot = m_transform.m_lastRot;
	m_zoom = m_transform.m_lastZoom;

	RecreateTransform(true);
}

void Camera::SetOrtho(float left, float right, float bottom, float top, float near_z, float far_z)
{
	m_ortho = glm::ortho(left, right, bottom, top, near_z, far_z);

	RecreateTransform(true);
}

void Camera::SetOrthoToWindow(float near_z, float far_z)
{
	m_ortho = glm::ortho(0.0f, (float)m_window->GetWidth(), 0.0f, (float)m_window->GetHeight());

	RecreateTransform(true);
}

const glm::mat4& Camera::GetTransform(bool skipUpdate)
{
	if (m_transform.m_lastPos != m_pos ||
		m_transform.m_lastRot != m_rot ||
		m_transform.m_lastZoom != m_zoom)
	{
		RecreateTransform(skipUpdate);
	}

	return m_transform.m_matrix;
}

void Camera::RecreateTransform(bool skipUpdate)
{
	if (!skipUpdate)
	{
		m_transform.Update(m_pos, m_rot, m_zoom);
	}

	glm::mat4 reverseTransMat = glm::translate(glm::mat4(1.0f), glm::vec3(-m_pos.x, -m_pos.y, 0.0f));
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoom, m_zoom, 1.0f));
	glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f),	glm::radians(m_rot), glm::vec3(0.0f, 0.0f, 1.0f));

	m_transform.m_matrix = m_ortho * reverseTransMat * rotMat * scaleMat;
}