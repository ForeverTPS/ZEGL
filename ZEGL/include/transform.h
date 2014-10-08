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

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "mymath.h"

class Transform
{
public:
	Transform(const Vector3f& pos = Vector3f(0, 0, 0), float rot = 0.0f, float scale = 1.0f);
	~Transform() {}

	Matrix4f GetTransformation() const;

	bool HasChanged() const;

	inline Vector3f* GetPos()                   { return &m_pos; }
	inline const Vector3f& GetPos()		const	{ return m_pos; }
	inline float GetRot()				const	{ return m_rot; }
	inline float GetScale()				const	{ return m_scale; }
	inline Vector3f GetTransformedPos()	const	{ return Vector3f(GetParentMatrix().Transform(m_pos)); }

	inline void SetPos(const Vector3f& pos)		{ m_pos = pos; }
	inline void SetRot(float rot)				{ m_rot = rot; }
	inline void SetScale(float scale)			{ m_scale = scale; }
	inline void SetParent(Transform* parent)	{ m_parent = parent; }

protected:
private:
	Transform(Transform const&) = delete;
	Transform& operator=(Transform const&) = delete;

	const Matrix4f& GetParentMatrix() const;

	void RecreateTransform()	const;
	void UpdateOldVals()		const;

	Vector3f			m_pos;
	float				m_rot;
	float				m_scale;
	mutable Matrix4f	m_transform;

	Transform*			m_parent;
	mutable	Matrix4f	m_parentMatrix;

	mutable Vector3f	m_oldPos;
	mutable float		m_oldRot;
	mutable float		m_oldScale;
};

#endif
