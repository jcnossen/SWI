#include "StdIncl.h"
#include "SWIApp.h"
#include "OpenGL.h"
#include "SqcConfig.h"
#include "GlyphRender.h"
#include "RenderSystem.h"
#include "SwarmOptimizer.h"

SWIApp::SWIApp()
{
	int nsquares = 8;
	
	//initRandomOptimizer(nsquares);

    optimizer = new SwarmOptimizer(SwarmConfig());
	optimizer->initialize(nsquares * 2, 60);

	

	SqcConfig cfg;
	std::vector<float> params;
	params.reserve(nsquares * 2);

	for (int j=0;j<optimizer->nelems;j++) {
		cfg.randomConfig(nsquares);
		cfg.collectParams(params);
		optimizer->setElem(j, &params[0]);
	}

}




SWIApp::~SWIApp()
{
	delete optimizer;
}

void SWIApp::initRandomOptimizer(int nsquares)
{
	std::vector<float> ranges = std::vector<float>(nsquares * 4);
	for (int i=0;i<nsquares*2;i++) {
		ranges[i*2]=-10.0f;
		ranges[i*2+1]=10.0f;
	}

	optimizer = new RandomOptimizer(ranges);
}

void SWIApp::draw()
{
	float s=40;
	float aspect = RenderUtil::height / (float)RenderUtil::width;
	RenderUtil::beginCamera(Box2(-s,-s*aspect,s,s*aspect));
	glColor3ub(0,0,255);
	glPushMatrix();
		best.render(20.0f);
		RenderUtil::drawCircle(best.center, best.radius, false);
	glPopMatrix();
	RenderUtil::endCamera();

	glColor4ub(255,255,255,255);

	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 50), 20.0f, SPrintf("r=%f", best.radius).c_str());
	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 70), 20.0f, "Swarm Intelligence Demo");
}

void SWIApp::tick()
{
	for (int i=0;i<10;i++)
		optimizerTick();
}


void SWIApp::optimizerTick()
{
	SqcConfig config;
	std::vector<float> params (optimizer->ndims);

	std::vector<std::pair<float,int> > ranking;

	while(dependencies.size()<optimizer->nelems) //JdR
	{
		dependencies.push_back(std::vector<int>(optimizer->ndims,-1)); //JdR
					d_trace("vul aan\n");
	}

	// Calculate fitness values for each element
	for (int i=0;i<optimizer->nelems;i++) {
		optimizer->getElem(i, &params[0]);
		
		config.dependencies=dependencies[i]; //JdR
		

		config.initFromParams(params);
		config.scaleFit();
		config.computeBoundingCircle();
		config.moveToCenter();
		optimizer->setFitness(i, 20000-config.radius);

		ranking.push_back(std::make_pair(config.radius,i)); //JdR

		if (best.radius == 0.0f || config.radius < best.radius) {
			best = config;
			d_trace("New best: %f\n", best.radius);
			for (int i=0;i<best.squares.size();i++) {
				d_trace(" (%f,%f);", best.squares[i].x, best.squares[i].y);
			}
			d_trace("\n");
		}
	}
	sort(ranking.begin(),ranking.end()); //JdR
	for(int i=0;i<ranking.size();i++) //JdR
		dependencies[ranking[ranking.size()-1-i].second]=dependencies[ranking[i].second]; //JdR

	optimizer->tick();
}
