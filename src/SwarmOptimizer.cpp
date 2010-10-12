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
    for(i=0;i<swarm.size();i++)
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
        if(rand()%1000<1000*config.randomParam)
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

	this->ndims = ndims;
	this->nelems = nelems;

	swarm.resize(nelems);

	for(ParticleVector::iterator j = swarm.begin(); j!= swarm.end(); ++j)
	{
		j->personalOptimum=0;
		j->friendOptimum=0;
		j->position.resize(ndims);
		j->velocity.resize(ndims);
		j->personalBest.resize(ndims);
		j->friendBest.resize(ndims);
	}

	setupGraph(); 
}


//This function assumes the new fitnessvalues have just been updated
void SwarmOptimizer::tick()
{
  //First update the personalBest and friendBest
	for(ParticleVector::iterator j = swarm.begin(); j!=swarm.end(); ++j)
	{
		//if there is an improvement...
		if(j->fitness>j->personalOptimum)
		{
			//update the personal best
			j->personalOptimum=j->fitness;
			j->personalBest=j->position;
      
			//perhaps also update the friends best
			if(j->fitness>j->friendOptimum)
			{
				j->friendOptimum=j->fitness;
				j->friendBest=j->position;
			}
      
      //Broadcast this result to friends
      for(int k=0;k<(int)j->friends.size();k++)
      {      
        int f=j->friends[k];
        if(j->fitness>swarm[f].friendOptimum)
        {
          swarm[f].friendOptimum=j->fitness;
          swarm[f].friendBest=j->position;
        }
      }
    }
  }

  //Then adjust the position and velocity vectors
	for(ParticleVector::iterator j = swarm.begin(); j != swarm.end(); ++j)
  {
    //update velocity  
    for(int i=0;i<ndims;i++) {
      j->velocity[i]*=config.omega;
      j->velocity[i]+=UniformRandom(0,config.phi1)*(j->personalBest[i]-j->position[i]);
      j->velocity[i]+=UniformRandom(0,config.phi2)*(j->friendBest[i]-j->position[i]);    
    }

    //update position  
    for(int i=0;i<ndims;i++)
			j->position[i] += j->velocity[i];
  }
}


void SwarmOptimizer::getElem(int elem, float* params) 
{
	const SwarmParticle& p = swarm[elem];
	for(int i=0;i<ndims;i++) 
		params[i] = p.position[i];
}



void SwarmOptimizer::setElem(int elem, float* params) 
{
	SwarmParticle& p = swarm[elem];
	for(int i=0;i<ndims;i++) 
		p.position[i] = params[i];
}

void SwarmOptimizer::setFitness(int elem, float fitness) {
	swarm[elem].fitness = fitness;
}
