#pragma once

class IOptimizer
{
public:
	virtual void initialize(int ndims, int nelems) {}
	virtual const char* name() = 0;
	virtual void tick(float* fitness) = 0; // taking fitness[nelems]
	virtual void getElem(float* params) = 0;
};

class SwarmOptimizer : public IOptimizer
{
public:
	SwarmOptimizer();

	const char* name() { return "PSO"; }
	void initialize(int ndims, int nelems);
	void tick(float* fitness);
	void getElem(float* params);
};


class GAOptimizer : public IOptimizer
{
	const char* name() { return "GA"; }
};
