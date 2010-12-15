#pragma once
#include "Optimizer.h"

// Use an Evolutionary Strategy with neighbour dependencies
class ESOptimizer : public IOptimizer
{
public:
	ESOptimizer(std::vector<float> minmax);  // 2 floats (min/max) for each dimension
	~ESOptimizer();

	void initialize(int ndims, int nelems);
	const char* name() { return "ES"; }
	void tick(float sigma);
	void getElem(int elem, float* params);
	void setElem(int elem, float* params);
	void setFitness(int elem, float fitness);

protected:
	float* elemRow(int elem) { return &matrix[elem * (ndims+nadapt)]; }

	std::vector<float> ranges;
	float *matrix;
	float *fitness;
	int nadapt;
};