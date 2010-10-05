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
	GlyphRenderer::getDefaultRenderer()->drawString(Vector2(100, 100), 20.0f, "Swarm Intelligence Demo");
}

void SWIApp::tick()
{
}

