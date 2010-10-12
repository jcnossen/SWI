#include "StdIncl.h"
#include "Optimizer.h"


RandomOptimizer::RandomOptimizer(std::vector<float> ranges) {
	matrix = 0;
	fitness = 0;
	this->ranges = ranges;
}

RandomOptimizer::~RandomOptimizer() {
	SafeDeleteArray(matrix);
	SafeDeleteArray(fitness);
}

void RandomOptimizer::initialize(int ndims, int nelems) {
	this->ndims = ndims;
	this->nelems = nelems;

	matrix = new float[ndims*nelems];
	fitness = new float[nelems];
}



void RandomOptimizer::getElem(int elem, float *params) {
	std::copy(elemRow(elem), elemRow(elem) + ndims, params);
}


void RandomOptimizer::setElem(int elem, float *params) {
	std::copy(params, params + ndims, elemRow(elem));
}


void RandomOptimizer::setFitness(int elem, float fitness) {
	this->fitness[elem] = fitness;
}



void RandomOptimizer::tick() {
	// generate new
	for (int i=0;i<nelems;i++) {
		for (int j=0;j<ndims;j++)
			matrix[i*ndims+j] = UniformRandom(ranges[j*2+0], ranges[j*2+1]);
	}
}

