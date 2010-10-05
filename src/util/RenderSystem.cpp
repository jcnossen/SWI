#include "StdIncl.h"
#include "OpenGL.h"

#include "PlatformAPI.h"
#include "RenderSystem.h"

#include <map>

// Screen width and height, flipped in case of pf_FlipScreen
int RenderUtil::width  =0;
int RenderUtil::height = 0;

typedef std::map<std::string, Texture*> TexturesMap;
static TexturesMap textures;

Texture* Texture::load(const std::string& file, bool antiAlias)
{
	TexturesMap::iterator ti = textures.find(file);
	Texture* texture = 0;
	if (ti == textures.end()) {
		texture = new Texture(pf_LoadGLTexture(file.c_str()));
		texture->setFiltering(antiAlias);
		texture->setWrapMode(true);
		texture->filename = file;
		textures[file] = texture;
	} else
		texture = ti->second;
	
	texture->refCount++;
	return texture;
}

void Texture::setFiltering(bool antiAlias)
{
	GLenum f = antiAlias ? GL_LINEAR : GL_NEAREST;
	glBindTexture(GL_TEXTURE_2D, glID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
}

void Texture::setWrapMode(bool repeat) 
{
	GLenum f = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	glBindTexture(GL_TEXTURE_2D, glID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, f);
}

void Texture::release()
{
	if (--refCount == 0) {
		textures.erase(filename);
		delete this;
	}
}

void Texture::addRef() {
	refCount++;
}

Texture::Texture(const GLTextureInfo& glt) {
	width=glt.width;
	height=glt.height;
	origWidth=glt.origWidth;
	origHeight=glt.origHeight;
	glID=glt.glID;
	refCount=0;
	coordScale=Vector2(origWidth / (float)width, origHeight / (float)height);
}


Texture::~Texture() {
	freeTexture();
}

void Texture::freeTexture() {
	if (glID) {
		glDeleteTextures(1, &glID);
		glID=0;
	}
}	

void RenderUtil::setScreenDims(int w,int h)
{
	width = w;
	height = h;
}

void RenderUtil::drawTexture(Box2 rc, Texture* texture, bool alpha)
{
	drawTexture(rc.a, rc.b, texture, alpha);
}


void RenderUtil::drawTexture(Vector2 bottomLeft, Vector2 topRight, Texture* texture, bool alpha)
{
	drawTexture(bottomLeft, topRight, Vector2(), texture->coordScale, texture, alpha);
}

void RenderUtil::drawTexture(Vector2 bottomLeft, Vector2 topRight, int x,int y,int w,int h, Texture* texture, bool alpha)
{
	Vector2 invDim = texture->invSize();
	Vector2 tcMin = Vector2(x,y) * invDim * texture->coordScale;
	Vector2 tcSize = Vector2(w,h) * invDim * texture->coordScale;
	
	drawTexture(bottomLeft, topRight, tcMin, tcSize, texture, alpha);
}


void RenderUtil::drawTexture(Vector2 bottomLeft, Vector2 topRight, Vector2 tcMin, Vector2 tcSize, Texture* texture, bool alpha) {

	float verts[] = {
		bottomLeft.x, bottomLeft.y,
		bottomLeft.x, topRight.y,
		topRight.x, topRight.y,
		topRight.x, bottomLeft.y
	};
	
	float tc[] = {
		tcMin.x, (tcMin.y+tcSize.y),
		tcMin.x, tcMin.y,
		(tcMin.x+tcSize.x), tcMin.y,
		(tcMin.x+tcSize.x), (tcMin.y+tcSize.y)
	};

//	GL_draw_quad(verts, true);
	GL_draw_quad_tc(verts, tc, texture, alpha);
}

// Setup the projection matrix 
void RenderUtil::setupProjection() 
{
	setupCamera(screenBox());

/*	float m[16];
	ProjectionMatrix(m, width, height, pf_FlipScreen());
	glLoadMatrixf(m);*/
	/*
	 x = ((y / h) - 0.5) * 2 = y/h*2 - 1
	 y = ((x / w) - 0.5) * 2 = x/w*2 - 1
	 
	 2D Matrix:
	 [ 0    2/h   -1 ]
	 [ 2/w  0     -1 ]
	 */
	/*
	Matrix22 ortho;
	ortho.v[1] = 2.0f / width;
	ortho.v[2] = 2.0f / height;
	ortho.t[0] = -1.0f;
	ortho.t[1] = -1.0f;

	float m44[16];
	ortho.transposed44Matrix(m44);

	glLoadMatrixf(m44);*/
}

void RenderUtil::setupCamera(Box2 box)
{
	const float zNear=0, zFar=1000;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(box.a.x, box.b.x, box.a.y, box.b.y, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

void RenderUtil::beginCamera(Box2 box)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	setupCamera(box);

}

void RenderUtil::endCamera()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void RenderUtil::drawRotatedTexture( Texture* tex, Vector2 pos, Vector2 size, float angle, Vector2 pivot )
{
	Vector2 x(angle);
	Vector2 y(x.ortho());

	Vector2 sx = size.x * x;
	Vector2 sy = size.y * y;
	Vector2 px = pivot.x * x;
	Vector2 py = pivot.y * y;

	// 4 vertices
	Vector2 vrt[4] = { 
		pos - px + (sy - py),
		pos + (sx - px) + (sy - py),
		pos + (sx - px) - py,
		pos - px - py
	};
	float tc[] = { 
		0.0f, 0.0f, 
		tex->coordScale.x, 0.0f, 
		tex->coordScale.x, tex->coordScale.y, 
		0.0f, tex->coordScale.y 
	};

	GL_draw_quad_tc(&vrt[0].x, tc, tex, true);

}

void RenderUtil::drawRotatedTexture( Texture* tex, Vector2 pos, Vector2 size, float angle )
{
	drawRotatedTexture(tex, pos, size, angle, size*.5f);
}
