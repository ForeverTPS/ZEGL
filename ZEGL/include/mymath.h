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

#ifndef MYMATH_H
#define MYMATH_H

#include <math.h>

#define MATH_PI 3.1415926535897932384626433832795
#define ToRadians(x) (float)(((x) * MATH_PI / 180.0f))
#define ToDegrees(x) (float)(((x) * 180.0f / MATH_PI))

template<typename T>
inline T Clamp(const T &a, const T &min, const T &max)
{
	if (a < min)
	{
		return min;
	}
	else if (a > max)
	{
		return max;
	}
	else
	{
		return a;
	}
}

template<typename T, unsigned int D>
class Vector
{
public:
	Vector() { }

	inline T Dot(const Vector<T, D>& r) const
	{
		T result = T(0);
		for (unsigned int i = 0; i < D; i++)
		{
			result += (*this)[i] * r[i];
		}
			
		return result;
	}
	
	inline T Max() const
	{
		T maxVal = (*this)[0];
		
		for (int i = 0; i < D; i++)
		{
			if ((*this)[i] > maxVal)
			{
				maxVal = (*this)[i];
			}
		}
		
		return maxVal;
	}
	
	inline T LengthSq() const { return this->Dot(*this); }
	inline T Length() const { return sqrt(LengthSq()); }
	inline Vector<T,D> Normalized() const { return *this/Length(); }
	inline Vector<T,D> Lerp(const Vector<T,D>& r, T lerpFactor) const { return (r - *this) * lerpFactor + *this; }

	inline Vector<T, D> operator+(const Vector<T,D>& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
		{
			result[i] = values[i] + r[i];
		}
		
		return result;
	}
	
	inline Vector<T, D> operator-(const Vector<T,D>& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
		{
			result[i] = values[i] - r[i];
		}
		
		return result;
	}
	
	inline Vector<T, D> operator*(const T& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
		{
			result[i] = values[i] * r;
		}
		
		return result;
	}
	
	inline Vector<T, D> operator/(const T& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
		{
			result[i] = values[i] / r;
		}
		
		return result;
	}
	
	inline Vector<T, D>& operator+=(const Vector<T,D>& r) 
	{
		for (unsigned int i = 0; i < D; i++)
		{
			(*this)[i] = values[i] + r[i];
		}
		
		return *this;
	}
	
	inline Vector<T, D>& operator-=(const Vector<T,D>& r) 
	{
		for (unsigned int i = 0; i < D; i++)
		{
			(*this)[i] = values[i] - r[i];
		}

		return *this;
	}
	
	inline Vector<T, D>& operator*=(const T& r) 
	{
		for (unsigned int i = 0; i < D; i++)
		{
			(*this)[i] = values[i] * r;
		}
		
		return *this;
	}
	
	inline Vector<T, D>& operator/=(const T& r) 
	{
		for (unsigned int i = 0; i < D; i++)
		{
			(*this)[i] = values[i] / r;
		}
		
		return *this;
	}
	
	T& operator [] (unsigned int i) { return values[i]; }
	T operator [] (unsigned int i) const { return values[i]; }
	
	inline bool operator==(const Vector<T,D>& r) const
	{
		for (unsigned int i = 0; i < D; i++)
		{
			if ((*this)[i] != r[i])
			{
				return false;
			}
		}

		return true;
	}
	
	inline bool operator!=(const Vector<T,D>& r) const { return !operator==(r); }

protected:
private:
	T values[D];
};

template<typename T>
class Vector2 : public Vector<T, 2>
{
public:
	Vector2() { }
	
	Vector2(const Vector<T, 2>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
	}
	
	Vector2(T x, T y)
	{
		(*this)[0] = x;
		(*this)[1] = y;
	}
	
	T Cross(const Vector2<T>& r) const
	{
		return GetX() * r.GetY() - GetY() * r.GetX();
	}
	
	inline T GetX() const { return (*this)[0]; }
	inline T GetY() const { return (*this)[1]; }
	
	inline void SetX(const T& x) { (*this)[0] = x; }
	inline void SetY(const T& y) { (*this)[1] = y; }
	
