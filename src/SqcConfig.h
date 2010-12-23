#pragma once

/*
Base square problem configuration
*/
class SqcConfig {
public:
	SqcConfig() {
		radius=0.0f;
		fitness=0.0f;
		overlap=0.0f;
	}
	virtual ~SqcConfig() {}

	void save(std::string file);
//	void load(std::string file);

	void render(float displayRadius=100.0);

	virtual void initFromParams(std::vector<float>& src) = 0; // load from src
	virtual std::vector<float> collectParams() = 0; // save params in dst
	virtual void randomConfig(int numSquares) = 0;
	virtual void calcSquarePositions() = 0;

	Box2 getBox(int i) {
		float hw=(i+1)*0.5f;
		Vector2 p = getSquare(i);
		return Box2(p.x-hw,p.y-hw,p.x+hw,p.y+hw);
	}

	void computeBoundingCircle();
	float overlapscore();
	void calcFitness();

	int nsquares() { return squarePos.size(); }
	Vector2 getSquare(int i) { return squarePos[i]; }

	// Updated by calcSquarePositions()
	std::vector<Vector2> squarePos; // square size is determined by order

	// Updated by calcFitness()
	float radius, fitness, overlap;
	Vector2 center;
};

/*
SqcConfig using Vector Coordinates
*/
class SqcConfigVC : public SqcConfig
{
public:
	SqcConfigVC() {}

	void multiply(Vector2 v);
	void scaleFit();
	
	// SqcConfig interface
	void calcSquarePositions();
	void initFromParams(std::vector<float>& src); // load from src
	std::vector<float> collectParams(); // save params in dst
	void randomConfig(int n);

protected:
	std::vector<Vector2> squares; // [nsquares()-1] since last square is fixed on (0,0)
};

/*
SqcConfig using Nabour Coordinates
*/
class SqcConfigNC : public SqcConfig
{
public:
	// SqcConfig interface
	void calcSquarePositions();
	void initFromParams(std::vector<float>& src) { ncoords=src; } // load from src
	std::vector<float> collectParams() {return ncoords;} // save params in dst
	void randomConfig(int nsq);

protected:
	std::vector<float> ncoords;
};
