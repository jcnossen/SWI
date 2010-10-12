#pragma once

#include "Optimizer.h"

#define ST_STAR 0
#define ST_CYCLE 1
#define ST_KN 2
#define ST_SQUARE 3
#define ST_RANDOM 4

struct SwarmConfig
{
  SwarmConfig() { 
		popSize=20; 
		graphType=4;
		randomParam=0.1f; 	
    phi1=phi2=1.49618f;
    omega=0.7298f;
	}

  int popSize;
  int graphType; // ST_*
  float randomParam;
  float phi1,phi2,omega;
};

class Particle
{
public:
  Particle() { personalOptimum=fitness=0.0f;}
  float fitness,personalOptimum,friendOptimum;
  
  std::vector<float> position,velocity,personalBest,friendBest;
  std::vector<int> friends; 
};

class SwarmOptimizer : public IOptimizer
{
  std::vector<float> globalOptimal; 
  std::vector<Particle> swarm;
	SwarmConfig config;

public:
	SwarmOptimizer(SwarmConfig cfg);
  void setupGraph();
	void tick(float *fitness);
	void initialize(int ndims, int nelems);

	static int numGraphTypes();
	static const char* graphTypeNames[];
};