	inline void Set(const T& x, const T& y) { SetX(x); SetY(y); }

protected:
private:
};

template<typename T>
class Vector3 : public Vector<T, 3>
{
public:
	Vector3() { }
	
	Vector3(const Vector<T, 3>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
	}
	
	Vector3(T x, T y, T z)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
	}
	
	inline Vector3<T> Cross(const Vector3<T>& r) const
	{
		T x = (*this)[1] * r[2] - (*this)[2] * r[1];
		T y = (*this)[2] * r[0] - (*this)[0] * r[2];
		T z = (*this)[0] * r[1] - (*this)[1] * r[0];

		return Vector3<T>(x, y, z);
	}
	
	inline Vector3<T> Rotate(T angle, const Vector3<T>& axis) const
	{
		const T sinAngle = sin(-angle);
		const T cosAngle = cos(-angle);
		
		return this->Cross(axis * sinAngle) +        //Rotation on local X
			(*this * cosAngle) +                     //Rotation on local Z
		    axis * this->Dot(axis * (1 - cosAngle)); //Rotation on local Y
	}
	
	inline Vector2<T> GetXY() const { return Vector2<T>(GetX(), GetY()); }
	inline Vector2<T> GetYZ() const { return Vector2<T>(GetY(), GetZ()); }
	inline Vector2<T> GetZX() const { return Vector2<T>(GetZ(), GetX()); }
	
	inline Vector2<T> GetYX() const { return Vector2<T>(GetY(), GetX()); }
	inline Vector2<T> GetZY() const { return Vector2<T>(GetZ(), GetY()); }
	inline Vector2<T> GetXZ() const { return Vector2<T>(GetX(), GetZ()); }
	
	inline T GetX() const { return (*this)[0]; }
	inline T GetY() const { return (*this)[1]; }
	inline T GetZ() const { return (*this)[2]; }
	
	inline void SetX(const T& x) { (*this)[0] = x; }
	inline void SetY(const T& y) { (*this)[1] = y; }
	inline void SetZ(const T& z) { (*this)[2] = z; }
	
	inline void Set(const T& x, const T& y, const T& z) { SetX(x); SetY(y); SetZ(z); }

protected:
private:
};

class Vector3f : public Vector3<float>
{
public:
	Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
	}

	Vector3f(const Vector3<float>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
	}

	inline float Length() const { return sqrtf(GetX() * GetX() + GetY() * GetY() + GetZ() * GetZ()); }
	inline float Dot(const Vector3f& v) const { return GetX() * v.GetX() + GetY() * v.GetY() + GetZ() * v.GetZ(); }

	inline Vector3f Cross(const Vector3f& v) const
	{
		const float _x = GetY() * v.GetZ() - GetZ() * v.GetY();
		const float _y = GetZ() * v.GetX() - GetX() * v.GetZ();
		const float _z = GetX() * v.GetY() - GetY() * v.GetX();

		return Vector3f(_x, _y, _z);
	}

	inline Vector3f Rotate(float angle, const Vector3f& axis) const
	{
		const float sin = sinf(-angle);
		const float cos = cosf(-angle);

		return this->Cross(axis * sin) +        //Rotation on local X
			(*this * cos) +                     //Rotation on local Z
			axis * this->Dot(axis * (1 - cos)); //Rotation on local Y
	}

	inline Vector3f Normalized() const
	{
		const float length = Length();

		return Vector3f(GetX() / length, GetY() / length, GetZ() / length);
	}

	inline Vector3f operator+(const Vector3f& r) const { return Vector3f(GetX() + r.GetX(), GetY() + r.GetY(), GetZ() + r.GetZ()); }
	inline Vector3f operator-(const Vector3f& r) const { return Vector3f(GetX() - r.GetX(), GetY() - r.GetY(), GetZ() - r.GetZ()); }
	inline Vector3f operator*(float f) const { return Vector3f(GetX() * f, GetY() * f, GetZ() * f); }
	inline Vector3f operator/(float f) const { return Vector3f(GetX() / f, GetY() / f, GetZ() / f); }

	inline bool operator==(const Vector3f& r) const { return GetX() == r.GetX() && GetY() == r.GetY() && GetZ() == r.GetZ(); }
	inline bool operator!=(const Vector3f& r) const { return !operator==(r); }

	inline Vector3f& operator+=(const Vector3f& r)
	{
		(*this)[0] += r.GetX();
		(*this)[1] += r.GetY();
		(*this)[2] += r.GetZ();

		return *this;
	}

	inline Vector3f& operator-=(const Vector3f& r)
	{
		(*this)[0] -= r.GetX();
		(*this)[1] -= r.GetY();
		(*this)[2] -= r.GetZ();

		return *this;
	}

	inline Vector3f& operator*=(float f)
	{
		(*this)[0] *= f;
		(*this)[1] *= f;
		(*this)[2] *= f;

		return *this;
	}

	inline Vector3f& operator/=(float f)
	{
		(*this)[0] /= f;
		(*this)[1] /= f;
		(*this)[2] /= f;

		return *this;
	}

	inline float GetX() const { return (*this)[0]; }
	inline float GetY() const { return (*this)[1]; }
	inline float GetZ() const { return (*this)[2]; }

	inline void SetX(float x) { (*this)[0] = x; }
	inline void SetY(float y) { (*this)[1] = y; }
	inline void SetZ(float z) { (*this)[2] = z; }

	inline void Set(float x, float y, float z) { (*this)[0] = x; (*this)[1] = y; (*this)[2] = z; }

