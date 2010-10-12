#pragma once

#include <string>
#include <vector>

#include "Vector2.h"

class SqcConfig {
public:
	void save(std::string file);
	void load(std::string file);

	void render();
	SqcConfig* interpolate(SqcConfig *b, float t);

	void collectParams(std::vector<float>& dst);
	void randomConfig(int n);

	void multiply(Vector2 v);
	void scalefix();
	bool isValid(); // any overlapping?
	Box2 getBox(int i) {
		float hw=(i+1)*0.5f;
		Vector2 p = squares[i];
		return Box2(p.x-hw,p.y-hw,p.x+hw,p.y+hw);
	}

	float radius, centerx, centery;
	void computeBoundingCircle();
	void scalefit();

protected:
	std::vector<Vector2> squares; // square size is determined by order
};

