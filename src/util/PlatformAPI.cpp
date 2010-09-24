#include "StdIncl.h"
#include "PlatformAPI.h"

#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#include "OpenGL.h"

typedef std::map<std::string, int> KeyMap;
static KeyMap keyMap;
std::string g_customDataDir;

void pf_DebugLog(const char *txt)
{
#ifdef _MSC_VER
	OutputDebugString(txt);
#else
	fprintf(stderr, txt);
#endif
}



template <typename T>
T nextPowerOfTwo(T n)
{
	std::size_t k=1;
	--n;
	do {
		n |= n >> k;
		k <<= 1;
	} while (n & (n + 1));
	return n + 1;
}


static GLuint uploadTexture(void* pixels, int w,int h, int depth, const char* imgName)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	switch (depth) {
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			break;
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
			break;
		default:
			throw ContentException(SPrintf("'%s' loaded, but pixel format not supported", imgName) );
	}

	return textureID;
}

GLTextureInfo pf_LoadGLTexture(const char *name) {
	std::string path = pf_GetDataDir() + name;

	SDL_Surface* surf = IMG_Load(path.c_str());
	if (!surf)
		throw ContentException(SPrintf("Failed to load image: %s", name));

	// decide how big to make the texture
	int w = nextPowerOfTwo(surf->w);
	int h = nextPowerOfTwo(surf->h);

	GLuint textureID;

	// Does it need resizing?
	if (w == surf->w && h == surf->h) {
		textureID = uploadTexture(surf->pixels, w, h, surf->format->BytesPerPixel, name);
	} else {
		// Note: SDL_BlitSurface doesnt seem to copy alpha
		uchar* tmp = new uchar[w*h*surf->format->BytesPerPixel];
		memset(tmp, 0, w*h*4);
		int srcPitch = surf->format->BytesPerPixel * surf->w;
		int dstPitch = surf->format->BytesPerPixel * w;
		for(int y=0;y<surf->h;y++)
			memcpy (&tmp[y*dstPitch], &((uchar*)surf->pixels)[y*srcPitch], srcPitch);

		textureID = uploadTexture(tmp, w, h, surf->format->BytesPerPixel, name);
		delete[] tmp;
	}

	GLTextureInfo tex;
	tex.origHeight = surf->h;
	tex.origWidth = surf->w;
	tex.glID = textureID;
	tex.width = w;
	tex.height = h;

	d_trace("loaded image: %s\n", path.c_str());
	SDL_FreeSurface(surf);
	return tex;
}

std::string pf_GetDataDir() {
	char buf[256];

	if (!g_customDataDir.empty()) 
		return g_customDataDir;

	DWORD len = GetModuleFileName(0, buf, sizeof(buf));

	// remove the filename
	int x=len-1;
	while (x>=0) {
		if (buf[x] == '/' || buf[x] == '\\')
			break;
		--x;
	}

	return std::string(buf, x) + "/data/";
}

void buildKeyMap() {

	for(int a=0;a<SDLK_LAST;a++) {
		const char *keyName = SDL_GetKeyName((SDLKey)a);

		if (keyName)
			keyMap[keyName] = a;
	}
}

int pf_GetKeyCode(const char*name) {
	KeyMap::iterator ki = keyMap.find(name);

	if (ki == keyMap.end()) {
		d_trace("GetKeyCode: %s is an invalid SDL key name.", name);
		return -1; // invalid name
	}

	return ki->second;
}


void pf_SetAppName(const char *name) {
	SDL_WM_SetCaption(name,0);
}

void pf_OpenWebsite(const char *url) {
	ShellExecute(NULL,"open",url,NULL,NULL,SW_SHOWNORMAL);
}
