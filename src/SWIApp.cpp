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
	RenderUtil::beginCamera(Box2(-s,-s,s,s));
	drawingConfig->render();
	RenderUtil::endCamera();

	glColor4ub(255,255,255,255);

	drawingConfig->computeBoundingCircle();
	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 50), 20.0f, 
		SPrintf("r=%f, x=%f, y=%f", drawingConfig->radius,drawingConfig->centerx,drawingConfig->centery).c_str());
//	d_trace("r=%f, midx=%f midy=%f\n",r,x,y);

	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 100), 20.0f, "Swarm Intelligence Demo");
}

void SWIApp::tick()
{
}

