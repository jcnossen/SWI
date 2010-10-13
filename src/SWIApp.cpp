#include "StdIncl.h"
#include "SWIApp.h"
#include "OpenGL.h"
#include "SqcConfig.h"
#include "GlyphRender.h"
#include "RenderSystem.h"
#include "SwarmOptimizer.h"

SWIApp::SWIApp()
{
	drawingConfig = new SqcConfig();
	drawingConfig->randomConfig(6);

	drawingConfig->scaleFit();

	swarmOptimizer = new SwarmOptimizer(SwarmConfig());
	int nsquares = 20;
	swarmOptimizer->initialize(nsquares * 2, 10);

	SqcConfig cfg;
	std::vector<float> params;
	params.reserve(nsquares * 2);

	for (int j=0;j<swarmOptimizer->nelems;j++) {
		cfg.randomConfig(nsquares);
		cfg.collectParams(params);
		swarmOptimizer->setElem(j, &params[0]);
	}

	best=0;
}


SWIApp::~SWIApp()
{
	delete drawingConfig;
}

void SWIApp::draw()
{
	float s=40;
	float aspect = RenderUtil::height / (float)RenderUtil::width;
	RenderUtil::beginCamera(Box2(-s,-s*aspect,s,s*aspect));
	glColor3ub(0,0,255);
	glPushMatrix();
//	RenderUtil::drawCircle(Vector2(), 10.0f, true);
	SqcConfig config;
	std::vector<float> bestp(swarmOptimizer->ndims);
	swarmOptimizer->getElem(best,&bestp[0]);
	config.initFromParams(bestp);
	config.scaleFit();
	config.computeBoundingCircle();
	config.moveToCenter();
	config.render(20.0f);
	RenderUtil::drawCircle(config.center, config.radius, false);
	glPopMatrix();
	RenderUtil::endCamera();

	glColor4ub(255,255,255,255);

//	drawingConfig->scaleFit();
//	drawingConfig->computeBoundingCircle();
//	drawingConfig->moveToCenter();

	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 50), 20.0f, SPrintf("r=%f", config.radius).c_str());
//	d_trace("r=%f, midx=%f midy=%f\n",r,x,y);

	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 100), 20.0f, "SwarmOptimizer Intelligence Demo");
}

void SWIApp::tick()
{
}

void SWIApp::swarmTick()
{
	SqcConfig config;

	std::vector<float> params (swarmOptimizer->ndims);
	float best_radius = 10000000.0f;

	// Calculate fitness values for each element
	for (int i=0;i<swarmOptimizer->nelems;i++) {
		swarmOptimizer->getElem(i, &params[0]);
		config.initFromParams(params);
		//config.scaleFit();

		if (config.isValid()) {
			config.computeBoundingCircle();
			swarmOptimizer->setFitness(i, -config.radius);
		} else {
			swarmOptimizer->setFitness(i, 10000.0f);
		}

		if(config.radius < best_radius) {
			best = i;
			best_radius = config.radius;
		}
	}

	swarmOptimizer->tick();
}


