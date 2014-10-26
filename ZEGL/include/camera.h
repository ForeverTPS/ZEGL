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

#include "mymath.h"

class Window;

class Camera
{
public:
	Camera(const Window* window = nullptr);
	virtual ~Camera() {};

	const Matrix4f& GetTransform(const Window* window);

	inline Vector2f	GetOrigin()	const	{ return m_origin; }
	inline Vector2f	GetPos()	const	{ return m_pos; }
	inline float	GetRot()	const	{ return m_rot; }
	inline float	GetZoom()	const	{ return m_zoom; }

	inline void	SetOrigin(float x, float y)			{ m_origin.SetX(x); m_origin.SetY(y); }
	inline void	SetOrigin(const Vector2f& origin)	{ m_origin = origin; }
	inline void	SetPos(float x, float y)			{ m_pos.SetX(x); m_pos.SetY(y); }
	inline void	SetPos(const Vector2f& pos)			{ m_pos = pos; }
	inline void	SetRot(float rot)					{ m_rot = rot; }
	inline void	SetZoom(float zoom)					{ m_zoom = zoom; }

protected:
private:
	//Camera(Camera const&) = delete;
	Camera& operator=(Camera const&) = delete;

	struct Transformation
	{
		Matrix4f	m_matrix;
		Vector2f	m_lastPos;
		float		m_lastRot;
		float		m_lastZoom;
		Vector2f	m_lastOrigin;

		void Update(const Vector2f& p, const Vector2f& origin, float zoom, float rotation)
		{ 
			m_lastPos = p; m_lastOrigin = origin; m_lastZoom = zoom; m_lastRot = rotation;
		}
	};

	void RecreateTransform(const Window* window);

	Vector2f		m_pos;
	float			m_rot;
	Vector2f		m_origin;
	float			m_zoom;
	Transformation	m_transform;
};

#endif