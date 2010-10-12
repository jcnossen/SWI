#include "StdIncl.h"
#include "SWIApp.h"
#include "OpenGL.h"
#include "SqcConfig.h"
#include "GlyphRender.h"
#include "RenderSystem.h"

SWIApp::SWIApp()
{
	drawingConfig = new SqcConfig();
	drawingConfig->randomConfig(10);

	drawingConfig->scalefix();
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
	RenderUtil::drawCircle(Vector2(), 10.0f, true);
	drawingConfig->render();

	RenderUtil::endCamera();

	glColor4ub(255,255,255,255);

	drawingConfig->computeBoundingCircle();
	drawingConfig->moveToCenter();
	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 50), 20.0f, 
		SPrintf("r=%f, x=%f, y=%f", drawingConfig->radius,drawingConfig->center.x,drawingConfig->center.y).c_str());
//	d_trace("r=%f, midx=%f midy=%f\n",r,x,y);


	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 100), 20.0f, "SwarmOptimizer Intelligence Demo");
}

void SWIApp::tick()
{
}

