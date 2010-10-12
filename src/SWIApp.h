#pragma once

#include "SqcConfig.h"

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
};

