#include "StdIncl.h"
#include "Vector2.h"
#include "Matrix22.h"


void Matrix22::transposed44Matrix(float *d) const {
	/* 
	0  1  2  3       0  4  8  12   v0  v1  0  t0
	4  5  6  7       1  5  9  13   v2  v3  0  t1
	8  9  10 11      2  6  10 14    0   0  1  0
	12 13 14 15      3  7  11 15    0   0  0  1
	*/
	for(int i=0;i<16;i++)
		d[i] = 0.0f;

	d[0] = v[0];
	d[1] = v[2];
	d[4] = v[1];
	d[5] = v[3];
	d[12] = t[0];
	d[13] = t[1];
	d[10] = d[15] = 1.0f;
}


Matrix22 Matrix22::multiply(const Matrix22& m) const {
	/*
	0 1 2   v0 v1 t0       mv0 mv1 mt0 
	3 4 5   v2 v3 t1	   mv2 mv3 mt1
	6 7 8   0  0  1          0  0  1

	((v0, v1, t0) * (mv0, mv2, 0)),  ((v0, v1, t0) * (mv1, mv3, 0)),  ((v0, v1, t0) * (mt0, mt1, 1)
	((v2, v3, t1) * (mv0, mv2, 0)),  ((v2, v3, t1) * (mv1, mv3, 0)),  ((v2, v3, t1) * (mt0, mt1, 1)
	0,  0,  1
	*/
	Matrix22 r;
	for(int i=0;i<2;i++) {
		for(int j=0;j<2;j++)
			r.v[i*2+j] = v[i*2+0]*m.v[0+j] + v[i*2+1]*m.v[2+j];
		r.t[i] = v[i*2+0]*m.t[0] + v[i*2+1]*m.t[1] + t[i];
	}
	return r;
}


float Vector2::distanceToRay(Vector2 a, Vector2 b)
{
	Vector2 o = (b - a).ortho();
	o.normalize();

	float d = a.dot(o);
	float dist = dot(o);

	return fabsf(dist - d);
}

float Vector2::distanceToLine(Vector2 a, Vector2 b)
{
	Vector2 d = b - a;
	float sa = d.dot(a);
	float sb = d.dot(b);
	float st = dot(d);

	bool useRay;

	if (sb < sa)
		useRay = st > sb && st < sa;
	else
		useRay = st > sa && st < sb;

	if (useRay)
		return distanceToRay(a, b);

	return std::min((*this - a).length(), (*this - b).length());
}

Vector2 Vector2::closestPosOnLine(Vector2 a, Vector2 b, float* t)
{
	Vector2 diff = b - a;
	float sa = a.dot(diff);
	float sb = b.dot(diff);
	float st = dot(diff);

	bool useRay;

	if (sb < sa)
		useRay = st > sb && st < sa;
	else
		useRay = st > sa && st < sb;

	if (useRay) {
		Vector2 o = (b - a).ortho();
		o.normalize();

		if (t) {
			float F = 1.0f/fabsf(sb-sa);
			*t = (sb<sa) ? (st-sb)*F : (st-sa)*F;
		}

		float d = a.dot(o);
		float dist = dot(o);

		// Project vector on line
		return *this + (d-dist) * o;
	}

	float distToA = (*this - a).length2();
	float distToB = (*this - b).length2();

	if (distToA < distToB) {
		if (t) *t = 0.0f;
		return a;
	}
	if (t) *t = 1.0f;
	return b;
}
