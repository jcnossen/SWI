/*
 *  GameAPI.h
 *  iPhoneGame
 *
 *  Created by Jelmer Cnossen on 11/5/09.
 *
 */

#ifndef PLATFORM_API_H
#define PLATFORM_API_H

#include <string>
#include "Vector2.h"

#define GAME_FPS 60
#define GAME_TICK (1.0f/GAME_FPS)

class IGameApp {
public:
	virtual void initialize(int w,int h) = 0;
	virtual void debugMap(std::string mapname) = 0;

	virtual void draw() = 0; // draw
	virtual void tick() = 0; // game tick
	virtual void destroy() = 0;
};

IGameApp* GetGameAppInstance ();

// Textures
struct GLTextureInfo {
	
	GLTextureInfo() {
		width=height=0;
		glID=0;
		origWidth=origHeight=0;
	}
	
	int width,height;
	int origWidth, origHeight;
	unsigned int glID;
};

// Load a texture from the data directory (no need to prefix with data dir path)
GLTextureInfo pf_LoadGLTexture(const char *name);


// Button handling
class ControlButton {
public:
	ControlButton() { state=false; }
	virtual void changed(bool down) = 0;
	virtual void clicked() = 0;
	virtual Box2 getRect() = 0;
	virtual const char* label() { return ""; }
	
	int key; // if keyboard controlled
	bool state;
	
	bool inside(Vector2 p) {
		return getRect().contains(p);
	}
};

void pf_ToggleMouse(bool enable);
void pf_AddCtlButton(ControlButton* b);
void pf_RemoveCtlButton(ControlButton* b);

void pf_DebugLog(const char *txt);

// translating SDL key names into a key code
int pf_GetKeyCode(const char*name);

void pf_OpenWebsite(const char *url);

void pf_SetAppName(const char *name);

// flip width and height? (like on iphone)
bool pf_FlipScreen();

std::string pf_GetDataDir();

#endif // PLATFORM_API_H


