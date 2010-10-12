#include "StdIncl.h"
#include "SqcConfig.h"
#include "OpenGL.h"
#include "Miniball.h"
#include "RenderSystem.h"

void SqcConfig::render(float displayRadius)
{
	float factor=displayRadius/radius;

	RenderUtil::drawCircle(Vector2(), displayRadius, true);
	for (int i=0;i<squares.size();i++) {
		float hw = (i+1)*.5f*factor;
		Vector2 p = squares[i]*factor;
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


//scale such that something touches and nothing overlaps
void SqcConfig::scaleFit()
{
    //first make sure no centers are equal
	bool check=true;
	while(check)
	{
	    check=false;
		for(int i=0;i<squares.size();i++)
			for(int j=i+1;j<squares.size();j++)
				if((squares[i]-squares[j]).length2() < 0.01f)
				{
					squares[i].x+=.5f;
					check=true;
				}
	}
	
	float factor=0.0;
	for(int i=0;i<squares.size();i++)
		for(int j=i+1;j<squares.size();j++)
	 	{
			float dx=fabs(squares[i].x-squares[j].x);
			float dy=fabs(squares[i].y-squares[j].y);
			float maxd=std::max(dx,dy);

			float goald=(i+j+2)/2.0;

			factor=std::max(factor,goald/maxd);
		}

	for(int i=0;i<squares.size();i++)
		squares[i]*=factor;
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
	center.x=p[0]; 
	center.y=p[1];
}

void SqcConfig::moveToCenter()
{
	for(int i=0;i<squares.size();i++)
	 	squares[i]-=center;
	center.x=center.y=0;
}