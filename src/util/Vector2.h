#pragma once

#include <cmath>


struct Vector2 {
	float x,y;

	Vector2() {
		x=y=0.0f;
	}

	Vector2(float X, float Y) {
		x=X; y=Y;
	}

	explicit Vector2(float angle) {
		x=cosf(angle);
		y=sinf(angle);
	}
	
	//Vector2(b2Vec2 v) {
	//	x=v.x;
	//	y=v.y;
	//}


	Vector2 operator-() const {
		return Vector2(-x,-y);
	}

	Vector2 operator+(Vector2 v) const {
		return Vector2(v.x+x,v.y+y);
	}

	Vector2 operator*(Vector2 v) const {
		return Vector2(v.x*x,v.y*y);
	}

	Vector2 operator/(Vector2 v) const {
		return Vector2(x/v.x,y/v.y);
	}

	friend Vector2 operator*(float a, Vector2 v) {
		return Vector2(a*v.x,a*v.y);
	}

	friend Vector2 operator/(float a, Vector2 v) {
		return Vector2(a/v.x,a/v.y);
	}

	friend Vector2 operator*(Vector2 v, float a) {
		return Vector2(a*v.x,a*v.y);
	}

	friend Vector2 operator/(Vector2 v, float a) {
		return Vector2(v.x/a,v.y/a);
	}

	Vector2 operator-(Vector2 v) const {
		return Vector2(x-v.x,y-v.y);
	}

	Vector2& operator+=(Vector2 v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	Vector2& operator-=(Vector2 v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vector2& operator*=(Vector2 v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}
	Vector2& operator*=(float v) {
		x *= v;
		y *= v;
		return *this;
	}
	Vector2& operator/=(Vector2 v) {
		x /= v.x;
		y /= v.y;
		return *this;
	}

	float length2() const {
		return x*x+y*y;
	}
	float length() const {
		return sqrtf(length2());
	}

	float distance(Vector2 v) {
		return (*this - v).length();
	}

	float normalize() {
		float len = length();
		x/=len;
		y/=len;
		return len;
	}
	
	Vector2 normalized() { 
		Vector2 cp(x,y);
		cp.normalize();
		return cp;
	}

	// returns original length
	float setLength(float l) {
		float len = length();
		if (len!=0.0f) {
			float f = l/len;
			x*=f;
			y*=f;
		}
		return len;
	}

	float distanceToLine(Vector2 a, Vector2 b);
	Vector2 closestPosOnLine(Vector2 a, Vector2 b, float*t = 0);
	float distanceToRay(Vector2 a, Vector2 b);


	Vector2 ortho() const {
		return Vector2(-y, x);
	}

	static float dot(const Vector2 & a, const Vector2& b) {
		return a.x*b.x+a.y*b.y;
	}

	float dot(const Vector2& a) {
		return x*a.x+y*a.y;
	}

	//operator b2Vec2() const {
	//	return b2Vec2(x,y);
	//}
	
	
};


struct Box2 {
	Box2(Vector2 a, Vector2 b) : a(a), b(b) {}
	Box2() {}
	Box2(float xs, float ys, float xe, float ye) {
		a.x=xs; a.y=ys; b.x=xe; b.y=ye;
	}

	float width() { return b.x-a.x; }
	float height() { return b.y-a.y; }
	
	bool contains(Vector2 p) {
		return (p.x>=a.x && p.x <= b.x) &&
				(p.y>=a.y && p.y <= b.y);
	}

	Vector2 size() {
		return b-a;
	}

	void move(Vector2 v) {
		a+=v;
		b+=v;
	}

	Box2 moved(Vector2 v) {
		return Box2(a+v,b+v);		
	}

	void extend(Vector2 v) {
		if (v.x < a.x) a.x = v.x;
		if (v.x > b.x) b.x = v.x;

		if (v.y < a.y) a.y = v.y;
		if (v.y > b.y) b.y = v.y;
	}

	Vector2 center() {
		return (a+b)*0.5f;
	}

	Box2 scaled(Vector2 s) {
		Vector2 mid = center();
		Vector2 hsize = (b-a)*0.5f;
		hsize *= s;

		return Box2(mid-hsize,mid+hsize);
	}

	// corners
	Vector2 bottomLeft() {return a;}
	Vector2 topLeft() { return Vector2(a.x,b.y);}
	Vector2 topRight() { return b; }
	Vector2 bottomRight() { return Vector2(b.x, a.y);}

	Box2 operator*(float f) {
		return Box2(a*f,b*f);
	}

	//operator b2AABB() {
	//	b2AABB aabb = {a,b};
	//	return aabb;
	//}

	// members
	Vector2 a,b;
};

struct int2 {
	int x,y;
	
	int2() { x=y=0; }
	int2(int X,int Y) { 
		x=X; y=Y;
	}

	int2(const Vector2& v) { x=(int)v.x; y=(int)v.y; }
	operator Vector2() const { return Vector2((float)x,(float)y); }
};

