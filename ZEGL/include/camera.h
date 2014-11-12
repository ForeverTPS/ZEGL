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

#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"

class Window;

class Camera
{
public:
	Camera(const Window* window);
	virtual ~Camera() {};

	void RevertToLast();

	const glm::mat4& GetTransform(bool skipUpdate = false);

	inline void	Translate(const glm::vec3& v) { m_pos += v; }

	inline glm::vec3	GetPos()	const	{ return m_pos; }
	inline float		GetRot()	const	{ return m_rot; }
	inline float		GetZoom()	const	{ return m_zoom; }

	inline void	SetPos(float x, float y, float z = 0.0f)	{ m_pos.x = x; m_pos.y = y; m_pos.z = z; }
	inline void	SetPos(const glm::vec3& pos)				{ m_pos = pos; }
	inline void	SetRot(float rot)							{ m_rot = rot; }
	inline void	SetZoom(float zoom)							{ m_zoom = zoom; }

	void SetOrtho(float left, float right, float bottom, float top, float near_z = -1.0f, float far_z = 1.0f);
	void SetOrthoToWindow(float near_z = -1.0f, float far_z = 1.0f);

protected:
private:
	//Camera(Camera const&) = delete;
	Camera& operator=(Camera const&) = delete;

	struct Transformation
	{
		glm::mat4	m_matrix;
		glm::vec3	m_lastPos;
		float		m_lastRot;
		float		m_lastZoom;

		void Update(const glm::vec3& pos, float rot, float zoom)
		{ 
			m_lastPos = pos; m_lastRot = rot; m_lastZoom = zoom;
		}
	};

	void RecreateTransform(bool skipUpdate = false);

	const Window*	m_window;

	glm::vec3		m_pos;
	float			m_rot;
	float			m_zoom;

	glm::mat4		m_ortho;

	Transformation	m_transform;
};

#endif