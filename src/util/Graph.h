#pragma once

#include "RenderSystem.h"


class Graph
{
public:
	Graph(int nTicks, float min, float max);

	void addTick(const float* d, int n);
	void addTick(const std::vector<float>& d);
	void render(Box2 box);
	std::pair<float,float> getMinMax();

protected:
	typedef std::list<std::vector<float> > GraphData;
	GraphData data;
	int numTicks;
	float valuemin, valuemax;
};

