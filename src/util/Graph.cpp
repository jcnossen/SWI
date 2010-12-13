#include "StdIncl.h"
#include "Graph.h"

Graph::Graph(int nTicks)
{
	numTicks = nTicks;
}

void Graph::addTick(const std::vector<float>& x)
{
	addTick(&x[0],x.size());
}

void Graph::addTick(const float* d, int n)
{
	if (data.size() == numTicks)
		data.pop_front();

	data.push_back(std::vector<float>(d,d+n));
}

void Graph::render()
{
	int s = data.size();
	float min, max;

	for (GraphData::iterator i = data.begin(); i!= data.end(); ++i) {
		
	}
}