protected:
private:
};

template<typename T>
class Vector4 : public Vector<T, 4>
{
public:
	Vector4() { }
	
	Vector4(const Vector<T, 4>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
		(*this)[3] = r[3];
	}
	
	Vector4(T x, T y, T z, T w)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = w;
	}
	
	inline T GetX() const { return (*this)[0]; }
	inline T GetY() const { return (*this)[1]; }
	inline T GetZ() const { return (*this)[2]; }
	inline T GetW() const { return (*this)[3]; }
	
	inline void SetX(const T& x) { (*this)[0] = x; }
	inline void SetY(const T& y) { (*this)[1] = y; }
	inline void SetZ(const T& z) { (*this)[2] = z; }
	inline void SetW(const T& w) { (*this)[3] = w; }
	
	inline void Set(const T& x, const T& y, const T& z, const T& w) { SetX(x); SetY(y); SetZ(z); SetW(w); }

protected:
private:
};

template<typename T, unsigned int D>
class Matrix
{
public:
	inline Matrix<T, D> InitIdentity()
	{
		for (unsigned int i = 0; i < D; i++)
		{
			for (unsigned int j = 0; j < D; j++)
			{
				m[i][j] = (i == j) ? T(1) : T(0);
			}
		}
		
		return *this;
	}
	
	inline Matrix<T, D> InitScale(const Vector<T,D - 1>& r)
	{
		for(unsigned int i = 0; i < D; i++)
		{
			for(unsigned int j = 0; j < D; j++)
			{
				m[i][j] = (i == j && i != D - 1) ? r[i] : T(0);
			}
		}
		
		m[D - 1][D - 1] = T(1);
		
		return *this;
	}
	
	inline Matrix<T, D> InitTranslation(const Vector<T,D - 1>& r)
	{
		for(unsigned int i = 0; i < D; i++)
		{
			for(unsigned int j = 0; j < D; j++)
			{
				if (i == D - 1 && j != D - 1)
				{
					m[i][j] = r[j];
				}
				else if (i == j)
				{
					m[i][j] = T(1);
				}
				else
				{
					m[i][j] = T(0);
				}
			}
		}
		
		m[D - 1][D - 1] = T(1);
		
		return *this;
	}

	inline Matrix<T, D> Transpose() const
	{
		Matrix<T, D> t;
		for (int j = 0; j < D; j++) 
		{
			for (int i = 0; i < D; i++) 
			{
				t[i][j] = m[j][i];
			}
		}

		return t;
	}

