#ifndef CAMERA_H
#define CAMERA_H

#include "mymath.h"

class Camera
{
public:
	Camera();
	virtual ~Camera() {};

	inline void		SetZoom(float zoom)	{ m_zoom = zoom; }
	inline float	GetZoom() const		{ return m_zoom; }

	inline void		SetOrigin(float x, float y) { m_origin.SetX(x); m_origin.SetY(y); }

	Matrix4f GetTransform();

protected:
private:
	void RecreateTransform();

	struct Transformation
	{
		Matrix4f	m_matrix;
		Vector2f	m_lastPos;
		float		m_lastRot;
		float		m_lastZoom;
		Vector2f	m_lastOrigin;

		void Update(const Vector2f& p, const Vector2f& origin, const float zoom, const float rotation)
		{ 
			m_lastPos = p; m_lastOrigin = origin; m_lastZoom = zoom; m_lastRot = rotation;
		}
	};

	Vector2f		m_pos;
	float			m_rot;
	Vector2f		m_origin;
	float			m_zoom;
	Transformation	m_transform;
};

#endif