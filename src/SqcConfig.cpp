#include "StdIncl.h"
#include "SqcConfig.h"
#include "OpenGL.h"

void SqcConfig::render()
{
	for (int i=0;i<squares.size();i++) {
		float hw = (i+1)*.5f;
		Vector2 p = squares[i];
		glColor(Color::Black);
		glRectf(p.x - hw, p.y - hw, p.x + hw, p.y + hw);
		glColor(Color::White);
		glRectLines(p.x - hw, p.y - hw, p.x + hw, p.y + hw);
	}
}

void SqcConfig::randomConfig(int n)
{
	squares.resize(n);

	for (int i=0;i<n;i++) {
		float angle = UnitRandom() * 2 * PI;
		float dist = UnitRandom() * n;

		squares[i] = Vector2(angle) * dist;
	}
}

void SqcConfig::scalefix()
{
	while (!isValid())
		multiply(Vector2(1.1f, 1.1f));
}

bool SqcConfig::isValid()
{
	for (int i=0;i<squares.size();i++) {
		for (int j=0;j<squares.size();j++) {
			if (i==j) continue;

			if (getBox(i).overlaps(getBox(j)))
				return false;
		}
	}
	return true;
}

void SqcConfig::multiply(Vector2 v)
{
	for(int i=0;i<squares.size();i++)
		squares[i] *= v;
}

