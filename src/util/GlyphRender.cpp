#include "StdIncl.h"

#include "OpenGL.h"
#include "RenderSystem.h"
#include "GlyphRender.h"
#include "CfgParser.h"

#include "PlatformAPI.h"

std::vector<GlyphRenderer*> GlyphRenderer::loadedFonts;

void GlyphRenderer::release() {
	if (--refCount==0) {
		delete this;
		loadedFonts.erase(std::find(loadedFonts.begin(),loadedFonts.end(),this));
	}
}

void GlyphRenderer::addRef() {
	refCount++;
}

GlyphRenderer::GlyphRenderer()
{
	chars=0;
	numChars=0;
	refCount=0;
}

GlyphRenderer::~GlyphRenderer() {
	SafeRelease(texture);
	SafeDeleteArray(chars);
}

void GlyphRenderer::initFont(const char* fontName, const char* charset, int* charInfo)
{
	numChars = strlen(charset);
	chars = new GlyphChar[numChars];

	for(int a=0;a<numChars;a++) {
		GlyphChar& ch = chars[a];
		charMap[charset[a]] = a;
		
		int* info = &charInfo[a*6];
		
		ch.character = charset[a];
		ch.x=info[0];
		ch.y=info[1];
		ch.w=info[2];
		ch.h=info[3];
		ch.kernx1=info[4];
		ch.kernx2=info[5];
	}
	GlyphChar& spaceCh = chars[charMap['a']];
	spaceWidth = spaceCh.kernx2-spaceCh.kernx1;

	texture = Texture::load(fontName, true);
}


Vector2 GlyphRenderer::getStringDims(const char* text) {
	float pos = 0.0f;
	float maxH = 0.0f;
	int len=strlen(text);
	
	for(int a=0;a<len;a++) {
		if (text[a] == ' ')
			pos += spaceWidth;
		else {
			int charIndex = charMap[text[a]];
			GlyphChar& ch = chars[charIndex];
			
			float w = ch.kernx2-ch.kernx1;
			pos += w;
			
			if (ch.h > maxH)
				maxH = ch.h;
		}
	}
	
	return Vector2(pos, maxH);
}

void GlyphRenderer::drawString(Vector2 bottomLeft, Vector2 topRight, const char *text) {
	Vector2 dims = getStringDims(text);
	Vector2 wantedSize = topRight - bottomLeft;
	drawStringWithScale(bottomLeft, Vector2(wantedSize.x/dims.x, wantedSize.y/dims.y), text);
}

void GlyphRenderer::drawString(Vector2 bottomLeft, float height, const char* text) 
{
	Vector2 dims = getStringDims(text);
	
	float scale = height / dims.y;
	
	drawStringWithScale(bottomLeft, Vector2(scale, scale), text);
}

/*

void GlyphRenderer::drawStringWithScale(Vector2 bottomLeft, Vector2 scale, const char* text) {
	int len = strlen(text);

	// 2 floats per vertex, 6 vertices per char
	int numverts = 6*len;
	int bufSize=numverts*sizeof(float)*2;
	float* verts = (float*)ALLOCA(bufSize);
	float* texcoords = (float*)ALLOCA(bufSize);
	
	memset(verts, 0, bufSize);
	memset(texcoords, 0, bufSize);

	float pos = 0.0f;
	float iw = 1.0f/texture->width;
	float ih = 1.0f/texture->height;
	
	for(int a=0;a<len;a++) {
		int charIndex = charMap[text[a]];
		GlyphChar ch = chars[charIndex];
		ch.w*=2;
		ch.h*=2;
		
	//	float w = (ch.kernx2-ch.kernx1) * scale.x;
		float w = ch.w * scale.x;
		float h = ch.h * scale.y;
		Vector2 bl (bottomLeft.x+pos, bottomLeft.y);
		Vector2 tr (bottomLeft.x+pos+w, bottomLeft.y+h);
		
		float* tri = &verts[a*6*2];
		float* txc = &texcoords[a*6*2];
		
		tri[0*2+0] = bl.x;			tri[0*2+1] = bl.y;
		txc[0*2+0] = ch.x;			txc[0*2+1] = ch.y+ch.h;
		tri[1*2+0] = bl.x;			tri[1*2+1] = tr.y;
		txc[1*2+0] = ch.x;			txc[1+2+1] = ch.y;
		tri[2*2+0] = tr.x;			tri[2*2+1] = tr.y;
		txc[2*2+0] = ch.x+ch.w;		txc[2*2+1] = ch.y;

		tri += 6; txc += 6;
		tri[0*2+0] = bl.x;			tri[0*2+1] = bl.y;
		txc[0*2+0] = ch.x;			txc[0*2+1] = ch.y+ch.h;
		tri[1*2+0] = tr.x;			tri[1*2+1] = tr.y;
		txc[1*2+0] = ch.x+ch.w;		txc[1*2+1] = ch.y;
		tri[2*2+0] = tr.x;			tri[2*2+1] = bl.y;
		txc[2*2+0] = ch.x+ch.w;		txc[2*2+1] = ch.y+ch.h;
		
		pos += w;
	}
	
	for (int a=0;a<len*6;a++) {
		texcoords[a*2+0] *= iw*texture->coordScale.x;
		texcoords[a*2+1] *= ih*texture->coordScale.y;
	}
	
	glColor4f(1,1,1,1);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture->glID);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
	
	glDrawArrays(GL_TRIANGLES, 0, len*6);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);	
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}
*/


