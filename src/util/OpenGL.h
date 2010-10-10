#pragma once

#include "Vector2.h"
#include "Color.h"


#ifdef TARGET_MOBILE

	#import <OpenGLES/ES1/gl.h>
	#import <OpenGLES/ES1/glext.h>

#else

	#include <SDL_opengl.h>

	// glVertex can't be used in OpenGL ES
	inline void glVertex(Vector2 v) {
		glVertex2f(v.x,v.y);
	}

	inline void glOrthof(float left, float right, float bottom, float top, float zNear, float zFar) {
		glOrtho(left, right, bottom, top, zNear, zFar);
	}

#endif


inline void glColor(Color c) {
	glColor4ub(c.c[0], c.c[1], c.c[2], c.c[3]);
}

inline void glLine(Vector2 a, Vector2 b, Color c) {
	Vector2 vrt[] = {a,b};

	glColor(c);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vrt);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}

inline void glQuad(Vector2* v, bool line=false) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, v);
	glDrawArrays(line ? GL_LINE_LOOP : GL_TRIANGLE_FAN, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
}


inline void glRectLines(float x1, float y1, float x2, float y2) {
	Vector2 v[4];
	v[0].x = v[3].x = x1;
	v[1].x = v[2].x = x2;
	v[0].y = v[1].y = y1;
	v[3].y = v[2].y = y2;
	glQuad(v, true);
}

