#pragma once

#include "Matrix22.h"

class XForm {
public:
	XForm() {
		angle=0.0f;
	}
	XForm (Vector2 pos, float ang) : position(pos), angle(ang)
	{}

	Vector2 position;
	float angle;

	Matrix22 toMatrix() {
		Matrix22 m = Matrix22::rotation(angle);
		m.t[0] += position.x;
		m.t[1] += position.y;
		return m;
	}

	Vector2 xAxis() {
		return Vector2(angle);
	}

	Vector2 yAxis() {
		return Vector2(angle).ortho();
	}

	Vector2 fromLocal(Vector2 v) {
		return toMatrix().apply(v);
	}
		 
};
