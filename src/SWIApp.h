#pragma once

class SqcConfig;

class SWIApp
{
public:
	SWIApp();
	~SWIApp();

	void tick();
	void draw();

private:
	SqcConfig* drawingConfig; // config that is currently being rendered
};

