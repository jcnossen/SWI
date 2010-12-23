//--------------------------------------------------------------------------------------------------------------
// SqcConfig
//
// This file contains 3 class:
//  SqcConfig: Abstract base class for the square-in-circle packing problem
//	SqcConfigVC: Class that stores the configuration with vector coordinates
//	SqcConfigNC: Class that stores the configuration with 'nabour' coordinates
//--------------------------------------------------------------------------------------------------------------
#include "StdIncl.h"
#include "SqcConfig.h"
#include "OpenGL.h"
#include "Miniball.h"
#include "RenderSystem.h"


//--------------------------------------------------------------------------------------------------------------
// SqcConfig base class
//--------------------------------------------------------------------------------------------------------------


void SqcConfig::render(float displayRadius)
{
	float factor=displayRadius/radius;

	RenderUtil::drawCircle(Vector2(), displayRadius, true);
	for (int i=0;i<nsquares();i++) {
		float hw = (i+1)*.5f*factor;
		Vector2 p = (getSquare(i)-center)*factor;
		glColor(Color::Black);
		glRectf(p.x - hw, p.y - hw, p.x + hw, p.y + hw);
		glColor(Color::Red);
		glRectLines(p.x - hw, p.y - hw, p.x + hw, p.y + hw);
	}
}

float SqcConfig::overlapscore()
{
	float sum = 0.0f;

	for (int i=0;i<nsquares();i++) {
		for (int j=0;j<i;j++) {
			float s = getBox(i).moveoutdistance(getBox(j));
			sum += s*s;
		}
	}
	return sum;
}


void SqcConfig::calcFitness()
{
	computeBoundingCircle();
	float os = 10*overlapscore();
	if (os > 0.0f) os += 10.0f;

	overlap = os;
	fitness = 1000 - radius - os;
}


void SqcConfig::computeBoundingCircle()
{
	Miniball<2> mb;
	for(int i=1;i<=(int)nsquares();i++)
	{
		Point<2> p;
		for(int d0=-1;d0<=1;d0+=2)
			for(int d1=-1;d1<=1;d1+=2)
			{
				Vector2 v = getSquare(i-1);
				p[0]=v.x+.5*i*d0;
				p[1]=v.y+.5*i*d1;
				mb.check_in(p);
			}
	}
	mb.build();

	radius=sqrt(mb.squared_radius());
	Point<2> p=mb.center();
	center.x=p[0]; 
	center.y=p[1];
}


//--------------------------------------------------------------------------------------------------------------
// SqcConfigVC: SqcConfig with Vector Coords
//--------------------------------------------------------------------------------------------------------------


void SqcConfigVC::randomConfig(int n)
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
void SqcConfigVC::scaleFit()
{
	std::vector<Vector2> sq;
	sq.reserve(nsquares());
	for (int i=0;i<nsquares();i++)
		sq.push_back(getSquare(i));

    //first make sure no centers are equal
	bool check=true;
	while(check)
	{
	    check=false;
		for(int i=0;i<sq.size();i++)
			for(int j=i+1;j<sq.size();j++)
				if((sq[i]-sq[j]).length2() < 0.01f)
				{
					sq[i].x+=.5f;
					check=true;
				}
	}
	
	float factor=0.0;
	for(int i=0;i<sq.size();i++)
		for(int j=i+1;j<sq.size();j++)
	 	{
			float dx=fabs(sq[i].x-sq[j].x);
			float dy=fabs(sq[i].y-sq[j].y);
			float maxd=std::max(dx,dy);

			float goald=(i+j+2)/2.0;

			factor=std::max(factor,goald/maxd);
		}

	for(int i=0;i<sq.size();i++)
		sq[i]*=factor;
	// move last to (0,0) and copy back to squares vector
	for(int i=0;i<sq.size()-1;i++)
		squares[i] = sq[i]-sq.back();
}

std::vector<float> SqcConfigVC::collectParams() {
	std::vector<float> params(squares.size()*2);
	for (int i=0;i<squares.size();i++) {
		params[i*2+0] = squares[i].x;
		params[i*2+1] = squares[i].y;
	}
	return params;
}

void SqcConfigVC::initFromParams(std::vector<float>& src) {
	squares.resize(src.size()/2);
	for(int i=0;i<squares.size();i++) {
		squares[i].x = src[i*2+0];
		squares[i].y = src[i*2+1];
	}
}

void SqcConfigVC::multiply(Vector2 v)
{
	for(int i=0;i<squares.size();i++)
		squares[i] *= v;
}


void SqcConfigVC::calcSquarePositions()
{
	squarePos.resize(squares.size()+1);
	for(int i=0;i<squares.size();i++)
		squarePos[i] = squares[i];
	squarePos[squares.size()] = Vector2();
}


//--------------------------------------------------------------------------------------------------------------
// SqcConfigNC: SqcConfig with Nabour Coords
//--------------------------------------------------------------------------------------------------------------


/*



*/

void SqcConfigNC::randomConfig(int numSquares)
{
}

void SqcConfigNC::calcSquarePositions()
{
	
}

