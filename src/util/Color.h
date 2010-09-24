#pragma once


struct Color {
	uchar c[4];

	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Yellow;
	static Color Purple;
	static Color White;
	static Color Black;
	static Color Grey;

	float r() { return c[0] / 255.0f; }
	float g() { return c[1] / 255.0f; }
	float b() { return c[2] / 255.0f; }
	float a() { return c[3] / 255.0f; }

	operator uint() const { return *((uint*)&c[0]); }
	Color() { set(0); }
	Color(uint c) { set(c); }
	Color(int r,int g,int b, int a=255) {
		set(((a&0xff)<<24) + ((b&0xff) << 16) + ((g&0xff) << 8) + (r&0xff));
	}
	Color(float r,float g,float b, float a=1.0f) {
		set((( 0xff & (int)(a*255.0f) )<<24) +
			(( 0xff & (int)(b*255.0f) )<<16) +
			(( 0xff & (int)(g*255.0f) )<< 8) +
			( 0xff & (int)(r*255.0f) ));
	}
	void set(uint col) { *((uint*)&c[0]) = *(uint*)&col; }
};
