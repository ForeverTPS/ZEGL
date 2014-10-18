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
#include <GL/glew.h>

Camera::Camera(const Window* window) :
	m_zoom(1.0f),
	m_pos(Vector2f(0.0f, 0.0f)),
	m_rot(0.0f),
	m_origin(Vector2f(0.0f, 0.0f))
{
	m_transform.Update(m_pos, m_origin, m_zoom, m_rot);

	RecreateTransform(window);
}

Matrix4f Camera::GetTransform(const Window* window)
{
	if (m_transform.m_lastPos != m_pos ||
		m_transform.m_lastRot != m_rot ||
		m_transform.m_lastOrigin != m_origin ||
		m_transform.m_lastZoom != m_zoom)
	{
		RecreateTransform(window);
	}

	return m_transform.m_matrix;
}

void Camera::RecreateTransform(const Window* window)
{
	m_transform.Update(m_pos, m_origin, m_zoom, m_rot);

	Matrix4f reverseTransMat, scaleMat, rotMat, transMat;
	reverseTransMat.InitTranslation(Vector3f(-m_pos.GetX(), -m_pos.GetY(), 0.0f));
	scaleMat.InitScale(Vector3f(m_zoom, m_zoom, 1.0f));
	rotMat.InitRotationEuler(0.0f, 0.0f, m_rot);
	transMat.InitTranslation(Vector3f(m_origin.GetX(), m_origin.GetY(), 0.0f));

	Matrix4f ortho;
	if (window)
	{
		ortho.InitOrthographic(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight(), -1.0f, 1.0f);
	}
	else
	{
		ortho.InitIdentity();
	}

	m_transform.m_matrix = ortho * reverseTransMat * rotMat * scaleMat * transMat;
}