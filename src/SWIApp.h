#pragma once

#include "SqcConfig.h"
#include "ThreadManager.h"

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

	std::vector<std::vector<int> > dependencies; //JdR

	SqcConfig best;
	ThreadManager threadManager;
};

