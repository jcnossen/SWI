#pragma once

class SqcConfig;
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
	void swarmTick();

private:
	IOptimizer* optimizer;

	void initRandomOptimizer(int nsquares);
	void optimizerTick();


	SqcConfig best;
	ThreadManager threadManager;
};