void GlyphRenderer::drawStringCentered(Vector2 center, Vector2 scale, const char *text)
{
	Vector2 dims = getStringDims(text); // pixel dimensions
	Vector2 bottomLeft = center - dims * scale * 0.5f;

	drawStringWithScale(bottomLeft, scale, text);
}


void GlyphRenderer::drawStringWithScale(Vector2 bottomLeft, Vector2 scale, const char* text) {
	int len = strlen(text);
	
	float pos = 0.0f;
	Vector2 id = texture->invSize();
	
//	glColor4f(0,0,0,0);
	
	int charIndex;
	for(int a=0;a<len;a++) {
		if (text[a] == ' ') {
			pos += spaceWidth * scale.x;
			continue;
		}
		charIndex = charMap[text[a]];
		GlyphChar& ch = chars[charIndex];

		//	float w = (ch.kernx2-ch.kernx1) * scale.x;
		float w = ch.w * scale.x;
		float h = ch.h * scale.y;
		pos -= ch.kernx1 * scale.x;
		
		Vector2 bl (bottomLeft.x+pos, bottomLeft.y);
		Vector2 tr (bottomLeft.x+pos+w, bottomLeft.y+h);
		
		pos += ch.kernx2 * scale.x;

		Vector2 invDim = texture->invSize();
		Vector2 tcMin = Vector2(ch.x,ch.y) * invDim;
		Vector2 tcSize = Vector2(ch.w,ch.h) * invDim;

		//tcSize.x=-tcSize.x;

		RenderUtil::drawTexture(bl, tr, tcMin, tcSize, texture, true);
	}
	
}



GlyphRenderer* GlyphRenderer::loadFromConfig( const char* cfgFile )
{
	for (int a=0;a<loadedFonts.size();a++) {
		if (loadedFonts[a]->config==cfgFile) {
			loadedFonts[a]->addRef();
			return loadedFonts[a];
		}
	}
	
	std::string path = GetPathFromFile(cfgFile);
	GlyphRenderer *gr = new GlyphRenderer();

	CfgList* cfg = CfgValue::LoadFile(pf_GetDataDir() + cfgFile);
	
	const char* image =cfg->GetLiteral("image");
	const char* charset = cfg->GetLiteral("charset");
	const char* datastr = cfg->GetLiteral("data");

	//parse data
	int datasize = strlen(charset) * 6;
	int* data = new int[datasize];

	int p=0;
	int ch=0;
	std::string cur;
	while (datastr[ch]) {
		if (datastr[ch] == ',') {
			data[p++] = atoi(cur.c_str());
			cur = "";
		} else 
			cur += datastr[ch];
		ch ++;
	}
	if (cur.size()>0)
		data[p++] = atoi(cur.c_str());
	assert(p==datasize);

	gr->initFont((path + image).c_str(), charset, data);
	
	delete cfg;

	loadedFonts.push_back(gr);
	gr->config=cfgFile;

	gr->addRef();
	return gr;
}

