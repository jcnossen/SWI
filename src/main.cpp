
#include "StdIncl.h"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include "PlatformAPI.h"
#include "OpenGL.h"
#include "GlyphRender.h"

#ifdef WIN32
#include <windows.h>
#endif

static bool quitApp = false;
static int gfxWidth = 800, gfxHeight = 600, gfxBPP = 24;

static GlyphRenderer* fontRenderer;

void System_Msg(std::string msg) {
#ifdef WIN32
	MessageBoxA(0, msg.c_str(), "Message:", MB_OK | MB_ICONERROR);
#endif
}


static Vector2 mousePos;
static bool enableMouse = true;

static void Initialize() {
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	int flags = SDL_OPENGL;

	if(!SDL_SetVideoMode(gfxWidth, gfxHeight, gfxBPP, flags)) {
		throw std::runtime_error("Failed to initialize display system");
	}

	SDL_ShowCursor(FALSE);

	fontRenderer = new GlyphRenderer();
	fontRenderer->loadFromConfig("Pericles.font.cfg");
}

static void Draw() {

	if (enableMouse) {
		glBegin(GL_TRIANGLES);
			glColor(Color::Black);
			glVertex(mousePos);
			glVertex(mousePos + Vector2(8, -8));
			glVertex(mousePos + Vector2(0, -12));
		glEnd();
		glBegin(GL_LINE_LOOP);
			glColor(Color::White);
			glVertex(mousePos);
			glVertex(mousePos + Vector2(8, -8));
			glVertex(mousePos + Vector2(0, -12));
		glEnd();
	}

	fontRenderer->drawString(Vector2(), 20.0f, "Swarm Intelligence Demo");

	SDL_GL_SwapBuffers();
}

static void Tick()
{
	// Mouse check (clicked buttons..)
	if (enableMouse) {
		bool mouseDown = (SDL_GetMouseState(0,0) & SDL_BUTTON_LEFT) != 0;


	}
}

static void HandleKeyEvent (SDLKey k, bool down) {
	if (k == SDLK_ESCAPE)
		quitApp=true;

}


static void HandleSDLEvent(SDL_Event* e) {
	if (e->type==SDL_KEYDOWN)
		HandleKeyEvent (e->key.keysym.sym, true);
	if (e->type==SDL_KEYUP)
		HandleKeyEvent (e->key.keysym.sym, false);
	if (e->type == SDL_MOUSEMOTION) 
		mousePos = Vector2(e->motion.x, gfxHeight - e->motion.y - 1);
	if (e->type == SDL_QUIT)
		quitApp=true;
}

#ifdef __MINGW32__
#undef main
#endif


int main(int argc, char* argv[]) {
	
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0) {
		System_Msg(SPrintf("SDL fails: %s", SDL_GetError()));
		return -1;
	}

#ifndef _DEBUG
	try {
#endif

		Initialize();

		uint lastTicks = SDL_GetTicks();
		srand(lastTicks);

		while(!quitApp) {
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				HandleSDLEvent(&event);
			}

			uint nTicks = SDL_GetTicks();
			uint diff = nTicks - lastTicks;
			lastTicks = nTicks;

			Draw();
		}

		SDL_Quit();
#ifndef _DEBUG
	}
	catch (ContentException& e) {
		System_Msg(std::string("Content Error: ")+e.what());
	}
	catch (std::exception& ce) {
		System_Msg(std::string("Exception: ")+ce.what());
	}
#endif

	return 0;
}