	inline Matrix<T,D> operator*(const Matrix<T,D>& r) const
	{
		Matrix<T,D> ret;
		for (unsigned int i = 0 ; i < D ; i++) 
		{
			for (unsigned int j = 0 ; j < D; j++) 
			{
				ret.m[i][j] = T(0);
				for (unsigned int k = 0; k < D; k++)
				{
					ret.m[i][j] += m[k][j] * r.m[i][k];
				}
			}
		}

		return ret;
	}
	
	inline Vector<T,D> Transform(const Vector<T,D>& r) const
	{
		Vector<T,D> ret;
		
		for (unsigned int i = 0; i < D; i++)
		{
			ret[i] = 0;
			for (unsigned int j = 0; j < D; j++)
			{
				ret[i] += m[j][i] * r[j];
			}
		}
		
		return ret;
	}
	
	inline Vector<T,D-1> Transform(const Vector<T,D-1>& r) const
	{
		Vector<T,D> r2;
		
		for (int i = 0; i < D - 1; i++)
		{
			r2[i] = r[i];
		}
			
		r2[D-1] = T(1);
		
		Vector<T,D> ret2 = Transform(r2);
		Vector<T,D-1> ret;
		
		for (int i = 0; i < D - 1; i++)
		{
			ret[i] = ret2[i];
		}
			
		return ret;
	}
	
	inline void Set(unsigned int x, unsigned int y, T val) { m[x][y] = val; }
	
	inline const T* operator[](int index) const { return m[index]; }
	inline T* operator[](int index) { return m[index]; }

protected:
private:
	T m[D][D];
};

template<typename T>
class Matrix3 : public Matrix<T, 3>
{
public:
	Matrix3() { }

	template<unsigned int D>
	Matrix3(const Matrix<T, D>& r)
	{
		if (D < 3)
		{
			this->InitIdentity();

			for (unsigned int i = 0; i < D; i++)
			for (unsigned int j = 0; j < D; j++)
				(*this)[i][j] = r[i][j];
		}
		else
		{
			for (unsigned int i = 0; i < 3; i++)
			for (unsigned int j = 0; j < 3; j++)
				(*this)[i][j] = r[i][j];
		}
	}
};

template<typename T>
class Matrix4 : public Matrix<T, 4>
{
public:
	Matrix4() { }

