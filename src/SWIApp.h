#pragma once

class SqcConfig;
#include "SqcConfig.h"
#include "ThreadManager.h"
#include "Graph.h"

class IOptimizer;

class SWIApp
{
public:
	SWIApp();
	~SWIApp();

	void tick();
	void draw();

private:
	IOptimizer* optimizer;

	void initRandomOptimizer(int nsquares);
	void optimizerTick();

	std::list<SqcConfig> best_list;
	SqcConfig best; // overall best
	ThreadManager threadManager;
	Graph graph;
};

