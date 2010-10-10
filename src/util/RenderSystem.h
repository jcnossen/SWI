#pragma once

#include "OpenGL.h"

struct GLTextureInfo;

class Texture {
public:
	int width, height;
	uint glID;
	bool hasAlpha; //requires blending?
	int origWidth, origHeight;
	Vector2 coordScale; // based on orig vs extended size
	std::string filename;

	Texture() {
		width=height=0;
		origWidth=origHeight=0;
		glID=0;
		refCount=0;
		hasAlpha = false;
	}
	Texture(const GLTextureInfo& o);

	void release();
	void addRef();

	Vector2 invSize() { return Vector2(1.0f/width, 1.0f/height); }
	Vector2 origSize() { return Vector2(origWidth, origHeight); }
	Vector2 size() { return Vector2(width, height); }
	void freeTexture();
	void setFiltering(bool antiAlias);
	void setWrapMode(bool repeat);

	// Texture manager
	static Texture* load(const char* name, bool antiAlias) {
		if (name) return load ((std::string)name, antiAlias);
		return 0;
	}
	static Texture* load(const std::string& name, bool antiAlias);
	
private:
	~Texture();
	int refCount;
};

class RenderUtil
{
public:
	static void setupProjection();
	static void setupCamera(Box2 box);
	static void beginCamera(Box2 box);
	static void endCamera();

	static int width, height;

	static void setScreenDims(int w,int h);
	static Box2 screenBox() {
		return Box2(0,0,width,height);
	}
	static Vector2 screenTopRight() {
		return Vector2(width, height);
	}

	static void drawRotatedTexture(Texture* tex, Vector2 pos, Vector2 size, float angle);	
	static void drawRotatedTexture(Texture* tex, Vector2 pos, Vector2 size, float angle, Vector2 pivot);	

	static void drawTexture(Box2 rc, Texture* texture, bool alpha=false);
	static void drawTexture(Vector2 bottomLeft, Vector2 topRight, Vector2 tcMin, Vector2 tcSize, Texture* texture, bool alpha=false);
	static void drawTexture(Vector2 bottomLeft, Vector2 topRight, Texture* texture, bool alpha=false);
	static void drawTexture(Vector2 bottomLeft, Vector2 topRight, int x1,int y1,int w,int h, Texture* texture, bool alpha);
};

template<typename T> class gl_data_type {};
template<> class gl_data_type<float> { public: static const GLenum value = GL_FLOAT; };
template<> class gl_data_type<ushort> { public: static const GLenum value = GL_UNSIGNED_SHORT; };
template<> class gl_data_type<uchar> { public: static const GLenum value = GL_UNSIGNED_BYTE; };
template<> class gl_data_type<char> { public: static const GLenum value = GL_BYTE; };

template<typename T>
class gl_data_type<T*> { };

template<typename TPos>
void GL_draw_vertex_array(TPos* pos, GLenum mode, int nelem, bool alpha=true) {
	if (alpha) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, gl_data_type<TPos>::value, 0, pos);
	glDrawArrays(mode, 0, nelem);
	if (alpha) glDisable(GL_BLEND);
	glDisableClientState(GL_VERTEX_ARRAY);
}


template<typename TPos, typename TTC>
void GL_draw_vertex_array_tc(TPos* pos, TTC* tc, GLenum mode, int nelem, Texture* texture, bool alpha=true) {
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, gl_data_type<TTC>::value, 0, tc);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->glID);
	GL_draw_vertex_array(pos, mode, nelem, alpha);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

template<typename TPos, typename TTC>
void GL_draw_quad_tc(TPos* pos, TTC* tc, Texture* texture, bool alpha=true) {
	GL_draw_vertex_array_tc(pos, tc, GL_TRIANGLE_FAN, 4, texture, alpha);
}

template<typename TPos>
void GL_draw_quad(TPos* pos, bool alpha=true) {
	GL_draw_vertex_array(pos, GL_TRIANGLE_FAN, 4, alpha);
}

