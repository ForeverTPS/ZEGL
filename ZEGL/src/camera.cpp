#include "camera.h"

Camera::Camera() : 
	m_zoom(1.0f),
	m_pos(Vector2f(0.0f, 0.0f)),
	m_rot(0.0f),
	m_origin(Vector2f(0.0f, 0.0f))
{
	m_transform.Update(m_pos, m_origin, m_zoom, m_rot);

	RecreateTransform();
}

Matrix4f Camera::GetTransform()
{
	if (m_transform.m_lastPos != m_pos ||
		m_transform.m_lastRot != m_rot ||
		m_transform.m_lastOrigin != m_origin ||
		m_transform.m_lastZoom != m_zoom)
	{
		RecreateTransform();
	}

	return m_transform.m_matrix;
}

void Camera::RecreateTransform()
{
	m_transform.Update(m_pos, m_origin, m_zoom, m_rot);

	Matrix4f reverseTransMat, scaleMat, rotMat, transMat;
	reverseTransMat.InitTranslation(Vector3f(-m_pos.GetX(), -m_pos.GetY(), 0.0f));
	scaleMat.InitScale(Vector3f(m_zoom, m_zoom, 1.0f));
	rotMat.InitRotationEuler(0.0f, 0.0f, m_rot);
	transMat.InitTranslation(Vector3f(m_origin.GetX(), m_origin.GetY(), 0.0f));

	m_transform.m_matrix = reverseTransMat * scaleMat * rotMat * transMat;
}