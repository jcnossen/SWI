#include "StdIncl.h"
#include "SqcConfig.h"
#include "OpenGL.h"
#include "Miniball.h"
#include "RenderSystem.h"

SqcConfig::SqcConfig() {
	radius=0.0f;
}

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
	}

	//dependencies=std::vector<int>(2*n,-1); //J
}


//scale such that something touches and nothing overlaps
void SqcConfig::scaleFit()
{
    
	//resolve dependencies
	for(int i=0;i<squares.size();i++) //JdR
	{
		for(int d=0;d<2;d++)
			if(rand()%dependencies.size()==0)
			{
				if(dependencies[i*2+d]>=0)
					dependencies[i*2+d]=-1;
				else
					do
					{
						dependencies[i*2+d]=rand()%squares.size();
					} 
					while(dependencies[i*2+d]!=i);
			}
	

		if(dependencies[i*2+0]>=0)
			squares[i].x=squares[dependencies[i*2+0]].x+(dependencies[i*2+0]+i+2)/2.0;
		if(dependencies[i*2+1]>=0)
			squares[i].y=squares[dependencies[i*2+1]].y+(dependencies[i*2+1]+i+2)/2.0;
	}
	


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

void SqcConfig::collectParams(std::vector<float> &params) {
	params.resize(squares.size()*2);
	for (int i=0;i<squares.size();i++) {
		params[i*2+0] = squares[i].x;
		params[i*2+1] = squares[i].y;
	}
}

void SqcConfig::initFromParams(std::vector<float>& src) {
	squares.resize(src.size()/2);
	for(int i=0;i<squares.size();i++) {
		squares[i].x = src[i*2+0];
		squares[i].y = src[i*2+1];
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
	center.x=p[0]; 
	center.y=p[1];
}

void SqcConfig::moveToCenter()
{
	for(int i=0;i<squares.size();i++)
	 	squares[i]-=center;
	center = Vector2();
}

