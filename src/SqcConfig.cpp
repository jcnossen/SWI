#include "StdIncl.h"
#include "SqcConfig.h"
#include "OpenGL.h"
#include "Miniball.h"

void SqcConfig::render()
{
	for (int i=0;i<squares.size();i++) {
		float hw = (i+1)*.5f;
		Vector2 p = squares[i];
		glColor(Color::Black);
		glRectf(p.x - hw, p.y - hw, p.x + hw, p.y + hw);
		glColor(Color::Red);
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
		//squares[i].x=squares[i].y=i;
		
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


//scale such that something touches and nothing overlaps
//perhaps also fix centeroverlaps?
void SqcConfig::scalefit()
{
	for(int i=0;i<squares.size();i++)
		for(int j=i+1;j<squares.size();j++)
	 	{


		}


}


void SqcConfig::multiply(Vector2 v)
{
	for(int i=0;i<squares.size();i++)
		squares[i] *= v;
}

void SqcConfig::computeBoundingCircle()
{
	Miniball<2> mb;
	for(int i=1;i<=(int)squares.size();i++)
	{
		Point<2> p;
		for(int d0=-1;d0<=1;d0+=2)
			for(int d1=-1;d1<=1;d1+=2)
		    {
				p[0]=squares[i-1].x+.5*i*d0; 
				p[1]=squares[i-1].y+.5*i*d1; 
				mb.check_in(p);
			}
	}
	mb.build();

	radius=sqrt(mb.squared_radius());
	Point<2> p=mb.center();
	centerx=p[0];
	centery=p[1];
}