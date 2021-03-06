#include "StdIncl.h"
#include "SwarmOptimizer.h"

const char* SwarmOptimizer::graphTypeNames[] = {
  "star topology",
  "cycle topology",
  "Kn topology",
  "square grid topology",
  "random graph",
	"multistar"
};

int SwarmOptimizer::numGraphTypes() { 
	return sizeof(graphTypeNames) / sizeof(const char*); 
}


int tickcount=0;

//Create friendship network graph.
//graphType values: 0 -> star topology
//                  1 -> cycle topology
//                  2 -> Kn topology
//                  3 -> square grid topology       
//                  4 -> random graph
//                  5 -> multiple small stars topology
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

	case ST_MULTISTAR:
	int root=std::sqrt(float(ss));
	for(i=0;i<ss;i++)
	{
		int fr=(i/root)*root;
		if(fr!=i)
		{
			swarm[i].friends.push_back(fr);
			swarm[fr].friends.push_back(i);
		}
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

	for(ParticleVector::iterator j = swarm.begin(); j != swarm.end(); ++j)
  {
    j->personalOptimum=0;
    j->friendOptimum=0;
    j->position.resize(ndims);
    j->velocity.resize(ndims,1.0);
    j->personalBest.resize(ndims);
		j->friendBest.resize(ndims);

		for (int i=0;i<ndims;i++)
			j->velocity[i]=UniformRandom(-10.0f, 10.0f);
  }

	setupGraph(); 
}


//This function assumes the new fitnessvalues have just been updated
void SwarmOptimizer::tick(float sigma)
{
  //First update the personalBest and friendBest
	for(ParticleVector::iterator j = swarm.begin(); j!=swarm.end(); ++j)
	{
		//if there is an improvement...
    if(j->fitness>=j->personalOptimum)
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
/*
	  //screw it up
	  if(tickcount%100==0)
		  for(int i=0;i<ndims;i++)
			  j->velocity[i]=((rand()%1000)-500)/1000.0;
	  if(tickcount%500==0) 
		  for(int i=0;i<ndims;i++)
			  j->position[i]+=(rand()%20)-10;
*/  
	  
	  //update velocity  
    for(int i=0;i<ndims;i++) {
      j->velocity[i]*=config.omega*sigma;
      j->velocity[i]+=UnitRandom() * config.phi1*sigma*(j->personalBest[i]-j->position[i]);
      j->velocity[i]+=UnitRandom() * config.phi2*sigma*(j->friendBest[i]-j->position[i]);    
			j->velocity[i]+=(UnitRandom()-0.5f) * sigma * .01f;
    }

    //update position  
    for(int i=0;i<ndims;i++)
			j->position[i] += j->velocity[i];
  }

  tickcount++;
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
	for(int i=0;i<ndims;i++) {
		p.position[i] = params[i];
		p.friendBest[i] = p.personalBest[i] = params[i];
	}
}

void SwarmOptimizer::setFitness(int elem, float fitness) {
	swarm[elem].fitness = fitness;
}
