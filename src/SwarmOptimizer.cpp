#include "StdIncl.h"
#include "SwarmOptimizer.h"

const char* SwarmOptimizer::graphTypeNames[] = {

  "star topology",
  "cycle topology",
  "Kn topology",
  "square grid topology",
  "random graph"
};

int SwarmOptimizer::numGraphTypes() { 
	return sizeof(graphTypeNames) / sizeof(const char*); 
}


//Create friendship network graph.
//graphType values: 0 -> star topology
//                  1 -> cycle topology
//                  2 -> Kn topology
//                  3 -> square grid topology       
//                  4 -> random graph
void SwarmOptimizer::setupGraph()
{
  int i,j;
	int ss = swarm.size();

  switch(config.graphType) {
	case ST_STAR:
		for(i=1;i<swarm.size();i++)
    {
      swarm[0].friends.push_back(i);
      swarm[i].friends.push_back(0);
    }
		break;
	case ST_CYCLE:
    for(i=0;i<populationSize;i++)
    {
      swarm[i].friends.push_back((i+1)%ss);
      swarm[(i+1)%ss].friends.push_back(i);
    }
		break;

	case ST_KN:
    for(i=0;i<ss;i++)
      for(j=0;j<ss;j++)
        if(j!=i)
          swarm[i].friends.push_back(j);
		break;
          
	case ST_SQUARE: {
    int z = (int)sqrtf(ss);
    if(z*z!=ss) z++;
  
    //horizontal edges
    for(j=0;j<z;j++)
      for(i=0;i<z-1;i++)
        if(j*z+i+1<ss)
        {
          swarm[j*z+i].friends.push_back(j*z+i+1);
          swarm[j*z+i+1].friends.push_back(j*z+i);
        }
  
    //vertical edges
    for(i=0;i<z;i++)
      for(j=0;j<z-1;j++)
        if((j+1)*z+i<ss)
        {
          swarm[j*z+i].friends.push_back((j+1)*z+i);
          swarm[(j+1)*z+i].friends.push_back(j*z+i);
        }
		break;}

	case ST_RANDOM:
    for(i=0;i<ss;i++)
      for(j=i+1;j<ss;j++)
        if(rand()%1000<1000*randomParam)
        {
          swarm[j].friends.push_back(i);
          swarm[i].friends.push_back(j);        
        }
  }

}


SwarmOptimizer::SwarmOptimizer(SwarmConfig cfg)
{
	config = cfg;
}

void SwarmOptimizer::initialize(int ndims, int nelems) {

	swarm.resize(nelems);

  for(int j=0;j<swarm.size();j++)
  {
    swarm[j].personalOptimum=0;
    swarm[j].friendOptimum=0;
    swarm[j].position.resize(ndims);
    swarm[j].velocity.resize(ndims);
    swarm[j].personalBest.resize(ndims);
		swarm[j].friendBest.resize(ndims);
  }

	setupGraph(); 
}


//This function assumes the new fitnessvalues have just been updated
void SwarmOptimizer::tick(float* fitness)
{
  //First update the personalBest and friendBest
  for(int j=0;j<swarm.size;j++)
  {
    //if there is an improvement...
    if(swarm[j].fitness>swarm[j].personalOptimum)
    {
      //update the personal best
      swarm[j].personalOptimum=swarm[j].fitness;
      swarm[j].personalBest=swarm[j].position;
      
      //perhaps also update the friends best
      if(swarm[j].fitness>swarm[j].friendOptimum)
      {
        swarm[j].friendOptimum=swarm[j].fitness;
        swarm[j].friendBest=swarm[j].position;
      }
      
      //Broadcast this result to friends
      for(int k=0;k<(int)swarm[j].friends.size();k++)
      {      
        int f=swarm[j].friends[k];
        if(swarm[j].fitness>swarm[f].friendOptimum)
        {
          swarm[f].friendOptimum=swarm[j].fitness;
          swarm[f].friendBest=swarm[j].position;
        }
      }
    }
  }

  //Then adjust the position and velocity vectors
  for(int j=0;j<populationSize;j++)
  {
    //update velocity  
    for(i=0;i<ndims;i++)
    {
      swarm[j].velocity[i]*=omega;
      swarm[j].velocity[i]+=Uniform(0,phi1)*(swarm[j].personalBest[i]-swarm[j].position[i]);
      swarm[j].velocity[i]+=Uniform(0,phi2)*(swarm[j].friendBest[i]-swarm[j].position[i]);    
    }

    //update position  
    for(i=0;i<dimension;i++)
      swarm[j].position[i]=max(paramRanges[i].min,min(paramRanges[i].max,swarm[j].position[i]+swarm[j].velocity[i]));
  }
}





