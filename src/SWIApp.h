#pragma once

class SqcConfig;
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
	SqcConfig* drawingConfig; // config that is currently being rendered
	IOptimizer* swarmOptimizer;
	int best;
};

