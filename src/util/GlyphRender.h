
#pragma once

#include <wchar.h>
#include "Vector2.h"
#include "PlatformAPI.h"

class Texture;

struct GlyphChar {
	wchar_t character;
	float x,y,w,h,kernx1,kernx2;
};


#define EG_ORIENTATION_LEFT 0
#define EG_ORIENTATION_RIGHT 1
#define EG_ORIENTATION_TOP 2
#define EG_ORIENTATION_BOTTOM 3
#define EG_ORIENTATION_CENTERED 4

class GlyphRenderer {
public:
	void release();
	void addRef();

	Texture* texture;

	typedef std::map<char, int> CharMap;
	CharMap charMap;
	int spaceWidth;
	std::string config;
	
	int numChars;
	GlyphChar* chars;

	GlyphRenderer();

	void initFont(const char *font, const char* charset, int* charData);
	void drawString(Vector2 bottomLeft, Vector2 topRight, const char* text);
	void drawString(Vector2 bottomLeft, float height, const char *text);
	void drawStringWithScale(Vector2 bottomLeft, Vector2 scale, const char *text);
	void drawStringCentered(Vector2 center, Vector2 scale, const char *text);

	Vector2 getStringDims(const char* text);

	static GlyphRenderer* getDefaultRenderer();
	static GlyphRenderer* loadFromConfig(const char* cfg);
private:
	int refCount;
	~GlyphRenderer();

	static std::vector<GlyphRenderer*> loadedFonts;
};

