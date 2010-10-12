#include "StdIncl.h"
#include "SWIApp.h"
#include "OpenGL.h"
#include "SqcConfig.h"
#include "GlyphRender.h"
#include "RenderSystem.h"
#include "SwarmOptimizer.h"


SWIApp::SWIApp()
{
<<<<<<< .mine	drawingConfig = new SqcConfig();
	drawingConfig->randomConfig(3);

	drawingConfig->scaleFit();

	swarmOptimizer = new SwarmOptimizer(SwarmConfig());
=======>>>>>>> .theirs	int nsquares = 3;
	
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

<<<<<<< .mine//	drawingConfig->scaleFit();
//	drawingConfig->computeBoundingCircle();
//	drawingConfig->moveToCenter();

	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 50), 20.0f, SPrintf("r=%f", config.radius).c_str());
//	d_trace("r=%f, midx=%f midy=%f\n",r,x,y);
	std::vector<float> c;
	config.collectParams(c);
	for(int i=0;i<c.size();i+=2)
		GlyphRenderer::getDefaultRenderer()->drawString(Vector2(40, 20*i), 20.0f, SPrintf("%f", c[i]).c_str());
	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 100), 20.0f, "SwarmOptimizer Intelligence Demo");
=======	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 50), 20.0f, SPrintf("r=%f", best.radius).c_str());
	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 100), 20.0f, "optimizer Intelligence Demo");
>>>>>>> .theirs}

void SWIApp::tick()
{
	SqcConfig config;
	std::vector<float> params (optimizer->ndims);

	// Calculate fitness values for each element
	for (int i=0;i<optimizer->nelems;i++) {
		optimizer->getElem(i, &params[0]);
		config.initFromParams(params);
		config.scaleFit();
		config.computeBoundingCircle();
		config.moveToCenter();
		optimizer->setFitness(i, 20000-config.radius);

		if (best.radius == 0.0f || config.radius < best.radius) {
			best = config;
		}
	}

	optimizer->tick();
}


