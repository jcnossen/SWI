#pragma once

#include "Vector2.h"



class Matrix22 {
public:
	float v[4];
	float t[2];

	Matrix22() {
		v[0]=v[1]=v[2]=v[3]=t[0]=t[1]=0.0f;
	}
	
	static Matrix22 identity() {
		Matrix22 m;
		m.v[0] = m.v[3] = 1.0f;
		return m;
	}
	
	Vector2 colX() const { return Vector2(v[0], v[2]); }
	Vector2 colY() const { return Vector2(v[1], v[3]); }
	void colX(const Vector2& x) { 
		v[0] = x.x;
		v[2] = x.y;
	}
	void colY(const Vector2& y) {
		v[1] = y.x;
		v[3] = y.y;
	}

	Vector2 apply(const Vector2& a) const {
		return Vector2(
			a.x * v[0] + a.y * v[1] + t[0], 
			a.x * v[2] + a.y * v[3] + t[1]);
	}

	static Matrix22 rotation(float a) {
		Vector2 x(a);
		Matrix22 rot;
		rot.colX(x);
		rot.colY(x.ortho());
		return rot;
	}

	Matrix22 rotated(float a) const {
		return rotation(a).multiply(*this);
	}

	static Matrix22 transform(Vector2 pos, Vector2 scale, float angle) {
		return Matrix22::scale(scale).rotated(angle).translated(pos);
	}

	Matrix22 transformed(Vector2 pos, Vector2 scale, float angle) {
		return transform(pos, scale, angle).multiply(*this);
	}

	static Matrix22 scale(Vector2 a) {
		Matrix22 s;
		s.v[0] = a.x;
		s.v[3] = a.y;
		return s;
	}

	Matrix22 scaled(Vector2 a) const {
		return scale(a).multiply(*this);
	}

	static Matrix22 translate(Vector2 a) {
		Matrix22 r;
		r.t[0] = a.x;
		r.t[1] = a.y;
		return r;
	}

	Matrix22 translated(Vector2 a) const {
		Matrix22 r(*this);
		r.t[0] += a.x;
		r.t[1] += a.y;
		return r;
	}

	void transposed44Matrix(float *d) const;

	Matrix22 multiply(const Matrix22& m) const;

	Matrix22 operator*(const Matrix22& m) const {
		return multiply(m);
	}

	const Matrix22& operator*=(const Matrix22& m) {
		*this = multiply(m);
		return *this;
	}
};
