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
	void optimizerTick();
	SqcConfig* createConfig();
	
	IOptimizer* optimizer;
	std::list<SqcConfig*> best_list;
	SqcConfig *best; // overall best
	ThreadManager threadManager;
	Graph graph;
	float sigma;
};