	template<unsigned int D>
	Matrix4(const Matrix<T, D>& r)
	{
		if (D < 4)
		{
			this->InitIdentity();
		
			for (unsigned int i = 0; i < D; i++)
			{
				for (unsigned int j = 0; j < D; j++)
				{
					(*this)[i][j] = r[i][j];
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				for (unsigned int j = 0; j < 4; j++)
				{
					(*this)[i][j] = r[i][j];
				}
			}
		}
	}

	inline Matrix4<T> InitPerspective(T fov, T aspectRatio, T zNear, T zFar)
	{
		const T zRange = zNear - zFar;
		const T tanHalfFOV = tanf(fov / T(2));

		(*this)[0][0] = T(1) / (tanHalfFOV * aspectRatio); (*this)[1][0] = T(0);   (*this)[2][0] = T(0);            (*this)[3][0] = T(0);
		(*this)[0][1] = T(0);                   (*this)[1][1] = T(1) / tanHalfFOV; (*this)[2][1] = T(0);            (*this)[3][1] = T(0);
		(*this)[0][2] = T(0);                   (*this)[1][2] = T(0);            (*this)[2][2] = (-zNear - zFar) / zRange; (*this)[3][2] = T(2)*zFar*zNear / zRange;
		(*this)[0][3] = T(0);                   (*this)[1][3] = T(0);            (*this)[2][3] = T(1);            (*this)[3][3] = T(0);

		return *this;
	}

	inline Matrix4<T> InitRotationFromDirection(const Vector3<T>& forward, const Vector3<T>& up)
	{
		Vector3<T> n = forward.Normalized();
		Vector3<T> u = Vector3<T>(up.Normalized()).Cross(n);
		Vector3<T> v = n.Cross(u);

		return InitRotationFromVectors(n, v, u);
	}

	inline Matrix4<T> InitRotationEuler(T rotateX, T rotateY, T rotateZ)
	{
		Matrix4<T> rx, ry, rz;

		const T x = rotateX;
		const T y = rotateY;
		const T z = rotateZ;

		rx[0][0] = T(1);	rx[1][0] = T(0);	rx[2][0] = T(0);	rx[3][0] = T(0);
		rx[0][1] = T(0);	rx[1][1] = cos(x);  rx[2][1] = -sin(x); rx[3][1] = T(0);
		rx[0][2] = T(0);	rx[1][2] = sin(x);  rx[2][2] = cos(x);	rx[3][2] = T(0);
		rx[0][3] = T(0);	rx[1][3] = T(0);	rx[2][3] = T(0);	rx[3][3] = T(1);

		ry[0][0] = cos(y);	ry[1][0] = T(0);    ry[2][0] = -sin(y); ry[3][0] = T(0);
		ry[0][1] = T(0);	ry[1][1] = T(1);    ry[2][1] = T(0);	ry[3][1] = T(0);
		ry[0][2] = sin(y);	ry[1][2] = T(0);    ry[2][2] = cos(y);	ry[3][2] = T(0);
		ry[0][3] = T(0);	ry[1][3] = T(0);    ry[2][3] = T(0);	ry[3][3] = T(1);

		rz[0][0] = cos(z);	rz[1][0] = -sin(z); rz[2][0] = T(0);    rz[3][0] = T(0);
		rz[0][1] = sin(z);	rz[1][1] = cos(z);	rz[2][1] = T(0);    rz[3][1] = T(0);
		rz[0][2] = T(0);	rz[1][2] = T(0);	rz[2][2] = T(1);    rz[3][2] = T(0);
		rz[0][3] = T(0);	rz[1][3] = T(0);	rz[2][3] = T(0);    rz[3][3] = T(1);

		*this = rz * ry * rx;

		return *this;
	}

	inline Matrix4<T> InitOrthographic(T left, T right, T bottom, T top, T near, T far)
	{
		const T width = (right - left);
		const T height = (top - bottom);
		const T depth = (far - near);

		(*this)[0][0] = T(2) / width;	(*this)[1][0] = T(0);			(*this)[2][0] = T(0);			(*this)[3][0] = -(right + left) / width;
		(*this)[0][1] = T(0);			(*this)[1][1] = T(2) / height;	(*this)[2][1] = T(0);			(*this)[3][1] = -(top + bottom) / height;
		(*this)[0][2] = T(0);			(*this)[1][2] = T(0);			(*this)[2][2] = T(-2) / depth;	(*this)[3][2] = -(far + near) / depth;
		(*this)[0][3] = T(0);			(*this)[1][3] = T(0);			(*this)[2][3] = T(0);			(*this)[3][3] = T(1);

		return *this;
	}

	inline Matrix4<T> InitRotationFromVectors(const Vector3<T>& n, const Vector3<T>& v, const Vector3<T>& u)
	{
		(*this)[0][0] = u.GetX();   (*this)[1][0] = u.GetY();   (*this)[2][0] = u.GetZ();   (*this)[3][0] = T(0);
		(*this)[0][1] = v.GetX();   (*this)[1][1] = v.GetY();   (*this)[2][1] = v.GetZ();   (*this)[3][1] = T(0);
		(*this)[0][2] = n.GetX();   (*this)[1][2] = n.GetY();   (*this)[2][2] = n.GetZ();   (*this)[3][2] = T(0);
		(*this)[0][3] = T(0);       (*this)[1][3] = T(0);       (*this)[2][3] = T(0);       (*this)[3][3] = T(1);

		return *this;
	}

protected:
private:
};

typedef Vector2<int> Vector2i;
typedef Vector3<int> Vector3i;
typedef Vector4<int> Vector4i;

typedef Vector2<float> Vector2f;
//typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;

typedef Matrix<int, 2> Matrix2i;
typedef Matrix3<int> Matrix3i;
typedef Matrix4<int> Matrix4i;

typedef Matrix<float, 2> Matrix2f;
typedef Matrix3<float> Matrix3f;
typedef Matrix4<float> Matrix4f;

#endif
