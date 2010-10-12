#pragma once

class IOptimizer
{
public:
	virtual ~IOptimizer() {}

	virtual void initialize(int ndims, int nelems) {}
	virtual const char* name() = 0;
	virtual void tick() = 0;
	virtual void getElem(int elem, float* params) = 0;
	virtual void setElem(int elem, float* params) = 0;
	virtual void setFitness(int elem, float fitness) = 0;

	int ndims, nelems;
};



class GAOptimizer : public IOptimizer
{
	const char* name() { return "GA"; }
};

// Just generate random stuff and keep the best
class RandomOptimizer : public IOptimizer
{
public:
	RandomOptimizer(std::vector<float> minmax);  // 2 floats (min/max) for each dimension
	~RandomOptimizer();

	void initialize(int ndims, int nelems);
	const char* name() { return "Random"; }
	void tick();
	void getElem(int elem, float* params);
	void setElem(int elem, float* params);
	void setFitness(int elem, float fitness);

protected:
	float* elemRow(int elem) { return &matrix[elem * ndims]; }

	std::vector<float> ranges;
	float *matrix;
	float *fitness;
};


