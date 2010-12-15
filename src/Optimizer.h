#pragma once

class IOptimizer
{
public:
	virtual void initialize(int ndims, int nelems) {}
	virtual const char* name() = 0;
	virtual void tick(float sigma) = 0;
	virtual void getElem(int elem, float* params) = 0;
	virtual void setElem(int elem, float* params) = 0;
	virtual void setFitness(int elem, float fitness) = 0;

	int ndims, nelems;
};



class GAOptimizer : public IOptimizer
{
	const char* name() { return "GA"; }
};

