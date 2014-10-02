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

#include "transform.h"

Transform::Transform(const Vector3f& pos, float rot, float scale) :
  m_pos(pos),
  m_rot(rot),
  m_scale(scale),
  m_parent(nullptr),
  m_parentMatrix(Matrix4f().InitIdentity()) 
{
	RecreateTransform();
}

bool Transform::HasChanged() const
{
	if (m_parent != 0 && m_parent->HasChanged())
	{
		return true;
	}

	if (m_pos != m_oldPos)
	{
		return true;
	}

	if (m_rot != m_oldRot)
	{
		return true;
	}

	if (m_scale != m_scale)
	{
		return true;
	}

	return false;
}

void Transform::UpdateOldVals() const
{
	m_oldPos = m_pos;
	m_oldRot = m_rot;
	m_oldScale = m_scale;
}

Matrix4f Transform::GetTransformation() const
{
	if (HasChanged())
	{
		RecreateTransform();
	}

	return m_transform;
}

const Matrix4f& Transform::GetParentMatrix() const
{
	if (m_parent != nullptr && m_parent->HasChanged())
	{
		m_parentMatrix = m_parent->GetTransformation();
	}

	return m_parentMatrix;
}

void Transform::RecreateTransform() const
{
	UpdateOldVals();

	Matrix4f translationMatrix;
	Matrix4f rotMat;
	Matrix4f scaleMatrix;

	translationMatrix.InitTranslation(Vector3f(m_pos.GetX(), m_pos.GetY(), 0.0f));
	rotMat.InitRotationEuler(0.0f, 0.0f, m_rot);
	scaleMatrix.InitScale(Vector3f(m_scale, m_scale, 1.0f));

	Matrix4f result = translationMatrix * rotMat * scaleMatrix;

	m_transform = GetParentMatrix() * result;
}