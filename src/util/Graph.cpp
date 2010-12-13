#include "StdIncl.h"
#include "Graph.h"

Graph::Graph(int nTicks, float min, float max)
{
	numTicks = nTicks;
	valuemin = min;
	valuemax = max;
}

void Graph::addTick(const std::vector<float>& x)
{
	addTick(&x[0],x.size());
}

void Graph::addTick(const float* d, int n)
{
	if (data.size() == numTicks)
		data.pop_front();

	data.push_back(std::vector<float>(d,d+n));
}

void Graph::render(Box2 box)
{
	int s = data.size();
//	std::pair<float,float> minmax = getMinMax();

	glColor3ub(128,127,128);
//	glRectf(box.a.x,box.a.y,box.b.x,box.b.y);

	glPushMatrix();
	glTranslatef(box.a.x,box.a.y,0.0f);	
	Vector2 size = box.size();
	glScalef(size.x, size.y, 1.0f);

	glScalef(1.0f/numTicks, 1.0f/(valuemax-valuemin), 1.0f);
	glTranslatef(0.0f, -valuemin, 0.0f);

	glRectf(0.0f,valuemin,numTicks,valuemax);

	const Color colors[] = {
		Color::Blue, Color::Green, Color::Black, Color::Purple, Color::Red, Color::Yellow
	};
	const int numcol=sizeof(colors)/sizeof(Color);

	if (!data.empty()) {
		glBegin(GL_LINES);
		GraphData::iterator i=data.begin();
		int x=0;
		for(GraphData::iterator prev=i++;i!=data.end();prev=i++, x++) {
			for (int y=0;y<std::min(prev->size(), i->size()); y++) {
				glColor(colors[y%numcol]);
				glVertex2f(x, (*prev)[y]);
				glVertex2f(x+1, (*i)[y]);
			}
		}
		glEnd();
	}


	glPopMatrix();
}

std::pair<float,float> Graph::getMinMax() {
	float min,max;
	for (GraphData::iterator i = data.begin(); i!= data.end(); ++i) {
		min=std::min(*std::min_element(i->begin(), i->end()),min);
		max=std::max(*std::max_element(i->begin(), i->end()),max);
	}
	return std::pair<float,float>(min,max);
}
