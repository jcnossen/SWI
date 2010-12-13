#include "StdIncl.h"
#include "ESOptimizer.h"


ESOptimizer::ESOptimizer(std::vector<float> ranges) {
	matrix = 0;
	fitness = 0;
	this->ranges = ranges;
}

ESOptimizer::~ESOptimizer() {
	SafeDeleteArray(matrix);
	SafeDeleteArray(fitness);
}

void ESOptimizer::initialize(int ndims, int nelems) {
	this->ndims = ndims;
	this->nelems = nelems;

	matrix = new float[ndims*nelems];
	fitness = new float[nelems];
}



void ESOptimizer::getElem(int elem, float *params) {
	std::copy(elemRow(elem), elemRow(elem) + ndims, params);
}


void ESOptimizer::setElem(int elem, float *params) {
	std::copy(params, params + ndims, elemRow(elem));
}


void ESOptimizer::setFitness(int elem, float fitness) {
	this->fitness[elem] = fitness;
}



void ESOptimizer::tick() {

	const int mu=15,sigma=.1;

	//rank the offspring
	std::vector<std::pair<float,int> > rank(nelems);
	for(int i=0;i<nelems;i++)
	{
		rank[i].first=-(this->fitness[i]);
		rank[i].second=i;
	}
	std::sort(rank.begin(),rank.end());

	std::vector<std::vector<float> > parents (mu,std::vector<float>(ndims));

	//select the parents
	for(int i=0;i<mu;i++)
	{
		int p=rank[i].second;
		for(int j=0;j<ndims;j++)
			parents[i][j]=matrix[p*ndims+j];
	}

	//create new offspring
	for(int i=0;i<nelems;i++)
	{
		int p=rand()%mu;
		for(int j=0;j<ndims;j++) 
			matrix[i*ndims+j]=parents[p][j]+RandomNormalDistributed(0,sigma);
	}



}
