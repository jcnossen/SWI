#include "StdIncl.h"
#include "SWIApp.h"
#include "OpenGL.h"
#include "SqcConfig.h"
#include "GlyphRender.h"
#include "RenderSystem.h"
#include "SwarmOptimizer.h"
#include "ESOptimizer.h"

SWIApp::SWIApp()
{
	int nsquares = 8;
	
	//initRandomOptimizer(nsquares);
	std::vector<float> ranges = std::vector<float>(nsquares * 4);
	for (int i=0;i<nsquares*2;i++) {
		ranges[i*2]=-10.0f;
		ranges[i*2+1]=10.0f;
	}

	SwarmConfig sc;
	sc.graphType = ST_MULTISTAR;
//	sc.phi1 = sc.phi2 = 0.4f;
	sc.omega = 0.7f;

  //optimizer = new ESOptimizer(ranges);
	optimizer = new SwarmOptimizer(sc);
	optimizer->initialize(nsquares * 2, 100);

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

void SWIApp::draw()
{
	float s=40;
	float aspect = RenderUtil::height / (float)RenderUtil::width;
	RenderUtil::beginCamera(Box2(-s,-s*aspect,s,s*aspect));
	glColor3ub(0,0,255);
	glPushMatrix();
		glTranslatef(-20.0f, 0.0f, 0.0f);
		best.render(15.0f);
		RenderUtil::drawCircle(best.center, best.radius, false);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(20.0f, 0.0f, 0.0f);
		last_best.render(15.0f);
		RenderUtil::drawCircle(last_best.center, last_best.radius, false);
	glPopMatrix();
	RenderUtil::endCamera();

	glColor4ub(255,255,255,255);

	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 50), 20.0f, SPrintf("Overall r=%f. F=%f", best.radius, best.fitness).c_str());
	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(500, 50), 20.0f, SPrintf("Last r=%f. F=%f", last_best.radius, last_best.fitness).c_str());
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

	// best of this generation
	last_best = SqcConfig();

	// Calculate fitness values for each element
	for (int i=0;i<optimizer->nelems;i++) {
		optimizer->getElem(i, &params[0]);

		config.initFromParams(params);
		//config.scaleFit();

		config.calcFitness();
		optimizer->setFitness(i, config.fitness);

		config.collectParams(params);
		optimizer->setElem(i, &params[0]);

		if (last_best.fitness == 0.0f || config.fitness > last_best.fitness)
			last_best = config;
	}

	if (best.fitness == 0.0f || last_best.fitness > best.fitness) {
		best = last_best;
		d_trace("New best radius: %f. Overlap: %f. Fitness: %f\n", best.radius, best.overlap, best.fitness);
		for (int i=0;i<best.squares.size();i++)
			d_trace(" (%f,%f);", best.squares[i].x, best.squares[i].y);
		d_trace("\n");
	}

	
	optimizer->tick();
}
