#pragma once

#include "RenderSystem.h"


class Graph
{
public:
	Graph(int nTicks);

	void addTick(const float* d, int n);
	void addTick(const std::vector<float>& d);
	void render();

protected:
	typedef std::list<std::vector<float> > GraphData;
	GraphData data;
	int numTicks;
};

